CC=gcc
CFLAGS=-c -std=c99
SOURCES=binom_tree.c binom_heap.c employee.c task3.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=run

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $(EXECUTABLE)

clean:
	rm -f *.o $(EXECUTABLE)