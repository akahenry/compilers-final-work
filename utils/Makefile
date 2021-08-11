CXX       := g++
CXX_FLAGS := -std=c++17 -ggdb

BIN     := .
SRC     := .
INCLUDE := .

LIBRARIES   :=
EXECUTABLE  := etapa{STEP_NUMBER}


all: $(BIN)/$(EXECUTABLE)

run: clean all
	clear
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/*.cpp
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) $^ -o $@ $(LIBRARIES)

clean:
	-rm $(BIN)/*.o
	-rm $(BIN)/$(EXECUTABLE)