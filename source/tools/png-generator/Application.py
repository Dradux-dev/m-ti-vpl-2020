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
        self.arguments.setup(self.forms.keys(), self.colors.keys())
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
        return f"Application_{self.current}.png"

    def selectRandomForm(self):
        formName = None
        form = None
        found = False

        # @todo Build check, that at least one form is available

        while not found:
            pos = random.randint(0, len(self.forms)-1)
            formName = list(self.forms.keys())[pos]
            form = self.forms[formName]
            found = form.isEnabled

        return formName, form

    def selectRandomGenerator(self):
        generator = None
        found = False

        # @todo Build check, that at least one generator is available

        while not found:
            pos = random.randint(0, len(self.colors)-1)
            name = list(self.colors.keys())[pos]
            generator = self.colors[name]
            generatorMode = generator.getColorMode()
            mode = self.args.color_mode
            found = generatorMode == mode and generator.isEnabled

        return generator

    def parseColor(self, colorStr, source):
        color = None

        if colorStr is not None:
            fill = colorStr.split(",")
            if len(fill) == 1:
                if self.args.color_mode == "monochrome" and fill[0] == "1":
                    color = (255, 255, 255)
                else:
                    color = (int(fill[0]), int(fill[0]), int(fill[0]))
            elif len(fill) == 3:
                color = (int(fill[0]), int(fill[1]), int(fill[2]))
            else:
                raise ValueError(f"No rule to handle {len(fill)} color components for {source}")

        return color

    def generateBoundingBox(self, offset, object_size):
        backgroundColor = self.parseColor(self.args.bounding_box_fill, "--bounding-box-fill")
        borderColor = self.parseColor(self.args.bounding_box_border, "--bounding-box-border")

        boundingBox = BoundingBox(offset, object_size)

        if backgroundColor is not None:
            boundingBox.setBackgroundColor(backgroundColor)

        if borderColor is not None:
            boundingBox.setBorderColor(borderColor)

        return boundingBox

    def generateObjectGenerator(self):
        generator = self.selectRandomGenerator()
        generator.addColor((255, 0, 0))
        generator.addColor((0, 255, 0))

        return generator

    def generateObject(self, image):
        object_size = self.args.size
        if object_size is None:
            min_size = self.args.min_size
            max_size = self.args.max_size
            object_size = random.randint(min_size, max_size)

        offset = (
            random.randint(self.args.margin, image.width - (self.args.margin + object_size)),
            random.randint(self.args.margin, image.height - (self.args.margin + object_size))
        )

        formInfo = self.selectRandomForm()
        formInfo[1].setSize(object_size)

        return offset, formInfo, self.generateObjectGenerator(), self.generateBoundingBox(offset, object_size)

    def generateImage(self):
        # generates every required image and saves it
        backgroundColor = self.parseColor(self.args.background_color, "--background-color")
        image = Image(self.args.width, self.args.height, self.args.color_mode, backgroundColor)

        object_count = self.args.object_count
        if object_count is None:
            min_objects = self.args.min_object_count
            max_objects = self.args.max_object_count
            object_count = random.randint(min_objects, max_objects)

        for current_object in range(object_count):
            offset, formInfo, generator, boundingBox = self.generateObject(image)
            formName = formInfo[0]
            form = formInfo[1]

            if form.renderBoundingBox:
                boundingBox.render(image)

            image.addForm(offset, form, generator)

        image.save(self.generateName())

    def prepareGeneration(self):
        # Convert self.args to dictionary to support [] operator
        args = vars(self.args)

        # Prepare Forms!
        for formName in self.forms.keys():
            form = self.forms[formName]

            if args[f"{formName.lower()}_disable"]:
                form.disable()
            else:
                form.enable()

            form.renderBoundingBox = self.args.render_bounding_box or args[f"{formName.lower()}_bounding_box"]

        # Prepare Generators
        for generatorName in self.colors.keys():
            generator = self.colors[generatorName]

            if args[f"{generatorName.lower()}_disable"]:
                generator.disable()
            else:
                generator.enable()

    def generate(self):
        self.prepareGeneration()

        for self.current in range(self.args.count):
            self.generateImage()

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
