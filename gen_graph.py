import sys
import os

dot_file = """
digraph G {
    node [shape=box];
    rankdir=LR;

    <<connections>>
    <<labels>>
}
"""

connections = ""
labels = ""
for line in sys.stdin:
    if(line.strip()[-1] == ';'):
        labels += str(int(line.strip().split(' ')[0],0)) + ' ' + " ".join(line.strip().split(' ')[1:]) + "\n    "
    else:
        connections += ' -> '.join([str(int(x,0)) for x in line.strip().split(', ')]) + ';\n    '


dot_file = dot_file.replace("<<connections>>", connections)
dot_file = dot_file.replace("<<labels>>", labels)

# print(dot_file)
os.system(f"echo '{dot_file}' | dot -Tsvg > AST.svg")
