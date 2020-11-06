import png
import random
import math

from classRectangle import Rectangle

width = 255
height = 255
objSize = 6
backColor = 1
objColor = 0

class Image:
        def __init__(self):
                self.width = 255
                self.height = 255
                self.backColor = 1
                self.colorMode = 'monochrome'
                self.Pixel = []

        def fill(self,color):
                # fill mit color
                pass

        def clear(self):
                # fill mit backColor
                pass

        def setPixel(self,x,y,color):
                # muss egal ob RGB oder ...
                pass

        def save(self,fileName):
                pass

        def addForm(self, offset, form, color):
                # offset soll tubel x,y sein
                # form ist instanz von reectangle erstmal
                # color ist int oder rgb tupel
                pass

class ColorGenerator:
    def __init__(self, color):
        self.color = color
        pass

    def clamp(self, value, min, max):
        if value < min:
            return min

        if value > max:
            return max

        return value

    def lerp(self, alpha, min, max):
        alpha = self.clamp(alpha, 0, 1)
        if isinstance(min, tuple) and isinstance(max, tuple):
            result = []
            for i in range(len(min)):
                result.append(min[i] + ((max[i] - min[i]) * alpha))

            return tuple(result)
        elif not isinstance(min, tuple) and isinstance(max, tuple):
            result = []
            for i in range(len(max)):
                result.append(min + ((max[i] - min) * alpha))

            return tuple(result)
        elif isinstance(min, tuple) and not isinstance(max, tuple):
            result = []
            for i in range(len(min)):
                result.append(min[i] + ((max - min[i]) * alpha))

            return tuple(result)
        else:
            return min[i] + ((max - min) * alpha)

    def scale(self, value, factor):
        if isinstance(value, tuple):
            result = []
            for i in range(len(value)):
                result.append( value[i] * factor )

            return tuple(result)
        else:
            return value * factor

    def __getColor(self, progress):
        return self.color

    def progress(self, current, total):
        return (
            current[0]/total[0],
            current[1]/total[1]
        )

    def getPixel(self, intensity, progress = (1, 1)):
        # intensity [0;1]
        # progress tuple(horizontal, vertical) für gradient
        return self.scale(self.__getColor(progress), intensity)

class Monochrome(ColorGenerator):
    def __init__(self, color):
        ColorGenerator.__init__(self, color)

    def getPixel(self, intensity, progress = (1, 1)):
        return round(ColorGenerator.getPixel(self, intensity, progress))

class Greyscale(ColorGenerator):
    def __init__(self, color):
        ColorGenerator.__init__(self, color)

class Rgb(ColorGenerator):
    def __init__(self, color):
        ColorGenerator.__init__(self, color)

class Gradient(ColorGenerator):
    def __init__(self, color, targetColor):
        ColorGenerator.__init__(self, color)
        self.targetColor = targetColor

class LinearGradient(Gradient):
    def __init__(self, color, targetColor, direction = (1, 1), progressOffset = None, progressScale = None):
        Gradient.__init__(self, color, targetColor)
        self.direction = direction

        minProgressValue = (direction[0] * 0 + direction[1] * 0) / 2
        maxProgressValue = (direction[0] * 1 + direction[1] * 1) / 2
        if progressOffset == None:
            #ensure that progress is starting at zero
            self.progressOffset = -minProgressValue
        else:
            self.progressOffset = progressOffset

        if progressScale == None:
            #ensure that progress can only go from 0 to 1
            scale = maxProgressValue - minProgressValue
            if scale == 0:
                scale = 1
            self.progressScale = 1 / scale
        else:
            self.progressScale = progressScale

    def __getColor(self, progress):
        scaledProgress = (
            progress[0] * self.direction[0],
            progress[1] * self.direction[1]
        )

        avgProgress = ((scaledProgress[0] + scaledProgress[1]) / 2)
        fixedProgress = avgProgress * self.progressScale + self.progressOffset
        print("offset=", self.progressOffset)
        print("scale=", self.progressScale)
        print("progress=", progress)
        print("scaledProgress=", scaledProgress)
        print("avgProgress=", avgProgress)
        print("fixedProgress=", fixedProgress)

        return self.lerp(fixedProgress, self.color, self.targetColor)

    def getPixel(self, intensity, progress = (1, 1)):
        # intensity [0;1]
        # progress tuple(horizontal, vertical) für gradient
        lerpedColor =  ColorGenerator.scale(self, self.__getColor(progress), intensity)
        return (int(lerpedColor[0]), int(lerpedColor[1]), int(lerpedColor[2]))

class LinearHorizontalGradient(LinearGradient):
    def __init__(self, color, targetColor):
        LinearGradient.__init__(self, color, targetColor, (1,0))

class LinearVerticalGradient(LinearGradient):
    def __init__(self, color, targetColor):
        LinearGradient.__init__(self, color, targetColor, (0, 1))

class RadialGradient(Gradient):
    def __init__(self, color, targetColor, center = (0.5, 0.5), progressScale = 1):
        Gradient.__init__(self, color, targetColor)
        self.center = center
        self.progressScale = progressScale

    def __getColor(self, progress):
        direction = (
            self.center[0] - progress[0],
            self.center[1] - progress[1]
        )

        distance = math.sqrt(direction[0]*direction[0] + direction[1]*direction[1]) * self.progressScale
        return self.lerp(distance, self.color, self.targetColor)

    def getPixel(self, intensity, progress = (1, 1)):
        # intensity [0;1]
        # progress tuple(horizontal, vertical) für gradient
        lerpedColor =  ColorGenerator.scale(self, self.__getColor(progress), intensity)
        return (int(lerpedColor[0]), int(lerpedColor[1]), int(lerpedColor[2]))


def generate(fileName,colorMode, colorGenerator):
        img = []

        for y in range(height):
                if(colorMode == 'rgb'):
                        row = ()
                        for x in range(width):
                                row = row + backColor
                else:
                        row = []
                        for x in range(width):
                                row.append(backColor)

                img.append(row)

        offsetX = random.randint(0, (width-objSize))
        offsetY = random.randint(0, (height-objSize))

        rec = Rectangle(objSize)
        l = rec.generate()

        for Pixel in l:
                progress = colorGenerator.progress((Pixel[1], Pixel[0]), (objSize, objSize))
                color = colorGenerator.getPixel(Pixel[2], progress)

                if(colorMode == 'rgb'):
                        #color = ( Pixel[2] * objColor[0] , Pixel[2] * objColor[1] , Pixel[2] * objColor[2] )
                        row = list( img[Pixel[0]+offsetY] )
                        x = ( Pixel[1]+offsetX ) *3
                        row[x] = color[0]
                        row[x+1] = color[1]
                        row[x+2] = color[2]
                        img[Pixel[0]+offsetY] = tuple(row)
                else:
                        #color = int( Pixel[2] )* objColor
                        img[Pixel[0]+offsetY][Pixel[1]+offsetX] = color


        if (colorMode == 'monochrome'):
                with open(fileName, 'wb') as f:
                        w = png.Writer(width, height, greyscale=True, bitdepth=1)
                        w.write(f, img)

        elif (colorMode == 'greyscale'):
                with open(fileName, 'wb') as f:
                        w = png.Writer(width, height, greyscale=True)
                        w.write(f, img)

        elif (colorMode == 'rgb'):
                with open(fileName, 'wb') as f:
                        w = png.Writer(width, height, greyscale=False)
                        w.write(f, img)

        else:
                print("colorMode unknown")

"""
generate('Rectangle_1.png','monochrome', Monochrome(0))


backColor = 80
objColor = 150
generate('Rectangle_2.png','greyscale', Greyscale(150))


backColor = (255,0,0)
objColor = (0,255,0)
generate('Rectangle_3.png','rgb', Rgb((0, 255, 0)))

"""
backColor = (0,0,0)
#generate('Rectangle_4.png','rgb', LinearGradient((255, 0, 0), (0, 255, 0), (1,1)))
#generate('Rectangle_5.png','rgb', RadialGradient((255, 255, 0), (0, 0, 0), (0.5, 0.5), 2))
#generate('Rectangle_6.png','rgb', RadialGradient((255, 0, 255), (0, 0, 255), (0, 0), 0.75))
#generate('Rectangle_7.png','rgb', LinearHorizontalGradient((255, 0, 0), (0, 255, 0)))
#generate('Rectangle_8.png','rgb', LinearVerticalGradient((255, 0, 0), (0, 255, 0)))
#generate('Rectangle_9.png','rgb', LinearGradient((255, 0, 0), (0, 255, 0), (1, 0.2)))
#generate('Rectangle_10.png','rgb', LinearGradient((255, 0, 0), (0, 255, 0), (-1, -1)))

def Choose(cond, t, f):
    if cond:
        return t

    return f

def Lerp(alpha, min, max):
    return min + ((max - min) * alpha)

def Normalize(v):
    l = math.sqrt(v[0]*v[0] + v[1]*v[1])
    if l == 0:
        return v

    return (v[0]/l, v[1]/l)

def Reciprocal(n):
    if n == 0:
        return 1

    return 1 / n

positions = [(0,0), (1,0), (0,1), (1,1)]
directions = [(1,1), (1,0), (0,1), (-1, -1)]

for d in directions:
    print("d=", d)

    minp = (d[0] + 0, d[1] + 0)
    print("minp=", minp)

    maxp = (d[0] + 1, d[1] + 1)
    print("maxp=", maxp)

    for p in positions:
        print("p=", p)

        np = (p[0] + d[0], p[1] + d[1])
        print("np=", np)

        final = (Choose(np[0] < 0, -np[0], np[0]), Choose(np[1] < 0, -np[1], np[1]))
        print("final=", final)

    print("=============================================")

"""

rec = Rectangle(width,height,objWidth,objHeight,backColor,objColor,'Rectangle_1.png','monochrome')
rec.generate()


backColor = 80
objColor = 150
rec2 = Rectangle(width,height,objWidth,objHeight,backColor,objColor,'Rectangle_2.png','greyscale')
rec2.generate()

backColor = (255,0,0)
objColor = (0,255,0)
width = 10
height = 10
objWidth = 2
objHeight = 2
rec3 = Rectangle(width,height,objWidth,objHeight,backColor,objColor,'Rectangle_3.png','rgb')
rec3.generate()

"""
#print("Dies ist ein Test und sollte drei Bilder erstellen")
