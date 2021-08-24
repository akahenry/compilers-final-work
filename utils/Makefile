CXX       := gcc
CXX_FLAGS := 

BIN     := .
SRC     := .
INCLUDE := .

LIBRARIES   := -lfl
EXECUTABLE  := etapa{STEP_NUMBER}

SCANNER := scanner.l
LEX_FILE := lex.yy.c


all: $(BIN)/$(EXECUTABLE)

run: clean all
	clear
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/*.c $(SRC)/$(LEX_FILE)
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) $^ -o $@ $(LIBRARIES)

$(SRC)/$(LEX_FILE): $(SRC)/$(SCANNER)
	flex -o $(SRC)/$(LEX_FILE) $(SRC)/$(SCANNER)

clean:
	-rm $(BIN)/*.o
	-rm $(BIN)/$(EXECUTABLE)