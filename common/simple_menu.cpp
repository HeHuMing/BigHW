/* 2353626 贺胡鸣 大数据 */
#include <iostream>
#include "../include/simple_menu.h"
using namespace std;

// 版本1：处理以NULL结尾的指针数组
void simple_menu(const char* const menu[], const char* valid_chars) 
{
	char choice;
	while (1) {
		cct_cls();
		cout << "---------------------------------" << endl;
		for (int i = 0; menu[i]; i++)
			cout << menu[i] << endl;
		cout << "---------------------------------" << endl;
		choice = _getche();
		if (strchr(valid_chars, choice) != NULL)
			break;
	}
}

// 版本2：处理以空串结尾的二维字符数组
template <size_t N>
void simple_menu(const char menu[][N], const char* valid_chars)
{
	char choice;
	while (1) {
		cct_cls();
		cout << "---------------------------------" << endl;
		for (int i = 0; menu[i][0] != '\0'; i++) {
			cout << menu[i] << endl;
		}
		cout << "---------------------------------" << endl;
		choice = _getche();
		if (strchr(valid_chars, choice) != NULL)
			break;
	}
}
template void simple_menu<80>(const char menu[][80], const char* valid_chars);
template void simple_menu<120>(const char menu[][120], const char* valid_chars);
template void simple_menu<50>(const char menu[][50], const char* valid_chars);

// 版本3：处理多行字符串
void simple_menu(const char* menu, const char* valid_chars) 
{
	char choice;
	while (1) {
		cct_cls();
		cout << "---------------------------------" << endl;
		cout << menu << endl;
		cout << "---------------------------------" << endl;
		choice = _getche();
		if (strchr(valid_chars, choice) != NULL)
			break;
	}
}