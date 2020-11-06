import math
from shapely.geometry import Point, Polygon

class Kreuz:

    def __init__(self, size=40):

        self.__objWidth = size
        self.__objHeight = size
        self.__thickness = 0.3*size
        
        

    def generate(self):

        objList = []
        
        cornerDistance = math.sqrt( pow(self.__thickness, 2) / 2)
        
        coords1 = [( 0 , cornerDistance ) , ( cornerDistance , 0 ) , 
        	( self.__objWidth , self.__objHeight - cornerDistance ) , ( self.__objWidth - cornerDistance , self.__objHeight ) ]
        poly1 = Polygon(coords1)
        
        coords2 = [( self.__objWidth - cornerDistance , 0 ) , ( self.__objWidth , cornerDistance ) ,
        	( cornerDistance , self.__objHeight ) , ( 0 , self.__objHeight - cornerDistance )]       
        poly2 = Polygon(coords2)
        

        for y in range(self.__objHeight):
            for x in range(self.__objWidth):
            	p = Point(x,y)
            	
            	if ( p.within(poly1) or p.within(poly2) ):
            		objList.append((x, y, 1))
            	else:
            		objList.append((x, y, 0))

        return objList



    def getWidth(self):
        return self.__objWidth



    def getHeight(self):
        return self.__objHeight
