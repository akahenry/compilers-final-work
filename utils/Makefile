# Grupo D
#   Henry Bernardo Kochenborger de Avila
#   Marcos Samuel Winkel Landi

CXX       := gcc
CXX_FLAGS := 

BIN     := .
SRC     := .
INCLUDE := .

LIBRARIES   := -lfl
EXECUTABLE  := etapa{STEP_NUMBER}

SCANNER := scanner.l
PARSER := parser.y
BISON_FILE := parser.tab.c
LEX_FILE := lex.yy.c


all: $(BIN)/$(EXECUTABLE)

run: clean all
	clear
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/*.c $(SRC)/$(LEX_FILE) $(SRC)/$(BISON_FILE)
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) $^ -o $@ $(LIBRARIES)

$(SRC)/$(BISON_FILE): $(SRC)/$(PARSER)
	bison -d $(SRC)/$(PARSER) -o $(SRC)/$(BISON_FILE)

$(SRC)/$(LEX_FILE): $(SRC)/$(SCANNER)
	flex -o $(SRC)/$(LEX_FILE) $(SRC)/$(SCANNER)

clean:
	-rm $(BIN)/*.o
	-rm $(BIN)/$(EXECUTABLE)