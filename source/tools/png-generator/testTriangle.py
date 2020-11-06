import random

from classTriangle import Triangle
from classImage import Image

width = 255
height = 255
objSize = 80
backColor = 1
objColor = 0


# Image Klasse
tri = Triangle(objSize)

image = Image(width,height,backColor,'monochrome')
image.clear()

offsetX = random.randint(0, (width-objSize-1))
offsetY = random.randint(0, (height-objSize-1))
offset = (offsetX,offsetY)

image.addForm(offset,tri,0)
image.save('Triangle_1.png')



backColor = 80
objColor = 150
tri2 = Triangle(objSize)

image2 = Image(width,height,backColor,'greyscale')
image2.clear()

offsetX = random.randint(0, (width-objSize-1))
offsetY = random.randint(0, (height-objSize-1))
offset = (offsetX,offsetY)

image2.addForm(offset,tri2,objColor)
image2.save('Triangle_2.png')


backColor = (255,0,0)
objColor = (0,255,0)
tri3 = Triangle(objSize)

image3 = Image(width,height,backColor,'rgb')
image3.clear()

offsetX = random.randint(0, (width-objSize-1))
offsetY = random.randint(0, (height-objSize-1))
offset = (offsetX,offsetY)

image3.addForm(offset,tri3,objColor)
image3.save('Triangle_3.png')

