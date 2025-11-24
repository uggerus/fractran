#include "fractran.h"

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
  test.runMachine(100);
  test.printSequence();

}

//compile: g++ prime-fractran.cpp -lgmp -lgmpxx
