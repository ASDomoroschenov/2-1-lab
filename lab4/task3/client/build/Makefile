CC=gcc
CFLAGS=-c -Wall -std=c99
LDFLAGS=-lm
SOURCES=../../src/string/string.c ../../src/check/check.c ../../src/sem/sem.c ../../src/shm/shm.c ../../src/operations/operations.c ../client.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=client

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXECUTABLE)

clean:
	rm -f ../src/*/*.o ../src/*.o $(EXECUTABLE)