CC=gcc
CFLAGS=-c -Wall -std=c99
LDFLAGS=-lm
SOURCES=../src/string.c ../src/ipc.c ../src/peasant.c ../src/hash_table.c ../src/list.c ./server.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=server

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXECUTABLE)

clean:
	rm -f ../src/*.o ../client/*.o ./*.o ../client/client $(EXECUTABLE)