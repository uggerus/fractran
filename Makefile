# Compiler
CXX = g++

# Flags: 
CXXFLAGS = -std=c++17 -Wall -Wextra -O3

# Linker flags
LDFLAGS = -lgmpxx -lgmp

# Executables
TARGET_MAIN = fractran
TARGET_TEST = test_fractran
TARGET_TEST_ARGS = test_args
TARGET_BENCH = benchmark_sim

# Source files
SRC_MAIN = fractran_interpreter.cpp
SRC_TEST = test-fractran.cpp
SRC_TEST_ARGS = test_fractran.cpp
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

# Compile Argument Parser Tests 
$(TARGET_TEST_ARGS): $(SRC_TEST_ARGS) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $(TARGET_TEST_ARGS) $(SRC_TEST_ARGS) $(LDFLAGS)

# Compile Benchmark
$(TARGET_BENCH): $(SRC_BENCH) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $(TARGET_BENCH) $(SRC_BENCH) $(LDFLAGS)

# Run Tests
test: $(TARGET_TEST)
	@echo "--- Executing Tests ---"
	./$(TARGET_TEST)
	@echo "\n=== Running Argument Tests ==="
	./$(TARGET_TEST_ARGS)

# Run Benchmark
benchmark: $(TARGET_BENCH)
	@echo "--- Executing Benchmark ---"
	./$(TARGET_BENCH)

clean:
	rm -f $(TARGET_MAIN) $(TARGET_TEST) $(TARGET_BENCH) $(TARGET_TEST_ARGS)

.PHONY: all clean test benchmark
