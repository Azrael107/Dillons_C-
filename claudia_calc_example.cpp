#include "claudia_calc_example.h"

#include <sys/types.h>

#include <fmt/core.h>

#include <iostream>

#include <map>

#include "spdlog/spdlog.h"
using namespace std;

namespace claudia_calc {

    // Define a map to store register values
    std::map<reg_name, double> registers = {
        {A, 0.0},
        {B, 0.0},
        {C, 0.0},
        {D, 0.0}
    };

/*
 * utility functions
 */

inline bool is_register(char const ch) { return tolower(ch) >= 'a' && ch <= 'd'; }

inline bool is_register(string const str) { return str.size() != 0 && is_register(str[0]); }

inline reg_name to_reg_name(char const ch) {
    assert(is_register(ch));
    return static_cast<reg_name>(tolower(ch) - 'a');
}

inline reg_name to_reg_name(string const str) {
    assert(is_register(str));
    return to_reg_name(str[0]);
}

inline char to_char(reg_name rn) { return static_cast<char>(rn + 'a'); }

/*
 * calculator functions
 */

inline void print_line() { cout << std::string(MENU_WIDTH, '-') << endl; }

inline void print_title(string const title) { cout << fmt::format("{:^{}}", title, MENU_WIDTH) << endl; }

void print_menu() {
    print_line();
    print_title("ClaudiaCalc");
    print_line();
    cout << "+\tAdd" << endl;
    cout << "-\tSubtract" << endl;
    cout << "*\tMultiply" << endl;
    cout << "/\tDivide" << endl;
    cout << "a-d\tEnter a number or string for A,B,C,D" << endl;
    cout << "1-4\tClear register A,B,C,D" << endl;
    cout << "m\tPrints the menu" << endl;
    cout << "p\tPrints the registers" << endl;
    cout << "q\tQuits the app" << endl;
    print_line();
}

void print_registers() {
    for (const auto& [reg, value] : registers) {
        spdlog::info("Register {}: {}", to_char(reg), value);
    }
}

void execute(string const cmd) {
    // validate command size
    if (cmd.size() == 0) {
        spdlog::error("Empty command");
        return;
    }
    // lower annd get first char of command
    char const cmd_ch = std::tolower(cmd[0]);

    switch (cmd_ch) {
        case 'a':
            spdlog::error("cmd={} not implemented", cmd_ch);
            break;
        case 'b':
            spdlog::error("cmd={} not implemented", cmd_ch);
            break;
        case 'c':
            spdlog::error("cmd={} not implemented", cmd_ch);
            break;
        case 'd':
            spdlog::error("cmd={} not implemented", cmd_ch);
            break;
        case '+':
            spdlog::error("cmd={} not implemented", cmd_ch);
            break;
        case '-':
            spdlog::error("cmd={} not implemented", cmd_ch);
            break;
        case '*':
            spdlog::error("cmd={} not implemented", cmd_ch);
            break;
        case '/':
            spdlog::error("cmd={} not implemented", cmd_ch);
            break;
        case '1':
            spdlog::error("cmd={} not implemented", cmd_ch);
            break;
        case '2':
            spdlog::error("cmd={} not implemented", cmd_ch);
            break;
        case '3':
            spdlog::error("cmd={} not implemented", cmd_ch);
            break;
        case '4':
            spdlog::error("cmd={} not implemented", cmd_ch);
            break;
        case 'm':
            print_menu();//spdlog::error("cmd={} not implemented", cmd_ch);
            break;
        case 'p':
            print_registers();//spdlog::error("cmd={} not implemented", cmd_ch);
            break;
        case 'q':
            break;
        default:
            spdlog::error("{} is an unknown command", cmd_ch);
            break;
    }
}

// start the calculator
void start() {
    string cmd = "";
    // print menu
    print_menu();
    while (cmd != "q") {
        cout << "Enter a command: ";
        cin >> cmd;
        spdlog::debug("cmd={}", cmd);
        execute(cmd);
    }
}
}  // namespace claudia_calc

using namespace claudia_calc;

int main() {
    spdlog::set_level(spdlog::level::debug);
    start();

    return 0;
}
