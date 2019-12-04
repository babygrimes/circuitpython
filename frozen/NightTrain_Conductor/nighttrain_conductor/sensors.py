try:
    import time
    monotonic = time.monotonic
except ImportError:
    import utime as time
    monotonic = time.time

class CSV:
    def __init__(self, fp):
        self.fp = fp

    def __iter__(self):
        return self

    def __next__(self):
        return [x.strip() for x in self.fp.readline().split(',')]

class Sensors:
    def __init__(self, filename):
        self.csv = CSV(open(filename))
        self.hdr = [x.lower() for x in next(self.csv)]
        self.next()
        self.ts = monotonic() - self.elt['time']

    def next(self):
        elt = next(self.csv)
        if elt == ['']:
            raise StopIteration

        self.elt = {k:float(v.strip()) for k,v in zip(self.hdr, elt)}

    def measure(self):
        dt = monotonic() - self.ts
        while dt > self.elt['time']:
            self.next()

        return self.elt
