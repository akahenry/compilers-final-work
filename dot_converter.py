import sys
import os

translator_dict = {}
count = 0
complete_dot = ""
inside_digraph_lines = []

for line in sys.stdin:
    if ("[label" in line):
        translator_dict[line.split(" [label")[0].strip()] = str(count)
        count += 1

    if (line.strip() != ""):
        inside_digraph_lines.append(line.strip() + '\n')

complete_dot = "".join(inside_digraph_lines)

for key in translator_dict:
    complete_dot = complete_dot.replace(key, translator_dict[key])

os.system(f"echo '{complete_dot}' > AST.dot")
