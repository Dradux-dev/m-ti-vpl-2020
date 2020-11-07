import os
import random
from pydoc import locate

from Arguments import Arguments
from Image import Image


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

    def generate(self):
        # generates every required image and saves it
        img = Image()
        form = self.selectRandomForm()
        img.addForm((30, 30), form, (255, 0, 0))
        img.save("Application.png")

    def run(self):
        self.scanColors()
        self.scanForms()
        self.parseArguments()
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
