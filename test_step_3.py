import os

input_dir = 'tests/step-3/inputs'
refs_dir = 'tests/step-3/reference-dots'

for file in os.listdir(input_dir):
    if (int(file[1:]) == 0):
        os.system(f'cat "{input_dir}/{file}" | ./bin/main | python3 gen_graph.py')
        os.rename('AST.svg', f'tests/step-3/outputs/{file}.svg')

for file in os.listdir(refs_dir):
    os.system(f"cat '{refs_dir}/{file}' | dot -Tsvg > tests/step-3/references/{file}.svg")
