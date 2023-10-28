CC = gcc
CFLAGS = -Wall -Wextra
SRCDIR = src
BUILDDIR = build

# List of source files in src directory and its subdirectories
SRC = $(shell find $(SRCDIR) -name "*.c")

# List of files .o in build directory
OBJ = $(patsubst $(SRCDIR)/%.c, $(BUILDDIR)/%.o, $(SRC))

# Executable name
TARGET = resolver

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)  # create folder if not exist
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILDDIR) ${TARGET}

run: 
	./$(TARGET)
