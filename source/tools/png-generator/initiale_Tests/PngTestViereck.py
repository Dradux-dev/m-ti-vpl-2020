import png
import random

width = 255
height = 255
objWidth = 40
objHeight = 40
backColor = 1
objColor = 0
img = []

for y in range(height):
	row = []
	for x in range(width):
		row.append(backColor)
	img.append(row)

offsetX = random.randint(0, (width-objWidth))
offsetY = random.randint(0, (height-objHeight))
	
for y in range(objHeight):
	for x in range(objWidth):
		img[y+offsetY][x+offsetX]= objColor 
	
with open('viereck.png', 'wb') as f:
	w = png.Writer(width, height, greyscale=True, bitdepth=1)
	w.write(f, img)
	
print("Dies ist ein Test und sollte ein Bild erstellen")
