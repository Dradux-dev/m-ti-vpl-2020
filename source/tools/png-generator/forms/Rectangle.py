from forms.Form import Form


class Rectangle(Form):
	"""
    @brief Rectangle

    Generate the form rectangle.

    @author Philipp Koopke
    """
    
    def __init__(self, size=40):
    	"""
        @brief Constructor

        Constructs the form rectangle with a size of 40.
        return a list of pixel which are in the rectangle   

        @param size the size of the rectangle as int.

        @author Philipp Koopke
        """
        Form.__init__(self, size)

    def generate(self):
    	"""
        @brief generate

        generate the form with the adjusted size

        @author Philipp Koopke
        """
        objList = []

        for y in range(self.getHeight):
            for x in range(self.getWidth):
                objList.append((x, y, 1))

        return objList
