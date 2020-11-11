import png
import sys

from Color import Color


class Image:
    def __init__(self, width=255, height=255, colorMode=Color.Mode.RGB, backColor=Color(0, 0, 0)):
        self.__width = width
        self.__height = height
        self.__colorMode = colorMode

        self.backColor = backColor

        self.pixels = []
        self.clear()

        if self.colorMode not in [Color.Mode.MONOCHROME, Color.Mode.GREYSCALE, Color.Mode.RGB]:
            raise ValueError(f"Unknown color mode: {self.colorMode}")

    @property
    def width(self):
        return self.__width

    @property
    def height(self):
        return self.__height

    @property
    def colorMode(self):
        return self.__colorMode

    def fill(self, color):
        self.pixels = []

        for y in range(self.height):
            row = []
            for x in range(self.width):
                row.append(Color.SwapMode(color, self.colorMode))

            self.pixels.append(row)

    def clear(self):
        self.fill(self.backColor)

    def setPixel(self, x, y, color):
        if x not in range(self.width) or y not in range(self.height):
            print(f"Pixel ({x}, {y}) is not in range({self.width}, {self.height}).")
            return

        self.pixels[y][x] = Color.SwapMode(color, self.colorMode)

    @staticmethod
    def __newrow(mode):
        if mode == Color.Mode.MONOCHROME or mode == Color.Mode.GREYSCALE:
            return []

        return ()

    @staticmethod
    def __newpng(width, height, mode):
        if mode == Color.Mode.MONOCHROME:
            return png.Writer(width, height, greyscale=True, bitdepth=1)
        elif mode == Color.Mode.GREYSCALE:
            return png.Writer(width, height, greyscale=True)

        return png.Writer(width, height, greyscale=False)

    def save(self, fileName):
        rawPixels = []
        for y in range(self.height):
            row = Image.__newrow(self.colorMode)

            for x in range(self.width):
                row = row + self.pixels[y][x].appendValue

            rawPixels.append(row)

        with open(fileName, 'wb') as f:
            w = Image.__newpng(self.width, self.height, self.colorMode)
            w.write(f, rawPixels)

    def addForm(self, offset, form, generator):
        objPixelList = form.generate()

        for pixel in objPixelList:
            pos = (pixel[0], pixel[1])
            dimension = (form.getWidth, form.getHeight)
            progress = generator.progress(pos, dimension)

            self.setPixel(
                x=pixel[0] + offset[0],
                y=pixel[1] + offset[1],
                color=generator.getPixel(pixel[2], progress)
            )
