import sys
import os

dot_file = """
digraph {
    <<connections>>
    <<labels>>
}
"""

connections = ""
labels = ""
for line in sys.stdin:
    if(line.strip()[-1] == ';'):
        labels += str(int(line.strip().split(' ')[0],0)) + ' ' \
            + '[label="' + " ".join(line.strip().split(' ')[1:]).replace('[label="', '')[:-3].replace('"', '\\"').replace("\\", "\\\\") + '"];\n    '
    else:
        connections += ' -> '.join([str(int(x,0)) for x in line.strip().split(', ')]) + '\n    '


dot_file = dot_file.replace("<<connections>>", connections)
dot_file = dot_file.replace("<<labels>>", labels)

print(dot_file)
os.system(f"echo '{dot_file}' > AST.dot")
#os.system(f"echo '{dot_file}' | dot -Tsvg > AST.svg")
