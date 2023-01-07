CC=gcc
CFLAGS=-c -Wall -std=c99
LDFLAGS=-lm
SOURCES=../../src/check/check.c ../../src/commands/commands.c ../../src/operations/operations.c ../../src/string/string.c ../../src/variables/variables.c ../../src/sem/sem.c ../../src/shm/shm.c ../../src/hash_table/hash_table.c ../../src/list/list.c ../server.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=server

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXECUTABLE)

clean:
	rm -f ../*.o ../../src/*/*.o ../../client/*.o server* ../../client/build/client*