/* 2353626 贺胡鸣 大数据 */
#include <iostream>
#include "../include/common_tools.h"
using namespace std;

bool is_space(const char& ch)
{
	for (int i = 0; i < 4; i++)
		if (ch == Space[i])
			return true;
	return false;
}

bool all_space(const string& str)
{
	for (size_t i = 0; i < str.size(); i++)
		if (!is_space(str[i]))
			return false;
	return true;
}

string trim(const LINE& str, const string trim_type, const bool not_ignore_linefeed)
{
	if (!not_ignore_linefeed) {  //忽略换行符时，直接进行trim操作
		if (trim_type == "none") { //忽略换行符，又不trim，那么只需要删掉每行文件最右边的\r\n或者\n即可
			string newstr;
			/*
			for (size_t i = 0; i < str.original_line.size(); i++)
				if ((str.original_line[i] != '\r') && (str.original_line[i] != '\n'))
					newstr += str.original_line[i];
					*/
			newstr = str.original_line;
			return newstr;
		}

		size_t start = 0;
		size_t end = str.original_line.size();

		if (trim_type == "left" || trim_type == "all") {
			while (start < end && is_space(str.original_line[start]))
				start++;
		}

		if (trim_type == "right" || trim_type == "all") {
			while (end > start && is_space(str.original_line[end - 1]))
				end--;
		}
		/*
		else {   //忽略换行符，所以trim掉\r\n或者\n
			while (end > start && (str.original_line[end - 1] == '\r' || str.original_line[end - 1] == '\n'))
				end--;
		}
		*/

		return str.original_line.substr(start, end - start);
	}
	else {  //不忽略换行符，只能进行trim none/left操作
		if (trim_type == "none") {
			string newstr = str.original_line;
			return newstr;  //直接返回即可
		}
		else {  //左trim操作
			size_t start = 0;
			size_t end = str.original_line.size();
			while (start < end && is_space(str.original_line[start]))
				start++;
			return str.original_line.substr(start, end - start);
		}
	}
}

void print_hex(const LINE& line, ostringstream& oss)
{
	unsigned char ch;
	int c = 0;
	int bufferchar[16] = { 0 };
	size_t i = 0;
	string str = line.trimmed_line;
	bool cr = false, lf = false; //记录是否打印回车和换行
	for (size_t j = 0; j < str.size(); j++) {
		if (str[j] == '\r') cr = true;
		if (str[j] == '\n') lf = true;
	}
	if (line.EOF_existed)
		str += '\x1a';
	else {
		if (line.CR_existed && !cr)
			str += '\r';
		if (line.LF_existed && !lf)
			str += '\n';
	}

	while (i < str.size()) {
		ch = str[i];
		i++;
		if ((c - 1) % 16 == 7)
			oss << "- ";
		if (c % 16 == 0) {
			oss << hex << setw(8) << setfill('0') << c << " : " << dec;
		}
		int ascii = (int)ch;
		oss << hex << setw(2) << setfill('0') << ascii << " " << dec;
		bufferchar[c % 16] = ascii;

		if (c % 16 == 15) {
			oss << " ";
			for (int i = 0; i < 16; i++) {
				if (bufferchar[i] >= 33 && bufferchar[i] <= 126)
					oss << (char)bufferchar[i];
				else
					oss << '.';
			}
			oss << endl;
		}

		c++;
	}
	if (c % 16 == 0) {
		return;
	}
	int mx;
	if (c % 16 >= 0 && c % 16 <= 8)
		mx = 51 - c % 16 * 3;
	else
		mx = 3 * (16 - c % 16) + 1;
	for (int i = 0; i < mx; i++)
		oss << " ";
	for (int i = 0; i < c % 16; i++) {
		if (bufferchar[i] >= 33 && bufferchar[i] <= 126)
			oss << (char)bufferchar[i];
		else
			oss << '.';
	}
	oss << endl;
}