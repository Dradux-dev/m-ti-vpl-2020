from BoundingBox import BoundingBox
import json
from functools import cmp_to_key

class Meta:
	"""
    @brief Meta

    Meta class to create JSON files for Images

    @author Philipp Koopke
    """
	class Entry:
		"""
    	@brief Entry

    	Entry class defines a entry for meta class

    	@author Philipp Koopke
    	"""
		def __init__(self, name, z, boundingBox):
			"""
		    @brief Constructor

		    Constructs a entry for meta class

		    @param name sets the formname 
		    @param z sets the z layer of the form
		    @param boundingBox sets the boundingBox object of the form

		    @author Philipp Koopke
		    """
			self.name = name
			self.z = z
			self.boundingBox = boundingBox

	def __init__(self):
		"""
		@brief Constructor

		Constructs a list of entries for meta data

	    @author Philipp Koopke
	    """	
		self.entries = []

	def addEntry(self, entry):
		"""
        @brief addEntry

        add an entry in entries
        
        @param entry is the entry which will be added to the list

        @author Philipp Koopke
        """
		self.entries.append(entry)

	def isOverlapping(self, boundingBox):
		"""
        @brief isOverlapping

        check if the boundingBox overlaps with an boundingBox in the entries
        
        @param boundingBox is the boundingBox which will be chekced against the entries

        @author Philipp Koopke
        """
		overlaps = [int(boundingBox.isOverlapping(entry.boundingBox)) for entry in self.entries]
		return sum(overlaps) >= 1

	def save(self, filename):
		"""
        @brief save

        stores the list of entries sorted in a JSON file   
        
        @param filename is name of the JSON file

        @author Philipp Koopke
        """
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
		"""
        @brief compare

        compares two entries to sort the list of entries
        
        @param item1 is the first entry to compare
        @param item2 is the second entry to compare

        @author Philipp Koopke
        """
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
		
	
