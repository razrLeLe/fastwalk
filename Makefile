TARGET = fastwalk

CXX = g++

CXXFLAGS = -std=c++11 \
	-I include \
	-I util 

LDFLAGS = -pthread

SRC = $(wildcard src/*.cpp) $(wildcard util/*.cpp)

OBJ = $(SRC:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

clean:
	rm $(OBJ) $(TARGET)

.PHONY: all clean
