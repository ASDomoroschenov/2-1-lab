CC=gcc
CFLAGS=-c -std=c99
SOURCES=./cfiles/binom_tree.c ./cfiles/fib_heap.c ./cfiles/employee.c ./cfiles/task3.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=sort

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $(EXECUTABLE)

clean:
	rm -f ./cfiles/*.o $(EXECUTABLE)