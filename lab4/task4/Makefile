CC=gcc
CFLAGS=-c -std=c99
LDFLAGS=-lm
SOURCES= ./src/string.c ./src/hash_table.c ./src/commands.c ./src/check.c ./src/execute_command.c ./src/interpreter.c ./src/task4.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=task4

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $(EXECUTABLE)

clean:
	rm -f ./src/*.o $(EXECUTABLE)