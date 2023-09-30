CC = gcc
CFLAGS = -Wall -Wextra
SRCDIR = src
BUILDDIR = build
BINDIR = bin
LOGDIR = logs
DBDIR = database

# List of source files in src directory and its subdirectories
SRC = $(shell find $(SRCDIR) -name "*.c")

# List of files .o in build directory
OBJ = $(patsubst $(SRCDIR)/%.c, $(BUILDDIR)/%.o, $(SRC))

# Executable name
TARGET = $(BINDIR)/main

all: $(TARGET)

$(TARGET): $(OBJ)
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) $^ -o $@

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)  # create folder if not exist
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILDDIR) $(BINDIR)

run: 
	./$(TARGET)
