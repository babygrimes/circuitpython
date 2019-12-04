from .colors import *

class LEDmanager:
    # shared across all subclasses
    gamma_cache = {}

    def __init__(self, segment, gamma, gamma_speed=1, color=WHITE, color_speed=1):
        self.seg = segment
        self.ct = len(segment)
        self.g = gamma
        self.gi = 0
        self.gs = gamma_speed
        self.c = color
        self.ci = 0
        self.cs = color_speed
        self.i = 0

        # fade transition support
        self.fade_from = None

    @classmethod
    def _get_cached_gamma(self, *args):
        return self.gamma_cache.get((self.__name__, args))

    @classmethod
    def _put_cached_gamma(self, gamma, *args):
        self.gamma_cache[(self.__name__, args)] = gamma

    @property
    def color(self):
        return self.c

    @color.setter
    def color(self, color):
        self.c = color

    @property
    def gamma_speed(self):
        return self.gs

    @gamma_speed.setter
    def gamma_speed(self, speed):
        self.gs = speed

    def seed_color(self, ct):
        self.ci -= (ct // self.cs)

    def seed_gamma(self, ct):
        self.gi -= (ct // self.gs)

    def tick(self, render=True):
        self.i += 1
        if self.i % self.gs == 0:
            self.update_gamma()

        if self.i % self.cs == 0:
            self.update_color()

        if render:
            self.render()
            if self.fade_from:
                self.fade_val += self.fade_rate
                if self.fade_val < 255:
                    self.fade_from.tick()
                    self.seg.blend(self.fade_from.seg, self.fade_val)
                else:
                    self.fade_from = None

    def fade(self, other, rate):
        other.seg = other.seg.detach()
        self.fade_from = other
        self.fade_rate = rate
        self.fade_val = 0

    def update_gamma(self):
        self.gi -= 1

    def update_color(self):
        self.ci -= 1

    def render(self):
        self.seg.render(self.g, self.gi, self.c, self.ci)

class ChaseOut(LEDmanager):
    def __init__(self, segment, density=0.2, tail=0.5, gamma_speed=1, color=WHITE, color_speed=1):
        ct = len(segment)
        gamma = self._get_cached_gamma(ct, density, tail)
        if not gamma:
            gamma = bytearray(ct + int(ct * tail))
            inc = 1.0 / (ct * density)
            for i in range(int(ct * density)):
                gamma[i] = int(255 * i * inc)

            gamma = self._set_direction(gamma)
            self._put_cached_gamma(gamma, ct, density, tail)

        super().__init__(segment, gamma=gamma, gamma_speed=gamma_speed, color=color, color_speed=color_speed)

    @staticmethod
    def _set_direction(gamma):
        return gamma

class ChaseIn(ChaseOut):
    @staticmethod
    def _set_direction(gamma):
        return bytearray(reversed(gamma))

    def render(self):
        self.seg.render(self.g, -self.gi, self.c, self.ci)

class KITT(LEDmanager):
    def __init__(self, segment, density=0.7, gamma_speed=1, color=WHITE, color_speed=1):
        ct = len(segment)
        gamma = self._get_cached_gamma(ct, density)
        if not gamma:
            gamma = bytearray(ct)

            num = int(ct * density) // 2
            inc = 1.0 / num
            for i in range(num):
                gamma[i] = int(255 * i * inc)

            gamma[num:num*2] = bytearray(reversed(gamma[:num]))
            self._put_cached_gamma(gamma, ct, density)

        super().__init__(segment, gamma=gamma, gamma_speed=gamma_speed, color=color, color_speed=color_speed)
        self.gd = -1
        self.gn = -int(ct * (1.0 - density))

    def update_gamma(self):
        gi = self.gi + self.gd
        if self.gn <= gi <= 0:
            self.gi = gi
            return

        self.gd = -self.gd
        return self.update_gamma()

class Pulse(LEDmanager):
    def __init__(self, segment, pwm=1, gamma_speed=1, color=WHITE, color_speed=1):
        ct = len(segment)
        b = self._get_cached_gamma(ct, pwm)
        if not b:
            b = bytearray([i*pwm for i in range(255//pwm)])
            b += bytearray(reversed(b))
            self._put_cached_gamma(b, ct, pwm)
        self.b = b

        super().__init__(segment, gamma=bytearray(1), gamma_speed=gamma_speed, color=color, color_speed=color_speed)

    def update_gamma(self):
        super().update_gamma()
        gi = self.gi % len(self.b)
        self.g[0] = self.b[gi]

class AlwaysOn(LEDmanager):
    def __init__(self, segment, *args, **kw):
        super().__init__(segment, gamma=b'\xff', *args, **kw)

class Blink(LEDmanager):
    def __init__(self, segment, duration=10, duty=2, gamma_speed=1, color=WHITE, color_speed=1):
        super().__init__(segment, gamma=bytearray(1), gamma_speed=gamma_speed, color=color, color_speed=color_speed)
        b = self._get_cached_gamma(duration, duty)
        if not b:
            b = bytearray([255] * duty) + bytearray(duration - duty)
            self._put_cached_gamma(b, duration, duty)
        self.b = b

    def update_gamma(self):
        super().update_gamma()
        bi = self.gi % len(self.b)
        if self.g[0] != self.b[bi]:
            self.g[0] = self.b[bi]

class WarpCore(LEDmanager):
    def __init__(self, segment, gamma_speed=1, color=WHITE, color_speed=1):
        ct = len(segment)
        assert (ct & 0x1) == 0x00
        mid = ct >> 1

        gamma = self._get_cached_gamma(ct)
        if not gamma:
            num = mid + (ct >> 2)

            gamma = bytearray(num)

            inc = 1.0 / mid
            for i in range(mid):
                gamma[i] = int(255 * i * inc)

            self._put_cached_gamma(gamma, ct)

        super().__init__(segment, gamma=gamma, gamma_speed=gamma_speed, color=color, color_speed=color_speed)
        self.split(mid)

    def render(self):
        self.lhs.render(self.g, self.gi, self.c, self.ci)
        self.rhs.render(self.g, self.gi, self.c, self.ci)

    def split(self, mid):
        self.lhs = self.seg[0:mid:-1]
        self.rhs = self.seg[mid:]

class Collider(WarpCore):
    def split(self, mid):
        self.lhs = self.seg[:mid]
        self.rhs = self.seg[mid::-1]

try:
    import random
except ImportError:
    import urandom as random

class Random(LEDmanager):
    def __init__(self, segment, *args, **kw):
        ct = len(segment)
        gamma = self._get_cached_gamma(ct)
        if not gamma:
            gamma = bytearray(ct)
            n = 0
            for i in range(ct):
                if n == 0:
                    bits = random.getrandbits(32)
                    n = 32
                gamma[i] = 255 * (bits & 0x1)
                bits >>= 1
                n -= 1

            self._put_cached_gamma(gamma, ct)

        super().__init__(segment, gamma=gamma, *args, **kw)

    def update_gamma(self):
        self.gi = random.getrandbits(16)
