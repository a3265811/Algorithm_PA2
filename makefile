CC=g++
LDFLAGS=-std=c++11 -O3 -lm -g
SOURCES=*.cpp
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=m11007428
INCLUDES=*.h

all: $(SOURCES) ./$(EXECUTABLE)

./$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

%.o:  %.c  ${INCLUDES}
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o ./$(EXECUTABLE) *.out

