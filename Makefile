# Compiler
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O3 # O3 is CRITICAL for vector optimization
LDFLAGS = -lgmpxx -lgmp

# Targets
TARGET_MAIN = fractran
TARGET_BENCH_FAST = benchmark_fast_exec

# Sources
SRC_MAIN = fractran_interpreter.cpp
SRC_BENCH_FAST = benchmark_fast.cpp

# Headers
HEADERS = fractran.h fractran_fast.h prime_utils.h arg_parser.h

all: $(TARGET_MAIN)

$(TARGET_MAIN): $(SRC_MAIN) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $(TARGET_MAIN) $(SRC_MAIN) $(LDFLAGS)

$(TARGET_BENCH_FAST): $(SRC_BENCH_FAST) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $(TARGET_BENCH_FAST) $(SRC_BENCH_FAST) $(LDFLAGS)

bench_fast: $(TARGET_BENCH_FAST)
	@./$(TARGET_BENCH_FAST)

clean:
	rm -f $(TARGET_MAIN) $(TARGET_BENCH_FAST) *.o

.PHONY: all clean bench_fast
