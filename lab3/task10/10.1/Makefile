CC=gcc
CFLAGS=-c -std=c99
SOURCES= ./cfiles/string.c ./cfiles/stack.c ./cfiles/tree.c ./cfiles/execute.c ./cfiles/task10_1.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=task10_1

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $(EXECUTABLE)

clean:
	rm -f ./cfiles/*.o $(EXECUTABLE)