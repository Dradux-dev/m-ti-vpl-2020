import os
import random
from pydoc import locate

from Arguments import Arguments
from Image import Image
from BoundingBox import BoundingBox
from PolyRect import PolyRect
from colors.Flat import Flat
from forms.Plus import Plus
from forms.Cross import Cross
from Meta import Meta
from Color import Color


class Application:
    def __init__(self):
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

        self.tests = {}

        self.metaClass = Meta

        self.args = None
        self.arguments = Arguments()

        self.addTest("overlapping", self.testOverlapping, "Tests the bounding box is overlapping functionality.")

    def parseArguments(self, arguments=None):
        tests = [(name, self.tests[name][1]) for name in self.tests.keys()]
        self.arguments.setup(self.forms.keys(), self.colors.keys(), tests)
        self.args = self.arguments.parse(arguments)

    def addTest(self, name, fn, description=""):
        if name in self.tests.keys():
            raise ValueError(f"Test {name} already exists")

        self.tests[name] = (fn, description)

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
            found = generator.isEnabled
            if found:
                print(f"Generator {name} selected")

        return generator

    def parseColor(self, colorStr, source):
        if colorStr is None:
            return None

        color = Color.FromString(colorStr, self.parseColorMode())
        if color is None:
            raise ValueError(f"Error in parsing {source}")

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
        generator.addColor(Color(255, 0, 0))
        generator.addColor(Color(0, 255, 0))

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

    def parseColorMode(self):
        colorModes = {
            Color.ModeString(Color.Mode.MONOCHROME): Color.Mode.MONOCHROME,
            Color.ModeString(Color.Mode.GREYSCALE): Color.Mode.GREYSCALE,
            Color.ModeString(Color.Mode.RGB): Color.Mode.RGB,
        }

        colorMode = colorModes[self.args.color_mode]
        if colorModes is None:
            raise ValueError(f"Unknown color mode {self.args.color_mode}")

        return colorMode

    def generateImage(self):
        # generates every required image and saves it
        backgroundColor = self.parseColor(self.args.background_color, "--background-color")
        image = Image(self.args.width, self.args.height, self.parseColorMode(), backgroundColor)
        meta = self.metaClass()

        object_count = self.args.object_count
        if object_count is None:
            min_objects = self.args.min_object_count
            max_objects = self.args.max_object_count
            object_count = random.randint(min_objects, max_objects)

        current_object = 0
        while current_object in range(object_count):
            offset, formInfo, generator, boundingBox = self.generateObject(image)
            formName = formInfo[0]
            form = formInfo[1]

            if not self.args.allow_overlapping:
                if meta.isOverlapping(boundingBox):
                    continue

            meta.addEntry(self.metaClass.Entry(formName, current_object, boundingBox))

            if form.renderBoundingBox:
                boundingBox.render(image)

            image.addForm(offset, form, generator)
            current_object += 1

        name = self.generateName()
        image.save(name)
        meta.save(name)

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

        if self.runTests():
            return

        for self.current in range(self.args.count):
            self.generateImage()

    def runTests(self):
        args = vars(self.args)
        testsPerformed = 0

        for name in self.tests.keys():
            fixedName = name.replace("-", "_")
            if args[f"test_{fixedName}"]:
                test = self.tests[name]
                fn = test[0]
                fn()
                testsPerformed += 1

        return testsPerformed >= 1

    def run(self, arguments=None):
        self.scanColors()
        self.scanForms()
        self.parseArguments(arguments)
        self.generate()

    def testOverlappingAddObject(self, image, formClass, offset, size, color):
        form = formClass(size)
        generator = Flat()
        generator.addColor(color)
        boundingBox = BoundingBox(offset, size)
        boundingBox.render(image)
        image.addForm(offset, form, generator)
        return boundingBox

    def testOverlapping(self):
        image = Image()
        plus = self.testOverlappingAddObject(image, formClass=Plus, offset=(40, 40), size=60, color=Color(255, 0, 0))
        cross = self.testOverlappingAddObject(image, formClass=Cross, offset=(60, 60), size=20, color=Color(0, 255, 0))
        image.save("test-overlapping.png")

        print(cross.isOverlapping(plus), plus.isOverlapping(cross))

    @staticmethod
    def replaceLast(s, old, new, occurence=1):
        li = s.rsplit(old, occurence)
        return new.join(li)


if __name__ == '__main__':
    app = Application()
    app.scanForms()
    app.scanColors()
    app.parseArguments("-h")
