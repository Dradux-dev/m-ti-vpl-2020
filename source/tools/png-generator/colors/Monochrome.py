from colors.ColorGenerator import ColorGenerator

class Monochrome(ColorGenerator):
    def __init__(self):
        ColorGenerator.__init__(self)

    def getPixel(self, intensity, progress=(1, 1)):
        return round(ColorGenerator.getPixel(self, intensity, progress))

    @staticmethod
    def getColorMode():
        return "monochrome"
