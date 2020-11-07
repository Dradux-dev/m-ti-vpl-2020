from forms.Rectangle import Rectangle
from DashedBorder import DashedBorder
from colors.Monochrome import Monochrome
from colors.Greyscale import Greyscale
from colors.Rgb import Rgb

from Image import Image


class BoundingBox:
    def __init__(self, position=(0, 0), size=40):
        self.background = Rectangle(size)
        self.border = DashedBorder(size)
        self.position = position

    def setPosition(self, position):
        self.position = position

    def setSize(self, size):
        self.background.setSize(size)
        self.border.setSize(size)

    def getTopLeft(self):
        return self.position

    def getBottomRight(self):
        return self.position[0] + self.background.getWidth, self.position[1] + self.background.getHeight

    def getWidth(self):
        return self.background.getWidth

    def getHeight(self):
        return self.background.getHeight

    @staticmethod
    def asGreyscale(color):
        return int((color[0] + color[1] + color[2]) / 3)

    @staticmethod
    def asMonochrome(color):
        return int(BoundingBox.asGreyscale(color) >= 128)

    @staticmethod
    def monochrome(color):
        generator = Monochrome()
        generator.addColor(color)
        return generator

    @staticmethod
    def greyscale(color):
        generator = Greyscale()
        generator.addColor(color)
        return generator

    @staticmethod
    def rgb(color):
        generator = Rgb()
        generator.addColor(color)
        return generator

    @staticmethod
    def convertColor(color, colorMode):
        colors = {
            "monochrome": BoundingBox.monochrome(BoundingBox.asMonochrome(color)),
            "greyscale": BoundingBox.greyscale(BoundingBox.asGreyscale(color)),
            "rgb": BoundingBox.rgb(color)
        }

        return colors[colorMode]

    def render(self, image, backgroundColor, borderColor):
        backgroundGenerator = BoundingBox.convertColor(backgroundColor, image.colorMode)
        image.addForm(self.position, self.background, backgroundGenerator)

        borderGenerator = BoundingBox.convertColor(borderColor, image.colorMode)
        image.addForm(self.position, self.border, borderGenerator)
