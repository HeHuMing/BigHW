/* 2353626 贺胡鸣 大数据 */
#include "90-01-b1-hanoi.h"
using namespace std;
/* ----------------------------------------------------------------------------------

	 本文件功能：
	1、放main函数
	2、初始化屏幕
	3、调用菜单函数（hanoi_menu.cpp中）并返回选项
	4、根据选项调用菜单各项对应的执行函数（hanoi_multiple_solutions.cpp中）

	 本文件要求：
	1、不允许定义全局变量（含外部全局和静态全局，const及#define不在限制范围内）
	2、静态局部变量的数量不限制，但使用准则也是：少用、慎用、能不用尽量不用
	3、按需加入系统头文件、自定义头文件、命名空间等

   ----------------------------------------------------------------------------------- */

   /***************************************************************************
	 函数名称：main
	 功    能：/
	 输入参数：/
	 返 回 值：/
	 说    明：/
   ***************************************************************************/
int main()
{
	/* 将这段复制到main的最前面 */
	cout << "请确认当前cmd窗口的大小为40行*120列以上，字体为新宋体/16，按C继续，Q退出" << endl;
	while (1) {
		char ch = _getch();
		if (ch == 'C' || ch == 'c')
			break;
		if (ch == 'Q' || ch == 'q')
			return 0;
	}

	/* 从这里继续你的程序 */
	char choice;
	while (1) {
		choice = menu(hanoi_prompts, 1);
		if (choice == '0') {
			cct_gotoxy(0, 38);
			return 0;
		}
		cout << endl << endl;
		if (choice == '1') {
			choice1();
			cout << endl;
			cout << "按回车键继续";
			while (_getch() != '\r');
		}
		else if (choice == '2') {
			choice2();
			cout << endl;
			cout << "按回车键继续";
			while (_getch() != '\r');
		}
		else if (choice == '3') {
			choice3();
			cout << endl;
			cout << "按回车键继续";
			while (_getch() != '\r');
		}
		else if (choice == '4') {
			choice4();
			cout << endl;
			to_be_continued("按回车键继续");
		}
		else if (choice == '5') {
			choice5();
			to_be_continued("按回车键继续");
		}
		else if (choice == '6') {
			choice6();
			cout << endl;
			to_be_continued("按回车键继续");
		}
		else if (choice == '7') {
			choice7();
			cout << endl;
			to_be_continued("按回车键继续");
		}

		else if (choice == '8') {
			choice8();
			cout << endl;
			to_be_continued("按回车键继续");
		}

		else if (choice == '9') {
			choice9();
			cout << endl;
			to_be_continued("按回车键继续", Status_Line_X, Status_Line_Y + 1);
		}
	}


	return 0;
}