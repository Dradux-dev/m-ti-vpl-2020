import png
import random

from classCircle import Circle
from classImage import Image

width = 255
height = 255
objSize = 80
backColor = 1
objColor = 0


# Image Klasse
circle = Circle(objSize)

image = Image(width,height,backColor,'monochrome')
image.clear()

offsetX = random.randint(0, (width-objSize-1))
offsetY = random.randint(0, (height-objSize-1))
offset = (offsetX,offsetY)

image.addForm(offset,circle,0)
image.save('Circle_1.png')



backColor = 80
objColor = 150
circle2 = Circle(objSize)

image2 = Image(width,height,backColor,'greyscale')
image2.clear()

offsetX = random.randint(0, (width-objSize-1))
offsetY = random.randint(0, (height-objSize-1))
offset = (offsetX,offsetY)

image2.addForm(offset,circle2,objColor)
image2.save('Circle_2.png')


backColor = (255,0,0)
objColor = (0,255,0)
circle3 = Circle(objSize)

image3 = Image(width,height,backColor,'rgb')
image3.clear()

offsetX = random.randint(0, (width-objSize-1))
offsetY = random.randint(0, (height-objSize-1))
offset = (offsetX,offsetY)

image3.addForm(offset,circle3,objColor)
image3.save('Circle_3.png')
print("Test")
