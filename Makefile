# Makefile

# Compiler
CXX = g++

# Compiler flags
# -std=c++11: Use C++11 standard (good baseline, std::stod, range-based for if needed)
# -Wall: Enable most warnings
# -Wextra: Enable extra warnings
# -g: Include debug information
CXXFLAGS = -std=c++11 -Wall -Wextra -g

# Linker flags (if any needed later)
LDFLAGS =

# Source files
SRCS = main.cpp calculator.cpp

# Object files (derived from source files)
OBJS = $(SRCS:.cpp=.o)

# Target executable name
TARGET = main

# Default target: build the executable
all: $(TARGET)

# Rule to link the executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

# Rule to compile C++ source files into object files
%.o: %.cpp calculator.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Target to clean up build artifacts
clean:
	rm -f $(OBJS) $(TARGET)

# Phony targets (targets that don't represent actual files)
.PHONY: all clean
