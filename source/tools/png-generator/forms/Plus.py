from forms.PolygonForm import PolygonForm


class Plus(PolygonForm):
    """
    @brief Plus

    Generate the form plus.

    @author Philipp Koopke
    """

    def __init__(self, size=40):
        """
        @brief Constructor

        Constructs the form plus with a size of 40.

        @param size the size of the plus as int.

        @author Philipp Koopke
        """
        PolygonForm.__init__(self, size)

    def generate(self):
        """
        @brief generate

        generate the form with the adjusted size
        return a list of pixel which are in the plus   

        @author Philipp Koopke
        """

	# define the poylgon form with two rectangles
        coords = [
            [(0, 0.35), (0, 0.65), (1, 0.65), (1, 0.35)],
            [(0.35, 0), (0.65, 0), (0.65, 1), (0.35, 1)]
        ]

        return self.generateMultiplePolygons(coords)
