import os

input_dir = 'tests/step-5/'

for file in os.listdir(input_dir):
    if  os.WEXITSTATUS(os.system(f'cat "{input_dir}/{file}" | ./bin/main > /dev/null')) != 0:
        print(f"Error on {file}")

