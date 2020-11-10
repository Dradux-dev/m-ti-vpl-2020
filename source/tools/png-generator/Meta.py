from BoundingBox import BoundingBox
import json
from functools import cmp_to_key

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

	def isOverlapping(self, boundingBox):
		overlaps = [int(boundingBox.isOverlapping(entry.boundingBox)) for entry in self.entries]
		return sum(overlaps) >= 1

	def save(self, filename):
		name = filename + '.json'
		data = {
			'forms': []
		}

		# in progress: sort
		sortedMeta = sorted(self.entries, key=cmp_to_key(Meta.compare ) )
		
		for entry in sortedMeta:
			data['forms'].append({
				'formName': entry.name,
				'position': {
					'x': entry.boundingBox.topLeft[0],
					'y': entry.boundingBox.topLeft[1]
				},
				'width': entry.boundingBox.width,
				'height': entry.boundingBox.height,
				'formLayer': entry.z
			})

		with open(name, 'w') as outfile:
			json.dump(data, outfile)
				
				
				
	@staticmethod
	def compare(item1, item2 ):
		if item1.boundingBox.isOverlapping(item2.boundingBox) == True:
			if item1.z > item2.z:
				return -1
			else:
				return 1
		else:
			if item1.boundingBox.areaSize > item2.boundingBox.areaSize:
				return -1
			elif item1.boundingBox.areaSize < item2.boundingBox.areaSize:
				return 1
			elif item1.z > item2.z:
				return -1
			elif item1.z < item2.z:
				return 1
			else:
				return 0
		
	
