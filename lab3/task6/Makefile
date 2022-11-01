CC=gcc -lm
CFLAGS=-c -std=c99
SOURCES=./cfiles/string.c ./cfiles/check.c ./cfiles/binom_tree.c ./cfiles/binom_heap.c ./cfiles/post.c ./cfiles/dialog.c ./cfiles/task6.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=post

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(CDFLAGS) $(OBJECTS) -o $(EXECUTABLE)

clean:
	rm -f ./cfiles/*.o $(EXECUTABLE)