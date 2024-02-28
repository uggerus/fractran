#include "gmpxx.h"
#include <iostream>
#include <vector>

class Fractran {
public:
  Fractran(std::vector<mpq_class> fractions, mpz_class num) : fractionList{fractions}, integer{num} {}
  void runMachine();


private:
  std::vector<mpq_class> fractionList;
  mpz_class integer;
};

void Fractran::runMachine() {
  bool loop_cond {true};
  while(loop_cond){
    std::cout << integer << '\n';
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
}



