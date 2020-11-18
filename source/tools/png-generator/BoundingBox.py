from shapely.geometry import Point, Polygon

from forms.Rectangle import Rectangle
from DashedBorder import DashedBorder
from colors.Flat import Flat
from Color import Color


class BoundingBox:
    """
    @brief Bounding Box

    Bounding box for every single generated from. This is able to be rendered on demand and provides functionality
    to check, if it is overlapping with another bounding box.

    @author Tarek Schwarzinger
    """

    def __init__(self, position=(0, 0), size=40):
        """
        @brief Constructor

        Constructs the bounding box at position P(0,0) and with a size of 40.

        @param position The position of the bounding box as tuple[int, int].
        @param size The size of the bounding box as int.

        @author Tarek Schwarzinger
        """
        self.background = Rectangle(size)
        self.border = DashedBorder(size)
        self.position = position
        self.backgroundColor = Color(114, 151, 219)
        self.borderColor = Color(0, 29, 84)

    def setPosition(self, position):
        """
        @brief Set new position

        Moves the bounding box to a new position.

        @param position The new position of the bounding box as tuple[int, int].

        :author Tarek Schwarzinger
        """
        self.position = position

    def setSize(self, size):
        """
        @brief Adjust size

        Adjusts the size of the bounding box to fit its new size.

        @param size The new size of the bounding box as int.

        @author Tarek Schwarzinger
        """
        self.background.setSize(size)
        self.border.setSize(size)

    def setBackgroundColor(self, color):
        """
        @brief Set background color

        Sets a new background color for this bounding box, in case it gets rendered.

        @param color The new background color of the bounding box as Color.

        @author Tarek Schwarzinger
        """
        self.backgroundColor = color

    def setBorderColor(self, color):
        """
        @brief Set border color

        Sets a new border color for this bounding box, in case it gets rendered.

        @param color The new border color of the bounding box as Color.

        @author Tarek Schwarzinger
        """
        self.borderColor = color

    @property
    def topLeft(self):
        """
        @brief Top left corner

        @return Returns the position of the top left corner of the bounding box as tuple[int, int].

        @author Tarek Schwarzinger
        """
        return self.position

    @property
    def topRight(self):
        """
        @brief Top right corner

        @return Returns the position of the top right corner of the bounding box as tuple[int, int].

        @author Tarek Schwarzinger
        """
        return self.position[0] + self.background.getWidth, self.position[1]

    @property
    def bottomRight(self):
        """
        @brief Bottom right corner

        @return Returns the position of the bottom right corner of the bounding box as tuple[int, int].

        @author Tarek Schwarzinger
        """
        return self.position[0] + self.background.getWidth, self.position[1] + self.background.getHeight

    @property
    def bottomLeft(self):
        """
        @brief Bottom left corner

        @return Returns the position of the bottom left corner of the bounding box as tuple[int, int].

        @author Tarek Schwarzinger
        """
        return self.position[0], self.position[1] + self.background.getHeight

    @property
    def width(self):
        """
        @brief Width

        @return Returns the width of the bounding box as int.

        @author Tarek Schwarzinger
        """
        return self.background.getWidth

    @property
    def height(self):
        """
        @brief Height

        @return Returns the height of the bounding box as int.

        @author Tarek Schwarzinger
        """
        return self.background.getHeight
    
    @property
    def areaSize(self):
        """
        @brief Size

        @return Returns the size of the bounding box (width*height).

        @author Philipp Koopke
        """
        return self.background.getHeight * self.background.getWidth

    @property
    def polygon(self):
        """
        @brief Polygon

        @return Returns the polygon that describes the bounding box.

        @author Tarek Schwarzinger
        """
        return Polygon([
            self.topLeft,
            self.topRight,
            self.bottomRight,
            self.bottomLeft
        ])

    @staticmethod
    def convertColor(color, colorMode):
        """
        @brief Color to Color Generator: Flat

        Creates a flat color generator for the bounding box.

        @param color The color that shall be used for the flat color generator.
        @param colorMode The color mode that is required for the bounding box, to fit the color mode of the image.

        @return Returns the created flat color generator.

        @author Tarek Schwarzinger
        """
        generator = Flat()
        generator.addColor(Color.SwapMode(color, colorMode))
        return generator

    def render(self, image):
        """
        @brief Render

        Renders the bounding box to the provided image.

        @param image The image that the bounding box shall be rendered in.

        @author Tarek Schwarzinger
        """
        backgroundGenerator = BoundingBox.convertColor(self.backgroundColor, image.colorMode)
        image.addForm(self.position, self.background, backgroundGenerator)

        borderGenerator = BoundingBox.convertColor(self.borderColor, image.colorMode)
        image.addForm(self.position, self.border, borderGenerator)

    def isOverlapping(self, other):
        """
        @brief Is overlapping

        Checks, if this bounding box is overlapping with the \p other bounding box.

        @param other The other bounding box.

        @return Returns True, if the bounding boxes are overlapping, otherwise False is returned.

        @attention If \p other is not an instance of BoundingBox a ValueError will be raised.

        @author Tarek Schwarzinger
        """
        if not isinstance(other, BoundingBox):
            raise ValueError(f"Other is not an instance of a bounding box: {other}")

        return self.polygon.intersects(other.polygon)

    def __str__(self):
        """
        @brief String conversion

        @return Converts the bounding box to a string representation.

        @author Tarek Schwarzinger
        """
        return f"BoundingBox(position:{self.topLeft}, width:{self.width}, height:{self.height}, size:{self.areaSize})"
