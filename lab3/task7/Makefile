CC=gcc
CFLAGS=-c -std=c99
SOURCES=./cfiles/string.c ./cfiles/list.c ./cfiles/citizen.c ./cfiles/task7.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=citizen

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $(EXECUTABLE)

clean:
	rm -f ./cfiles/*.o $(EXECUTABLE)