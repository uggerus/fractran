#include <iostream>
#include <fstream>
#include <cassert>
#include <filesystem>
#include "arg_parser.h"

namespace fs = std::filesystem;

void pass(std::string name) {
    std::cout << "[PASS] " << name << std::endl;
}

void test_cli_simple() {
    // Scenario: ./fractran 3/2 5
    // Should parse: Prog=[3/2], Input=5, Steps=1000 (default)
    std::vector<std::string> args = {"3/2", "5"};
    FractranConfig conf = parseFractranArgs(args);

    assert(conf.success);
    assert(conf.program.size() == 1);
    assert(conf.program[0] == mpq_class(3, 2));
    assert(conf.input == 5);
    assert(conf.steps == 1000);
    pass("CLI Simple (3/2 5)");
}

void test_cli_mixed_order() {
    // Scenario: ./fractran 5 3/2 20
    // Logic: 3/2 is frac. First int (5) is Input. Second int (20) is Steps.
    std::vector<std::string> args = {"5", "3/2", "20"};
    FractranConfig conf = parseFractranArgs(args);

    assert(conf.success);
    assert(conf.program.size() == 1);
    assert(conf.input == 5);
    assert(conf.steps == 20);
    pass("CLI Mixed Order (5 3/2 20)");
}

void test_cli_multiple_fractions() {
    // Scenario: ./fractran 1/2 3/4 5/6 10
    std::vector<std::string> args = {"1/2", "3/4", "5/6", "10"};
    FractranConfig conf = parseFractranArgs(args);

    assert(conf.program.size() == 3);
    assert(conf.program[1] == mpq_class(3, 4));
    assert(conf.input == 10);
    pass("CLI Multiple Fractions");
}

void test_file_parsing() {
    // 1. Create a temp file
    std::string filename = "temp_test_prog.frac";
    std::ofstream out(filename);
    out << "# Comment line\n";
    out << "3/2 5/4 # Inline comment\n";
    out << "Input: 100\n";
    out.close();

    // 2. Test parsing
    std::vector<std::string> args = {filename};
    FractranConfig conf = parseFractranArgs(args);

    // 3. Cleanup
    fs::remove(filename);

    // 4. Assertions
    assert(conf.success);
    assert(conf.program.size() == 2);
    assert(conf.program[0] == mpq_class(3, 2));
    assert(conf.input == 100);
    pass("File Parsing with Input inside");
}

void test_file_override() {
    // 1. Create a temp file (Has Input: 100)
    std::string filename = "temp_override.frac";
    std::ofstream out(filename);
    out << "3/2\nInput: 100\n";
    out.close();

    // 2. Test Override Input: ./fractran file 200
    // Should ignore 100 and use 200.
    std::vector<std::string> args = {filename, "200", "1000"};
    FractranConfig conf = parseFractranArgs(args);

    // 3. Cleanup
    fs::remove(filename);

    assert(conf.success);
    assert(conf.input == 200); 
    assert(conf.steps == 1000); // Default steps
    pass("File Input Override");
}

void test_file_override_steps() {
    // 1. Create a temp file (Has Input: 100)
    std::string filename = "temp_steps.frac";
    std::ofstream out(filename);
    out << "3/2\nInput: 100\n";
    out.close();

    // 2. Test Steps Override: ./fractran file 50
    // Logic: Input is in file. Only 1 arg remains. It must be Steps.
    std::vector<std::string> args = {filename, "50"};
    FractranConfig conf = parseFractranArgs(args);

    // 3. Cleanup
    fs::remove(filename);

    assert(conf.success);
    assert(conf.input == 100); // Kept from file
    assert(conf.steps == 50);  // Overridden
    pass("File Steps Override (Input in file)");
}

void test_file_embedded_steps() {
    // Scenario: File contains "Steps: 500"
    std::string filename = "temp_embedded_steps.frac";
    std::ofstream out(filename);
    out << "3/2\nInput: 10\nSteps: 500\n";
    out.close();

    std::vector<std::string> args = {filename};
    FractranConfig conf = parseFractranArgs(args);

    fs::remove(filename);
    assert(conf.success);
    assert(conf.input == 10);
    assert(conf.steps == 500); // Should read 500 from file, not default 1000
    pass("File Parsing (Embedded 'Steps:')");
}

void test_file_steps_priority() {
    // Scenario: File says "Steps: 500", but CLI says "50"
    // Expectation: CLI wins
    std::string filename = "temp_steps_priority.frac";
    std::ofstream out(filename);
    out << "3/2\nInput: 10\nSteps: 500\n";
    out.close();

    // Call: ./fractran file 50
    std::vector<std::string> args = {filename, "50"};
    FractranConfig conf = parseFractranArgs(args);

    fs::remove(filename);
    assert(conf.success);
    assert(conf.steps == 50); // CLI (50) overrides File (500)
    pass("File Parsing (CLI Steps override Embedded Steps)");
}

int main() {
    std::cout << "--- Testing Argument Parser ---\n";
    test_cli_simple();
    test_cli_mixed_order();
    test_cli_multiple_fractions();
    test_file_parsing();
    test_file_override();
    test_file_override_steps();
    test_file_embedded_steps();
    test_file_steps_priority();
    std::cout << "-------------------------------\n";
    std::cout << "All Argument tests passed.\n";
    return 0;
}
