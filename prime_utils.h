#ifndef PRIME_UTILS_H
#define PRIME_UTILS_H

#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <iostream>
#include <gmpxx.h>

// Manages the mapping between Prime Numbers and Vector Indices
// Example: 2->0, 3->1, 5->2, 7->3
class PrimeRegistry {
public:
    // Register a prime and get its index
    int getIndex(int prime) {
        if (primeToIdx.find(prime) == primeToIdx.end()) {
            int idx = primes.size();
            primes.push_back(prime);
            primeToIdx[prime] = idx;
        }
        return primeToIdx[prime];
    }

    int getPrime(int idx) const {
        return primes[idx];
    }

    size_t size() const { return primes.size(); }

    // Factorize a GMP integer into our vector format
    // Returns: The vector of exponents, and any "residue" (factors we don't track)
    std::pair<std::vector<int>, mpz_class> factorize(mpz_class n) {
        std::vector<int> exponents(primes.size(), 0);
        
        for (size_t i = 0; i < primes.size(); ++i) {
            int p = primes[i];
            while (n % p == 0) {
                exponents[i]++;
                n /= p;
            }
        }
        return {exponents, n};
    }

    // Reconstruct GMP integer from vector
    mpz_class reconstruct(const std::vector<int>& exponents, mpz_class residue) {
        mpz_class result = residue;
        for (size_t i = 0; i < exponents.size(); ++i) {
            if (exponents[i] > 0) {
                mpz_class p_pow;
                mpz_ui_pow_ui(p_pow.get_mpz_t(), primes[i], exponents[i]);
                result *= p_pow;
            }
        }
        return result;
    }

private:
    std::vector<int> primes;
    std::map<int, int> primeToIdx;
};

#endif
