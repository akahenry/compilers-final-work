CXX       := g++
CXX_FLAGS := -std=c++17 -ggdb

BIN     := bin
SRC     := src
INCLUDE := include

TEMP := temp
UTILS := utils
UTILS_MAKEFILE := Makefile

LIBRARIES   :=
EXECUTABLE  := main

STEP_NUMBER := 1


all: $(BIN)/$(EXECUTABLE)

run: clean all
	clear
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/*.cpp
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) $^ -o $@ $(LIBRARIES)

clean:
	-rm $(BIN)/*
	-rm -r $(TEMP)

compress:
	mkdir $(TEMP)
	cp -a $(INCLUDE)/. $(TEMP)/
	cp -a $(SRC)/. $(TEMP)/
	sed 's/{STEP_NUMBER}/'"$(STEP_NUMBER)/g" $(UTILS)/$(UTILS_MAKEFILE) > $(TEMP)/$(UTILS_MAKEFILE)
	tar cvzf etapa$(STEP_NUMBER).tgz $(TEMP)/
	# rm -r $(TEMP)