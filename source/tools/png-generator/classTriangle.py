from shapely.geometry import Point, Polygon

class Triangle:

    def __init__(self, size=40):

        self.__objWidth = size
        self.__objHeight = size
        
        

    def generate(self):

        objList = []
        
        coords = [( (self.__objWidth/2) -0.5 , -0.5), ( -0.5 , self.__objHeight ), ( self.__objWidth , self.__objHeight )]
        poly = Polygon(coords)

        for y in range(self.__objHeight):
            for x in range(self.__objWidth):
            	p = Point(x,y)
            	
            	if ( p.within(poly) ):
            		objList.append((x, y, 1))
            	else:
            		objList.append((x, y, 0))

        return objList



    def getWidth(self):
        return self.__objWidth



    def getHeight(self):
        return self.__objHeight
