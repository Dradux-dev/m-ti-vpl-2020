import png
import random

from classRectangle import Rectangle

width = 255
height = 255
objWidth = 40
objHeight = 40
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

	offsetX = random.randint(0, (width-objWidth))
	offsetY = random.randint(0, (height-objHeight))
		
	rec = Rectangle(objWidth,objHeight)
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

generate('Rectangle_1.png','monochrome')

backColor = 80
objColor = 150
generate('Rectangle_2.png','greyscale')


backColor = (255,0,0)
objColor = (0,255,0)
generate('Rectangle_3.png','rgb')

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
