CXX       := gcc
CXX_FLAGS := -xc++

BIN     := .
SRC     := .
INCLUDE := .

LIBRARIES   := -lstdc++ -shared-libgcc -lfl
EXECUTABLE  := etapa{STEP_NUMBER}


all: $(BIN)/$(EXECUTABLE)

run: clean all
	clear
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/*.c $(SRC)/*.cpp
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) $^ -o $@ $(LIBRARIES)

clean:
	-rm $(BIN)/*.o
	-rm $(BIN)/$(EXECUTABLE)