import png
import sys

class Image:

	def __init__(self,width=255,height=255,backColor = 1,colorMode = 'monochrome'):
	
		self.width = width
		self.height = height
		self.backColor = backColor
		self.colorMode = colorMode
		
		self.Pixel = []
		self.clear()
		self.isFilled = 0
		
		if (self.colorMode != 'monochrome' and self.colorMode != 'greyscale' and self.colorMode != 'rgb'):
			print("colorMode unknown:" + str(self.colorMode) )
			sys.exit(1)
			
		if ( self.checkColor(self.backColor) == 0 ):
			print("backColor passt nicht in Farbraum" + str(self.backColor) )
			sys.exit(1)
	
	
	
	def fill(self,color):
		# fill mit color
		
		self.Pixel = []
		
		for y in range(self.height):
			if(self.colorMode == 'rgb'):
				row = ()
				for x in range(self.width):
					row = row + color
			else:
				row = []
				for x in range(self.width):
					row.append(color)
				
			self.Pixel.append(row)
			
		self.isFilled = 1
	
	
	
	def clear(self):
		# fill mit backColor
		
		self.fill(self.backColor)
	
	
	
	def setPixel(self,x,y,color):
		# muss egal ob RGB oder ... 
		
		if ( x not in range(self.width)  or  y not in range(self.height) ):
			print("Pixel not in range")
			return
			
		if ( self.checkColor(color) == 1):
		
			if(self.colorMode == 'rgb'):
				row = list( self.Pixel[y] )
				xPos = ( x ) *3
				row[xPos] = color[0] 
				row[xPos+1] = color[1] 
				row[xPos+2] = color[2] 
				self.Pixel[y] = tuple(row)
			else:
				self.Pixel[y][x] = color
		else:
			print("color passt nicht in Farbraum" + str(color) ) 
		
	
		
	def save(self,fileName):
	
		if ( self.isFilled == 1 ):
			if (self.colorMode == 'monochrome'):
				with open(fileName, 'wb') as f:
					w = png.Writer(self.width, self.height, greyscale=True, bitdepth=1)
					w.write(f, self.Pixel)
						
			elif (self.colorMode == 'greyscale'):
				with open(fileName, 'wb') as f:
					w = png.Writer(self.width, self.height, greyscale=True)
					w.write(f, self.Pixel)
					
			elif (self.colorMode == 'rgb'):
				with open(fileName, 'wb') as f:
					w = png.Writer(self.width, self.height, greyscale=False)
					w.write(f, self.Pixel)
		else:
			print("keine Daten zum Bild erstellen")
				
	
		
	def addForm(self, offset, form, color):
		# offset soll tupel x,y sein
		# form ist instanz von reectangle erstmal
		# color ist int oder rgb tupel
		
		objPixelList = form.generate()
			
		for Pixel in objPixelList:
				
			if(self.colorMode == 'rgb'):
				pixColor = ( color[0] , color[1] , color[2] )
						
			else: 
				pixColor = color				
			
			self.setPixel( Pixel[0] + offset[0] , Pixel[1] + offset[1], pixColor )
		
	
	
	def fitForm(self, offset, form):
		# diese Funktion soll testen, ob eine Form in ein Bild passt
		# im erfolg soll 1 zurückgegeben werden

		fit = 0		
		offsetX = offset[0]
		offsetY = offset[1]
		
		objWidth = form.getWidth
		objHeigth = form.getHeight

		if( offsetX >= 0  and  offsetY >= 0  and  (offsetX + objWidth) <= self.width  and  (offsetY + objHeigth) <= self.height ):
			fit = 1
		
		return fit
		
	
		
	def checkColor(self,color):
		# diese Funktion soll testen, ob die angegeben Farbe in den hinterlegten colorMode passt
		# im erfolg soll 1 zurückgegeben werden
		
		fit = 0
		
		if (self.colorMode == 'monochrome'):
			if( color == 0 or color == 1 ):
				fit = 1
		elif (self.colorMode == 'greyscale'):
			if( color >= 0 and color <= 255 ):
				fit = 1
		elif (self.colorMode == 'rgb'):
			if( color[0] >= 0 and color[0] <= 255   and  color[1] >= 0 and color[1] <= 255 and  color[2] >= 0 and color[2] <= 255 ):
				fit = 1
		
		return fit
