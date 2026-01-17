/* 2353626 贺胡鸣 大数据 */
#include "../90-01-b1/90-01-b1-hanoi.h"
#include "../include/common_menu.h"
using namespace std;
/* ----------------------------------------------------------------------------------

	 本文件功能：
	1、放被 hanoi_main.cpp 调用的菜单函数，要求显示各菜单项，读入正确的选项后返回

	 本文件要求：
	1、不允许定义外部全局变量（const及#define不在限制范围内）
	2、不允许定义静态全局变量（全局变量的使用准则是：少用、慎用、能不用尽量不用）
	3、静态局部变量的数量不限制，但使用准则也是：少用、慎用、能不用尽量不用
	4、按需加入系统头文件、自定义头文件、命名空间等

   ----------------------------------------------------------------------------------- */


   /***************************************************************************
	 函数名称：menu
	 功    能：菜单的显示与选择
	 输入参数：const char* const* items, bool case_sensitive = false
	 返 回 值：char选择
	 说    明：此函数的要解决的是选项为数字/字母时的通用性问题，要进一步拓展思路，考虑一个程序的菜单中同时包含数字、字母、字母区分大小写、其它字符等各种情况，唯一的共同点就是按单个字符选择某项） 
   ***************************************************************************/
char menu(const char* const* items, int opt)
{
    cct_cls();
    cout << "---------------------------------" << endl;

    // 显示菜单项
    for (int i = 0; items[i] != nullptr; i++) {
        cout << items[i] << endl;
    }

    cout << "---------------------------------" << endl;
    cout << "[请选择:] ";

    char choice;
    while (1) {
        choice = _getch();
        if (opt == 1) {
            if (choice >= '0' && choice <= '9') {
                putchar(choice);
                break;
            }
            else
                continue;
        }
        if (opt == 2) {
            if (choice >= 'A' && choice <= 'K' || choice == 'Q') {
                putchar(choice);
                break;
            }
            else if (choice >= 'a' && choice <= 'k' || choice == 'q') {
                putchar(choice);
                choice -= 32;
                break;
            }
            else
                continue;
        }
        if (opt == 3) {
            if (choice >= 'A' && choice <= 'K' || choice == 'Q') {
                putchar(choice);
                break;
            }
            else if (choice >= 'a' && choice <= 'k' || choice == 'q') {
                putchar(choice);
                break;
            }
            else
                continue;
        }
    }
    cout << endl;
    return choice;
}