# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic

# SFML flags for linking
SFML_FLAGS = -lsfml-graphics -lsfml-window -lsfml-system

# Executable name
TARGET = monopoly_game

# Source files
SRCS = main.cpp game.cpp streetTile.cpp railroadTile.cpp player.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Rule to compile the project
all: $(TARGET)

# Rule to create the executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(SFML_FLAGS)

# Rule to compile source files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule to clean the build directory
clean:
	rm -f $(OBJS) $(TARGET)

# Phony target to prevent issues with file names matching target names
.PHONY: all clean
