CC = gcc
CFLAGS = -Wall -g `pkg-config sdl3 sdl3-image --cflags`
LDFLAGS = `pkg-config sdl3 sdl3-image --libs`

SRC_DIR = src
OBJ_DIR = obj

# Find all .c files in src directory and subdirectories
SRC = $(shell find $(SRC_DIR) -name '*.c')
# Convert the .c file paths to .o file paths in the obj directory
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
TARGET = game

all: $(TARGET)

# Create the target binary
$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

# Compile each source file to an object file
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)  # Create the necessary directories
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm -rf $(OBJ_DIR) $(TARGET)

.PHONY: all clean