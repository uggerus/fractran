#ifndef FRACTRAN_H
#define FRACTRAN_H

#include <gmpxx.h>
#include <iostream>
#include <vector>

class Fractran {
public:
    Fractran(const std::vector<mpq_class>& fractions, mpz_class num, bool enableHistory = false) {
        fractionList = fractions;
        integer = num;
        halted = false;
        totalSteps = 0;
        recordHistory = enableHistory;
    }

    void runMachine(int steps);
    void printSequence();
    
    bool isHalted() const { return halted; }
    mpz_class getLastNumber() const { return integer; }
    const std::vector<mpz_class>& getHistory() const { return numberList; }
    unsigned long long getStepCount() const { return totalSteps; }

private:
    std::vector<mpq_class> fractionList;
    mpz_class integer;
    std::vector<mpz_class> numberList;
    bool halted;
    unsigned long long totalSteps;
    bool recordHistory;
};

inline void Fractran::runMachine(int steps) {
    if (halted) return;

    bool match_found = true;
    int current_batch_steps = 0;

    while (current_batch_steps < steps && match_found) {
        if (recordHistory) {
            numberList.push_back(integer);
        }
        
        match_found = false;

        for (const auto& frac : fractionList) {
            mpz_class num = frac.get_num();
            mpz_class den = frac.get_den();

            if (integer % den == 0) {
                integer = (integer / den) * num;
                match_found = true;
                current_batch_steps++;
                totalSteps++; 
                break; 
            }
        }
    }

    if (!match_found) {
        halted = true;
    }
}

inline void Fractran::printSequence() {
    if (!recordHistory) {
        std::cout << "History disabled for this run (pass 'true' to constructor to enable)." << std::endl;
        // Still print the final number so the user isn't completely blind
        std::cout << "Final Value: " << integer << std::endl;
        return;
    }
    for (const auto& i : numberList) {
        std::cout << i << ", ";
    }
    if (halted) {
        std::cout << "HALT" << std::endl;
    } else {
        std::cout << "..." << std::endl;
    }
}

#endif // FRACTRAN_H



