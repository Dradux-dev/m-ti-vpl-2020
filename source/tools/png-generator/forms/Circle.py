import math

from forms.Form import Form


class Circle(Form):
    """
    @brief Circle

    Generate the form circle.

    @author Philipp Koopke
    """

    def __init__(self, size=40):
       """
        @brief Constructor

        Constructs the form circle with a size of 40.

        @param size the size of the circle as int.

        @author Philipp Koopke
        """
        Form.__init__(self, size)
        self.__radius = size / 2

    def setSize(self, size):
    	"""
        @brief setSize

        Adjusts the size of the circle

        @param size the new size of the circle as int.

        @author Philipp Koopke
        """
        Form.setSize(self, size)
        self.__radius = size / 2

    def generate(self):
    	"""
        @brief generate

        generate the form with the adjusted size
        return a list of pixel which are in the circle   

        @author Philipp Koopke
        """
        objList = []

	# calculate the center of the circle
        middleX = (self.getWidth / 2) - 0.5
        middleY = (self.getHeight / 2) - 0.5

        for y in range(self.getHeight):
            for x in range(self.getWidth):
            	# calculate the distance from the current position to center of the circle
                distance = math.sqrt(pow(middleY - y, 2) + pow(middleX - x, 2))

				# check if the position is in circle
                if distance <= self.__radius:
                    objList.append((x, y, 1))

        return objList
