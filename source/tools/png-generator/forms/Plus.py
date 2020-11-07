from forms.PolygonForm import PolygonForm


class Plus(PolygonForm):
    def __init__(self, size=40):
        PolygonForm.__init__(self, size)

    def generate(self):
        coords = [
            [(0, 0.35), (0, 0.65), (1, 0.65), (1, 0.35)],
            [(0.35, 0), (0.65, 0), (0.65, 1), (0.35, 1)]
        ]

        return self.generateMultiplePolygons(coords)
