import array
import nighttrain
import sys

class Strip:
    def __init__(self, ct):
        self.ct = ct

    def __len__(self):
        return self.ct

    def segment(self, i, ct, brightness=255, direction=1):
        raise NotImplementedError('segment() called on abstract base')

class Segment:
    # any immutable empty sequence
    amputation_buffer = b''

    @classmethod
    def _expand_amputation_buffer(self, ct):
        if len(self.amputation_buffer) < ct:
            self.amputation_buffer = memoryview(array.array('I', range(ct)))

    def __init__(self, buf, ct=None, brightness=255, direction=1):
        self.buf = buf
        if ct is None:
            ct = len(buf)
        self.ct = ct
        self.brightness = brightness
        self.direction = direction

    def __len__(self):
        return self.ct

    def __getitem__(self, i):
        assert isinstance(i, (int, slice))
        if isinstance(i, slice):
            start, stop, step = i.indices(self.ct)
        else:
            start, stop, step = i, (i + 1), 1

        if step not in (1, -1):
            raise ValueError('step ({}) must be +1 or -1'.format(step))

        return self._subset(start, stop, step)

    def _render(self, g, gi, c, ci):
        raise NotImplementedError('_render() called on abstract base')

    def _subset(self, start, stop, step):
        return self.__class__(self.buf[start:stop], brightness=self.brightness, direction=(step * self.direction))

    def detach(self):
        self._expand_amputation_buffer(len(self.buf))

        return self.__class__(self.amputation_buffer[:len(self.buf)], ct=self.ct, brightness=self.brightness, direction=self.direction)

    def render(self, g, gi, c, ci):
        self._render(g, gi, c, ci)

        if self.direction < 0:
            self.reverse()

    def blend(self, other):
        raise NotImplementedError('blend() called on abstract base')

    def reverse(self):
        raise NotImplementedError('reverse() called on abstract base')

class StripCollection(list):
    def __init__(self, n, ct):
        super().__init__()
        self.ct = ct
        self.extend(self._strip(ct) for i in range(n))
        self.buf = tuple(s.buf for s in self)

    def _strip(self, ct):
        raise NotImplementedError('_strip() called on abstract base')

    def show(self):
        raise NotImplementedError('show() called on abstract base')

class SK6812segment(Segment):
    def _render(self, g, gi, c, ci):
        nighttrain.sk6812_render(self.brightness, self.buf, g, gi, c, ci)

    def blend(self, other, bias):
        nighttrain.sk6812_blend(self.buf, other.buf, bias)

    def reverse(self):
        nighttrain.skxxxx_reverse(self.buf)

class SK6812strip(Strip):
    def __init__(self, ct):
        super().__init__(ct)
        self.buf = array.array('I', (0 for x in range(ct)))
        self.led = memoryview(self.buf)

    def segment(self, i, ct, brightness=255, direction=1):
        return SK6812segment(self.led[i:i+ct], brightness=brightness, direction=direction)

    def show(self):
        nighttrain.sk6812_parallel_spi(self.buf)

class SK6812collection(StripCollection):
    def _strip(self, ct):
        return SK6812strip(ct)

    def show(self):
        nighttrain.sk6812_parallel_spi(*self.buf)

class SK9822segment(Segment):
    def _render(self, g, gi, c, ci):
        nighttrain.sk9822_render_hardware_gamma(self.brightness, self.buf, g, gi, c, ci)

    def blend(self, other, bias):
        nighttrain.sk9822_blend_hardware_gamma(self.buf, other.buf, bias)

    def reverse(self):
        nighttrain.skxxxx_reverse(self.buf)

class SK9822strip(Strip):
    def __init__(self, ct):
        super().__init__(ct)
        self.buf = array.array('I', (0 for x in range(ct + 2 + (ct >> 6) + (1 if (ct & 0x3f) else 0))))
        self.led = memoryview(self.buf)[1:1+ct]

    def segment(self, i, ct, brightness=255, direction=1):
        return SK9822segment(self.led[i:i+ct], brightness=brightness, direction=direction)

    def show(self):
        nighttrain.sk9822_parallel_spi(self.buf)

class SK9822collection(StripCollection):
    def _strip(self, ct):
        return SK9822strip(ct)

    def show(self):
        nighttrain.sk9822_parallel_spi(*self.buf)

class TERMIOsegment(Segment):
    @classmethod
    def _expand_amputation_buffer(self, ct):
        if len(self.amputation_buffer) < ct:
            self.amputation_buffer = bytearray(ct)

    def _subset(self, start, stop, step):
        return self.__class__(self.buf[20*start:20*stop], ct=(stop - start), brightness=self.brightness, direction=(step * self.direction))

    def _render(self, g, gi, c, ci):
        nighttrain.termio_render(self.buf, g, gi, c, ci)

    def blend(self, other, bias):
        nighttrain.termio_blend(self.buf, other.buf, bias)

    def reverse(self):
        nighttrain.termio_reverse(self.buf)

TERMIO_PIXEL = b'\033[38;2;000;000;000m*'

class TERMIOstrip(Strip):
    def __init__(self, ct, row, col):
        super().__init__(ct)
        start = '\033[{};{}H\033[0m\033[1m['.format(row, col).encode()
        end = b'\033[0m]'

        self.buf = bytearray(len(start) + (20*ct) + len(end))
        self.buf[:len(start)] = start
        self.buf[-len(end):] = end

        self.led = memoryview(self.buf)[len(start):-len(end)]
        for i in range(0, len(self.led), 20):
            self.led[i:i+20] = TERMIO_PIXEL

    def segment(self, i, ct, brightness=255, direction=1):
        return TERMIOsegment(self.led[20*i:20*(i+ct)], ct=ct, brightness=brightness, direction=direction)

    def show(self):
        sys.stdout.write(self.buf)

class TERMIOcollection(StripCollection):
    def __init__(self, n, ct, row, col):
        self.row = row
        self.col = col
        super().__init__(n, ct)

    def _strip(self, ct):
        row = self.row
        self.row += 1

        return TERMIOstrip(ct, row, self.col)

    def show(self):
        for s in self:
            s.show()
