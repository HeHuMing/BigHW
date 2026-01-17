/* 2353626 ºØºúÃù ´óÊý¾Ý */
#pragma once
#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include "line.h"
using namespace std;

const char Space[4] = { ' ','\t','\r','\n' };

string trim(const LINE& str, const string trim_type, const bool not_ignore_linefeed);
bool all_space(const string& str);
bool is_space(const char& ch);
void print_hex(const LINE& line, ostringstream& oss);