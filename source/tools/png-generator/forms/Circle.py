import math

from forms.Form import Form


class Circle(Form):
    def __init__(self, size=40):
        Form.__init__(self, size)
        self.__radius = size / 2

    def setSize(self, size):
        Form.setSize(self, size)
        self.__radius = size / 2

    def generate(self):
        objList = []

        middleX = (self.getWidth / 2) - 0.5
        middleY = (self.getHeight / 2) - 0.5

        for y in range(self.getHeight):
            for x in range(self.getWidth):
                distance = math.sqrt(pow(middleY - y, 2) + pow(middleX - x, 2))

                if distance <= self.__radius:
                    objList.append((x, y, 1))

        return objList
