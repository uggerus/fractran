# Compiler
CXX = g++

# Flags: -O3 is HIGHLY recommended for benchmarking GMP
CXXFLAGS = -std=c++17 -Wall -Wextra -O3

# Linker flags
LDFLAGS = -lgmpxx -lgmp

# Executables
TARGET_MAIN = prime_game
TARGET_TEST = test_fractran
TARGET_BENCH = benchmark

# Source files
SRC_MAIN = prime-fractran.cpp
SRC_TEST = test_fractran.cpp
SRC_BENCH = benchmark.cpp
HEADERS = fractran.h

# Default target
all: $(TARGET_MAIN)

# Compile Main
$(TARGET_MAIN): $(SRC_MAIN) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $(TARGET_MAIN) $(SRC_MAIN) $(LDFLAGS)

# Compile Tests
$(TARGET_TEST): $(SRC_TEST) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $(TARGET_TEST) $(SRC_TEST) $(LDFLAGS)

# Compile Benchmark
$(TARGET_BENCH): $(SRC_BENCH) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $(TARGET_BENCH) $(SRC_BENCH) $(LDFLAGS)

# Run Tests
test: $(TARGET_TEST)
	@echo "--- Executing Tests ---"
	./$(TARGET_TEST)

# Run Benchmark
benchmark: $(TARGET_BENCH)
	@echo "--- Executing Benchmark ---"
	./$(TARGET_BENCH)

clean:
	rm -f $(TARGET_MAIN) $(TARGET_TEST) $(TARGET_BENCH)

.PHONY: all clean test benchmark
