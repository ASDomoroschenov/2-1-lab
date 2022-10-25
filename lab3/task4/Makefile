CC=gcc
CFLAGS=-c -std=c99
SOURCES=./cfiles/csv_file.c ./cfiles/message.c ./cfiles/string.c ./cfiles/task4.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=message

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $(EXECUTABLE)

clean:
	rm -f *.o $(EXECUTABLE)
	rm -f ./csv_file/*