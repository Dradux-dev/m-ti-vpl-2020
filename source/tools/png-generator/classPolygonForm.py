from shapely.geometry import Point, Polygon


class PolygonForm:

	def __init__(self, size=40):
		
		self.__objWidth = size
		self.__objHeight = size
		
		
	def generateMultiplePolygons(self,templates):
		
			
		objList = []
		polys = []
		
		for template in templates:
			coords = []
			for p in template:
				coords.append( ( p[0] * self.__objWidth, p[1] * self.__objHeight ) )
			
			polys.append( Polygon(coords) )

		for y in range(self.__objHeight):
			for x in range(self.__objWidth):
				p = Point(x,y)
				inside = [ int (p.within(poly) ) for poly in polys ]
				
				if ( sum(inside) >= 1 ):
					objList.append((x, y, 1))

		return objList
	
	
	def generatePolygon(self,template):

		return self.generateMultiplePolygons( [template] )
		
		
	def getWidth(self):
		return self.__objWidth



	def getHeight(self):
		return self.__objHeight

