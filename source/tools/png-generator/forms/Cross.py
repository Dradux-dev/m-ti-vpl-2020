from forms.PolygonForm import PolygonForm


class Cross(PolygonForm):
    """
    @brief Cross

    Generate the form cross.

    @author Philipp Koopke
    """

    def __init__(self, size=40):
        """
        @brief Constructor

        Constructs the form cross with a size of 40.

        @param size the size of the cross as int.

        @author Philipp Koopke
        """
        PolygonForm.__init__(self, size)

    def generate(self):
        """
        @brief generate

        generate the form with the adjusted size
        return a list of pixel which are in the cross   

        @author Philipp Koopke
        """
        cornerDistance = 0.2

	# define the poylgon form with two rectangles
        coords = [
            [(0, cornerDistance), (cornerDistance, 0), (1, 1 - cornerDistance), (1 - cornerDistance, 1)],
            [(1 - cornerDistance, 0), (1, cornerDistance), (cornerDistance, 1), (0, 1 - cornerDistance)]
        ]

        return self.generateMultiplePolygons(coords)
