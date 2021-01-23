from forms.PolygonForm import PolygonForm


class Triangle(PolygonForm):
	"""
    @brief Triangle

    Generate the form triangle.

    @author Philipp Koopke
    """
    def __init__(self, size=40):
    	"""
        @brief Constructor

        Constructs the form triangle with a size of 40.
        return a list of pixel which are in the triangle   

        @param size the size of the triangle as int.

        @author Philipp Koopke
        """
        PolygonForm.__init__(self, size)

    def generate(self):
    	"""
        @brief generate

        generate the form with the adjusted size

        @author Philipp Koopke
        """
        
        # define a polygon form
        coords = [(0.5, 0), (0, 1), (1, 1)]
        return self.generatePolygon(coords)
