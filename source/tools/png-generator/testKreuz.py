import random

from classKreuz import Kreuz
from classImage import Image

width = 255
height = 255
objSize = 80
backColor = 1
objColor = 0


# Image Klasse
kreuz = Kreuz(objSize)

image = Image(width,height,backColor,'monochrome')
image.clear()

offsetX = random.randint(0, (width-objSize-1))
offsetY = random.randint(0, (height-objSize-1))
offset = (offsetX,offsetY)

image.addForm(offset,kreuz,0)
image.save('Kreuz_1.png')



backColor = 80
objColor = 150
kreuz2 = Kreuz(objSize)

image2 = Image(width,height,backColor,'greyscale')
image2.clear()

offsetX = random.randint(0, (width-objSize-1))
offsetY = random.randint(0, (height-objSize-1))
offset = (offsetX,offsetY)

image2.addForm(offset,kreuz2,objColor)
image2.save('Kreuz_2.png')


backColor = (255,0,0)
objColor = (0,255,0)
kreuz3 = Kreuz(objSize)

image3 = Image(width,height,backColor,'rgb')
image3.clear()

offsetX = random.randint(0, (width-objSize-1))
offsetY = random.randint(0, (height-objSize-1))
offset = (offsetX,offsetY)

image3.addForm(offset,kreuz3,objColor)
image3.save('Kreuz_3.png')

