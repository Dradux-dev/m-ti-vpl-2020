from forms.Form import Form


class Rectangle(Form):
    def __init__(self, size=40):
        Form.__init__(self, size)

    def generate(self):
        objList = []

        for y in range(self.getHeight):
            for x in range(self.getWidth):
                objList.append((x, y, 1))

        return objList
