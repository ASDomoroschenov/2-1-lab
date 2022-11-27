CC=gcc
CFLAGS=-c -std=c99
LDFLAGS=-lm
SOURCES= ./src/string.c ./src/stack_char.c ./src/stack_double.c ./src/expression.c ./src/execute.c ./src/task5.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=task5

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $(EXECUTABLE)

clean:
	rm -f ./src/*.o ./log/* $(EXECUTABLE)