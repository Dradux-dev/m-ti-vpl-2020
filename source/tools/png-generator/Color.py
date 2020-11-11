import random
from enum import Enum


class Color:
    class Mode(Enum):
        INVALID = 0
        MONOCHROME = 1
        GREYSCALE = 2
        RGB = 3

    class Range:
        def __init__(self, a, b):
            self.__a = a
            self.__b = b

        @property
        def get(self):
            return Color.Random(self.__a, self.__b)

    def __init__(self, r, g, b, mode=Mode.RGB):
        if isinstance(r, tuple):
            self.__r = int(max(0, min(255, r[0])))
            self.__g = int(max(0, min(255, r[1])))
            self.__b = int(max(0, min(255, r[2])))
            self.__mode = g
        else:
            self.__r = int(max(0, min(255, r)))
            self.__g = int(max(0, min(255, g)))
            self.__b = int(max(0, min(255, b)))
            self.__mode = mode

    @property
    def r(self):
        return self.__r

    @property
    def g(self):
        return self.__g

    @property
    def b(self):
        return self.__b

    @property
    def mode(self):
        return self.__mode

    @mode.setter
    def mode(self, newmode):
        if newmode not in [Color.Mode.MONOCHROME, Color.Mode.GREYSCALE, Color.Mode.RGB]:
            raise ValueError(f"Unknown new color mode {newmode}")

        self.__mode = newmode

    @property
    def modeName(self):
        return Color.ModeString(self.__mode)

    def asGreyscale(self):
        # Weighted method or luminosity method
        return int(0.3 * self.__r + 0.59 * self.__g + 0.11 * self.__b)
        # Average method
        # return int((self.__r + self.__g + self.__b) / 3)

    def asMonochrome(self):
        return int(self.asGreyscale() >= 128)

    def convert(self, mode):
        if mode == Color.Mode.MONOCHROME:
            return self.asMonochrome()
        elif mode == Color.Mode.GREYSCALE:
            return self.asGreyscale()
        elif mode == Color.Mode.RGB:
            return self.__r, self.__g, self.__b

        raise ValueError(f"Invalid color mode {mode} found")

    @property
    def value(self):
        return self.convert(self.__mode)

    @property
    def appendValue(self):
        value = self.value
        if isinstance(value, tuple):
            return value

        return [value]

    @staticmethod
    def SwapMode(c, mode):
        return Color(c.r, c.g, c.b, mode)

    @staticmethod
    def ModeString(mode):
        t = {
            Color.Mode.MONOCHROME: "monochrome",
            Color.Mode.GREYSCALE: "greyscale",
            Color.Mode.RGB: "rgb"
        }

        return t[mode]

    @staticmethod
    def Random(a, b):
        assert(isinstance(a, Color))
        assert(isinstance(b, Color))
        assert(a.mode == b.mode)

        return Color(
            random.randint(a.r, b.r),
            random.randint(a.g, b.g),
            random.randint(a.b, b.b),
            a.mode
        )

    @staticmethod
    def __MonochromeFromString(s):
        # "0" -> Black
        # "1" -> White
        assert (s.isnumeric())
        value = int(s)
        assert (value == 0 or value == 1)
        return Color(value * 255, value * 255, value * 255, Color.Mode.MONOCHROME)

    @staticmethod
    def __GreyscaleFromString(s):
        # "[0..255]" -> Grey Value
        # "fixed,[0..255]" -> Color Greyscale
        # "random,[0..255],[0..255]" -> Color.Range(Value1, Value2)
        if s.isnumeric():
            # Clamped integer value [0..255]
            value = max(0, min(int(s), 255))
            return Color(value, value, value, Color.Mode.GREYSCALE)

        elif s.startswith("fixed,"):
            parts = s.split(",")
            if parts[1].isnumeric():
                value = max(0, min(int(parts[1]), 255))
                return Color(value, value, value, Color.Mode.GREYSCALE)
            else:
                raise ValueError(f"Expected second part of {s} to be number between 0 and 255.")
        elif s.startswith("random,"):
            parts = s.split(",")
            if len(parts) != 3:
                raise ValueError(f"Expected 2 arguments for random, but found {len(parts)} in {s}")

            if not parts[1].isnumeric():
                raise ValueError(f"Expected second part of {s} to be a number between 0 and 255")

            if not parts[2].isnumeric():
                raise ValueError(f"Expected third part of {s} to be a number between 0 and 255")

            minValue = max(0, min(int(parts[1]), 255))
            maxValue = max(0, min(int(parts[2]), 255))
            return Color.Range(
                Color(minValue, minValue, minValue, Color.Mode.GREYSCALE),
                Color(maxValue, maxValue, maxValue, Color.Mode.GREYSCALE)
            )

        else:
            raise ValueError(f"Can't parse greyscale color {s}")

    @staticmethod
    def __RgbFromString(s):
        # "[0..255],[0..255],[0..255]" RGB
        # "fixed,[0..255],[0..255],[0..255]" RGB
        # "random,[0..255],[0..255],[0..255],[0..255],[0..255],[0..255]" RGBmin,RGBmax
        parts = s.split(",")
        if parts[0].isnumeric():
            r = max(0, min(int(parts[0]), 255))
            g = max(0, min(int(parts[1]), 255))
            b = max(0, min(int(parts[2]), 255))
            return Color(r, g, b, Color.Mode.RGB)

        elif parts[0] == "fixed":
            if not parts[1].isnumeric():
                raise ValueError(f"Expected second part of {s} to be a number between 0 and 255")

            if not parts[2].isnumeric():
                raise ValueError(f"Expected third part of {s} to be a number between 0 and 255")

            if not parts[3].isnumeric():
                raise ValueError(f"Expected fourth part of {s} to be a number between 0 and 255")

            r = max(0, min(int(parts[1]), 255))
            g = max(0, min(int(parts[2]), 255))
            b = max(0, min(int(parts[3]), 255))
            return Color(r, g, b, Color.Mode.RGB)

        elif parts[0] == "random":
            if not parts[1].isnumeric():
                raise ValueError(f"Expected second part of {s} to be a number between 0 and 255")

            if not parts[2].isnumeric():
                raise ValueError(f"Expected third part of {s} to be a number between 0 and 255")

            if not parts[3].isnumeric():
                raise ValueError(f"Expected fourth part of {s} to be a number between 0 and 255")

            if not parts[4].isnumeric():
                raise ValueError(f"Expected fifth part of {s} to be a number between 0 and 255")

            if not parts[5].isnumeric():
                raise ValueError(f"Expected sixth part of {s} to be a number between 0 and 255")

            if not parts[6].isnumeric():
                raise ValueError(f"Expected seventh part of {s} to be a number between 0 and 255")

            r_min = max(0, min(int(parts[1]), 255))
            g_min = max(0, min(int(parts[2]), 255))
            b_min = max(0, min(int(parts[3]), 255))
            r_max = max(0, min(int(parts[4]), 255))
            g_max = max(0, min(int(parts[5]), 255))
            b_max = max(0, min(int(parts[6]), 255))
            return Color.Range(
                Color(r_min, g_min, b_min, Color.Mode.RGB),
                Color(r_max, g_max, b_max, Color.Mode.RGB)
            )

        else:
            raise ValueError(f"Can't parse rgb color {s}")

    @staticmethod
    def FromString(s, mode=Mode.RGB):
        if s is None:
            raise ValueError("Can't parse None string")

        t = {
            Color.Mode.MONOCHROME: Color.__MonochromeFromString,
            Color.Mode.GREYSCALE: Color.__GreyscaleFromString,
            Color.Mode.RGB: Color.__RgbFromString
        }

        parser = t[mode]
        if parser is None:
            raise ValueError(f"Unknown color mode {mode}")

        return parser(s)

    def __str__(self):
        return f"{self.modeName}: {self.r}, {self.g}, {self.b} -> {self.value}"

