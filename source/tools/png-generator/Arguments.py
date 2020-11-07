import argparse


class Arguments:
    def __init__(self):
        self.parser = argparse.ArgumentParser(description="Tool to create simple PNGs")
        self.groupImage = None
        self.groupObject = None
        self.groupGenerator = None
        self.groupForms = None
        self.groupColors = None

    def setupImage(self):
        self.groupImage = self.parser.add_argument_group(title="Image", description="Image properties")
        self.groupImage.add_argument("--width", type=int, help="The width of the image that shall be created")
        self.groupImage.add_argument("--height", type=int, help="The height of the image that shall be created")
        self.groupImage.add_argument("--color-mode", type=str, default="rgb", help="The color mode of the generated images. This automatically disables all color generators that does not match the specified color mode. Available color modes are: monochrome, greyscale and rgb")

    def setupObject(self):
        self.groupObject = self.parser.add_argument_group(title="Objects", description="Object properties")
        self.groupObject.add_argument("--size", type=int,
                                  help="The size of the objects that shall be created This will overwrite --min-size and --max-size and sets the size to a fixed value.")
        self.groupObject.add_argument("--min-size", type=int, help="The minimum size of an object")
        self.groupObject.add_argument("--max-size", type=int, help="The maximum size of an object")

    def setupGenerator(self):
        self.groupGenerator = self.parser.add_argument_group(title="Generation", description="Image generating properties")
        self.groupGenerator.add_argument("--count", type=int, default=1,
                               help="The count of different images that shall be created")
        self.groupGenerator.add_argument("--object-count", type=int, default=1,
                               help="The count of objects that shall be created in the image")
        self.groupGenerator.add_argument("--min-object-count", type=int, default=1,
                               help="The minimum object count that shall be generated")
        self.groupGenerator.add_argument("--max-object-count", type=int, default=1,
                               help="The maximum object count that shall be generated")
        self.groupGenerator.add_argument("--margin", type=int, default=0,
                               help="Margin that every object needs to keep to the image borders")
        self.groupGenerator.add_argument("--allow-overlapping", action="store_true",
                               help="Allows the bounding boxes of objects to overlap. This can also lead to overlapping objects.")
        self.groupGenerator.add_argument("--monochrome", type=str, default="0,1",
                                     help="Monochrome color setup. <BackgroundColor>,<ObjectColor> with 0 for black and 1 for white.")
        self.groupGenerator.add_argument("--greyscale", type=str, default="fixed,64/fixed,192/64",
                                     help="Geyscale color setup. Color values have to be in range from 0 (black) to 255 (white). Format is <Background>/<Object>[/<Distance>]. Background and object can be fixed,<Color> or random,<min>,<max> with min <= x <= max. Distance is a number that describes the minimum color distance between background and object, that has to taken care of.")
        self.groupGenerator.add_argument("--rgb", type=str, default="fixed,0,0,0/fixed,255,0,0,100",
                                     help="RGB color setup. Color values are always RGB and each component has to be in a range from 0 to 255. Format is <Background>/<Object>[/<Distance>. Background and object color can be fixed,<R>,<G>,<B> or random,<Rmin>,<Gmin>,<Bmin>,<Rmax>,<Gmax>,<Bmax>. The distance has to be a number which represents the minimum distance between two colors, if both colors are treated as 3D vectors.")
        self.groupGenerator.add_argument("--gradient", type=str, default="fixed,0,0,0/fixed,255,0,0/fixed,0,255,0/100",
                                     help="Gradient color setup. Color values are always RGB and each component has to be in a range form 0 to 255. Format is <Background>/<ObjectFrom>/<ObjectTo>[/<Distance]. Background, ObjectFrom and ObjectTo can be fixed,<R>,<G>,<B> or random<Rmin>,<Gmin>,<Bmin>,<Rmax>,<Gmax>,<Bmax>. The distance has to be a number, which represents the minimum distance between Background and ObjectFrom aswell as Background and ObjectTo, if all colors are treated as 3D vectors.")
        self.groupGenerator.add_argument("--render-bounding-box", action="store_true",
                               help="Renders the bounding boxes for all generated objects")
        self.groupGenerator.add_argument("--bounding-box-fill", type=str, help="The background color of the bounding box")
        self.groupGenerator.add_argument("--bounding-box-border", type=str, help="The border color of the bounding box")

    def setupForms(self, forms=[]):
        if len(forms) > 0:
            self.groupForms = self.parser.add_argument_group(title="Forms", description="Form properties")
            for name in forms:
                self.groupForms.add_argument(f"--{name.lower()}-disable", action="store_true", default=False, help=f"Disables the {name.lower()}")
                self.groupForms.add_argument(f"--{name.lower()}-bounding-box", action="store_true", default=False,
                           help=f"Enables bounding box rendering for the {name.lower()}")

    def setupColors(self, colors=[]):
        if len(colors) > 0:
            self.groupColors = self.parser.add_argument_group(title="Color Generators",
                                                    description="Disable not required color generators. All color generators that does not match the specified color mode of the image, are disable automatically.")
            for name in colors:
                self.groupColors.add_argument(f"--{name.lower()}-disable", action="store_true", help=f"Disables the {name.lower()} color generator")

    def setup(self, forms=[], colors=[]):
        self.parser.add_argument("--verbose", "-v", action="count", default=0, help="Performs the actions silently")

        self.setupImage()
        self.setupObject()
        self.setupGenerator()
        self.setupForms(forms)
        self.setupColors(colors)

    def parse(self, arguments=None):
        args = None
        if isinstance(arguments, str):
            args = self.parser.parse_args(arguments.split())
        else:
            args = self.parser.parse_args()

        print(args)
        return args
