#ifndef FRACTRAN_H
#define FRACTRAN_H

#include <gmpxx.h>
#include <iostream>
#include <vector>

class Fractran {
public:
    Fractran(const std::vector<mpq_class>& fractions, mpz_class num) {
        fractionList = fractions;
        integer = num;
        halted = false;
    }

    void runMachine(int steps);
    void printSequence();
    
    // -- NEW: Getters for Testing --
    bool isHalted() const { return halted; }
    mpz_class getLastNumber() const { return integer; }
    const std::vector<mpz_class>& getHistory() const { return numberList; }

private:
    std::vector<mpq_class> fractionList;
    mpz_class integer;
    std::vector<mpz_class> numberList;
    bool halted;
};

inline void Fractran::runMachine(int steps) {
    if (halted) return;

    bool match_found = true;
    int current_step = 0;

    while (current_step < steps && match_found) {
        numberList.push_back(integer);
        match_found = false;

        for (const auto& frac : fractionList) {
            mpz_class num = frac.get_num();
            mpz_class den = frac.get_den();

            if (integer % den == 0) {
                integer = (integer / den) * num;
                match_found = true;
                break; 
            }
        }
        current_step++;
    }

    if (!match_found) {
        halted = true;
    }
}

inline void Fractran::printSequence() {
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



