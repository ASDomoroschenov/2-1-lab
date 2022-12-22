CC=gcc
CFLAGS=-c -std=c99
LDFLAGS=-lm
SOURCES=./src/string.c ./src/list.c ./src/hash.c ./src/define.c ./src/task9.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=task9

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $(EXECUTABLE)

clean:
	rm -f ./src/*.o $(EXECUTABLE)