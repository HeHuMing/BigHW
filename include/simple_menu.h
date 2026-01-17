/* 2353626 贺胡鸣 大数据 */
#pragma once
#include <conio.h>
#include <cctype>
#include <cstring>
#include "cmd_console_tools.h"

/* simple_menu使用函数重载 */
void simple_menu(const char* const menu[], const char* valid_chars);

template <size_t N>
void simple_menu(const char menu[][N], const char* valid_chars);
// 提前声明可能用得到的版本，比如80，120，50...
extern template void simple_menu<80>(const char menu[][80], const char* valid_chars);
extern template void simple_menu<120>(const char menu[][120], const char* valid_chars);
extern template void simple_menu<50>(const char menu[][50], const char* valid_chars);

void simple_menu(const char* menu, const char* valid_chars);

