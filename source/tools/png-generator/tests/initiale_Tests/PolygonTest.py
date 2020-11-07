from shapely.geometry import Point, Polygon

# Create Point objects
p1 = Point(2, 2)
p2 = Point(2, 4)

# Create a Polygon
coords = [(1, 1), (1, 3), (3,3), (3, 1)]
poly = Polygon(coords)


print(p1)
print(p2)
print(poly)

if( p1.within(poly) ):
	print("p1 in")
else:
	print("p1 out")

if( p2.within(poly) ):
	print("p2 in")
else:
	print("p2 out")
