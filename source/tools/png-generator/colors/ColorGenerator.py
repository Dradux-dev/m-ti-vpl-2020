class ColorGenerator:
    def __init__(self):
        self.colors = []
        pass

    def clearColors(self):
        self.colors = []

    def addColor(self, color):
        self.colors.append(color)

    @staticmethod
    def colorsRequired():
        return 1

    @staticmethod
    def clamp(value, min, max):
        if value < min:
            return min

        if value > max:
            return max

        return value

    def lerp(self, alpha, min, max):
        alpha = self.clamp(alpha, 0, 1)
        if isinstance(min, tuple) and isinstance(max, tuple):
            result = []
            for i in range(len(min)):
                result.append(min[i] + ((max[i] - min[i]) * alpha))

            return tuple(result)
        elif not isinstance(min, tuple) and isinstance(max, tuple):
            result = []
            for i in range(len(max)):
                result.append(min + ((max[i] - min) * alpha))

            return tuple(result)
        elif isinstance(min, tuple) and not isinstance(max, tuple):
            result = []
            for i in range(len(min)):
                result.append(min[i] + ((max - min[i]) * alpha))

            return tuple(result)
        else:
            return min + ((max - min) * alpha)

    @staticmethod
    def scale(value, factor):
        if isinstance(value, tuple):
            result = []
            for i in range(len(value)):
                result.append( value[i] * factor )

            return tuple(result)
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

    @staticmethod
    def getColorMode():
        return None
