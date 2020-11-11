from colors.ColorGenerator import ColorGenerator
import random


class Picker(ColorGenerator):
    def __init__(self):
        ColorGenerator.__init__(self)
        self.color = None

    def colorsRequired(self):
        # Infinite amount
        return -1

    def __getColor(self, progress):
        if self.color is None:
            self.newForm()

        return self.color

    def getPixel(self, intensity, progress=(1, 1)):
        # intensity [0;1]
        # progress tuple(horizontal, vertical) für gradient
        return ColorGenerator.scale(self.__getColor(progress), intensity)

    def newForm(self):
        if len(self.colors) == 0:
            raise ValueError(f"Requires at least 1 color")

        self.color = self.colors[random.randint(0, len(self.colors)-1)]
