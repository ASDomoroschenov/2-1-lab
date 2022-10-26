CC=gcc
CFLAGS=-c -std=c99
SOURCES=./cfiles/student.c ./cfiles/string.c ./cfiles/task5.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=student

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $(EXECUTABLE)

clean:
	rm -f ./cfiles/*.o $(EXECUTABLE)
	rm -f res_files/*