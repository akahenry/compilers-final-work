This is the final work for Compilers Course at the Federal University of Rio Grande do Sul (UFRGS).

# Setup

sudo apt-get install libc6-dev libfl-dev

# TODO MAKEFILE
bison -d src/parser.y -o src/parser.tab.c
flex -o src/lex.yy.c src/scanner.l
gcc src/lex.yy.c src/parser.tab.c src/main.c -o bin/parser -lfl
