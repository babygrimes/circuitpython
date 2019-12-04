#!/usr/bin/env python

import gc
import micropython
import sys

try:
    import time
    monotonic = time.monotonic
except ImportError:
    import utime as time
    monotonic = time.time

import nighttrain

from . import animate
from . import ledstrip
from . import lowpass
from . import sensors

def altitude(p, t=20.0):
    # https://physics.stackexchange.com/questions/333475/how-to-calculate-altitude-from-current-temperature-and-pressure
    # return (pow(101325.0/p, 0.190223) - 1.0) * (t+273.15) * 504.74662
    return nighttrain.altitude_from_baro(p, t)

class Environment:
    def __init__(self):
        self.baro = lowpass.Lowpass(1000.0)
        self.accel = lowpass.Lowpass(25.0)
        self.temp = lowpass.Lowpass(1.0)
        self.velo = lowpass.Lowpass(75.0)

        self.ts = None
        self.i = 0

    @property
    def initialized(self):
        return hasattr(self, 'acceleration')

    def update(self, sensors):
        baro = self.baro.update(sensors['press'])
        accel = self.accel.update(sensors['totalacc'])
        ts = sensors['time']
        alt = altitude(baro)

        try:
            if self.i == 5:
                alt -= self.msl
                self.velocity = self.velo.update((alt - self.altitude) / (ts - self.ts))
                self.altitude = alt
                self.acceleration = accel
            else:
                self.i += 1
                if self.i == 5:
                    self.altitude = 0.0
                    self.msl = alt
        finally:
            self.ts = ts

class State:
    def __init__(self):
        self.leds = []

        sname = "          " + self.__class__.__name__ + "          "
        print("\033[1;{}H{}".format((80-len(sname)//2), sname))

    @classmethod
    def initialize(self, strips, segments):
        self.strips = strips
        self.segments = segments

    def tock(self, led):
        led.tick()

    def tick(self):
        for led in self.leds:
            self.tock(led)

        self.strips.show()

    def update(self, env):
        return self

class PowerOn(State):
    def __init__(self):
        super().__init__()

        self.leds = [animate.ChaseOut(s, color=animate.WHEEL, density=0.2, tail=0.1) for s in self.segments]
        for i, led in enumerate(self.leds):
            n = (i * 5)
            led.seed_gamma(n)
            led.seed_color(n)

        self.ts = monotonic()

    def update(self, env):
        if (monotonic() - self.ts) < 10.0:
            return self

        return Ready()

class Ready(State):
    def __init__(self):
        super().__init__()

        self.leds = [animate.Blink(s, color=animate.WHITE, gamma_speed=3, duration=6, duty=3) for s in self.segments]
        self.ts = monotonic()

    def update(self, env):
        if (monotonic() - self.ts) < 2.0:
            return self

        return Pad()

class FlightStage(State):
    def update(self, env):
        super().update(env)
        print("\033[30;40Ht: {:5.2f} baro: {:5.0f} acc: {:5.2f} alt: {:7.2f} vel: {:>7.2f} mem: {:>6d}".format(env.ts, env.baro.x, env.acceleration, env.altitude, env.velocity, gc.mem_free()))
        return self

class Pad(FlightStage):
    def __init__(self):
        super().__init__()

        self.leds = [animate.KITT(s, color=animate.RED, gamma_speed=2) for s in self.segments]

    def update(self, env):
        super().update(env)
        if env.acceleration < 3.0:
            return self

        return Boost(self.leds)

class Boost(FlightStage):
    def __init__(self, leds):
        super().__init__()

        self.leds = [animate.Pulse(s, color=animate.MAGENTA, pwm=15) for s in self.segments]
        for i, led in enumerate(leds):
            self.leds[i].fade(led, 30)

    def update(self, env):
        super().update(env)
        if env.acceleration > 3.0:
            return self

        return Coast(self.leds)

class Coast(FlightStage):
    def __init__(self, leds):
        super().__init__()

        self.leds = [animate.ChaseOut(s, color=animate.YELLOW, tail=0.3) for s in self.segments]
        self.ts = 0
        for i, led in enumerate(leds):
            self.leds[i].fade(led, 30)

    def tock(self, led):
        for i in range(self.ts):
            led.tick(render=False)
        led.tick()

    def update(self, env):
        super().update(env)
        vel = int(max(0, env.velocity))
        if env.velocity < 100:
            self.ts = 0
            speed = max(1, (100-vel) // 10)
            for led in self.leds:
                led.gamma_speed = speed
        else:
            self.ts = max(0, (vel-100) // 50)
            for led in self.leds:
                led.gamma_speed = 1

        if env.velocity > 0.0:
            return self

        return Apogee(env.ts)

class Apogee(FlightStage):
    def __init__(self, ts):
        super().__init__()

        self.leds = [animate.Random(s, gamma_speed=5) for s in self.segments]
        self.ts = ts + 2.0

    def update(self, env):
        super().update(env)

        if env.ts <= self.ts:
            return self

        return InitialDescent()

class InitialDescent(FlightStage):
    def __init__(self):
        super().__init__()

        self.leds = [animate.WarpCore(s, color=animate.WHEEL, gamma_speed=2) for s in self.segments]

        for i in range(1, len(self.leds)//2):
            for led in (self.leds[i], self.leds[-(i+1)]):
                led.seed_gamma(i*4)

    def update(self, env):
        super().update(env)

        if env.altitude > 300.0:
            return self

        return FinalDescent()

class FinalDescent(FlightStage):
    def __init__(self):
        super().__init__()

        self.leds = [animate.Collider(s, color=animate.WHEEL, gamma_speed=2) for s in self.segments]

        for i, led in enumerate(self.leds):
            led.seed_color(i*15)

        self.gamma_speed = 2

    def tock(self, led):
        if self.gamma_speed == 0:
            led.tick(render=False)
        led.tick()

    def update(self, env):
        # XXX: replay ends
        if not env:
            return Down()

        super().update(env)

        if self.gamma_speed == 2 and env.altitude < 200.0:
            self.gamma_speed = 1
            for led in self.leds:
                led.gamma_speed = 1
        elif self.gamma_speed == 1 and env.altitude < 100.0:
            self.gamma_speed = 0

        if env.altitude > 100.0 or abs(env.velocity) > 3.5:
            return self

        return Down()

class Down(State):
    def __init__(self):
        super().__init__()

        self.leds = [animate.ChaseIn(s, color=animate.WHITE, density=0.9, tail=0.1) for s in self.segments]

        for i, led in enumerate(self.leds):
            led.seed_gamma(i*20)

initialize = State.initialize
