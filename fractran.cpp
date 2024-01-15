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



int main() {
  std::vector<mpq_class> conwayPrimeFractions;
  conwayPrimeFractions.push_back({17, 91});
  conwayPrimeFractions.push_back({78, 85});
  conwayPrimeFractions.push_back({19, 51});
  conwayPrimeFractions.push_back({23,38});
  conwayPrimeFractions.push_back({29, 33});
  conwayPrimeFractions.push_back({77, 29});
  conwayPrimeFractions.push_back({95, 23});
  conwayPrimeFractions.push_back({77, 19});
  conwayPrimeFractions.push_back({1, 17});
  conwayPrimeFractions.push_back( {11, 13});
  conwayPrimeFractions.push_back({13, 11});
  conwayPrimeFractions.push_back({15, 2});
  conwayPrimeFractions.push_back({1, 7});
  conwayPrimeFractions.push_back( {55,1});

  mpz_class number = 2;

  Fractran test {Fractran(conwayPrimeFractions, number)};
  test.runMachine();

  



}
