class Form:
    def __init__(self, size=40):
        self.__objWidth = size
        self.__objHeight = size
        self.__enabled = True
        self.__renderBoundingBox = False

    @property
    def getWidth(self):
        return self.__objWidth

    @property
    def getHeight(self):
        return self.__objHeight

    def setSize(self, size):
        self.__objWidth = size
        self.__objHeight = size

    def enable(self):
        self.__enabled = True

    def disable(self):
        self.__enabled = False

    def isEnabled(self):
        return self.__enabled

    @property
    def renderBoundingBox(self):
        return self.__renderBoundingBox

    @renderBoundingBox.setter
    def renderBoundingBox(self, value):
        self.__renderBoundingBox = value

