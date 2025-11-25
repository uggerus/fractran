#include <iostream>
#include <vector>
#include <cassert>
#include "fractran.h"

// Helper to print checkmarks
void pass(std::string name) {
    std::cout << "[PASS] " << name << std::endl;
}

void test_basic_multiplication() {
    // Program: Multiply by 3/2
    // Input: 2
    // Expected: 2 * (3/2) = 3
    std::vector<mpq_class> prog = { mpq_class(3, 2) };
    Fractran machine(prog, 2);
    
    machine.runMachine(1);
    
    assert(machine.getLastNumber() == 3);
    pass("Basic Multiplication (2 -> 3)");
}

void test_priority_logic() {
    // Program: [5/2, 3/2]
    // Input: 2
    // Both denominators divide 2, but 5/2 comes first.
    // Expected: 2 * (5/2) = 5. (NOT 3)
    std::vector<mpq_class> prog = { mpq_class(5, 2), mpq_class(3, 2) };
    Fractran machine(prog, 2);
    
    machine.runMachine(1);
    
    assert(machine.getLastNumber() == 5);
    pass("Priority Logic (First fraction match)");
}

void test_halting() {
    // Program: [1/3]
    // Input: 2
    // 2 is not divisible by 3. Should halt immediately.
    std::vector<mpq_class> prog = { mpq_class(1, 3) };
    Fractran machine(prog, 2);
    
    machine.runMachine(5);
    
    assert(machine.isHalted() == true);
    assert(machine.getLastNumber() == 2); // Should not have changed
    pass("Halting Logic");
}

void test_addition() {
    // Program: [3/2] (Simplest adder)
    // Input: 2^2 * 3^3 = 4 * 27 = 108
    // Logic: Replaces factor 2 with factor 3.
    // Result should be 2^0 * 3^(2+3) = 3^5 = 243.
    
    std::vector<mpq_class> prog = { mpq_class(3, 2) };
    mpz_class input = 108; // 2^2 * 3^3
    Fractran machine(prog, input);
    
    machine.runMachine(100); // Should finish well before 100
    
    assert(machine.getLastNumber() == 243);
    assert(machine.isHalted() == true); // Should halt when out of 2s
    pass("Addition Algorithm (2^a * 3^b -> 3^(a+b))");
}

void test_large_numbers() {
    // GMP Check: Ensure we don't overflow standard integers
    // Program: [2/1] (Doubler)
    // Input: 1
    // Run 70 times. 2^70 is much larger than a 64-bit integer.
    
    std::vector<mpq_class> prog = { mpq_class(2, 1) };
    Fractran machine(prog, 1);
    
    machine.runMachine(70);
    
    mpz_class result = machine.getLastNumber();
    mpz_class expected;
    mpz_ui_pow_ui(expected.get_mpz_t(), 2, 70); // GMP power function
    
    assert(result == expected);
    pass("Large Number Support (GMP)");
}

void test_step_counter() {
    // Program: [2/1] (Doubles input forever)
    std::vector<mpq_class> prog = { mpq_class(2, 1) };
    Fractran machine(prog, 1);

    // Run 10 steps
    machine.runMachine(10);
    assert(machine.getStepCount() == 10);

    // Run 5 more steps (should accumulate to 15)
    machine.runMachine(5);
    assert(machine.getStepCount() == 15);

    pass("Step Counter Accumulation");
}

void test_BBf15() {
  std::vector<mpq_class> fractans = { mpq_class(1, 45),
				  mpq_class(4, 5),
				  mpq_class(3, 2),
				  mpq_class(25, 3)};
  Fractran machine(fractans, 2);
  machine.runMachine(100);

  //std::cout << machine.getStepCount() << std::endl;
  assert(machine.getStepCount() == 28);

  pass("BBf15");
}

void test_BBf16() {
  std::vector<mpq_class> fractans = { mpq_class(1, 45),
				  mpq_class(4, 5),
				  mpq_class(3, 2),
				  mpq_class(125, 3)};
  Fractran machine(fractans, 2);
  machine.runMachine(100);

  assert(machine.getStepCount() == 53);

  pass("BBf16");
}

void test_BBf17() {
  std::vector<mpq_class> fractans = { mpq_class(5, 6),
				  mpq_class(49, 2),
				  mpq_class(3, 5),
				  mpq_class(40, 7)};
  Fractran machine(fractans, 2);
  machine.runMachine(1000);

  //std::cout << machine.getStepCount() << std::endl;

  assert(machine.getStepCount() == 107);

  pass("BBf17");
}

void test_BBf21() {
  std::vector<mpq_class> fractans = { mpq_class(7, 15),
				  mpq_class(4, 3),
				  mpq_class(27, 14),
				  mpq_class(5, 2),
				  mpq_class(9, 5)};
  Fractran machine(fractans, 2);
  machine.runMachine(100000000);

  std::cout << machine.getStepCount() << std::endl;

  assert(machine.getStepCount() == 107);

  pass("BBf21");
}
  

int main() {
    std::cout << "Running Fractran Test Suite..." << std::endl;
    std::cout << "------------------------------" << std::endl;

    test_basic_multiplication();
    test_priority_logic();
    test_halting();
    test_addition();
    test_large_numbers();
    test_step_counter();
    test_BBf15();
    test_BBf16();
    test_BBf17();
    //test_BBf21();

    std::cout << "------------------------------" << std::endl;
    std::cout << "All tests passed successfully." << std::endl;
    return 0;
}
