CC=gcc
CFLAGS=-c -std=c99
SOURCES= ./cfiles/string.c ./cfiles/list.c ./cfiles/execute.c ./cfiles/task10_2.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=task10_2

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $(EXECUTABLE)

clean:
	rm -f ./cfiles/*.o $(EXECUTABLE)