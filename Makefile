# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2
LDFLAGS = -lm

# Target executable
TARGET = test_binary80

# Source files
SOURCES = binary80.c test_binary80.c
OBJECTS = $(SOURCES:.c=.o)
HEADERS = binary80.h

# Default target
all: $(TARGET)

# Build executable
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

# Compile source files
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build files
clean:
	rm -f $(OBJECTS) $(TARGET)

# Rebuild everything
rebuild: clean all

# Run tests
test: $(TARGET)
	./$(TARGET)

# Install (optional)
install: $(TARGET)
	install -m 755 $(TARGET) /usr/local/bin/

# Uninstall (optional)
uninstall:
	rm -f /usr/local/bin/$(TARGET)

.PHONY: all clean rebuild test install uninstall
