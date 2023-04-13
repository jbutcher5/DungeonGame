CC      = clang++
INCLUDE = src
OBJ     = build
SRC     = src
SRCS    = $(SRC)/main.cpp
SRCS    += $(shell find $(SRC) -type f -name '*.cpp')
OBJS    = $(patsubst $(SRC)/%.cpp,$(OBJ)/%.o,$(SRCS))
EXE     = game
CFLAGS  = -I$(INCLUDE) -std=c++17 -pedantic
LDLIBS  = -lm -lraylib

.PHONY: clean

$(OBJ)/%.o: $(SRC)/%.cpp
	@mkdir -p "$(@D)"
	@echo "Compiling: $< -> $@"
	@$(CC) -c -g $(CFLAGS) $< -o $@

$(EXE): $(OBJS)
	@echo "Building final executable: $@"
	@$(CC) $^ -g $(LDLIBS) -o $@

$(OBJ):
	mkdir -p $@

format: $(SRC)
	clang-format $^ -i

clean:
	rm -rf $(OBJ) $(EXE)
