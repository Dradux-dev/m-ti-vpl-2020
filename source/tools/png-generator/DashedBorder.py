from forms.PolygonForm import PolygonForm


class DashedBorder(PolygonForm):
    def __init__(self, size=40, thickness=4, length=10, margin=2):
        PolygonForm.__init__(self, size)
        self.__thickness = thickness
        self.__length = length
        self.__margin = margin
        self.coords = None

        self.__updateCoords()

    def __addRect(self, topLeft: tuple, bottomRight: tuple) -> None:
        top = topLeft[1] / self.getHeight
        left = topLeft[0] / self.getWidth
        bottom = bottomRight[1] / self.getHeight
        right = bottomRight[0] / self.getWidth
        self.coords.append([(left, top), (right, top), (right, bottom), (left, bottom)])

    def __addCorners(self):
        # Top Left
        self.__addRect((0, 0), (self.__length, self.__thickness))
        self.__addRect((0, 0), (self.__thickness, self.__length))

        # Top Right
        self.__addRect((self.getWidth-self.__length, 0), (self.getWidth, self.__thickness))
        self.__addRect((self.getWidth-self.__thickness, 0), (self.getWidth, self.__length))

        # Bottom Left
        self.__addRect((0, self.getHeight-self.__thickness), (self.__length, self.getHeight))
        self.__addRect((0, self.getHeight-self.__length), (self.__thickness, self.getHeight))

        # Bottom Right
        self.__addRect((self.getWidth-self.__length, self.getHeight-self.__thickness), (self.getWidth, self.getHeight))
        self.__addRect((self.getWidth-self.__thickness, self.getHeight-self.__length), (self.getWidth, self.getHeight))

    def __addHorizontalLine(self, xmin, xmax, y):
        for x in range(xmin, xmax, self.__length+self.__margin):
            length = min(self.__length, xmax-x)
            self.__addRect((x, y), (x+length, y+self.__thickness))

    def __addVerticalLine(self, ymin, ymax, x):
        for y in range(ymin, ymax, self.__length+self.__margin):
            length = min(self.__length, ymax-y)
            self.__addRect((x, y), (x+self.__thickness, y+length))

    def __updateCoords(self):
        self.coords = []

        self.__addCorners()

        unitLength = self.__length + self.__margin + 1

        # Top Border
        self.__addHorizontalLine(unitLength, self.getWidth-unitLength, 0)

        # Bottom Border
        self.__addHorizontalLine(unitLength, self.getWidth - unitLength, self.getHeight - self.__thickness)

        # Left Border
        self.__addVerticalLine(unitLength, self.getHeight - unitLength, 0)

        # Right Border
        self.__addVerticalLine(unitLength, self.getHeight - unitLength, self.getWidth - self.__thickness)

    def setSize(self, size):
        PolygonForm.setSize(self, size)
        self.__updateCoords()

    def generate(self):
        return PolygonForm.generateMultiplePolygons(self, self.coords)
