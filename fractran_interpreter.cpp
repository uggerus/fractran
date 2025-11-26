#include <iostream>
#include <vector>
#include <string>
#include "fractran.h"
#include "arg_parser.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <fractions...> <input> [steps]\n";
        std::cout << "       " << argv[0] << " <file> [input_override] [steps]\n";
        return 1;
    }

    // Convert argv to vector<string> (skipping argv[0])
    std::vector<std::string> args;
    for (int i = 1; i < argc; ++i) {
        args.push_back(argv[i]);
    }

    // Parse
    FractranConfig config = parseFractranArgs(args);

    if (!config.success) {
        std::cerr << "Error: " << config.errorMessage << std::endl;
        return 1;
    }

    // Execution
    std::cout << "--- FRACTRAN Interpreter ---" << std::endl;
    std::cout << "Fractions: " << config.program.size() << std::endl;
    std::cout << "Input:     " << config.input << std::endl;
    std::cout << "Max Steps: " << config.steps << std::endl;
    std::cout << "----------------------------" << std::endl;

    Fractran machine(config.program, config.input, true);
    machine.runMachine(config.steps);
    machine.printSequence();
    
    std::cout << "Total Steps: " << machine.getStepCount() << std::endl;

    return 0;
}
