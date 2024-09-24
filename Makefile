# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic

# Executable names
TARGET = monopoly_game
TEST_TARGET = test_monopoly

# Source files
SRCS = main.cpp streetTile.cpp railroadTile.cpp player.cpp game.cpp
TEST_SRCS = test.cpp

# Object files for the main executable
OBJS = streetTile.o railroadTile.o player.o game.o

# Object files for the test executable
TEST_OBJS = test.o streetTile.o railroadTile.o player.o game.o

# Rule to compile the project
all: $(TARGET) $(TEST_TARGET)

# Rule to create the main executable
$(TARGET): main.o $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) main.o $(OBJS)

# Rule to create the test executable (without main.o)
$(TEST_TARGET): $(TEST_OBJS)
	$(CXX) $(CXXFLAGS) -o $(TEST_TARGET) $(TEST_OBJS)

# Rule to compile source files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule to clean the build directory
clean:
	rm -f $(OBJS) $(TEST_OBJS) main.o $(TARGET) $(TEST_TARGET)

# Phony target to prevent issues with file names matching target names
.PHONY: all clean
