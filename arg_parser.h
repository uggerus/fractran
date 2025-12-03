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

struct FractranConfig {
    std::vector<mpq_class> program;
    mpz_class input;
    int steps = 1000;
    bool success = true;
    std::string errorMessage;
};

inline bool isInteger(const std::string& s) {
    return !s.empty() && std::all_of(s.begin(), s.end(), ::isdigit);
}

inline bool hasSuffix(const std::string& str, const std::string& suffix) {
    return str.size() >= suffix.size() &&
           str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

// UPDATED: Now accepts out_steps by reference
inline bool parseFileContent(const std::string& filepath, std::vector<mpq_class>& out_prog, std::string& out_input, int& out_steps) {
    std::ifstream file(filepath);
    if (!file.is_open()) return false;

    std::string line;
    while (std::getline(file, line)) {
        size_t comment_pos = line.find('#');
        if (comment_pos != std::string::npos) {
            line = line.substr(0, comment_pos);
        }

        std::stringstream ss(line);
        std::string segment;
        while (ss >> segment) {
            if (segment == "Input:") {
                if (ss >> segment) out_input = segment;
            } else if (segment == "Steps:") { // NEW: Parse Steps from file
                if (ss >> segment && isInteger(segment)) {
                     try { out_steps = std::stoi(segment); } catch(...) {}
                }
            } else {
                try {
                    mpq_class frac(segment);
                    if (frac.get_den() != 0) out_prog.push_back(frac);
                } catch (...) { }
            }
        }
    }
    return true;
}

inline FractranConfig parseFractranArgs(const std::vector<std::string>& args) {
    FractranConfig config;
    
    if (args.empty()) {
        config.success = false;
        config.errorMessage = "No arguments provided.";
        return config;
    }

    std::string input_str;
    bool steps_set_by_cli = false;
    std::string target_file;
    int file_steps = -1; // Sentinel value

    // Strategy 1: Check for .frac File
    if (hasSuffix(args[0], ".frac") && fs::exists(args[0])) {
        target_file = args[0];
    } else if (fs::exists(args[0] + ".frac")) {
        target_file = args[0] + ".frac";
    }

    if (!target_file.empty()) {
        // --- FILE MODE ---
        if (!parseFileContent(target_file, config.program, input_str, file_steps)) {
            config.success = false;
            config.errorMessage = "Failed to read file: " + target_file;
            return config;
        }

        // Apply file steps if found (can be overridden later by CLI)
        if (file_steps > 0) {
            config.steps = file_steps;
        }

        // Handle overrides
        size_t next_arg_idx = 1;
        if (next_arg_idx < args.size()) {
            std::string arg = args[next_arg_idx];
            if (isInteger(arg)) {
                if (input_str.empty()) {
                    input_str = arg; // Must be input
                    next_arg_idx++;
                } else {
                    if (next_arg_idx + 1 < args.size()) {
                         input_str = arg; // Override Input
                         next_arg_idx++;
                    } else {
                         config.steps = std::stoi(arg); // Override Steps
                         steps_set_by_cli = true;
                         next_arg_idx++;
                    }
                }
            }
        }

        // Final check for steps override
        if (!steps_set_by_cli && next_arg_idx < args.size()) {
             if(isInteger(args[next_arg_idx])) {
                 config.steps = std::stoi(args[next_arg_idx]);
             }
        }

    } else {
        // --- CLI LIST MODE ---
        for (const auto& arg : args) {
            if (arg.find('/') != std::string::npos) {
                try {
                    std::string clean_arg = arg;
                    clean_arg.erase(std::remove(clean_arg.begin(), clean_arg.end(), ','), clean_arg.end());
                    config.program.push_back(mpq_class(clean_arg));
                } catch (...) {}
            } else if (isInteger(arg)) {
                if (input_str.empty()) {
                    input_str = arg;
                } else if (!steps_set_by_cli) {
                    config.steps = std::stoi(arg);
                    steps_set_by_cli = true;
                }
            }
        }
    }

    if (config.program.empty()) {
        config.success = false;
        config.errorMessage = "No valid fractions found (or file not found).";
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
