from BoundingBox import BoundingBox


class Meta:
    class Entry:
        name = "FormName"
        z = 0
        boundingBox = BoundingBox()

    def __init__(self):
        self.entries = []

    def addEntry(self, entry):
        self.entries.append(entry)

    def save(self, filename):
        pass
