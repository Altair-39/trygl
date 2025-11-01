# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -g -Iinclude
LDFLAGS = -lglfw -lGL -ldl -lm -lassimp

# Directories
SRC_DIR = src
BIN_DIR = bin

# Targets
TARGET = $(BIN_DIR)/opengl_app
SRC = $(SRC_DIR)/main.cpp $(SRC_DIR)/glad.c

# Create bin directory if it doesn't exist
$(shell mkdir -p $(BIN_DIR))

# Default target
all: $(TARGET)

# Link the executable
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Clean build files
clean:
	rm -rf $(BIN_DIR)

# Run the program
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
