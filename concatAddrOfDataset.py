import pathlib

path = pathlib.Path().parent.resolve()
# path = str(path) + "/code/dataset/"
path = "../dataset/"
print(path)
file = open("/home/seyyedh/workspace/cuda/HW03/codes/aatrain.txt", "r")
targetFile = open(
    "/home/seyyedh/workspace/cuda/HW03/codes/trainlist.txt", "w")

# file = open("/home/seyyedh/workspace/cuda/HW03/codes/aatest1.txt", "r")
# targetFile = open(
#     "/home/seyyedh/workspace/cuda/HW03/codes/testlist.txt", "w")

lines = file.readlines()

for i in lines:
    s = ""
    s = str(path) + i
    targetFile.write(s)

file.close()
targetFile.close()
