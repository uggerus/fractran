# Compiler to use
CXX = g++

# Compiler flags:
# -std=c++17: Use C++17 standard
# -Wall -Wextra: Enable warning messages
# -O2: Optimize for speed (crucial for number crunching)
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

# Linker flags:
# -lgmpxx: Link C++ wrapper for GMP
# -lgmp: Link standard C GMP library
LDFLAGS = -lgmpxx -lgmp

# The name of the executable file
TARGET = fractran_sim

# The source file(s)
SRC = prime-fractran.cpp

# Default target (what happens when you just type 'make')
all: $(TARGET)

# Rule to link the object file to create the executable
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)

# Clean rule to remove the executable
clean:
	rm -f $(TARGET)

# Phony targets help avoid conflicts with files named 'all' or 'clean'
.PHONY: all clean
