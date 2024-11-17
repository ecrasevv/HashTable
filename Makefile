CC = gcc
CFLAGS = -Wall

SOURCES = src/main.c src/hash_table.c
OBJECTS = $(SOURCES:.c=.o)
TARGET = hash_program

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)

.PHONY: clean

clean:
	@rm -f $(TARGET) $(OBJECTS) core
