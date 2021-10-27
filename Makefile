CXX       := gcc
CXX_FLAGS := 

BIN     := bin
SRC     := src
INCLUDE := include

TEMP := temp
UTILS := utils
UTILS_MAKEFILE := Makefile

LIBRARIES   := -lfl
EXECUTABLE  := main
EXECUTABLE_DEBUG  := debug

STEP_NUMBER := 5

SCANNER := scanner.l
PARSER := parser.y
BISON_FILE := parser.tab.c
BISON_HEADER:= parser.tab.h
BISON_OUTPUT := parser.output
LEX_FILE := lex.yy.c
ANALYZER := analyzer


all: $(BIN)/$(EXECUTABLE)

run: clean all
	clear
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/*.c $(SRC)/$(LEX_FILE) $(SRC)/$(BISON_FILE)
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) $^ -o $@ $(LIBRARIES)

clean:
	-rm $(BIN)/*
	-rm -r $(TEMP)
	-rm $(SRC)/$(BISON_FILE) $(SRC)/$(BISON_OUTPUT) $(SRC)/$(LEX_FILE)
	-rm $(INCLUDE)/$(BISON_HEADER)

$(SRC)/$(BISON_FILE): $(SRC)/$(PARSER)
	bison -d $(SRC)/$(PARSER) -o $(SRC)/$(BISON_FILE)
	mv $(SRC)/$(BISON_HEADER) $(INCLUDE)/$(BISON_HEADER)

$(SRC)/$(LEX_FILE): $(SRC)/$(SCANNER)
	flex -o $(SRC)/$(LEX_FILE) $(SRC)/$(SCANNER)

$(BIN)/$(ANALYZER): $(SRC)/$(LEX_FILE) $(SRC)/*.l
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) $< -o $@ $(LIBRARIES)

compress:
	mkdir $(TEMP)
	cp -a $(INCLUDE)/. $(TEMP)/
	cp -a $(SRC)/. $(TEMP)/
	-rm $(TEMP)/$(LEX_FILE) $(TEMP)/$(BISON_FILE) $(TEMP)/$(BISON_HEADER) $(TEMP)/.gitkeep
	sed 's/{STEP_NUMBER}/'"$(STEP_NUMBER)/g" $(UTILS)/$(UTILS_MAKEFILE) > $(TEMP)/$(UTILS_MAKEFILE)
	cd $(TEMP) && tar cvzf $(PWD)/etapa$(STEP_NUMBER).tgz * && cd -
	-rm -r $(TEMP)

exec: all
	$(BIN)/$(EXECUTABLE)

test: 
	./test.sh

debug: $(SRC)/*.c $(SRC)/$(LEX_FILE) $(SRC)/$(BISON_FILE)
	$(CXX) $(CXX_FLAGS) -g -I$(INCLUDE) $^ -o $(BIN)/$(EXECUTABLE_DEBUG) $(LIBRARIES)