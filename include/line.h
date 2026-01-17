/* 2353626 贺胡鸣 大数据 */
#pragma once

#include <iostream>
#include <string>
using namespace std;

//记录文件每一行的状态
struct LINE {
	bool CR_existed = false;       //存在\r
	bool LF_existed = false;       //存在\n
	bool EOF_existed = false;      //存在EOF
	string original_line;          //记录从文件读完原始的一行，包括空格，回车换行
	string trimmed_line;           //记录trim后的行
};