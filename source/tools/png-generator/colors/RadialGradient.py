import math

from colors.Gradient import Gradient
from colors.ColorGenerator import ColorGenerator
from Color import Color


class RadialGradient(Gradient):
    def __init__(self, center=(0.5, 0.5), progressScale=1):
        Gradient.__init__(self)
        self.center = center
        self.progressScale = progressScale

    def __getColor(self, progress):
        if len(self.colors) < self.colorsRequired():
            raise ValueError(f"Only got {len(self.colors)} colors, but requires {self.colorsRequired()} colors")
        
        direction = (
            self.center[0] - progress[0],
            self.center[1] - progress[1]
        )

        distance = math.sqrt(direction[0]*direction[0] + direction[1]*direction[1]) * self.progressScale
        return self.lerp(distance, self.colors[0], self.colors[1])

    def getPixel(self, intensity, progress=(1, 1)):
        # intensity [0;1]
        # progress tuple(horizontal, vertical) für gradient
        return ColorGenerator.scale(self.__getColor(progress), intensity)
