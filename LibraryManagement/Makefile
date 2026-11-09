
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude

TARGET = main
SOURCES = main.cpp src/Book.cpp src/Library.cpp

all:
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

run: all
	./$(TARGET)

clean:
	rm -f $(TARGET)
