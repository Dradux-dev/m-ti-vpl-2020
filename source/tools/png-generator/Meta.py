from BoundingBox import BoundingBox
import json

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
    	name = filename + '.json'
        data = {}
	data['forms'] = []
	
	# in progress: sort
	sortedMeta = []
	
	for part in meta
	
		if( len(sortedMeta) == 0):  #sorted list is empty
			sortedMeta.append(part)
		else:
			for index in range( len(sortedMeta) )
				#todo
			
	
	for obj in sortedMeta
		objName = obj[0]
		objBox = obj[1]
		data['forms'].append({ 
			'formName': objName,
			'upper left corner': objBox.topLeft,
			'width': objBox.width,
			'height': objBox.height
		}) 
        
        with open(name, 'w') as outfile:
    		json.dump(data, outfile)
    
        pass
