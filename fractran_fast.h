#ifndef FRACTRAN_FAST_H
#define FRACTRAN_FAST_H

#include <vector>
#include "prime_utils.h"

// A "compiled" fraction optimized for vector operations
struct FastFraction {
    int id;
    
    // Requirements: To run, state[idx] must be >= req[idx]
    // We only store the indices that matter to save time
    std::vector<std::pair<int, int>> requirements;

    // Operations: state[idx] += delta
    // Example: 2/3 means reg[1]-- and reg[0]++
    std::vector<std::pair<int, int>> deltas;

    // Optimization: Which fractions should we check after this one runs?
    // If this fraction adds to register 5, we only need to check 
    // fractions that have 5 in their denominator (and come before this one).
    std::vector<int> nextCandidates;
};

class FractranFast {
public:
    FractranFast(const std::vector<mpq_class>& raw_fractions, mpz_class input) {
        stepCount = 0;
        halted = false;

        // 1. Scan fractions to find all relevant primes
        for (const auto& f : raw_fractions) {
            scanPrimes(f.get_num());
            scanPrimes(f.get_den());
        }

        // 2. Compile fractions into Vector Ops
        for (size_t i = 0; i < raw_fractions.size(); ++i) {
            compileFraction(i, raw_fractions[i]);
        }

        // 3. Build Dependency Graph (The "Transition Elimination" optimization)
        buildDependencyGraph();

        // 4. Convert Input Integer to State Vector
        auto result = registry.factorize(input);
        state = result.first;
        residue = result.second;
    }

    void run(int max_steps) {
        if (halted) return;

        // The "Candidate List" controls which fractions we check.
        // Initially (or after a non-match), we must check everyone (0 to N).
        // But after a match, we might be able to check a smaller list.
        const std::vector<int>* candidates = &allFractions;

        for (int s = 0; s < max_steps; ++s) {
            bool match = false;

            // Iterate ONLY through the candidates
            for (int fracIdx : *candidates) {
                const auto& frac = ops[fracIdx];
                
                // 1. Check Requirements (Is divisible?)
                bool possible = true;
                for (const auto& req : frac.requirements) {
                    if (state[req.first] < req.second) {
                        possible = false;
                        break;
                    }
                }

                if (possible) {
                    // 2. Execute Move (Vector Addition)
                    for (const auto& d : frac.deltas) {
                        state[d.first] += d.second;
                    }

                    // 3. Update Loop State
                    stepCount++;
                    match = true;

                    // OPTIMIZATION: Next step, only check fractions that 
                    // depend on the primes we just added.
                    candidates = &frac.nextCandidates;
                    break; 
                }
            }

            if (!match) {
                halted = true;
                return;
            }
        }
    }

    mpz_class getNumber() {
        return registry.reconstruct(state, residue);
    }

    unsigned long long getSteps() const { return stepCount; }

private:
    PrimeRegistry registry;
    std::vector<int> state; // The Registers
    mpz_class residue;      // Primes in input not tracked by program
    std::vector<FastFraction> ops;
    unsigned long long stepCount;
    bool halted;

    // Cache of "Check everything 0..N"
    std::vector<int> allFractions; 

    // Helper: Find primes in a number and register them
    void scanPrimes(mpz_class n) {
        if (n == 0) return; // Should not happen
        if (n == 1) return;
        
        // Simple trial division for scanning (only runs once at startup)
        // For real-world use, this handles small primes (up to ~1000) fine.
        mpz_class temp = n;
        for (int i = 2; i * i <= temp; ++i) {
            if (temp % i == 0) {
                registry.getIndex(i);
                while (temp % i == 0) temp /= i;
            }
        }
        if (temp > 1) {
            // It's a prime (or a large prime factor)
            registry.getIndex(temp.get_si());
        }
    }

    void compileFraction(int id, mpq_class f) {
        FastFraction op;
        op.id = id;

        // Denominator -> Requirements & Subtractions
        auto den_factors = registry.factorize(f.get_den());
        for (size_t i = 0; i < den_factors.first.size(); ++i) {
            int count = den_factors.first[i];
            if (count > 0) {
                op.requirements.push_back({(int)i, count});
                op.deltas.push_back({(int)i, -count});
            }
        }

        // Numerator -> Additions
        auto num_factors = registry.factorize(f.get_num());
        for (size_t i = 0; i < num_factors.first.size(); ++i) {
            int count = num_factors.first[i];
            if (count > 0) {
                op.deltas.push_back({(int)i, count});
            }
        }
        
        ops.push_back(op);
        allFractions.push_back(id);
    }

    // OPTIMIZATION IMPLEMENTATION
    void buildDependencyGraph() {
        for (auto& source : ops) {
            // Default: If we execute 'source', who do we check next?
            // Standard rule: Check 0, 1, 2...
            
            // Optimized rule: Only check fractions 'k' if:
            // 1. k <= source.id (Standard Fractran priority)
            // 2. OR 'source' added a register that 'k' needs.
            
            // Wait, the PDF Logic is:
            // "Eliminate k if source didn't add to any of k's required registers."
            
            for (int k = 0; k < (int)ops.size(); ++k) {
                // We always assume we restart from the top (0) to current (source.id)
                // We actually don't check fractions AFTER source.id, because 
                // if source.id ran, we implicitly restart at 0.
                
                // We only store 0..source.id in the candidate list.
                // But we can prune this list!
                
                bool relevant = false;
                
                // If k is the very first fraction, we must always check it 
                // (unless we can prove we can't run it).
                
                // Logic: Does 'source' produce anything 'target' consumes?
                // OR: Was 'target' previously runnable but skipped? (No, because if it was runnable, we would have run it!)
                
                // Therefore: 'target' (k < source) can ONLY run if 'source' 
                // replenished a resource 'target' was missing.
                
                for (auto& delta : source.deltas) {
                    if (delta.second > 0) { // Source produces this
                        int reg = delta.first;
                        // Does target consume this?
                        for (auto& req : ops[k].requirements) {
                            if (req.first == reg) {
                                relevant = true;
                                break;
                            }
                        }
                    }
                    if (relevant) break;
                }
                
                // Always include self? Not necessarily, but safe to.
                if (k == source.id) relevant = true;

                if (relevant) {
                    source.nextCandidates.push_back(k);
                }
            }
            
            // Fallback: If pruning was too aggressive (or empty), just check everything 0..id
            if (source.nextCandidates.empty()) {
                 for(int i=0; i<=source.id; ++i) source.nextCandidates.push_back(i);
            }
        }
    }
};

#endif
