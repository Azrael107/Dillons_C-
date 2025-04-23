#define __cplusplus 201703L
#include "claudia_calc_example.h"
#include <sys/types.h>
#include <fmt/core.h>
#include <iostream>
#include <map>
#include <variant>
#include <iomanip>
#include <sstream>
#include "spdlog/spdlog.h"
using namespace std;

namespace claudia_calc {

    // Define a variant type to hold either double or string values
    using RegisterValue = variant<double, string>;
    
    // Define a map to store register values
    std::map<reg_name, RegisterValue> registers = {
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

// Helper function to format values to 3 decimal places or show strings
string format_value(const RegisterValue& val) {
    if (holds_alternative<double>(val)) {
        double num = get<double>(val);
        ostringstream oss;
        oss << fixed << setprecision(3) << num;
        string s = oss.str();
        // Remove trailing zeros and . if not needed
        s.erase(s.find_last_not_of('0') + 1, string::npos);
        if (s.back() == '.') {
            s.pop_back();
        }
        return s + " (number)";
    } else {
        return "\"" + get<string>(val) + "\" (string)";
    }
}

/*
 * calculator functions
 */

inline void print_line() { cout << std::string(MENU_WIDTH, '-') << endl; }

inline void print_title(string const title) { cout << fmt::format("{:^{}}", title, MENU_WIDTH) << endl; }

void print_menu() {
    print_line();
    print_title("ClaudiaCalc v2.0");
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
        cout << "Register " << to_char(reg) << ": " << format_value(value) << endl;
    }
}

// Helper function to parse input (could be number or string)
// Helper function to parse input (could be number or string)
RegisterValue parse_input(const string& input) {
    try {
        size_t pos;
        double num = stod(input, &pos);
        if (pos == input.length()) {
            // Entire string was parsed as a number
            return RegisterValue{in_place_index<0>, num};  // Explicit construction for double
        }
    } catch (const std::exception&) {
        // Not a number, treat as string
    }
    return RegisterValue{in_place_index<1>, input};  // Explicit construction for string
}

void perform_operation(char op, const RegisterValue& val1, const RegisterValue& val2) {
    if (holds_alternative<double>(val1) && holds_alternative<double>(val2)) {
        double num1 = get<double>(val1);
        double num2 = get<double>(val2);
        
        switch (op) {
            case '+': cout << num1 + num2 << endl; break;
            case '-': cout << num1 - num2 << endl; break;
            case '*': cout << num1 * num2 << endl; break;
            case '/': 
                if (num2 == 0) {
                    cout << "Error: Division by zero" << endl;
                } else {
                    cout << num1 / num2 << endl;
                }
                break;
        }
    } else {
        string str1 = holds_alternative<string>(val1) ? get<string>(val1) : to_string(get<double>(val1));
        string str2 = holds_alternative<string>(val2) ? get<string>(val2) : to_string(get<double>(val2));
        
        switch (op) {
            case '+': cout << str1 + str2 << endl; break;
            case '-': 
                if (holds_alternative<string>(val1) && holds_alternative<string>(val2)) {
                    size_t pos = str1.find(str2);
                    if (pos != string::npos) {
                        cout << str1.erase(pos, str2.length()) << endl;
                    } else {
                        cout << str1 << endl;
                    }
                } else {
                    cout << "Error: Invalid operation between string and number" << endl;
                }
                break;
            case '*': 
                if (holds_alternative<double>(val1) && holds_alternative<string>(val2)) {
                    int count = static_cast<int>(get<double>(val1));
                    if (count <= 0) {
                        cout << "" << endl;
                    } else {
                        string result;
                        for (int i = 0; i < count; ++i) {
                            result += str2;
                        }
                        cout << result << endl;
                    }
                } else if (holds_alternative<string>(val1) && holds_alternative<double>(val2)) {
                    int count = static_cast<int>(get<double>(val2));
                    if (count <= 0) {
                        cout << "" << endl;
                    } else {
                        string result;
                        for (int i = 0; i < count; ++i) {
                            result += str1;
                        }
                        cout << result << endl;
                    }
                } else {
                    cout << "Error: Invalid string multiplication" << endl;
                }
                break;
            case '/': 
                cout << "Error: Cannot divide strings" << endl;
                break;
        }
    }
}

void execute(string const cmd) {
    if (cmd.empty()) {
        cout << "Error: Empty command" << endl;
        return;
    }

    char cmd_ch = tolower(cmd[0]);

    switch (cmd_ch) {
        case 'a':
        case 'b':
        case 'c':
        case 'd': {
            string input;
            cout << "Enter a number or string for register " << cmd_ch << ": ";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            getline(cin, input);
            
            if (input.empty()) {
                cout << "Error: Empty input" << endl;
                return;
            }
            
            registers[to_reg_name(cmd_ch)] = parse_input(input);
            cout << "Register " << cmd_ch << " set to " << format_value(registers[to_reg_name(cmd_ch)]) << endl;
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
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return;
            }
            if (!is_register(reg1) || !is_register(reg2)) {
                cout << "Error: Invalid registers" << endl;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return;
            }
            
            cout << to_char(to_reg_name(reg1)) << " " << cmd_ch << " " 
                 << to_char(to_reg_name(reg2)) << " = ";
            perform_operation(cmd_ch, registers[to_reg_name(reg1)], registers[to_reg_name(reg2)]);
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

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void start() {
    string cmd = "";
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
    std::cout << "Author: Dillonphan" << std::endl;
    std::cout << "Email: dillonphan408@gmail.com" << std::endl;
    start();
    return 0;
}