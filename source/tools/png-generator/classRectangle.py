
class Rectangle:
	
	def __init__(self,objWidth=40,objHeigth=40):
	
		self.__objWidth = objWidth
		self.__objHeight = objHeigth


	def generate(self):
			
		objList=[]
		
		for y in range(self.__objHeight):
			for x in range(self.__objWidth):
				objList.append( ( y , x , 1) )
		
		return objList
		
	def getWidth(self):
		return self.__objWidth
	
	def getHeight(self):
		return self.__objHeight
