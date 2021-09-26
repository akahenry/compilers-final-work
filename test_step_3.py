import os

input_dir = 'tests/step-3/inputs'
refs_dir = 'tests/step-3/reference-dots'

print("generating outputs")
for file in os.listdir(input_dir):
    print(f"output {file}")
    os.system(f'cat "{input_dir}/{file}" | ./bin/main | python3 gen_graph.py | python3 dot_converter.py')
    os.rename('AST.dot', f'tests/step-3/outputs/{file}.dot')

    os.system(f"cat tests/step-3/outputs/{file}.dot | python3 dot_converter.py")
    os.rename('AST.dot', f'tests/step-3/outputs/converted{file}.dot')

for file in os.listdir(refs_dir):
    if (file != "converted"):
        os.system(f"cat {refs_dir}/{file} | python3 dot_converter.py")
        os.rename('AST.dot', f'{refs_dir}/converted/{file}')

print("comparing asts")
for file in os.listdir(input_dir):
    with open(f'tests/step-3/outputs/converted{file}.dot', 'r') as f1:
        with open(f"{refs_dir}/converted/{file}.ref.dot", 'r') as f2:
            f1l = f1.readlines()
            f1l.sort()
            f2l = f2.readlines()
            f2l.sort()
            if ("\n".join(f1l) == "\n".join(f2l)):
                print(f'{file}: SUCCESS')
            else:
                print(f'{file}: FAIL')

print("checking memory leaks")
failed_files = ""
failed = False
for file in os.listdir(input_dir):
    if ((os.system(f"cat {input_dir}/{file} | valgrind ./bin/main") >> 8) != 0):
        failed = True
        failed_files += file + "\n"

if not failed:
    print("SUCCESS")
else:
    print("FAILED")
    print(failed_files)
