import png
import sys

from Color import Color


class Image:
    """
    @brief Image

    Image class to create PNG files

    @author Philipp Koopke
    """
    
    def __init__(self, width=255, height=255, colorMode=Color.Mode.RGB, backColor=Color(0, 0, 0)):
    	"""
        @brief Constructor

        Constructs the image with width of 255 Pixels, height of 255, colorMode in RGB and a black background Color

        @param width sets the width of the image
        @param height sets the height of the image
        @param colorMode sets the colorMode of the image, can be monochrome, greyscale or RGB
        @param backColor sets the backgrond color of the image

        @author Philipp Koopke
        """
        
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
        """
        @brief width

        returns the width of the image

        @author Philipp Koopke
        """
        
        return self.__width

    @property
    def height(self):
        """
        @brief height

        returns the height of the image

        @author Philipp Koopke
        """
        
        return self.__height

    @property
    def colorMode(self):
        """
        @brief colorMode

        returns the colorMode of the image

        @author Philipp Koopke
        """
        
        return self.__colorMode

    def fill(self, color):
        """
        @brief fill

        fills every pixel of the Image with a color 
        
        @param color is the color, which will be used to fill the Image 

        @author Philipp Koopke
        """
        
        self.pixels = []

        for y in range(self.height):
            row = []
            for x in range(self.width):
                row.append(Color.SwapMode(color, self.colorMode))

            self.pixels.append(row)

    def clear(self):
        """
        @brief clear

        fills the Image with the backgrundcolor

        @author Philipp Koopke
        """
        
        self.fill(self.backColor)

    def setPixel(self, x, y, color):
        """
        @brief setPixel

        set one pixel of the Image
        
        @param x is the x coord of the pixel
        @param y is the y coord of the pixel
        @param color is the color of the pixel 

        @author Philipp Koopke
        """
        
        if x not in range(self.width) or y not in range(self.height):
            print(f"Pixel ({x}, {y}) is not in range({self.width}, {self.height}).")
            return

        self.pixels[y][x] = Color.SwapMode(color, self.colorMode)

    @staticmethod
    def __newrow(mode):
        """
        @brief __newrow

        create a row of pixels 
        
        @param mode ist the color mode for the row

        @author ???
        """
        
        if mode == Color.Mode.MONOCHROME or mode == Color.Mode.GREYSCALE:
            return []

        return ()

    @staticmethod
    def __newpng(width, height, mode):
        """
        @brief __newpng

        create an image with width, height and color mode
        
        @param width is the width of the image
        @param height is the heigth the image
        @param mode is the color mode of the image

        @author ????
        """
        
        if mode == Color.Mode.MONOCHROME:
            return png.Writer(width, height, greyscale=True, bitdepth=1)
        elif mode == Color.Mode.GREYSCALE:
            return png.Writer(width, height, greyscale=True)

        return png.Writer(width, height, greyscale=False)

    def save(self, fileName):
       """
        @brief save

        save the pixels as PNG file with the fileName
        
        @param fileName is the name of the PNG file

        @author Philipp Koopke
        """
    
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
        """
        @brief addForm

        add a form to the picture at Position offset with the color, from the color generator
        
        @param offset is the position, where to insert the form
        @param form is the form to be added to the image
        @param generator ist the color generator, which should be used

        @author Philipp Koopke and Tarek Schwarzinger
        """
    
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

    def __str__(self):
        """
        @brief __str__

        ???
        
        @author ????
        """
        
        return f"Image(width: {self.width}, height:{self.height}, colorMode: {Color.ModeString(self.colorMode)})"
