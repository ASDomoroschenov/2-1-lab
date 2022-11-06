CC=gcc
CFLAGS=-c -std=c99
SOURCES= ./cfiles/string.c ./cfiles/check.c ./cfiles/dbl_list.c ./cfiles/binom_tree.c ./cfiles/binom_heap.c ./cfiles/execute.c ./cfiles/task9.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=task9

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $(EXECUTABLE)

clean:
	rm -f ./cfiles/*.o $(EXECUTABLE)