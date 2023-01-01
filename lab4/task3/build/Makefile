CC=gcc
CFLAGS=-c -Wall -std=c99
LDFLAGS=-lm
SOURCES=../src/string/string.c ../src/check/check.c ../src/operations/operations.c ../src/variables/variables.c ../src/commands/commands.c ../src/interpreter/interpreter.c ../src/task3.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=task3

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXECUTABLE)

clean:
	rm -f ../src/*/*.o ../src/*.o $(EXECUTABLE)