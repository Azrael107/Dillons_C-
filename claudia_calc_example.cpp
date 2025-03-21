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
        cout << "Register " << to_char(reg) << ": " << value << endl;
    }
}

void execute(string const cmd) {
    if (cmd.empty()) {
        cout << "Error: Empty command" << endl;
        return;
    }

    char cmd_ch = std::tolower(cmd[0]);

    switch (cmd_ch) {
        case 'a':
        case 'b':
        case 'c':
        case 'd': {
            double value;
            cout << "Enter a number for register " << cmd_ch << ": ";
            if (!(cin >> value)) {
                cout << "Error: Invalid input" << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Flush input buffer
                return;
            }
            registers[to_reg_name(cmd_ch)] = value;
            cout << "Register " << cmd_ch << " set to " << value << endl;
            break;
        }
        case '+':
        case '-':
        case '*':
        case '/': {
            char reg1, reg2;
            cout << "Enter two registers (e.g., A B): ";
            if (!(cin >> reg1 >> reg2)) {
                cout << "Error: Invalid input" << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Flush input buffer
                return;
            }
            if (!is_register(reg1) || !is_register(reg2)) {
                cout << "Error: Invalid registers" << endl;
                return;
            }
            double val1 = registers[to_reg_name(reg1)];
            double val2 = registers[to_reg_name(reg2)];

            if (cmd_ch == '/' && val2 == 0) {
                cout << "Error: Division by zero" << endl;
                return;
            }

            double result = (cmd_ch == '+') ? (val1 + val2)
                           : (cmd_ch == '-') ? (val1 - val2)
                           : (cmd_ch == '*') ? (val1 * val2)
                           : (val1 / val2);
            cout << reg1 << " " << cmd_ch << " " << reg2 << " = " << result << endl;
            break;
        }
        case '1':
        case '2':
        case '3':
        case '4': {
            reg_name reg = static_cast<reg_name>(cmd_ch - '1');
            registers[reg] = 0.0;
            cout << "Cleared register " << to_char(reg) << endl;
            break;
        }
        case 'm':
            print_menu();
            break;
        case 'p':
            print_registers();
            break;
        case 'q':
            return;
        default:
            cout << "Error: Unknown command '" << cmd_ch << "'" << endl;
            break;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Ensure input buffer is cleared
}

// start the calculator
void start() {
    string cmd = "";
    // print menu
    print_menu();
    while (cmd != "q") {
        cout << "Enter a command: ";
        cin >> cmd;
        execute(cmd);
    }
}
}  // namespace claudia_calc

using namespace claudia_calc;

int main() {
    start();
    return 0;
}