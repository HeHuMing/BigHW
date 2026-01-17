/* 2353626 贺胡鸣 大数据 */
#include <iostream>
#include "../include/class_tc.h"
using namespace std;

#define Version 2

void txt_compare::print_prompt(const int& diff_count)
{
    if (!same) {
        oss << endl;
        if (AT_MOST_ERROR_LINE)
            oss << "在指定检查条件下共" << diff_count << "行有差异[已到设定的最大差异值]." << endl;
        else
            oss << "在指定检查条件下共" << diff_count << "行有差异." << endl;
        int max_prompt = (display_type == "normal") ? 9 : 8;
        for (int i = 0; i < max_prompt; i++)
            oss << prompt[i] << endl;
    }
    else
        oss << "在指定检查条件下完全一致." << endl;
}

int get_purelen(const string& str)
{
    int len = 0;
    for (size_t i = 0; i < str.size(); i++) {
        if (str[i] != '\r' && str[i] != '\n')
            len++;
    }
    return len;
}

int txt_compare::getmaxlen(const string& str1, const string& str2)
{
    int len1 = 0, len2 = 0;
    for (size_t i = 0; i < str1.size(); i++) {
        if (str1[i] != '\r' && str1[i] != '\n')
            len1++;
    }
    for (size_t i = 0; i < str2.size(); i++) {
        if (str2[i] != '\r' && str2[i] != '\n')
            len2++;
    }
    return len1 > len2 ? len1 : len2;
}

static string del_crlf(const string& str)
{
    //如果有换行或者回车符，删掉
    if (str.empty())
        return str;
    string result;
    for (size_t i = 0; i < str.size(); i++)
        if (str[i] != '\r' && str[i] != '\n')
            result += str[i];
    
    return result;
}

static int check_substr(const string& line1, const string& line2)  //传入line1，line2
{
    //检测一个串str1是否为另一个串str2的子串
    //如果line1==line2，返回1
    //如果line1是line2的子串，返回2
    //如果line2是line1的子串，返回3
    //如果line1和line2无相等或子串关系，返回0
    const string str1 = del_crlf(line1), str2 = del_crlf(line2);
    if (str1 == str2) return 1; //相等返回1
    //检测1是否为2的子串
    if (str1.empty())
        return 2;
    if (str1.size() < str2.size()) {
        int flag1 = 1;
        for (size_t i = 0; i < str1.size(); i++) {
            if (str1[i] != str2[i]) {
                flag1 = 0;
                break;
            }
        }
        if (flag1) return 2;
    }
    //检测2是否为1的子串
    if (str2.empty())
        return 3;
    if (str2.size() < str1.size()) {
        int flag2 = 1;
        for (size_t i = 0; i < str2.size(); i++) {
            if (str2[i] != str1[i]) {
                flag2 = 0;
                break;
            }
        }
        if (flag2) return 3;
    }
    return 0;
}

static void print_ruler(const int& len, ostringstream& oss)
{
    oss << "        ";
    for (int i = 0; i < len; i++) oss << "-";
    oss << endl;
    oss << "        ";
    for (int i = 0; i < len; i++) {
        if (i % 10 == 0) oss << (i / 10) % 10;
        else  oss << " ";
    }
    oss << endl;
    oss << "        ";
    for (int i = 0; i < len; i++)
        oss << i % 10;
    oss << endl;
    oss << "        ";
    for (int i = 0; i < len; i++) oss << "-";
    oss << endl;
}

bool txt_compare::is_empty(const string& str)
{
    if (trim_type != "none")
        return str.empty();
    else {
        if (str.size() == 1 && str[0] == '\n')
            return true;
        else if (str.size() == 2 && str[0] == '\r' && str[1] == '\n')
            return true;
        else if (str == "")
            return true;
        else
            return false;
    }
}

static bool nothing(const string& str)
{
    return (str == "");
}

static bool rin(const string& str)
{
    if (nothing(str))
        return false;
    for (size_t i = 0; i < str.size(); i++)
        if (str[i] == '\r')
            return true;
    return false;
}

static bool empty_line(const string& str, const string& trim_type)
{
#if Version==1
    if (trim_type == "none")
        return (str == "\n");
    return nothing(str);
#elif Version==2
    if (str.empty())
        return true;
    for (size_t i = 0; i < str.size(); i++)
        if (str[i] != ' ' && str[i] != '\t' && str[i] != '\r' && str[i] != '\n')
            return false;
    return true;
#endif
}

txt_compare::txt_compare(string p_filename1, string p_filename2, string p_trim_type, string p_display_type,
	int p_line_skip, int p_line_offset, int p_line_max_diffnum, int p_line_max_linenum,
	bool p_ignore_blank, bool p_not_ignore_linefeed, bool p_debug)
{
	filename1 = p_filename1;
	filename2 = p_filename2;
	trim_type = p_trim_type;
	display_type = p_display_type;
	line_skip = p_line_skip;
	line_offset = p_line_offset;
	line_max_diffnum = p_line_max_diffnum;
	line_max_linenum = p_line_max_linenum;
	ignore_blank = p_ignore_blank;
	not_ignore_linefeed = p_not_ignore_linefeed;
	debug = p_debug;
}

txt_compare::txt_compare(istringstream& p_iss1, istringstream& p_iss2, string p_trim_type, string p_display_type,
    int p_line_skip, int p_line_offset, int p_line_max_diffnum, int p_line_max_linenum,
    bool p_ignore_blank, bool p_not_ignore_linefeed, bool p_debug)
{
    iss1 = &p_iss1;
    iss2 = &p_iss2;
    trim_type = p_trim_type;
    display_type = p_display_type;
    line_skip = p_line_skip;
    line_offset = p_line_offset;
    line_max_diffnum = p_line_max_diffnum;
    line_max_linenum = p_line_max_linenum;
    ignore_blank = p_ignore_blank;
    not_ignore_linefeed = p_not_ignore_linefeed;
    debug = p_debug;
}

void txt_compare::openfile()
{
	in1.open(filename1, ios::in | ios::binary);
	in2.open(filename2, ios::in | ios::binary);
    if (filename1 == filename2) {
        oss << endl << "[--严重错误--] --file1 和 --file2 指定的文件名 [" << filename1 << "] 相同.\n\n";
        success_open_file = false;
        return;
    }
	if (in1.is_open() == 0) {
        oss << endl << "[--严重错误--] 文件[" << filename1 << "]无法打开.\n\n\n";
        success_open_file = false;
		return;
	}
    if (in2.is_open() == 0) {
        oss << endl << "[--严重错误--] 文件[" << filename2 << "]无法打开.\n\n\n";
        success_open_file = false;
        return;
    }
}

void txt_compare::closefile() 
{
    in1.close();
    in2.close();
}
void txt_compare::readline(LINE& line, istringstream& in)
{
    // 清空输出行（避免残留旧数据）
    line.original_line.clear();
    line.trimmed_line.clear();
    line.EOF_existed = false;

    char ch;
    bool line_end = false;

    // 逐字符读取，直到文件结束或遇到行结束符
    while (in.get(ch)) {
        // 先将当前字符添加到行中
        line.original_line.push_back(ch);

        // 检查是否是行结束符
        if (ch == '\n') {
            line.LF_existed = true;
            // 找到 \n 行结束
            break;
        }
        else if (ch == '\r') {
            line.CR_existed = true;
            // 处理 Windows 换行 \r\n
            if (in.peek() == '\n') {
                line.LF_existed = true;
                // 读取 \n 字符
                in.get(ch);
                line.original_line.push_back(ch);
            }
            break;
        }
    }

    // 检查是否到达文件末尾
    if (in.eof()) {
        line.EOF_existed = true;
    }
    // 处理流错误（非 EOF 错误）
    else if (!in.good()) {
        line.EOF_existed = true;
    }
}

void txt_compare::readline(LINE& line, ifstream& in) 
{
#if Version==1
    // 清空输出行（避免残留旧数据）
    line.clear();
    is_eof = false;

    char ch;
    // 逐字符读取，直到文件结束或遇到换行符
    while (in.get(ch)) {
        // 处理 \r（回车符）
        if (ch == '\r') {
            // 如果 CR_CRLF_not_equal 为 true，保留 \r
            if (not_ignore_linefeed) {
                line.push_back(ch);
            }

            // 检查下一个字符是否是 \n（Windows 换行 \r\n）
            if (in.peek() == '\n') {
                // 读取 \n 字符
                in.get(ch);
                // 无论 CR_CRLF_not_equal 状态，都添加 \n
                line.push_back('\n');
                // 换行符处理完成，退出循环
                break;
            }
            // （根据需求，不考虑单独 \r 的情况，此处无需处理）
        }
        // 处理 \n（Linux 换行）
        else if (ch == '\n') {
            line.push_back('\n');
            break;
        }
        // 普通字符直接添加
        else {
            line.push_back(ch);
        }
    }

    // 检查是否到达文件末尾
    if (in.eof()) {
        is_eof = true;
        // 处理文件末尾无换行符的情况（最后一行内容仍需保留）
        // 注：如果文件为空，line 会保持空，is_eof 为 true
    }
    // 处理流错误（非 EOF 错误）
    else if (!in.good()) {
        is_eof = true; // 错误状态也视为 EOF（可根据需求调整）
    }
#elif Version==2
    // 清空输出行（避免残留旧数据）
    line.original_line.clear();
    line.trimmed_line.clear();
    line.EOF_existed = false;

    char ch;
    bool line_end = false;

    // 逐字符读取，直到文件结束或遇到行结束符
    while (in.get(ch)) {
        // 先将当前字符添加到行中
        line.original_line.push_back(ch);

        // 检查是否是行结束符
        if (ch == '\n') {
            line.LF_existed = true;
            // 找到 \n 行结束
            break;
        }
        else if (ch == '\r') {
            line.CR_existed = true;
            // 处理 Windows 换行 \r\n
            if (in.peek() == '\n') {
                line.LF_existed = true;
                // 读取 \n 字符
                in.get(ch);
                line.original_line.push_back(ch);
            }
            break;
        }
    }

    // 检查是否到达文件末尾
    if (in.eof()) {
        line.EOF_existed = true;
    }
    // 处理流错误（非 EOF 错误）
    else if (!in.good()) {
        line.EOF_existed = true;
    }
#endif
}

int txt_compare::get_global_maxlen()
{
    LINE line1, line2;
    string str1, str2;
    //从文件1中读一行
    int len1 = 0, len2 = 0;
    while (1) {
        readline(line1, in1);
        str1 = trim(line1, trim_type, not_ignore_linefeed);
        line1.trimmed_line = str1;
        if (line1.EOF_existed) break;
        if (!(ignore_blank && is_empty(line1.trimmed_line))) {
            int tmp1 = get_purelen(str1);
            if (line1.CR_existed) tmp1++;
            if (line1.LF_existed) tmp1++;
            if (tmp1 > len1)
                len1 = tmp1;
        }
        else continue;
    }
    //cout << linenum1 << ": " << line1 << endl;

    //从文件2中读一行
    while (1) {
        readline(line2, in2);
        str2 = trim(line2, trim_type, not_ignore_linefeed);
        line2.trimmed_line = str2;
        if (line2.EOF_existed) break;
        if (!(ignore_blank && is_empty(line2.trimmed_line))) {
            int tmp2 = get_purelen(str2);
            if (line2.CR_existed) tmp2++;
            if (line2.LF_existed) tmp2++;
            if (tmp2 > len2)
                len2 = tmp2;
        }
    }
    //cout << linenum2 << ": " << line2 << endl;
    in1.clear();
    in1.seekg(0, ios::beg);
    in2.clear();
    in2.seekg(0, ios::beg);
    return (len1 > len2) ? len1 : len2;
}

void txt_compare::print_highlight(const LINE& line1, const LINE& line2, const int& linenum1, const int& linenum2, const int& ch_pos)
{
#if Version == 1
    if (!is_eof1 && !is_eof2) {  //两个都没EOF
        if(check_substr(line1,line2,not_ignore_linefeed))
            oss << "第[" << linenum1 << " / " << linenum2 << "]行 - 文件2的尾部有多余字符：" << endl;
        else if(check_substr(line2,line1, not_ignore_linefeed))
            oss << "第[" << linenum1 << " / " << linenum2 << "]行 - 文件1的尾部有多余字符：" << endl;
        else if (ch_pos == -3)
            oss << "第[" << linenum1 << " / " << linenum2 << "]行 - 文件1的尾部有多余字符：" << endl;
        else if (ch_pos == -4)
            oss << "第[" << linenum1 << " / " << linenum2 << "]行 - 文件2的尾部有多余字符：" << endl;
        else if(all_space(line2))
            oss << "第[" << linenum1 << " / " << linenum2 << "]行 - 文件1的尾部有多余字符：" << endl;
        else if(all_space(line1))
            oss << "第[" << linenum1 << " / " << linenum2 << "]行 - 文件2的尾部有多余字符：" << endl;
        else
            oss << "第[" << linenum1 << " / " << linenum2 << "]行 - 第[" << ch_pos << "]个字符开始有差异：" << endl;
    }
    else if (!is_eof1 && is_eof2) {  //文件2能读到EOF
        if (nothing(line2) && !empty_line(line1, trim_type))
            oss << "第[" << linenum1 << " / " << linenum2 << "]行 - 文件1的尾部有多余字符：" << endl;
        else
            oss << "第[" << linenum1 << " / " << linenum2 << "]行 - 文件1仍有内容/文件2已结束：" << endl;
    }
    else if (is_eof1 && !is_eof2) {  //文件1能读到EOF
        if (nothing(line1) && !empty_line(line2, trim_type))
            oss << "第[" << linenum1 << " / " << linenum2 << "]行 - 文件2的尾部有多余字符：" << endl;
        else
            oss << "第[" << linenum1 << " / " << linenum2 << "]行 - 文件1已结束/文件2仍有内容：" << endl;
    }
    else {  //两个都能明确读到EOF
        if(check_substr(line1,line2, not_ignore_linefeed))
            oss << "第[" << linenum1 << " / " << linenum2 << "]行 - 文件2的尾部有多余字符：" << endl;
        else if(check_substr(line2,line1, not_ignore_linefeed))
            oss << "第[" << linenum1 << " / " << linenum2 << "]行 - 文件1的尾部有多余字符：" << endl;
        else
            oss << "第[" << linenum1 << " / " << linenum2 << "]行 - 第[" << ch_pos << "]个字符开始有差异：" << endl;
    }

#elif Version == 2
    //  提示词逻辑处理板块
    if (!line1.EOF_existed && !line2.EOF_existed) {  //两个文件都没结束
        if (empty_line(line1.trimmed_line, trim_type) && empty_line(line2.trimmed_line, trim_type))
            oss << "第[" << linenum1 << " / " << linenum2 << "]行 - 行结束符不同\n";
        else if (empty_line(line1.trimmed_line, trim_type) && !empty_line(line2.trimmed_line, trim_type))
            oss << "第[" << linenum1 << " / " << linenum2 << "]行 - 文件2有多余字符\n";
        else if (!empty_line(line1.trimmed_line, trim_type) && empty_line(line2.trimmed_line, trim_type))
            oss << "第[" << linenum1 << " / " << linenum2 << "]行 - 文件1有多余字符\n";
        else {
            int result = check_substr(line1.trimmed_line, line2.trimmed_line);
            if (result == 1)
                oss << "第[" << linenum1 << " / " << linenum2 << "]行 - 行结束符不同\n";
            else if (result == 2)
                oss << "第[" << linenum1 << " / " << linenum2 << "]行 - 文件2有多余字符\n";
            else if (result == 3)
                oss << "第[" << linenum1 << " / " << linenum2 << "]行 - 文件1有多余字符\n";
            else
                oss << "第[" << linenum1 << " / " << linenum2 << "]行 - 第[" << ch_pos << "]个字符开始有差异\n";
        }
    }
    else if (line1.EOF_existed && !line2.EOF_existed) {  //文件1结束，文件2没结束
        if (empty_line(line1.trimmed_line, trim_type) && empty_line(line2.trimmed_line, trim_type))
            oss << "第[" << linenum1 << " / " << linenum2 << "]行 - 行结束符不同\n";
        else if (empty_line(line1.trimmed_line, trim_type) && !empty_line(line2.trimmed_line, trim_type))
            oss << "第[" << linenum1 << " / " << linenum2 << "]行 - 文件1已结束/文件2仍有内容\n";
        else if(!empty_line(line1.trimmed_line, trim_type) && empty_line(line2.trimmed_line, trim_type))
            oss << "第[" << linenum1 << " / " << linenum2 << "]行 - 文件1有多余字符\n";
        else { //全部非空
            int result = check_substr(line1.trimmed_line, line2.trimmed_line);
            if (result == 1)
                oss << "第[" << linenum1 << " / " << linenum2 << "]行 - 行结束符不同\n";
            else if (result == 2)
                oss << "第[" << linenum1 << " / " << linenum2 << "]行 - 文件1已结束/文件2仍有内容\n";
            else if (result == 3)
                oss << "第[" << linenum1 << " / " << linenum2 << "]行 - 文件1有多余字符\n";
            else
                oss << "第[" << linenum1 << " / " << linenum2 << "]行 - 第[" << ch_pos << "]个字符开始有差异\n";
        }
    }
    else if (!line1.EOF_existed && line2.EOF_existed) {  //文件1没结束，文件2结束
        if (empty_line(line1.trimmed_line, trim_type) && empty_line(line2.trimmed_line, trim_type))
            oss << "第[" << linenum1 << " / " << linenum2 << "]行 - 行结束符不同\n";
        else if (empty_line(line1.trimmed_line, trim_type) && !empty_line(line2.trimmed_line, trim_type))
            oss << "第[" << linenum1 << " / " << linenum2 << "]行 - 文件2有多余字符\n";
        else if (!empty_line(line1.trimmed_line, trim_type) && empty_line(line2.trimmed_line, trim_type))
            oss << "第[" << linenum1 << " / " << linenum2 << "]行 - 文件1仍有内容/文件2已结束\n";
        else { //全部非空
            int result = check_substr(line1.trimmed_line, line2.trimmed_line);
            if (result == 1)
                oss << "第[" << linenum1 << " / " << linenum2 << "]行 - 行结束符不同\n";
            else if (result == 2)
                oss << "第[" << linenum1 << " / " << linenum2 << "]行 - 文件2有多余字符\n";
            else if (result == 3)
                oss << "第[" << linenum1 << " / " << linenum2 << "]行 - 文件1仍有内容/文件2已结束\n";
            else
                oss << "第[" << linenum1 << " / " << linenum2 << "]行 - 第[" << ch_pos << "]个字符开始有差异\n";
        }
    }
    else {   //两个文件都结束了
        //不可能两个全空，即<EOF> & <EOF>，这种情况进不来这个函数
        if (empty_line(line1.trimmed_line, trim_type) && !empty_line(line2.trimmed_line, trim_type))
            oss << "第[" << linenum1 << " / " << linenum2 << "]行 - 文件1已结束/文件2仍有内容\n";
        else if (!empty_line(line1.trimmed_line, trim_type) && empty_line(line2.trimmed_line, trim_type))
            oss << "第[" << linenum1 << " / " << linenum2 << "]行 - 文件1仍有内容/文件2已结束\n";
        else { //全部非空
            int result = check_substr(line1.trimmed_line, line2.trimmed_line);
            //也不用考虑相等，这种情况也进不来这个函数
            if (result == 2)
                oss << "第[" << linenum1 << " / " << linenum2 << "]行 - 文件1已结束/文件2仍有内容\n";
            else if (result == 3)
                oss << "第[" << linenum1 << " / " << linenum2 << "]行 - 文件1仍有内容/文件2已结束\n";
            else
                oss << "第[" << linenum1 << " / " << linenum2 << "]行 - 第[" << ch_pos << "]个字符开始有差异\n";
        }
    }
#endif

    /*
    //特判，复刻larry的错误
    if (empty_line(line1, trim_type) && empty_line(line2, trim_type) && same_until_eof) {
        if ((is_eof1 && !is_eof2) || (!is_eof1 && is_eof2))
            return;
    }
    */

    // 新修改：不论normal还是detailed全部都要print_ruler
    print_ruler(max_len, oss);

    size_t mx_len = (line1.trimmed_line.size() < line2.trimmed_line.size()) ? line1.trimmed_line.size() : line2.trimmed_line.size();
    bool cr1 = false, cr2 = false;  //记录是否打印过<CR>
    bool lf1 = false, lf2 = false;  //记录是否打印过<LF>
    // 打印line1
    out1.clear();
    out2.clear();
    oss << "文件1 : ";
    /*         新版不再打印EMPTY
    if (!line1.EOF_existed && empty_line(line1, trim_type)) {
        if (!ignore_blank) {
            if (!(not_ignore_linefeed && rin(line1))) {
                int tmp = oss.str().size();
                for (int j = 0; j < 7; j++)
                    highlight_pos.push_back(tmp + j);
                oss << "<EMPTY>";
                out1 += "<EMPTY>";
            }
        }
    }
    */
    for (size_t i = 0; i < mx_len; i++) {  
        if (line1.trimmed_line[i] == '\n') {
            oss << "<LF>";
            out1 += "<LF>";
            lf1 = true;
        }
        else if (line1.trimmed_line[i] == '\r') {
            //highlight_pos.push_back(oss.str().size());
            oss << "<CR>";
            out1 += "<CR>";
            cr1 = true;
        }
        else if (line1.trimmed_line[i] == line2.trimmed_line[i]) {
            oss << line1.trimmed_line[i];
            out1 += line1.trimmed_line[i];
        }
        else {
            highlight_pos.push_back((int)oss.str().size());
            oss << line1.trimmed_line[i];
            out1 += line1.trimmed_line[i];
        }
    }
    if (line1.trimmed_line.size() > mx_len) {
        for (size_t i = mx_len; i < line1.trimmed_line.size(); i++) {
            if (line1.trimmed_line[i] == '\n') {
                oss << "<LF>";
                out1 += "<LF>";
                lf1 = true;
            }
            else if (line1.trimmed_line[i] == '\r') {
                //highlight_pos.push_back(oss.str().size());
                oss << "<CR>";
                out1 += "<CR>";
                cr1 = true;
            }
            else {
                highlight_pos.push_back((int)oss.str().size());
                oss << line1.trimmed_line[i];
                out1 += line1.trimmed_line[i];
            }
        }
    }
    cct_setcolor(COLOR_BLACK, COLOR_WHITE);
    if (line1.EOF_existed) {
        oss << "<EOF>";
        out1 += "<EOF>";
    }
    else {
        if (line1.CR_existed && !cr1) {
            oss << "<CR>";
            out1 += "<CR>";
        }
        if (line1.LF_existed && !lf1) {
            oss << "<LF>";
            out1 += "<LF>";
        }
    }

    // 打印line2
    oss << endl << "文件2 : ";
    /*   新版不再打印empty
    if (!line2.EOF_existed && empty_line(line2, trim_type)) {
        if (!ignore_blank) {
            if (!(not_ignore_linefeed && rin(line2))) {
                int tmp = oss.str().size();
                for (int j = 0; j < 7; j++)
                    highlight_pos.push_back(tmp + j);
                oss << "<EMPTY>";
                out2 += "<EMPTY>";
            }
        }
    }
    */
    for (size_t i = 0; i < mx_len; i++) {
        if (line2.trimmed_line[i] == '\n') {
            oss << "<LF>";
            out2 += "<LF>";
            lf2 = true;
        }
        else if (line2.trimmed_line[i] == '\r') {
            //highlight_pos.push_back(oss.str().size());
            oss << "<CR>";
            out2 += "<CR>";
            cr2 = true;
        }
        else if (line1.trimmed_line[i] == line2.trimmed_line[i]) {
            oss << line2.trimmed_line[i];
            out2 += line2.trimmed_line[i];
        }
        else {
            highlight_pos.push_back((int)oss.str().size());
            oss << line2.trimmed_line[i];
            out2 += line2.trimmed_line[i];
        }
    }
    if (line2.trimmed_line.size() > mx_len) {
        for (size_t i = mx_len; i < line2.trimmed_line.size(); i++) {
            if (line2.trimmed_line[i] == '\n') {
                oss << "<LF>";
                out2 += "<LF>";
                lf2 = true;
            }
            else if (line2.trimmed_line[i] == '\r') {
                //highlight_pos.push_back(oss.str().size());
                oss << "<CR>";
                out2 += "<CR>";
                cr2 = true;
            }
            else {
                highlight_pos.push_back((int)oss.str().size());
                oss << line2.trimmed_line[i];
                out2 += line2.trimmed_line[i];
            }
        }
    }
    if (line2.EOF_existed) {
        oss << "<EOF>";
        out2 += "<EOF>";
    }
    else {
        if (line2.CR_existed && !cr2) {
            oss << "<CR>";
            out2 += "<CR>";
        }
        if (line2.LF_existed && !lf2) {
            oss << "<LF>";
            out2 += "<LF>";
        }
    }
    
    // 打印hex部分
    if (display_type == "normal") 
        oss << endl << endl;
    else if (display_type == "detailed") {
        oss << endl << "文件1(HEX) : " << endl;
        print_hex(line1, oss);
            
        oss << "文件2(HEX) : " << endl;
        print_hex(line2, oss);
   
        oss << endl;
    }
}

int txt_compare::compare(bool is)
{
    if (!is) {
        openfile();
        if (!success_open_file)
            return -1;
    }

    line_maxlen = get_global_maxlen();
    int width = (this->line_maxlen / 10 + 1) * 10 + 8 + 2;
    if (display_type == "detailed" && width < 80)
        width = 80;

    int skip1 = 0, skip2 = 0;
    if (line_offset < 0)
        skip1 += (-line_offset);
    else if (line_offset > 0)
        skip2 += line_offset;
    skip1 += line_skip;
    skip2 += line_skip;

    int cmpnum = 0; //记录被比较过的行数
    int diff_count = 0; //记录错误的行数
    int linenum1 = 0, linenum2 = 0;
    LINE line1, line2;
    string str1, str2; //用于trim后作比较，不影响line1/2
    bool is_eof1 = false, is_eof2 = false;

    bool cmp_results_print = false;
    
    //每次比较一行
    while (1) {
        /*****************************************************************************/
        //从文件1中读一行
        while (1) {
            if (!is)
                readline(line1, in1);
            else
                readline(line1, *iss1);
            str1 = trim(line1, trim_type, not_ignore_linefeed);
            line1.trimmed_line = str1;
            linenum1++;
            if (line1.EOF_existed) break;
            if (!(ignore_blank && is_empty(line1.trimmed_line))) {
                if (skip1 > 0)
                    skip1--;
                else if (skip1 == 0)
                    break;
            }
            else continue;
        }
        //cout << linenum1 << ": " << line1 << endl;

        //从文件2中读一行
        while (1) {
            if (!is)
                readline(line2, in2);
            else
                readline(line2, *iss2);
            linenum2++;
            str2 = trim(line2, trim_type, not_ignore_linefeed);
            line2.trimmed_line = str2;
            if (line2.EOF_existed) break;
            if (!(ignore_blank && is_empty(line2.trimmed_line))) {
                if (skip2 > 0)
                    skip2--;
                else if (skip2 == 0)
                    break;
            }
        }
        //cout << linenum2 << ": " << line2 << endl;

        //动态求当前比较的两行的长度line_maxlen
        max_len = getmaxlen(line1.original_line, line2.original_line);
        max_len = (max_len / 10 + 2) * 10 + 1;


        /*****************************************************************************/
        int ch_pos = -1;
        bool current_diff = false;
        //先判断文件是否相同
        if (line1.EOF_existed == line2.EOF_existed) {
            cmpnum++;
            //重新修改比较逻辑
            if (!not_ignore_linefeed) {  // 忽略换行符时
                if (del_crlf(line1.trimmed_line) == del_crlf(line2.trimmed_line))
                    current_diff = false;
                else
                    current_diff = true;
            }  
            else {  // 不忽略换行符时
                current_diff = (line1.trimmed_line == line2.trimmed_line) ? false : true;
            }
            if (same) same = current_diff ? false : true; //same=true才改，不然不改
            //cout << same << endl;
            if (!line1.EOF_existed && !line2.EOF_existed && current_diff)
                same_until_eof = false;
            /*
            if (trim(line1,"right",not_ignore_linefeed)==trim(line2,"right",not_ignore_linefeed) && current_diff) {//如果一行字符串是另一行字符串的子串，ch_pos=-3：2是1的子串，ch_pos=-4：1是2的子串
                if (line1.size() > line2.size()) ch_pos = -3;
                if (line1.size() < line2.size()) ch_pos = -4;
            }
            */
        }
        else {  //一个EOF，一个还没EOF
            cmpnum++;
            if (ignore_blank) {  //忽略空行时
                if (del_crlf(line1.trimmed_line) == del_crlf(line2.trimmed_line))
                    current_diff = false;
                else
                    current_diff = true;
            }
            else {  //不忽略空行时
                current_diff = true;
            }
            if (same) same = current_diff ? false : true; //same=true才改，不然不改
            //cout << same << endl;
        }
        size_t mxlen;
        if (line1.trimmed_line.size() != line2.trimmed_line.size())
            mxlen = (line1.trimmed_line.size() < line2.trimmed_line.size()) ? line1.trimmed_line.size() : line2.trimmed_line.size();
        else
            mxlen = line1.trimmed_line.size();
        int flag = 1; //记录是否只有行结束符不同
        for (size_t i = 0; i < mxlen; i++)
            if (line1.trimmed_line[i] != line2.trimmed_line[i]) {
                //if (line1[i] == '\r') {linux1 = true; ch_pos = -2;}
                //if (line2[i] == '\r') {linux2 = true; ch_pos = -2;}
                if (line1.trimmed_line[i] != '\r' && line1.trimmed_line[i] != '\n' && line2.trimmed_line[i] != '\r' && line2.trimmed_line[i] != '\n')
                {
                    ch_pos = i; flag = 0;
                }  //如果不同的地方不是CR或LF，那么flag置为0
                else if (flag) { ch_pos = -2; } //如果不同的地方是换行或回车符，但之前的字符比较都相同，那么ch_pos返回-2
                break;
            }


        if (display_type != "none" && !cmp_results_print) {
            oss << "比较结果输出：" << endl;
            oss << setw(width) << setfill('=') << "" << endl;
            cmp_results_print = true;
            
        }

        //再做display详细打印
        if (current_diff) {
            diff_count++;
            if (display_type == "normal") {
                //cout << (int)(line1.back()) << endl;
                print_highlight(line1, line2, linenum1, linenum2, ch_pos);
            }
            else if (display_type == "detailed") {
                print_highlight(line1, line2, linenum1, linenum2, ch_pos);
            }
            // none 时不操作
            if (diff_count == line_max_diffnum) { // 达到文件最大错误行数，直接结束运行
                AT_MOST_ERROR_LINE = true;
                break;
            }
        }
        //cout << cmpnum << "*" << endl;
        if (cmpnum == line_max_linenum) // 达到文件最大比较行数，不再比较
            break;


        if (line1.EOF_existed && line2.EOF_existed) //两个文件都读完
            break;
        if (!line1.EOF_existed && !line2.EOF_existed) { // 两个文件都还在读
            continue;
        }
        if ((line1.EOF_existed && !line2.EOF_existed) || (!line1.EOF_existed && line2.EOF_existed)) {
            break;
        }
    }

    if (display_type != "none") {
        if (!same)
            oss << setw(width) << setfill('=') << "";
        print_prompt(diff_count);
        oss << setw(width) << setfill('=') << "" << endl;
    }

    if (display_type == "none")
        oss << (same ? ("文件相同.") : ("文件不同.")) << endl;
   
    if (!is)
        closefile();
    return diff_count; //新增：返回比对不一致的行数
}

void txt_compare::result()
{
	string str = oss.str();
    //比对highlight_pos，相同位置的字符高亮显示，其余正常
    if (highlight_pos.empty()) {
        cout << str;
        return;
    }
    
    int idx = 0;
    for (size_t i = 0; i < str.size(); i++) {
        if (i == highlight_pos[idx]) {
            cct_setcolor(COLOR_HYELLOW, COLOR_HRED);
            cout << str[i];
            cct_setcolor(COLOR_BLACK, COLOR_WHITE);
            if (idx < (int)highlight_pos.size() - 1)
                idx++;
        }
        else
            cout << str[i];
    }
}