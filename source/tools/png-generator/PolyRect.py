from forms.PolygonForm import PolygonForm


class PolyRect(PolygonForm):
    def __init__(self):
        PolygonForm.__init__(self, 10)

    def generate(self):
        return self.generatePolygon([(0, 0), (1, 0), (1, 1), (0, 1)])