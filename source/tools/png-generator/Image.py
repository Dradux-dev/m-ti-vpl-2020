import png
import sys


class Image:
    def __init__(self, width=255, height=255, backColor=(0, 0, 0), colorMode='rgb'):
        self.width = width
        self.height = height
        self.backColor = backColor
        self.colorMode = colorMode

        self.Pixel = []
        self.clear()

        if self.colorMode != 'monochrome' and self.colorMode != 'greyscale' and self.colorMode != 'rgb':
            print(f"Unknown color mode: {self.colorMode}")
            sys.exit(1)

        if self.checkColor(self.backColor) == 0:
            print(f"Background color {self.backColor} does not match color space {self.colorMode}")
            sys.exit(1)

    def fill(self, color):
        self.Pixel = []

        for y in range(self.height):
            if self.colorMode == 'rgb':
                row = ()
                for x in range(self.width):
                    row = row + color
            else:
                row = []
                for x in range(self.width):
                    row.append(color)

            self.Pixel.append(row)

    def clear(self):
        self.fill(self.backColor)

    def setPixel(self, x, y, color):
        if x not in range(self.width) or y not in range(self.height):
            print(f"Pixel ({x}, {y}) is not in range({self.width}, {self.height}).")
            return

        if self.checkColor(color) == 1:
            if self.colorMode == 'rgb':
                row = list(self.Pixel[y])
                xPos = x * 3
                row[xPos] = color[0]
                row[xPos + 1] = color[1]
                row[xPos + 2] = color[2]
                self.Pixel[y] = tuple(row)
            else:
                self.Pixel[y][x] = color
        else:
            print(f"Color {color} does not match color space {self.colorMode}")

    def save(self, fileName):
        if self.colorMode == 'monochrome':
            with open(fileName, 'wb') as f:
                w = png.Writer(self.width, self.height, greyscale=True, bitdepth=1)
                w.write(f, self.Pixel)

        elif self.colorMode == 'greyscale':
            with open(fileName, 'wb') as f:
                w = png.Writer(self.width, self.height, greyscale=True)
                w.write(f, self.Pixel)

        elif self.colorMode == 'rgb':
            with open(fileName, 'wb') as f:
                w = png.Writer(self.width, self.height, greyscale=False)
                w.write(f, self.Pixel)

    def addForm(self, offset, form, color):
        objPixelList = form.generate()

        """@todo swap color to generator"""
        for Pixel in objPixelList:
            self.setPixel(Pixel[0] + offset[0], Pixel[1] + offset[1], color)

    def fitForm(self, offset, form):
        # diese Funktion soll testen, ob eine Form in ein Bild passt
        # im erfolg soll 1 zurückgegeben werden

        offsetX = offset[0]
        offsetY = offset[1]

        objWidth = form.getWidth
        objHeight = form.getHeight

        return int(offsetX >= 0 and
                   offsetY >= 0 and
                   (offsetX + objWidth) <= self.width and
                   (offsetY + objHeight) <= self.height)

    def checkColor(self, color):
        # diese Funktion soll testen, ob die angegeben Farbe in den hinterlegten colorMode passt
        # im erfolg soll 1 zurückgegeben werden
        if self.colorMode == 'monochrome':
            return int(color == 0 or color == 1)
        elif self.colorMode == 'greyscale':
            return int(color in range(256))
        elif self.colorMode == 'rgb':
            return int(color[0] in range(256) and
                       color[1] in range(256) and
                       color[2] in range(256))

        return 0
