# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic

# SFML flags for linking
SFML_FLAGS = -lsfml-graphics -lsfml-window -lsfml-system

# Executable name
TARGET = monopoly_game

# Test executable name
TEST_TARGET = test_game

# Source files
SRCS = main.cpp game.cpp streetTile.cpp railroadTile.cpp player.cpp board.cpp

# Test source files (replace test.cpp with your actual test files)
TEST_SRCS = test.cpp game.cpp streetTile.cpp railroadTile.cpp player.cpp board.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Test object files
TEST_OBJS = $(TEST_SRCS:.cpp=.o)

# Rule to compile the project
all: $(TARGET)

# Rule to create the executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(SFML_FLAGS)

# Rule to create the test executable
$(TEST_TARGET): $(TEST_OBJS)
	$(CXX) $(CXXFLAGS) -o $(TEST_TARGET) $(TEST_OBJS) $(SFML_FLAGS)

# Rule to run tests
test: $(TEST_TARGET)
	./$(TEST_TARGET)

# Rule to compile source files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule to clean the build directory
clean:
	rm -f $(OBJS) $(TEST_OBJS) $(TARGET) $(TEST_TARGET)

# Phony target to prevent issues with file names matching target names
.PHONY: all clean test
