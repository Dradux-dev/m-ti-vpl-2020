import random

from classPlus import Plus
from classImage import Image

width = 255
height = 255
objSize = 80
backColor = 1
objColor = 0


# Image Klasse
plus = Plus(objSize)

image = Image(width,height,backColor,'monochrome')
image.clear()

offsetX = random.randint(0, (width-objSize-1))
offsetY = random.randint(0, (height-objSize-1))
offset = (offsetX,offsetY)

image.addForm(offset,plus,0)
image.save('Plus_1.png')



backColor = 80
objColor = 150
plus2 = Plus(objSize)

image2 = Image(width,height,backColor,'greyscale')
image2.clear()

offsetX = random.randint(0, (width-objSize-1))
offsetY = random.randint(0, (height-objSize-1))
offset = (offsetX,offsetY)

image2.addForm(offset,plus2,objColor)
image2.save('Plus_2.png')


backColor = (255,0,0)
objColor = (0,255,0)
plus3 = Plus(objSize)

image3 = Image(width,height,backColor,'rgb')
image3.clear()

offsetX = random.randint(0, (width-objSize-1))
offsetY = random.randint(0, (height-objSize-1))
offset = (offsetX,offsetY)

image3.addForm(offset,plus3,objColor)
image3.save('Plus_3.png')

