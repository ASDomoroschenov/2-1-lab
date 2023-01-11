CC=gcc
CFLAGS=-c -Wall -std=c99
LDFLAGS=-lm
SOURCES=../src/string/string.c ../src/configuration/configuration.c ../src/gets/gets.c ../src/check/check.c ../src/operations/operations.c ../src/variables/variables.c ../src/interpreter/interpreter.c ../src/task8.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=task8

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $(EXECUTABLE)

clean:
	rm -f ../src/*.o ../src/*/*.o $(EXECUTABLE)
