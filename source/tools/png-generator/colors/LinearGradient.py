from colors.Gradient import Gradient
from colors.ColorGenerator import  ColorGenerator


class LinearGradient(Gradient):
    def __init__(self, coordinates=[(0, 0), (1, 1)], progressScale=1):
        Gradient.__init__(self)
        self.coordinates = coordinates
        self.progressScale = progressScale

    def __getColor(self, progress):
        scaledProgress = (
            ColorGenerator.lerp(self, progress[0], self.coordinates[0][0], self.coordinates[1][0]),
            ColorGenerator.lerp(self, progress[1], self.coordinates[0][1], self.coordinates[1][1])
        )

        avgProgress = ColorGenerator.clamp(((scaledProgress[0] + scaledProgress[1]) / 2), 0, 1)
        return self.lerp(avgProgress, self.colors[0], self.colors[1])

    def getPixel(self, intensity, progress=(1, 1)):
        # intensity [0;1]
        # progress tuple(horizontal, vertical) für gradient
        lerpedColor = ColorGenerator.scale(self, self.__getColor(progress), intensity)
        return int(lerpedColor[0]), int(lerpedColor[1]), int(lerpedColor[2])

