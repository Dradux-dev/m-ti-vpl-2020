from datetime import datetime
import time

class NameGenerator:
    
    def __init__(self, pattern=""):
        self.__pattern = pattern

    def getRandomName(self):
        dateTime = datetime.now()
        timeStr =   (str(dateTime.year) + 
                    "_" + 
                    str(dateTime.month) + 
                    "_" + 
                    str(dateTime.day) + 
                    "_" + 
                    str(dateTime.hour) + 
                    "_" + 
                    str(dateTime.minute) + 
                    "_" + 
                    str(dateTime.second) + 
                    "_" + 
                    str(dateTime.microsecond))

        if (self.__pattern == ""):
            return timeStr
        else:
            return self.__pattern + "_" + timeStr

