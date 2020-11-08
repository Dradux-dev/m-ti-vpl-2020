import os
import random
from pydoc import locate

from Arguments import Arguments
from Image import Image
from BoundingBox import BoundingBox
from PolyRect import PolyRect
from colors.Rgb import Rgb



class Application:
    def __init__(self):
        self.args = None
        self.forms = {}
        self.formBlacklist = [
            "Form",
            "PolygonForm"
        ]
        self.colors = {}
        self.colorBlacklist = [
            "ColorGenerator",
            "Gradient"
        ]
        self.arguments = Arguments()

    def parseArguments(self, arguments=None):
        self.arguments.setup(["rectangle"], self.colors.keys())
        self.args = self.arguments.parse(arguments)

    def blacklistForm(self, name):
        if name not in self.formBlacklist:
            self.formBlacklist.append(name)

    def addForm(self, name, form):
        if name not in self.formBlacklist:
            print(f"Added form {name}")
            self.forms[name] = form()

    def scanForms(self):
        for file in os.listdir("./forms"):
            if file.endswith(".py"):
                name = Application.replaceLast(file, ".py", "")
                locateString = "forms." + name + "." + name
                classVar = locate(locateString)
                self.addForm(name, classVar)

    def blacklistColor(self, name):
        if name not in self.colorBlacklist:
            self.colorBlacklist.append(name)

    def addColor(self, name, generator):
        if name not in self.colorBlacklist:
            self.colors[name] = generator()

    def scanColors(self):
        for file in os.listdir("./colors"):
            if file.endswith(".py"):
                name = Application.replaceLast(file, ".py", "")
                locateString = "colors." + name + "." + name
                classVar = locate(locateString)
                self.addColor(name, classVar)

    def generateName(self):
        # generates a filename <YYYY><MM><DD>-<Counter>
        # and returns it
        pass

    def selectRandomForm(self):
        form = None
        found = False

        while not found:
            pos = random.randint(0, len(self.forms)-1)
            name = list(self.forms.keys())[pos]
            form = self.forms[name]
            found = form.isEnabled()

        return form

    def selectRandomGenerator(self):
        generator = None
        found = False

        while not found:
            pos = random.randint(0, len(self.colors)-1)
            name = list(self.colors.keys())[pos]
            generator = self.colors[name]
            generatorMode = generator.getColorMode()
            mode = self.args.color_mode
            found = generatorMode == mode

        return generator

    def generate(self):
        # generates every required image and saves it
        offset = (30, 30)
        object_size = 200

        img = Image()
        form = self.selectRandomForm()
        form.setSize(object_size)

        generator = self.selectRandomGenerator()
        generator.addColor((255, 0, 0))
        generator.addColor((0, 255, 0))

        boundingBox = BoundingBox(offset, object_size)
        boundingBox.render(img, (114, 151, 219), (0, 29, 84))

        pink = Rgb()
        pink.addColor((255, 0, 255))
        pr = PolyRect()
        img.addForm((0, 0), pr, pink)


        img.addForm(offset, form, generator)
        img.save("Application.png")

    def run(self, arguments=None):
        self.scanColors()
        self.scanForms()
        self.parseArguments(arguments)
        self.generate()

    @staticmethod
    def replaceLast(s, old, new, occurence=1):
        li = s.rsplit(old, occurence)
        return new.join(li)


if __name__ == '__main__':
    app = Application()
    app.scanForms()
    app.scanColors()
    app.parseArguments("-h")
