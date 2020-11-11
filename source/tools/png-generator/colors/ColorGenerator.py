from Color import Color


class ColorGenerator:
    def __init__(self):
        self.colors = []
        self.__enabled = True
        pass

    def enable(self):
        self.__enabled = True

    def disable(self):
        self.__enabled = False

    @property
    def isEnabled(self):
        return self.__enabled

    def clearColors(self):
        self.colors = []

    def addColor(self, color):
        self.colors.append(color)

    @staticmethod
    def colorsRequired():
        return 1

    @staticmethod
    def clamp(value, minValue, maxValue):
        return max(minValue, min(maxValue, value))

    def lerp(self, alpha, minColor, maxColor):
        alpha = self.clamp(alpha, 0, 1)
        if isinstance(minColor, Color) and isinstance(maxColor, Color):
            return Color(
                self.lerp(alpha, minColor.r, maxColor.r),
                self.lerp(alpha, minColor.g, maxColor.g),
                self.lerp(alpha, minColor.b, maxColor.b)
            )

        return minColor + ((maxColor - minColor) * alpha)

    @staticmethod
    def scale(value, factor):
        if isinstance(value, Color):
            return Color(
                value.r * factor,
                value.g * factor,
                value.b * factor
            )
        else:
            return value * factor

    def __getColor(self, progress):
        return self.colors[0]

    @staticmethod
    def progress(current, total):
        return (
            current[0]/total[0],
            current[1]/total[1]
        )

    def getPixel(self, intensity, progress=(1, 1)):
        # intensity [0;1]
        # progress tuple(horizontal, vertical) für gradient
        return self.scale(self.__getColor(progress), intensity)
