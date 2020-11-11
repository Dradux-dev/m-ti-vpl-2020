from shapely.geometry import Point, Polygon

from forms.Rectangle import Rectangle
from DashedBorder import DashedBorder
from colors.Flat import Flat
from Color import Color


class BoundingBox:
    def __init__(self, position=(0, 0), size=40):
        self.background = Rectangle(size)
        self.border = DashedBorder(size)
        self.position = position
        self.backgroundColor = Color(114, 151, 219)
        self.borderColor = Color(0, 29, 84)

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
    def convertColor(color, colorMode):
        generator = Flat()
        generator.addColor(Color.SwapMode(color, colorMode))
        return generator

    def render(self, image):
        backgroundGenerator = BoundingBox.convertColor(self.backgroundColor, image.colorMode)
        image.addForm(self.position, self.background, backgroundGenerator)

        borderGenerator = BoundingBox.convertColor(self.borderColor, image.colorMode)
        image.addForm(self.position, self.border, borderGenerator)

    def isOverlapping(self, other):
        if not isinstance(other, BoundingBox):
            raise ValueError(f"Other is not an instance of a bounding box: {other}")

        return self.polygon.intersects(other.polygon)
