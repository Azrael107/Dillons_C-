#pragma once

#include <iostream>

using namespace std;

#define NUM_REGISTERS 4
#define MENU_WIDTH 140

#ifndef CLAUDIA_CALC_EXAMPLE_H
#define CLAUDIA_CALC_EXAMPLE_H


namespace claudia_calc {
enum reg_name { A = 0, B, C, D };
enum operation : char { PLUS = '+', MINUS = '-', MULTIPLY = '*', DIVIDE = '/' };
/*
 * STUDENTS CAN DEFINE ANY DATA STRUCTURES OR DEFINTIONS THEY MAY NEED HERE
 */

}  // namespace claudia_calc
#endif