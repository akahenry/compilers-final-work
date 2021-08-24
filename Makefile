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

STEP_NUMBER := 1

SCANNER := scanner.l
LEX_FILE := lex.yy.c
ANALYZER := analyzer


all: $(BIN)/$(EXECUTABLE)

run: clean all
	clear
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/*.c $(SRC)/$(LEX_FILE)
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) $^ -o $@ $(LIBRARIES)

clean:
	-rm $(BIN)/*
	-rm -r $(TEMP)

$(SRC)/$(LEX_FILE): $(SRC)/$(SCANNER)
	flex -o $(SRC)/$(LEX_FILE) $(SRC)/$(SCANNER)

$(BIN)/$(ANALYZER): $(SRC)/$(LEX_FILE) $(SRC)/*.l
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) $< -o $@ $(LIBRARIES)

compress:
	mkdir $(TEMP)
	cp -a $(INCLUDE)/. $(TEMP)/
	cp -a $(SRC)/. $(TEMP)/
	sed 's/{STEP_NUMBER}/'"$(STEP_NUMBER)/g" $(UTILS)/$(UTILS_MAKEFILE) > $(TEMP)/$(UTILS_MAKEFILE)
	cd $(TEMP) && tar cvzf $(PWD)/etapa$(STEP_NUMBER).tgz * && cd -
	rm -r $(TEMP)