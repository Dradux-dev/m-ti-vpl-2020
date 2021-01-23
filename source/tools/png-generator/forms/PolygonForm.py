from shapely.geometry import Point, Polygon

from forms.Form import Form


class PolygonForm(Form):
    """
    @brief PolygonForm

    This is a class to generate polygon forms. A form can contain several subforms.

    @author Philipp Koopke
    """
    
    def __init__(self, size=40):
        """
        @brief Constructor

        Constructs the polygon form with a size of 40.

        @param size the size of the polygon form as int.

        @author Philipp Koopke
        """
        Form.__init__(self, size)

    def generateMultiplePolygons(self, templates):
        """
        @brief generateMultiplePolygons

	generate a polygon form coords for a multiple polygon
	return a list of pixel which are in the polygon        

        @param template is a list of single polygon. Every single polygon will descripe throw a list of coords. The values must be betweent 0 and 1.

        @author Philipp Koopke
        """
        objList = []
        polys = []

	# create a list of polygons from the coords
        for template in templates:
            coords = []
            for p in template:
                coords.append((p[0] * self.getWidth, p[1] * self.getHeight))

            polys.append(Polygon(coords))


        for y in range(self.getHeight):
            for x in range(self.getWidth):
                p = Point(x, y)
                
                # check if the point is in one of the polygons
                inside = [int(p.intersects(poly)) for poly in polys]

                if sum(inside) >= 1:
                    objList.append((x, y, 1))

        return objList

    def generatePolygon(self, template):
        """
        @brief generatePolygon

	generate a polygon form coords for a singe polygon
	return a list of pixel which are in the polygon           

        @param template is a list of coords for single polygon. The values must be betweent 0 and 1.

        @author Philipp Koopke
        """
        return self.generateMultiplePolygons([template])

