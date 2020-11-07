from forms.PolygonForm import PolygonForm


class Cross(PolygonForm):
    def __init__(self, size=40):
        PolygonForm.__init__(self, size)

    def generate(self):
        cornerDistance = 0.2

        coords = [
            [(0, cornerDistance), (cornerDistance, 0), (1, 1 - cornerDistance), (1 - cornerDistance, 1)],
            [(1 - cornerDistance, 0), (1, cornerDistance), (cornerDistance, 1), (0, 1 - cornerDistance)]
        ]

        return self.generateMultiplePolygons(coords)
