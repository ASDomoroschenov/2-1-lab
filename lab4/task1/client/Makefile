CC=gcc
CFLAGS=-c -std=c99
LDFLAGS=-lm
SOURCES=../src/string.c ../src/ipc.c ./client.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=client

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(filename) $(OBJECTS) -o $(EXECUTABLE)

clean:
	rm -f ./src/*.o $(EXECUTABLE)