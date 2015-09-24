TARGET = p1
LIBS = -lm
CC = g++
CFLAGS = -c

.PHONY: default all clean

default: $(TARGET)
all: default

OBJECTS = $(patsubst %.cc, %.o, $(wildcard *.cc))
HEADERS = $(wildcard *.h)

%.o: %.cc $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@ -lrt -fopenmp -g



.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -Wall $(LIBS) -o $@ -lrt -fopenmp 


clean:
	-rm -f *.o
	-rm -f $(TARGET)
