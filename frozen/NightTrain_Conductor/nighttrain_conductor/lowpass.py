class Lowpass:
    def __init__(self, R):
        self.R = float(R)
        self.p = None
        self.x = None

    def update(self, z):
        if self.p is None:
            self.p = z
            return z

        if self.x is None:
            self.x = z
            return z

        x = self.x
        v = (0.5 * x) + (0.25 * (self.p + z))

        if abs(v - x) > self.R:
            v = x

        self.p = x
        self.x = v

        return v
