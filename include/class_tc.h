/* 2353626 贺胡鸣 大数据 */
#pragma once
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include "class_aat.h"
#include "class_cft.h"
#include "common_tools.h"
#include "cmd_console_tools.h"
#include "line.h"

const string prompt[9] = {
	"阅读提示：",
	"\t1、每行的行结束符用<CR>/<LF>/<CR><LF>/<EOF>标出(方便看清行结束符的类型)",
	"\t2、如果每行仅有<CR>/<LF>/<CR><LF>/<EOF>，则表示空行",
	"\t3、文件结束标记为<EOF>",
	"\t4、两行相同列位置的差异字符用亮色标出",
	"\t5、每行中的CR/LF/VT/BS/BEL用X标出(方便看清隐含字符)",
	"\t6、每行尾的多余的字符用亮色标出，VT/BS/BEL用亮色X标出(方便看清隐含字符)",
	"\t7、中文因为编码问题，差异位置可能报在后半个汉字上，但整个汉字都亮色标出",
	"\t8、用--display detailed可以得到更详细的信息"
};

class txt_compare {
private:
	string filename1, filename2;
	string trim_type, display_type;
	int line_skip;
	int line_offset;
	int line_max_diffnum;
	int line_max_linenum;
	bool ignore_blank;
	bool not_ignore_linefeed;
	bool debug;
	ifstream in1, in2;
	istringstream* iss1, * iss2;
	ostringstream oss;
	vector<int> highlight_pos;
	bool same = true;
	bool linux1 = false, linux2 = false;
	bool success_open_file = true;
	bool AT_MOST_ERROR_LINE = false; //记录是否达到最大错误行数
	bool same_until_eof = true; //记录文件读到eof前有没有不同行

	string out1, out2;
	int line_maxlen = 0;  //记录整个文件所有有效行的最大长度
	int max_len = 0;    //记录当前被比较的两行的最大长度

public:
	txt_compare(string p_filename1, string p_filename2, string p_trim_type, string p_display_type,
		int p_line_skip, int p_line_offset, int p_line_max_diffnum, int p_line_max_linenum, 
		bool p_ignore_blank, bool p_not_ignore_linefeed, bool p_debug);
	txt_compare(istringstream& p_iss1, istringstream& p_iss2, string p_trim_type, string p_display_type,
		int p_line_skip, int p_line_offset, int p_line_max_diffnum, int p_line_max_linenum,
		bool p_ignore_blank, bool p_not_ignore_linefeed, bool p_debug);
	void openfile();
	void closefile();
	void readline(LINE& line, ifstream& in);
	void readline(LINE& line, istringstream& in);
	void print_highlight(const LINE& line1, const LINE& line2, const int& linenum1, const int& linenum2, const int& ch_pos);
	int compare(bool is);
	void result();
	bool is_empty(const string& str);
	int getmaxlen(const string& str1, const string& str2);
	void print_prompt(const int& diff_count);
	int get_global_maxlen();
};