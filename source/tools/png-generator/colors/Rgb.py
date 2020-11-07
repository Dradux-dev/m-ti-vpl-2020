from colors.ColorGenerator import ColorGenerator


class Rgb(ColorGenerator):
    def __init__(self):
        ColorGenerator.__init__(self)

    @staticmethod
    def getColorMode():
        return "rgb"
