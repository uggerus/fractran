# Compiler
CXX = g++

# Flags: C++17, Warnings, Optimization
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

# Linker flags for GMP
LDFLAGS = -lgmpxx -lgmp

# Executables
TARGET_MAIN = prime_game
TARGET_TEST = test_fractran

# Source files
SRC_MAIN = prime-fractran.cpp
SRC_TEST = test_fractran.cpp
HEADERS = fractran.h

# Default target: Build the main game
all: $(TARGET_MAIN)

# Compile Main Game
$(TARGET_MAIN): $(SRC_MAIN) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $(TARGET_MAIN) $(SRC_MAIN) $(LDFLAGS)

# Compile Test Suite
$(TARGET_TEST): $(SRC_TEST) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $(TARGET_TEST) $(SRC_TEST) $(LDFLAGS)

# Run Tests
test: $(TARGET_TEST)
	@echo "--- Executing Tests ---"
	./$(TARGET_TEST)

# Clean up
clean:
	rm -f $(TARGET_MAIN) $(TARGET_TEST)

.PHONY: all clean test
