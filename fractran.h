#include "gmpxx.h"
#include <iostream>
#include <vector>

class Fractran {
public:
  Fractran(std::vector<mpq_class> fractions, mpz_class num) {
    fractionList = fractions;
    integer = num;
    halted = false;
  }
  void runMachine(int steps);
  void printSequence();


private:
  std::vector<mpq_class> fractionList;
  mpz_class integer;
  std::vector<mpz_class> numberList;
  bool halted;
};

void Fractran::runMachine(int steps) {
  bool loop_cond {true};
  for(auto i = 0; i < steps && loop_cond; ++i){
    numberList.push_back(integer);
    loop_cond = false;
    for(mpq_class i: fractionList) {
      mpq_class result = integer * i;
      if(result.get_den() == 1) {
	integer = result;
	loop_cond = true;
	break;
      }
    }
  }
  //halted = loop_cond;
}

void Fractran::printSequence() {
  for(auto i: numberList) {
    std::cout << i << ", ";
  }
  std::cout << "END\n";
}



