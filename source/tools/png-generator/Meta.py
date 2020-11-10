from BoundingBox import BoundingBox


class Meta:
    class Entry:
        def __init__(self, name, z, boundingBox):
            self.name = name
            self.z = z
            self.boundingBox = boundingBox

    def __init__(self):
        self.entries = []

    def addEntry(self, entry):
        self.entries.append(entry)

    def save(self, filename):
        pass
