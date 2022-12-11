CC=gcc
CFLAGS=-c -std=c99
LDFLAGS=-lm
SOURCES= ./src/string.c ./src/stack.c ./src/check.c ./src/expression.c ./src/bin_tree.c ./src/operations.c ./src/table.c ./src/task6.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=task6

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $(EXECUTABLE)

clean:
	rm -f ./src/*.o $(EXECUTABLE)