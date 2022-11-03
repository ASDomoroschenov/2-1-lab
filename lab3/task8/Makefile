CC=gcc
CFLAGS=-c -std=c99
SOURCES= ./cfiles/string.c ./cfiles/bin_tree.c ./cfiles/pref_tree.c ./cfiles/parse.c ./cfiles/task8.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=task8

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $(EXECUTABLE)

clean:
	rm -f ./cfiles/*.o $(EXECUTABLE)