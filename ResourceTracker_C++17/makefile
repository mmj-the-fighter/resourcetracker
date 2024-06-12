CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

SOURCES = main.cpp DirectoryWalker.hpp QuickProfiler.hpp
EXECUTABLE = app

all: $(EXECUTABLE)

$(EXECUTABLE): $(SOURCES)
    $(CXX) $(CXXFLAGS) -o $(EXECUTABLE) $(SOURCES)

clean:
    rm -f $(EXECUTABLE)

.PHONY: all clean