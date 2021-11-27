import os

for testNumber in range(33):
    os.system(f"cat tests/step-6/testeE56/ijk{testNumber:02d} | bin/main -O > pythonTestOut")

    with open("pythonTestOut", "r") as f:
        i = 1
        lastArg1 = ""
        lastArg2 = ""
        for line in f.readlines():
            if (line[:-1].split(" ")[0] == "mov"):
                currentArg1 = line[:-1].split(" ")[1]
                currentArg2 = line[:-1].split(" ")[1]

                if (currentArg1 == lastArg2 and currentArg2 == lastArg1):
                    print(f"Test ijk{testNumber}: optimization candidate found on line {i}")

                lastArg1 = currentArg1
                lastArg2 = currentArg2
            i += 1

    os.system("rm pythonTestOut")