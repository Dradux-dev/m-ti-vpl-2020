from shapely.geometry import Point, Polygon

from forms.Rectangle import Rectangle
from DashedBorder import DashedBorder
from colors.Monochrome import Monochrome
from colors.Greyscale import Greyscale
from colors.Rgb import Rgb


class BoundingBox:
    def __init__(self, position=(0, 0), size=40):
        self.background = Rectangle(size)
        self.border = DashedBorder(size)
        self.position = position
        self.backgroundColor = (114, 151, 219)
        self.borderColor = (0, 29, 84)

    def setPosition(self, position):
        self.position = position

    def setSize(self, size):
        self.background.setSize(size)
        self.border.setSize(size)

    def setBackgroundColor(self, color):
        self.backgroundColor = color

    def setBorderColor(self, color):
        self.borderColor = color

    @property
    def topLeft(self):
        return self.position

    @property
    def topRight(self):
        return self.position[0] + self.background.getWidth, self.position[1]

    @property
    def bottomRight(self):
        return self.position[0] + self.background.getWidth, self.position[1] + self.background.getHeight

    @property
    def bottomLeft(self):
        return self.position[0], self.position[1] + self.background.getHeight

    @property
    def width(self):
        return self.background.getWidth

    @property
    def height(self):
        return self.background.getHeight
    
    @property
    def areaSize(self):
        return self.background.getHeight * self.background.getWidth

    @property
    def polygon(self):
        return Polygon([
            self.topLeft,
            self.topRight,
            self.bottomRight,
            self.bottomLeft
        ])

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

    def render(self, image):
        backgroundGenerator = BoundingBox.convertColor(self.backgroundColor, image.colorMode)
        image.addForm(self.position, self.background, backgroundGenerator)

        borderGenerator = BoundingBox.convertColor(self.borderColor, image.colorMode)
        image.addForm(self.position, self.border, borderGenerator)

    def isOverlapping(self, other):
        if not isinstance(other, BoundingBox):
            raise ValueError(f"Other is not an instance of a bounding box: {other}")

        return Point(other.topLeft).intersects(self.polygon) or \
            Point(other.topRight).intersects(self.polygon) or \
            Point(other.bottomLeft).intersects(self.polygon) or \
            Point(other.bottomRight).intersects(self.polygon) or \
            Point(self.topLeft).intersects(other.polygon) or \
            Point(self.topRight).intersects(other.polygon) or \
            Point(self.bottomLeft).intersects(other.polygon) or \
            Point(self.bottomRight).intersects(other.polygon)
