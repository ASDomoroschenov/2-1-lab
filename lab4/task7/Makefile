CC=gcc
CFLAGS=-c -std=c99
LDFLAGS=-lm
SOURCES=./src/string.c ./src/array_vars.c ./src/stack.c ./src/expression.c ./src/execute.c ./src/task7.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=task7

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $(EXECUTABLE)

clean:
	rm -f ./src/*.o $(EXECUTABLE)