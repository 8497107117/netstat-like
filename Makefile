CXX = g++
CFLAGS = -g -Wall
TARGET = hw1
OBJECTS = main.o command.o

all: $(TARGET)

hw1: $(OBJECTS)
		$(CXX) -o $@ $^
		$(OBJECTS): %.o: %.cpp %.h
			$(CXX) -o $@ -c $< $(CFLAGS)

clean:
		rm -f *.o hw1
