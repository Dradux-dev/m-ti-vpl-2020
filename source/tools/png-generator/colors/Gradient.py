from colors.ColorGenerator import ColorGenerator


class Gradient(ColorGenerator):
    def __init__(self):
        ColorGenerator.__init__(self)

    @staticmethod
    def getColorMode():
        return "rgb"

    @staticmethod
    def colorsRequired():
        return 2