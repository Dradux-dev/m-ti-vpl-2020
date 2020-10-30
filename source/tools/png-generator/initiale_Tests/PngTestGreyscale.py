import png

width = 255
height = 255
img = []
for y in range(height):
	row = []
	for x in range(width):
		#row = row + tuple(y) #(y,y,y)
		row.append(y)
	img.append(row)
with open('grey.png', 'wb') as f:
	w = png.Writer(width, height, greyscale=True)
	w.write(f, img)
	
print("Dies ist ein Test und sollte ein Bild erstellen")
