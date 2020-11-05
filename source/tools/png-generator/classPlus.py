from shapely.geometry import Point, Polygon

class Plus:

    def __init__(self, size=40):

        self.__objWidth = size
        self.__objHeight = size
        self.__thickness = 0.3*size
        
        

    def generate(self):

        objList = []
        
        middleX = (self.__objWidth / 2) - 0.5
        middleY = (self.__objHeight / 2) - 0.5
        halfThickness = self.__thickness /2
        
        coords = [( 0 , middleY - halfThickness ) , ( middleX - halfThickness , middleY - halfThickness ) , ( middleX - halfThickness , 0 ),
        	( middleX + halfThickness , 0 ) , ( middleX + halfThickness , middleY - halfThickness ) , ( self.__objWidth , middleY - halfThickness ),
        	( self.__objWidth , middleY + halfThickness ) , ( middleX + halfThickness , middleY + halfThickness ) , ( middleX + halfThickness , self.__objHeight),
        	( middleX - halfThickness , self.__objHeight) , ( middleX - halfThickness , middleY + halfThickness ) , ( 0 , middleY + halfThickness )]
        	
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
