#include <iostream>
#include <vector>
#include <chrono> 
#include <iomanip> 
#include "fractran.h"

int main() {
    // 1. Setup Conway's Prime Game (A heavy arithmetic workload)
    std::vector<mpq_class> primes_prog = {
        mpq_class(17, 91), mpq_class(78, 85), mpq_class(19, 51), mpq_class(23, 38),
        mpq_class(29, 33), mpq_class(77, 29), mpq_class(95, 23), mpq_class(77, 19),
        mpq_class(1, 17),  mpq_class(11, 13), mpq_class(13, 11), mpq_class(15, 2),
        mpq_class(1, 7),   mpq_class(55, 1)
    };

    int TARGET_STEPS = 20000; // Run enough steps to get a stable measurement

    std::cout << "--- FRACTRAN SIMULATOR BENCHMARK ---" << std::endl;
    std::cout << "Program: Conway's Prime Game" << std::endl;
    std::cout << "Target Steps: " << TARGET_STEPS << std::endl;
    std::cout << "History Recording: DISABLED (Pure Compute Speed)" << std::endl;
    std::cout << "Running..." << std::endl;

    // 2. Initialize Machine (History DISABLED)
    Fractran machine(primes_prog, 2, false);

    // 3. Start Timer
    auto start_time = std::chrono::high_resolution_clock::now();

    // 4. Run Workload
    machine.runMachine(TARGET_STEPS);

    // 5. Stop Timer
    auto end_time = std::chrono::high_resolution_clock::now();

    // 6. Calculate Metrics
    std::chrono::duration<double> elapsed = end_time - start_time;
    double seconds = elapsed.count();
    unsigned long long actual_steps = machine.getStepCount();
    double steps_per_second = actual_steps / seconds;

    // 7. Report
    std::cout << "------------------------------------" << std::endl;
    std::cout << "Time Elapsed:    " << std::fixed << std::setprecision(4) << seconds << "s" << std::endl;
    std::cout << "Steps Completed: " << actual_steps << std::endl;
    std::cout << "Performance:     " << std::fixed << std::setprecision(2) << steps_per_second << " Steps/Sec" << std::endl;
    std::cout << "Final Integer Bits: " << machine.getLastNumber().get_str(2).length() << " bits" << std::endl;
    std::cout << "------------------------------------" << std::endl;

    return 0;
}
