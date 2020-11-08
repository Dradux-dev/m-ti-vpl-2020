from shapely.geometry import Point, Polygon

from forms.Form import Form


class PolygonForm(Form):
    def __init__(self, size=40):
        Form.__init__(self, size)

    def generateMultiplePolygons(self, templates):
        objList = []
        polys = []

        for template in templates:
            coords = []
            for p in template:
                coords.append((p[0] * self.getWidth, p[1] * self.getHeight))

            polys.append(Polygon(coords))

        for y in range(self.getHeight):
            for x in range(self.getWidth):
                p = Point(x, y)
                inside = [int(p.intersects(poly)) for poly in polys]

                if sum(inside) >= 1:
                    objList.append((x, y, 1))

        return objList

    def generatePolygon(self, template):
        return self.generateMultiplePolygons([template])

