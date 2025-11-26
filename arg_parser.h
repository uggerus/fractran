#ifndef ARG_PARSER_H
#define ARG_PARSER_H

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <filesystem>
#include <gmpxx.h>

namespace fs = std::filesystem;

// Struct to hold the result of parsing
struct FractranConfig {
    std::vector<mpq_class> program;
    mpz_class input;
    int steps = 1000;
    bool success = true;
    std::string errorMessage;
};

// --- Helper: Check if string is a pure integer ---
inline bool isInteger(const std::string& s) {
    return !s.empty() && std::all_of(s.begin(), s.end(), ::isdigit);
}

// --- Helper: Parse a file ---
inline bool parseFileContent(const std::string& filepath, std::vector<mpq_class>& out_prog, std::string& out_input) {
    std::ifstream file(filepath);
    if (!file.is_open()) return false;

    std::string line;
    while (std::getline(file, line)) {
        // Strip comments
        size_t comment_pos = line.find('#');
        if (comment_pos != std::string::npos) {
            line = line.substr(0, comment_pos);
        }

        std::stringstream ss(line);
        std::string segment;
        while (ss >> segment) {
            if (segment == "Input:") {
                if (ss >> segment) out_input = segment;
            } else {
                try {
                    mpq_class frac(segment);
                    if (frac.get_den() != 0) out_prog.push_back(frac);
                } catch (...) { /* Ignore garbage */ }
            }
        }
    }
    return true;
}

// --- MAIN PARSING LOGIC ---
inline FractranConfig parseFractranArgs(const std::vector<std::string>& args) {
    FractranConfig config;
    
    if (args.empty()) {
        config.success = false;
        config.errorMessage = "No arguments provided.";
        return config;
    }

    std::string input_str;
    bool steps_set = false;

    // STRATEGY 1: Check if first arg is a file
    if (fs::exists(args[0]) && fs::is_regular_file(args[0])) {
        if (!parseFileContent(args[0], config.program, input_str)) {
            config.success = false;
            config.errorMessage = "Failed to read file: " + args[0];
            return config;
        }

        // Handle overrides (Input or Steps)
        size_t next_arg_idx = 1;
        if (next_arg_idx < args.size()) {
            std::string arg = args[next_arg_idx];
            if (isInteger(arg)) {
                if (input_str.empty()) {
                    input_str = arg; // Input wasn't in file, so this is Input
                    next_arg_idx++;
                } else {
                    // Input WAS in file.
                    // If there's another arg after this, this is an Input Override.
                    // If this is the last arg, it is Steps.
                    if (next_arg_idx + 1 < args.size()) {
                         input_str = arg; // Override
                         next_arg_idx++;
                    } else {
                         config.steps = std::stoi(arg);
                         steps_set = true;
                         next_arg_idx++;
                    }
                }
            }
        }

        if (!steps_set && next_arg_idx < args.size()) {
             if(isInteger(args[next_arg_idx])) {
                 config.steps = std::stoi(args[next_arg_idx]);
             }
        }

    } else {
        // STRATEGY 2: Command Line List
        for (const auto& arg : args) {
            if (arg.find('/') != std::string::npos) {
                try {
                    // Remove commas if present
                    std::string clean_arg = arg;
                    clean_arg.erase(std::remove(clean_arg.begin(), clean_arg.end(), ','), clean_arg.end());
                    config.program.push_back(mpq_class(clean_arg));
                } catch (...) {}
            } else if (isInteger(arg)) {
                if (input_str.empty()) {
                    input_str = arg;
                } else if (!steps_set) {
                    config.steps = std::stoi(arg);
                    steps_set = true;
                }
            }
        }
    }

    // Validation
    if (config.program.empty()) {
        config.success = false;
        config.errorMessage = "No valid fractions found.";
        return config;
    }
    
    if (input_str.empty()) {
        config.success = false;
        config.errorMessage = "No start integer found.";
        return config;
    }

    try {
        config.input = mpz_class(input_str);
    } catch (...) {
        config.success = false;
        config.errorMessage = "Invalid integer format.";
    }

    return config;
}

#endif
