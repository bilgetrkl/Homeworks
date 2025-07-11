CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic
TARGET = gtu_cpu
SOURCES = main.cpp cpu.cpp memory.cpp parser.cpp
HEADERS = cpu.h memory.h instruction.h parser.h

all: $(TARGET)

$(TARGET): $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all clean