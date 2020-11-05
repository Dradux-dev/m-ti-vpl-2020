import math

class Circle:

    def __init__(self, size=40):

        self.__objWidth = size
        self.__objHeight = size
        self.__radius = size / 2

    def generate(self):

        objList = []

        middleX = (self.__objWidth / 2) - 0.5
        middleY = (self.__objHeight / 2) - 0.5

        for y in range(self.__objHeight):
            for x in range(self.__objWidth):
                distance = math.sqrt(pow(middleY-y, 2) + pow(middleX-x,2))
                
                if (distance <= self.__radius):
                    objList.append((x, y, 1))
                else:
                    objList.append((x, y, 0))

        return objList

    def getWidth(self):
        return self.__objWidth

    def getHeight(self):
        return self.__objHeight
