import os

output = open("externalFiles.ini", 'w')

files = os.listdir()

for f in files:
    print()
        
for f in files:
    if f[-3:] == "png":
        output.write(f + "\n")
        print(f)
    
output.close()
        
