#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>
#include "fractran.h"      // The old engine
#include "fractran_fast.h" // The new engine

int main() {
    // CONWAY PRIME GAME
    std::vector<mpq_class> primes_prog = {
        mpq_class(17, 91), mpq_class(78, 85), mpq_class(19, 51), mpq_class(23, 38),
        mpq_class(29, 33), mpq_class(77, 29), mpq_class(95, 23), mpq_class(77, 19),
        mpq_class(1, 17),  mpq_class(11, 13), mpq_class(13, 11), mpq_class(15, 2),
        mpq_class(1, 7),   mpq_class(55, 1)
    };

    int STEPS = 100000;

    std::cout << "--- SPEED SHOWDOWN: GMP vs VECTORS ---" << std::endl;
    std::cout << "Steps: " << STEPS << std::endl;

    // 1. Benchmark Old Engine
    auto start = std::chrono::high_resolution_clock::now();
    Fractran oldMachine(primes_prog, 2, false);
    oldMachine.runMachine(STEPS);
    auto end = std::chrono::high_resolution_clock::now();
    double oldTime = std::chrono::duration<double>(end - start).count();

    // 2. Benchmark New Engine
    start = std::chrono::high_resolution_clock::now();
    FractranFast newMachine(primes_prog, 2);
    newMachine.run(STEPS);
    end = std::chrono::high_resolution_clock::now();
    double newTime = std::chrono::duration<double>(end - start).count();

    // 3. Validation
    // Note: Step counts might differ by 1 due to the Halt check definition we discussed earlier
    // But the final number should match.
    
    std::cout << "\nResults:" << std::endl;
    std::cout << "Old Engine (GMP):    " << std::fixed << std::setprecision(4) << oldTime << "s" << std::endl;
    std::cout << "New Engine (Vector): " << std::fixed << std::setprecision(4) << newTime << "s" << std::endl;
    std::cout << "Speedup:             " << (oldTime / newTime) << "x FASTER" << std::endl;
    
    // Verify Correctness
    mpz_class val1 = oldMachine.getLastNumber();
    mpz_class val2 = newMachine.getNumber();
    
    if (val1 == val2) {
        std::cout << "\n[SUCCESS] Final states match perfectly." << std::endl;
    } else {
        std::cout << "\n[FAIL] States diverged!" << std::endl;
        std::cout << "Old: " << val1 << std::endl;
        std::cout << "New: " << val2 << std::endl;
    }

    return 0;
}
