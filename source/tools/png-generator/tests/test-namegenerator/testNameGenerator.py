from datetime import datetime

from classNameGenerator import NameGenerator

namegen1 = NameGenerator()

print("Empty string test: ")
print(namegen1.getRandomName())

print("")

for x in range(6):
    print("Loop test: ")
    print(namegen1.getRandomName())

print("")

namegen2 = NameGenerator("triangle")
print("name test:")
print(namegen2.getRandomName())


