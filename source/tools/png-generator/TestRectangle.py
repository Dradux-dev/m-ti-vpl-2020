import png
import random

from classRectangle import Rectangle
from classImage import Image

width = 255
height = 255
objSize = 80
backColor = 1
objColor = 0



def generate(fileName,colorMode):	
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
		if(colorMode == 'rgb'):
			color = ( Pixel[2] * objColor[0] , Pixel[2] * objColor[1] , Pixel[2] * objColor[2] )
			row = list( img[Pixel[0]+offsetY] )
			x = ( Pixel[1]+offsetX ) *3
			row[x] = color[0] 
			row[x+1] = color[1] 
			row[x+2] = color[2] 
			img[Pixel[0]+offsetY] = tuple(row)
		else:
			color = int( Pixel[2] )* objColor
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




# Image Klasse
rec = Rectangle(objSize)

image = Image(width,height,backColor,'monochrome')
image.clear()

offsetX = random.randint(0, (width-objSize-1))
offsetY = random.randint(0, (height-objSize-1))
offset = (offsetX,offsetY)

image.addForm(offset,rec,0)
image.save('Rectangle_1.png')



backColor = 80
objColor = 150
rec2 = Rectangle(objSize)

image2 = Image(width,height,backColor,'greyscale')
image2.clear()

offsetX = random.randint(0, (width-objSize-1))
offsetY = random.randint(0, (height-objSize-1))
offset = (offsetX,offsetY)

image2.addForm(offset,rec2,objColor)
image2.save('Rectangle_2.png')


backColor = (255,0,0)
objColor = (0,255,0)
rec3 = Rectangle(objSize)

image3 = Image(width,height,backColor,'rgb')
image3.clear()

offsetX = random.randint(0, (width-objSize-1))
offsetY = random.randint(0, (height-objSize-1))
offset = (offsetX,offsetY)

image3.addForm(offset,rec3,objColor)
image3.save('Rectangle_3.png')


# generate funktion
"""
generate('Rectangle_1.png','monochrome')

backColor = 80
objColor = 150
generate('Rectangle_2.png','greyscale')


backColor = (255,0,0)
objColor = (0,255,0)
generate('Rectangle_3.png','rgb')

"""

#alte Rectangle Klasse
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
