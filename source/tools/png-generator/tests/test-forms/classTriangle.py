from classPolygonForm import PolygonForm

class Triangle(PolygonForm):

	def __init__(self, size=40):

		PolygonForm.__init__(self,size)
		
		
		
	def generate(self):
	
		coords = [ (0.5, 0) , (0,1) , (1,1) ]
		return self.generatePolygon(coords)




