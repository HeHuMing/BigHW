/* 2353626 贺胡鸣 大数据 */
#pragma once

#include <iostream>
#include <Windows.h>
#include <iomanip>
#include <conio.h>
#include "../include/cmd_console_tools.h"
#include "../include/cmd_hdc_tools.h"
#include "../include/common_menu.h"
#include "../include/common_prompts.h"

/* 将所有const常量定义的头文件包含进来 */
#include "90-01-b1-hanoi_const_value.h"

/* ------------------------------------------------------------------------------------------------------

	 本文件功能：
	1、为了保证 hanoi_main.cpp/hanoi_menu.cpp/hanoi_multiple_solutions.cpp 能相互访问函数的函数声明
	2、一个以上的cpp中用到的宏定义（#define）或全局只读（const）变量，个数不限
	3、可以参考 cmd_console_tools.h 的写法（认真阅读并体会）
   ------------------------------------------------------------------------------------------------------ */

void choice1();
void choice2();
void choice3();
void choice4();
void choice5();
void choice6();
void choice7();
void choice8();
void choice9();
void to_be_continued(const char prompt[], const int pos_x = Status_Line_X, const int pos_y = Status_Line_Y + 2);

#define Time 50
