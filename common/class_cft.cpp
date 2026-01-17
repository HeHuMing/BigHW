/* 2353626 贺胡鸣 大数据 */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <ctime>
#include <sstream>
#include <cstring>
/* 添加自己需要的头文件，注意限制 */
#include "../include/class_cft.h"
using namespace std;

/* 给出各种自定义函数的实现（已给出的内容不全） */


static string my_lower_str(const string str)
{
	if (str == SIMPLE_GNAME)
		return "";
	string result;
	for (size_t i = 0; i < str.size(); i++) {
		if (str[i] >= 'A' && str[i] <= 'Z')
			result.push_back((char)(str[i] + 32));
		else
			result.push_back(str[i]);
	}
	return result;
}

bool is_valid_octet(const std::string& str)
{
	if (str.empty())
		return false;
	for (char c : str)
		if (!isdigit(static_cast<unsigned char>(c)))
			return false;
	if (str.size() > 1 && str[0] == '0') {
		return false;
	}
	int value;
	std::istringstream iss(str);
	if (!(iss >> value) || iss.eof() == false)
		return false;
	return value >= 0 && value <= 255;
}

bool is_valid_ippaddr(const std::string ipaddr, unsigned int& hexip) {
	istringstream iss(ipaddr);
	string part;
	int parts[4];
	int count = 0;

	while (std::getline(iss, part, '.') && count < 4) {
		if (!is_valid_octet(part))
			return false;
		parts[count] = std::stoi(part);
		count++;
	}

	if (count != 4 || iss.eof() == false)
		return false;

	hexip = (static_cast<uint32_t>(static_cast<uint8_t>(parts[0])) << 24) |
		(static_cast<uint32_t>(static_cast<uint8_t>(parts[1])) << 16) |
		(static_cast<uint32_t>(static_cast<uint8_t>(parts[2])) << 8) |
		static_cast<uint32_t>(static_cast<uint8_t>(parts[3]));

	return true;
}

/***************************************************************************
  函数名称：config_file_tools构造函数
  功    能：初始化cfgname、item_separate_character_type
  输入参数：const char* const _cfgname, const enum BREAK_CTYPE _ctype
  返 回 值：/
  说    明：重载，适用于char* const形式的文件名
***************************************************************************/
config_file_tools::config_file_tools(const char* const _cfgname, const enum BREAK_CTYPE _ctype)
{
	/* 按需完成 */
	cfgname = _cfgname;
	item_separate_character_type = _ctype;
}

/***************************************************************************
  函数名称：config_file_tools构造函数
  功    能：初始化cfgname、item_separate_character_type
  输入参数：const string& _cfgname, const enum BREAK_CTYPE _ctype
  返 回 值：/
  说    明：重载，适用于string形式的文件名
***************************************************************************/
config_file_tools::config_file_tools(const string& _cfgname, const enum BREAK_CTYPE _ctype)
{
	/* 按需完成 */
	cfgname = _cfgname;
	item_separate_character_type = _ctype;
}

/***************************************************************************
  函数名称：config_file_tools析构函数
  功    能：
  输入参数：
  返 回 值：/
  说    明：
***************************************************************************/
config_file_tools::~config_file_tools()
{
	/* 按需完成 */

}

/***************************************************************************
  函数名称：extract
  功    能：提取一行字符串，去除前后空格和注释部分
  输入参数：const string str
  返 回 值：string
  说    明：先截断；或#或//开始的注释，再去除前后的空格/tab，剩下的是有效内容
***************************************************************************/
string extract(const string str) 
{
	if (str.empty()) 
		return "";
	
	string result = str;
	size_t comment_pos = string::npos;
	size_t pos;

	if ((pos = result.find("//")) != string::npos) 
		comment_pos = pos;
	if ((pos = result.find(';')) != string::npos && (comment_pos == string::npos || pos < comment_pos)) 
		comment_pos = pos;
	if ((pos = result.find('#')) != string::npos && (comment_pos == string::npos || pos < comment_pos)) 
		comment_pos = pos;
	
	// remove the comment 
	if (comment_pos != string::npos) 
		result = result.substr(0, comment_pos);
	
	// remove the space/tab
	size_t start = result.find_first_not_of(" \t");
	if (start == string::npos) 
		return "";
	size_t end = result.find_last_not_of(" \t");

	return result.substr(start, end - start + 1);
}

/***************************************************************************
  函数名称：extract_group
  功    能：提取有效组名
  输入参数：const string str
  返 回 值：string
  说    明：先去除[],再做一次去除空格/tab的操作，不为空则有效，为空则返回空串
***************************************************************************/
string extract_group(const string str)
{
	if (str.empty())
		return "";
	string tmp = str.substr(1, str.size() - 2);
	string result = extract(tmp);
	return result;
}


/***************************************************************************
  函数名称：is_read_succeeded
  功    能：判断读配置文件是否成功
  输入参数：/
  返 回 值：true - 成功，已读入所有的组/项
		   false - 失败，文件某行超长/文件全部是注释语句
  说    明：打开配置文件，并且判断是否打开/读取成功
***************************************************************************/
bool config_file_tools::is_read_succeeded() const
{
	/* 按需完成，根据需要改变return的值 */
	ifstream in;
	in.open(cfgname, ios::in);
	if (in.is_open() == 0) 
		return false;

	string line;
	bool have_valid_content = false;
	int line_count = 0;

	while (getline(in, line)) {
		if (line.length() >= MAX_LINE) {
			ostringstream oss;
			time_t current_time = time(nullptr);
			tm* local_tm = localtime(&current_time);
			oss << local_tm->tm_year << "-" << local_tm->tm_mon << "-" << local_tm->tm_mday << " " << local_tm->tm_hour << ":" << local_tm->tm_min << ":" << local_tm->tm_sec << " ";
			cout << oss.str() << "非法格式的配置文件，第[" << (line_count + 1) << "]行超过最大长度(1024)." << endl << endl
				<< "[--严重错误--]" << oss.str() << "非法格式的配置文件，第[" << (line_count + 1) << "]行超过最大长度(1024).." << endl;
			return false;
		}
		
		// judge whether the first char is ';' or '#' or "//"
		size_t start = line.find_first_not_of(" \t");
		if (start == string::npos)  // empty line
			continue;
		if (line[start] == ';' || line[start] == '#' || (line.length() > start + 1 && line[start] == '/' && line[start + 1] == '/')) // comment line
			continue;

		string extracted_line = extract(line);
		cfg_list.push_back(extracted_line);
		have_valid_content = true;
	}

	in.close();
	if (!have_valid_content)
		return false;
	return true;
}

/***************************************************************************
  函数名称：get_all_group
  功    能：返回配置文件中的所有组
  输入参数：vector <string>& ret : vector 中每项为一个组名
  返 回 值：读到的组的数量（简单配置文件的组数量为1，组名为""）
  说    明：如果有多个group相同，均当作一个组，同名组的所有项目合并到一个组中去，不去重
***************************************************************************/
int config_file_tools::get_all_group(vector <string>& ret)
{
	/* 按需完成，根据需要改变return的值 */
	int group_count = 0, flag = 1;
	bool have_named_group = false;
	for (size_t i = 0; i < cfg_list.size(); i++) {
		if (i == 0 && cfg_list[i].front() != '[' && cfg_list[i].back() != ']') { // simple or mixed configuration file
			flag = 0;
			ret.push_back(SIMPLE_GNAME);
		}
		if (cfg_list[i].front() == '[' && cfg_list[i].back() == ']') {
			have_named_group = true;
			/*
			string tmp1 = cfg_list[i];
			string tmp = extract_group(tmp1);
			auto it = find(ret.begin(), ret.end(), tmp);
			if (it == ret.end()) {
				ret.push_back(tmp);
				group_count++;
			}
			*/
			string tmp = cfg_list[i];
			bool in_ret = false;
			for (size_t i = 0; i < ret.size(); i++) {
				if (tmp == ret[i]) {
					in_ret = true;
					break;
				}
			}
			if (!in_ret) {
				ret.push_back(tmp);
				group_count++;
			}
		}
	}
	
	if (!have_named_group)  // simple configuration file
		return 1;
	else if (flag) // ordinary conf
		return group_count;
	else // mixed conf
		return group_count + 1;
}

/***************************************************************************
  函数名称：get_all_item
  功    能：查找指定组的所有项并返回项的原始内容
  输入参数：const char* const group_name：组名
		   vector <string>& ret：vector 中每项为一个项的原始内容
		   const bool is_case_sensitive = false : 组名是否大小写敏感，true-大小写敏感 / 默认false-大小写不敏感
  返 回 值：项的数量，0表示空
  说    明：
***************************************************************************/
int config_file_tools::get_all_item(const char* const group_name, vector <string>& ret, const bool is_case_sensitive)
{
	/* 按需完成，根据需要改变return的值 */
	string q_group = (is_case_sensitive) ? group_name : my_lower_str(group_name);
	string cur_group;
	int item_num = 0;
	for (size_t i = 0; i < cfg_list.size(); i++) {
		if (i == 0 && cfg_list[i].front() != '[' && cfg_list[i].back() != ']')  // simple or mixed configuration file
			cur_group = "";
		if (cfg_list[i].front() == '[' && cfg_list[i].back() == ']')   // named group
			cur_group = (is_case_sensitive) ? cfg_list[i] : my_lower_str(cfg_list[i]);
		else if (q_group == cur_group) {   // item in the query group
			item_num++;
			ret.push_back(cfg_list[i]);
		}
	}

	return item_num;
}

/***************************************************************************
  函数名称：get_all_item
  功    能：组名/项目为string方式，其余同上
  输入参数：const string& group_name, vector <string>& ret, const bool is_case_sensitive
  返 回 值：item_num
  说    明：
***************************************************************************/
int config_file_tools::get_all_item(const string& group_name, vector <string>& ret, const bool is_case_sensitive)
{
	/* 按需完成，根据需要改变return的值 */
	return this->get_all_item(group_name.c_str(), ret, is_case_sensitive);
}

/***************************************************************************
  函数名称：item_get_raw
  功    能：取某项的原始内容（=后的所有字符，string方式）
  输入参数：const char* const group_name
		   const char* const item_name
		   string &ret
		   const bool group_is_case_sensitive = false : 组名是否大小写敏感，true-大小写敏感 / 默认false-大小写不敏感
		   const bool item_is_case_sensitive = false  : 项名是否大小写敏感，true-大小写敏感 / 默认false-大小写不敏感
  返 回 值：成功返回1，否则返回0
  说    明：item_separate_character_type : Equal / Space
***************************************************************************/
int config_file_tools::item_get_raw(const char* const group_name, const char* const item_name, string& ret, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	/* 按需完成，根据需要改变return的值 */
	string q_group = (group_is_case_sensitive) ? group_name : my_lower_str(group_name);
	string q_item = (item_is_case_sensitive) ? item_name : my_lower_str(item_name);
	string cur_group, cur_item;
	int success = 0;
	for (size_t i = 0; i < cfg_list.size(); i++) {
		if (i == 0 && cfg_list[i].front() != '[' && cfg_list[i].back() != ']')  // simple or mixed configuration file
			cur_group = "";
		if (cfg_list[i].front() == '[' && cfg_list[i].back() == ']')   // named group
			cur_group = (group_is_case_sensitive) ? cfg_list[i] : my_lower_str(cfg_list[i]);
		else if (q_group == cur_group) {  // 在当前组下
			string tmp = (item_is_case_sensitive) ? cfg_list[i] : my_lower_str(cfg_list[i]);
			string rec = cfg_list[i]; // record the original line 
			size_t pos = tmp.find((item_separate_character_type == BREAK_CTYPE::Equal) ? "=" : " ");
			if (pos != string::npos) {  // 有分隔符
				string cur_item = extract(tmp.substr(0, pos));
				if (cur_item == q_item) {
					ret = extract(rec.substr(pos + 1));
					success = 1;
					break;
				}
			}
		}
	}
	return success;
}

/***************************************************************************
  函数名称：item_get_raw
  功    能：组名/项目为string方式，其余同上
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
int config_file_tools::item_get_raw(const string& group_name, const string& item_name, string& ret, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	/* 本函数已实现 */
	return this->item_get_raw(group_name.c_str(), item_name.c_str(), ret, group_is_case_sensitive, item_is_case_sensitive);
}

/***************************************************************************
  函数名称：item_get_null
  功    能：取某项的内容，返回类型为char型
  输入参数：const char* const group_name               ：组名
		   const char* const item_name                ：项名
		   const bool group_is_case_sensitive = false : 组名是否大小写敏感，true-大小写敏感 / 默认false-大小写不敏感
		   const bool item_is_case_sensitive = false  : 项名是否大小写敏感，true-大小写敏感 / 默认false-大小写不敏感
  返 回 值：1 - 该项的项名存在
		   0 - 该项的项名不存在
  说    明：
***************************************************************************/
int config_file_tools::item_get_null(const char* const group_name, const char* const item_name, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	/* 按需完成，根据需要改变return的值 */
	string q_group = (group_is_case_sensitive) ? group_name : my_lower_str(group_name);
	string q_item = (item_is_case_sensitive) ? item_name : my_lower_str(item_name);
	string cur_group, cur_item;
	int success = 0;
	for (size_t i = 0; i < cfg_list.size(); i++) {
		if (i == 0 && cfg_list[i].front() != '[' && cfg_list[i].back() != ']')  // simple or mixed configuration file
			cur_group = "";
		if (cfg_list[i].front() == '[' && cfg_list[i].back() == ']')   // named group
			cur_group = (group_is_case_sensitive) ? cfg_list[i] : my_lower_str(cfg_list[i]);
		else if (q_group == cur_group) {  // 在当前组下
			string tmp = (item_is_case_sensitive) ? cfg_list[i] : my_lower_str(cfg_list[i]);
			size_t pos = tmp.find((item_separate_character_type == BREAK_CTYPE::Equal) ? "=" : " ");
			if (pos != string::npos) {  // 有分隔符
				string cur_item = extract(tmp.substr(0, pos));
				if (cur_item == q_item) {
					success = 1;
					break;
				}
			}
		}
	}
	return success;
}

/***************************************************************************
  函数名称：item_get_null
  功    能：组名/项目为string方式，其余同上
  输入参数：
  返 回 值：
  说    明：因为工具函数一般在程序初始化阶段被调用，不会在程序执行中被高频次调用，
		   因此这里直接套壳，会略微影响效率，但不影响整体性能（对高频次调用，此方法不适合）
***************************************************************************/
int config_file_tools::item_get_null(const string& group_name, const string& item_name, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	/* 本函数已实现 */
	return this->item_get_null(group_name.c_str(), item_name.c_str(), group_is_case_sensitive, item_is_case_sensitive);
}

bool char_in_set(const char ch, const char* const set)
{
	if (set == nullptr) return true;
	for (int i = 0; set[i] != '\0'; i++)
		if (ch == set[i])
			return true;
	return false;
}

/***************************************************************************
  函数名称：item_get_char
  功    能：取某项的内容，返回类型为char型
  输入参数：const char* const group_name               ：组名
		   const char* const item_name                ：项名
		   char& value                                ：读到的char的值（返回1时可信，返回0则不可信）
		   const char* const choice_set = nullptr     ：合法的char的集合（例如："YyNn"表示合法输入为Y/N且不分大小写，该参数有默认值nullptr，表示全部字符，即不检查）
		   const char def_value = DEFAULT_CHAR_VALUE  ：读不到/读到非法的情况下的默认值，该参数有默认值DEFAULT_CHAR_VALUE，分两种情况
															当值是   DEFAULT_CHAR_VALUE 时，返回0（值不可信）
															当值不是 DEFAULT_CHAR_VALUE 时，令value=def_value并返回1
		   const bool group_is_case_sensitive = false : 组名是否大小写敏感，true-大小写敏感 / 默认false-大小写不敏感
		   const bool item_is_case_sensitive = false  : 项名是否大小写敏感，true-大小写敏感 / 默认false-大小写不敏感
  返 回 值：1 - 取到正确值
			   未取到值/未取到正确值，设置了缺省值（包括设为缺省值）
		   0 - 未取到（只有为未指定默认值的情况下才会返回0）
  说    明：
***************************************************************************/
int config_file_tools::item_get_char(const char* const group_name, const char* const item_name, char& value,
	const char* const choice_set, const char def_value, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	/* 按需完成，根据需要改变return的值 */
	int success = 0;
	string raw;
	success = item_get_raw(group_name, item_name, raw, group_is_case_sensitive, item_is_case_sensitive);
	if (def_value == DEFAULT_CHAR_VALUE) {
		if (success) {
			istringstream iss(raw);
			char tmp;
			iss >> tmp;
			success = !iss.fail();
			if (success) {
				value = (char_in_set(tmp, choice_set) ? tmp : def_value);
				success = (char_in_set(tmp, choice_set) ? 1 : 0);
			}
			else {
				value = def_value;
				success = 0;
			}
		}
		else {
			value = def_value;
			success = 0;
		}
	}
	else {
		if (success) {
			istringstream iss(raw);
			char tmp;
			iss >> tmp;
			if (!iss.fail())
				value = (char_in_set(tmp, choice_set) ? tmp : def_value);
			else
				value = def_value;
			success = 1;
		}
		else {
			value = def_value;
			success = 1;
		}
	}
	return success;
}

/***************************************************************************
  函数名称：
  功    能：组名/项目为string方式，其余同上
  输入参数：
  返 回 值：
  说    明：因为工具函数一般在程序初始化阶段被调用，不会在程序执行中被高频次调用，
		   因此这里直接套壳，会略微影响效率，但不影响整体性能（对高频次调用，此方法不适合）
***************************************************************************/
int config_file_tools::item_get_char(const string& group_name, const string& item_name, char& value,
	const char* const choice_set, const char def_value, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	/* 本函数已实现 */
	return this->item_get_char(group_name.c_str(), item_name.c_str(), value, choice_set, def_value, group_is_case_sensitive, item_is_case_sensitive);
}

/***************************************************************************
  函数名称：item_get_int
  功    能：取某项的内容，返回类型为int型
  输入参数：const char* const group_name               ：组名
		   const char* const item_name                ：项名
		   int& value                                 ：读到的int的值（返回1时可信，返回0则不可信）
		   const int min_value = INT_MIN              : 期望数据范围的下限，默认为INT_MIN
		   const int max_value = INT_MAX              : 期望数据范围的上限，默认为INT_MAX
		   const int def_value = DEFAULT_INT_VALUE    ：读不到/读到非法的情况下的默认值，该参数有默认值 DEFAULT_INT_VALUE，分两种情况
															当值是   DEFAULT_INT_VALUE 时，返回0（值不可信）
															当值不是 DEFAULT_INT_VALUE 时，令value=def_value并返回1
		   const bool group_is_case_sensitive = false : 组名是否大小写敏感，true-大小写敏感 / 默认false-大小写不敏感
		   const bool item_is_case_sensitive = false  : 项名是否大小写敏感，true-大小写敏感 / 默认false-大小写不敏感
  返 回 值：取到合法值/默认值返回1，返回1时可信，返回0则不可信
  说    明：
***************************************************************************/
int config_file_tools::item_get_int(const char* const group_name, const char* const item_name, int& value,
	const int min_value, const int max_value, const int def_value, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	/* 按需完成，根据需要改变return的值 */
	int success = 0;
	string raw;
	success = item_get_raw(group_name, item_name, raw, group_is_case_sensitive, item_is_case_sensitive);
	if (def_value == DEFAULT_INT_VALUE) {
		if (success) {
			istringstream iss(raw);
			int tmp;
			iss >> tmp;
			if (!iss.fail()) {
				value = (tmp >= min_value && tmp <= max_value) ? tmp : def_value;
				success = (tmp >= min_value && tmp <= max_value) ? 1 : 0;
			}
			else {
				value = def_value;
				success = 0;
			}
		}
		else {
			value = def_value;
			success = 0;
		}
	}
	else {
		if (success) {
			istringstream iss(raw);
			int tmp;
			iss >> tmp;
			if (!iss.fail())
				value = (tmp >= min_value && tmp <= max_value) ? tmp : def_value;
			else
				value = def_value;
			success = 1;
		}
		else {
			value = def_value;
			success = 1;
		}
	}
	return success;
}

/***************************************************************************
  函数名称：
  功    能：组名/项目为string方式，其余同上
  输入参数：
  返 回 值：
  说    明：因为工具函数一般在程序初始化阶段被调用，不会在程序执行中被高频次调用，
		   因此这里直接套壳，会略微影响效率，但不影响整体性能（对高频次调用，此方法不适合）
***************************************************************************/
int config_file_tools::item_get_int(const string& group_name, const string& item_name, int& value,
	const int min_value, const int max_value, const int def_value, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	/* 本函数已实现 */
	return this->item_get_int(group_name.c_str(), item_name.c_str(), value, min_value, max_value, def_value, group_is_case_sensitive, item_is_case_sensitive);
}

/***************************************************************************
  函数名称：item_get_double
  功    能：取某项的内容，返回类型为double型
  输入参数：const char* const group_name                  ：组名
		   const char* const item_name                   ：项名
		   double& value                                 ：读到的int的值（返回1时可信，返回0则不可信）
		   const double min_value = __DBL_MIN__          : 期望数据范围的下限，默认为INT_MIN
		   const double max_value = __DBL_MAX__          : 期望数据范围的上限，默认为INT_MAX
		   const double def_value = DEFAULT_DOUBLE_VALUE ：读不到/读到非法的情况下的默认值，该参数有默认值DEFAULT_DOUBLE_VALUE，分两种情况
																当值是   DEFAULT_DOUBLE_VALUE 时，返回0（值不可信）
																当值不是 DEFAULT_DOUBLE_VALUE 时，令value=def_value并返回1
		   const bool group_is_case_sensitive = false     : 组名是否大小写敏感，true-大小写敏感 / 默认false-大小写不敏感
		   const bool item_is_case_sensitive = false      : 项名是否大小写敏感，true-大小写敏感 / 默认false-大小写不敏感
  返 回 值：success
  说    明：/
***************************************************************************/
int config_file_tools::item_get_double(const char* const group_name, const char* const item_name, double& value,
	const double min_value, const double max_value, const double def_value, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	/* 按需完成，根据需要改变return的值 */
	int success = 0;
	string raw;
	success = item_get_raw(group_name, item_name, raw, group_is_case_sensitive, item_is_case_sensitive);
	if (def_value == DEFAULT_DOUBLE_VALUE) {
		if (success) {
			istringstream iss(raw);
			double tmp;
			iss >> tmp;
			if (!iss.fail()) {
				value = (tmp >= min_value && tmp <= max_value) ? tmp : def_value;
				success = (tmp >= min_value && tmp <= max_value) ? 1 : 0;
			}
			else {
				value = def_value;
				success = 0;
			}
		}
		else {
			value = def_value;
			success = 0;
		}
	}
	else {
		if (success) {
			istringstream iss(raw);
			double tmp;
			iss >> tmp;
			if (!iss.fail())
				value = (tmp >= min_value && tmp <= max_value) ? tmp : def_value;
			else
				value = def_value;
			success = 1;
		}
		else {
			value = def_value;
			success = 1;
		}
	}
	return success;
}

/***************************************************************************
  函数名称：item_get_double
  功    能：组名/项目为string方式，其余同上
  输入参数：const string& group_name, const string& item_name, double& value,
	const double min_value, const double max_value, const double def_value, const bool group_is_case_sensitive, const bool item_is_case_sensitive
  返 回 值：success
  说    明：因为工具函数一般在程序初始化阶段被调用，不会在程序执行中被高频次调用，
		   因此这里直接套壳，会略微影响效率，但不影响整体性能（对高频次调用，此方法不适合）
***************************************************************************/
int config_file_tools::item_get_double(const string& group_name, const string& item_name, double& value,
	const double min_value, const double max_value, const double def_value, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	/* 本函数已实现 */
	return this->item_get_double(group_name.c_str(), item_name.c_str(), value, min_value, max_value, def_value, group_is_case_sensitive, item_is_case_sensitive);
}

/***************************************************************************
  函数名称：item_get_cstring
  功    能：取某项的内容，返回类型为char * / char []型
  输入参数：const char* const group_name                  ：组名
		   const char* const item_name                   ：项名
		   char *const value                             ：读到的C方式的字符串（返回1时可信，返回0则不可信）
		   const int str_maxlen                          ：指定要读的最大长度（含尾零）
																如果<1则返回空串(不是DEFAULT_CSTRING_VALUE，虽然现在两者相同，但要考虑default值可能会改)
																如果>MAX_STRLEN 则上限为MAX_STRLEN
		   const char* const def_str                     ：读不到情况下的默认值，该参数有默认值DEFAULT_CSTRING_VALUE，分两种情况
																当值是   DEFAULT_CSTRING_VALUE 时，返回0（值不可信）
																当值不是 DEFAULT_CSTRING_VALUE 时，令value=def_value并返回1（注意，不是直接=）
		   const bool group_is_case_sensitive = false : 组名是否大小写敏感，true-大小写敏感 / 默认false-大小写不敏感
		   const bool item_is_case_sensitive = false  : 项名是否大小写敏感，true-大小写敏感 / 默认false-大小写不敏感
  返 回 值：success
  说    明：1、为简化，未对\"等做转义处理，均按普通字符
		   2、含尾零的最大长度为str_maxlen，调用时要保证有足够空间
		   3、如果 str_maxlen 超过了系统预设的上限 MAX_STRLEN，则按 MAX_STRLEN 取
***************************************************************************/
int config_file_tools::item_get_cstring(const char* const group_name, const char* const item_name, char* const value,
	const int str_maxlen, const char* const def_value, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	/* 按需完成，根据需要改变return的值 */
	int mxlen;
	if (str_maxlen < 1) {
		value[0] = '\0';
		return 0;
	}
	else if (str_maxlen > MAX_STRLEN)
		mxlen = MAX_STRLEN;
	else
		mxlen = str_maxlen;

	int success = 0;
	string raw;
	success = item_get_raw(group_name, item_name, raw, group_is_case_sensitive, item_is_case_sensitive);
	if (def_value == DEFAULT_CSTRING_VALUE) {
		if (success) {
			istringstream iss(raw);
			string tmp;
			iss >> tmp;
			success = !iss.fail();
			if (success && tmp != "") {
				/*
				strncpy(value, tmp.c_str(), mxlen - 1);
				value[mxlen - 1] = '\0';
				*/
				size_t copy_len = (tmp.length() < static_cast<size_t>(mxlen - 1)) ? tmp.length() : static_cast<size_t>(mxlen - 1);
				strncpy(value, tmp.c_str(), copy_len);
				value[copy_len] = '\0';
			}
			else {
				size_t copy_len = (strlen(def_value) < static_cast<size_t>(mxlen - 1)) ? strlen(def_value) : static_cast<size_t>(mxlen - 1);
				strncpy(value, def_value, copy_len);
				value[copy_len] = '\0';
			}
		}
		else {
			size_t copy_len = (strlen(def_value) < static_cast<size_t>(mxlen - 1)) ? strlen(def_value) : static_cast<size_t>(mxlen - 1);
			strncpy(value, def_value, copy_len);
			value[copy_len] = '\0';
			success = 0;
		}
	}
	else {
		if (success) {
			istringstream iss(raw);
			string tmp;
			iss >> tmp;
			success = !iss.fail();
			if (success && tmp!="") {
				size_t copy_len = (tmp.length() < static_cast<size_t>(mxlen - 1)) ? tmp.length() : static_cast<size_t>(mxlen - 1);
				strncpy(value, tmp.c_str(), copy_len);
				value[copy_len] = '\0';
				success = 1;
			}
			else {
				size_t copy_len = (strlen(def_value) < static_cast<size_t>(mxlen - 1)) ? strlen(def_value) : static_cast<size_t>(mxlen - 1);
				strncpy(value, def_value, copy_len);
				value[copy_len] = '\0';
				success = 1;
			}
		}
		else {
			size_t copy_len = (strlen(def_value) < static_cast<size_t>(mxlen - 1)) ? strlen(def_value) : static_cast<size_t>(mxlen - 1);
			strncpy(value, def_value, copy_len);
			value[copy_len] = '\0';
			success = 1;
		}
	}
	return success;
}

/***************************************************************************
  函数名称：item_get_cstring
  功    能：组名/项目为string方式，其余同上
  输入参数：const string& group_name, const string& item_name, char* const value,
	const int str_maxlen, const char* const def_value, const bool group_is_case_sensitive, const bool item_is_case_sensitive
  返 回 值：success
  说    明：因为工具函数一般在程序初始化阶段被调用，不会在程序执行中被高频次调用，
		   因此这里直接套壳，会略微影响效率，但不影响整体性能（对高频次调用，此方法不适合）
***************************************************************************/
int config_file_tools::item_get_cstring(const string& group_name, const string& item_name, char* const value,
	const int str_maxlen, const char* const def_value, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	/* 本函数已实现 */
	return item_get_cstring(group_name.c_str(), item_name.c_str(), value, str_maxlen, def_value, group_is_case_sensitive, item_is_case_sensitive);
}

/***************************************************************************
  函数名称：item_get_string
  功    能：取某项的内容，返回类型为 string 型
  输入参数：const char* const group_name               ：组名
		   const char* const item_name                ：项名
		   string &value                              ：读到的string方式的字符串（返回1时可信，返回0则不可信）
		   const string &def_value                    ：读不到情况下的默认值，该参数有默认值DEFAULT_STRING_VALUE，分两种情况
															当值是   DEFAULT_STRING_VALUE 时，返回0（值不可信）
															当值不是 DEFAULT_STRING_VALUE 时，令value=def_value并返回1
		   const bool group_is_case_sensitive = false : 组名是否大小写敏感，true-大小写敏感 / 默认false-大小写不敏感
		   const bool item_is_case_sensitive = false  : 项名是否大小写敏感，true-大小写敏感 / 默认false-大小写不敏感
  返 回 值：success
  说    明：为简化，未对\"等做转义处理，均按普通字符
***************************************************************************/
int config_file_tools::item_get_string(const char* const group_name, const char* const item_name, string& value,
	const string& def_value, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	/* 按需完成，根据需要改变return的值 */
	string raw;
	int success = 0;
	success = item_get_raw(group_name, item_name, raw, group_is_case_sensitive, item_is_case_sensitive);
	if (def_value == DEFAULT_STRING_VALUE) {
		if (success) {
			istringstream iss(raw);
			string tmp;
			iss >> tmp;
			value = (!iss.fail()) ? tmp : def_value;
			success = !iss.fail();
		}
		else {
			value = def_value;
			success = 0;
		}
	}
	else {
		if (success) {
			istringstream iss(raw);
			string tmp;
			iss >> tmp;
			value = (!iss.fail()) ? tmp : def_value;
			success = 1;
		}
		else {
			value = def_value;
			success = 1;
		}
	}
	return success;
}

/***************************************************************************
  函数名称：item_get_string
  功    能：组名/项目为string方式，其余同上
  输入参数：const string& group_name, const string& item_name, string& value,
	const string& def_value, const bool group_is_case_sensitive, const bool item_is_case_sensitive
  返 回 值：success
  说    明：因为工具函数一般在程序初始化阶段被调用，不会在程序执行中被高频次调用，
		   因此这里直接套壳，会略微影响效率，但不影响整体性能（对高频次调用，此方法不适合）
***************************************************************************/
int config_file_tools::item_get_string(const string& group_name, const string& item_name, string& value,
	const string& def_value, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	/* 本函数已实现 */
	return this->item_get_string(group_name.c_str(), item_name.c_str(), value, def_value, group_is_case_sensitive, item_is_case_sensitive);
}

/***************************************************************************
  函数名称：item_get_ipaddr
  功    能：取某项的内容，返回类型为 IPv4 地址的32bit整型（主机序）
  输入参数：const char* const group_name               ：组名
		   const char* const item_name                ：项名
		   unsigned int &value                        ：读到的IP地址，32位整型方式（返回1时可信，返回0则不可信）
		   const unsigned int &def_value              ：读不到情况下的默认值，该参数有默认值DEFAULT_IPADDR_VALUE，分两种情况
															当值是   DEFAULT_IPADDR_VALUE 时，返回0（值不可信）
															当值不是 DEFAULT_IPADDR_VALUE 时，令value=def_value并返回1
		   const bool group_is_case_sensitive = false : 组名是否大小写敏感，true-大小写敏感 / 默认false-大小写不敏感
		   const bool item_is_case_sensitive = false  : 项名是否大小写敏感，true-大小写敏感 / 默认false-大小写不敏感
  返 回 值：success
  说    明：
***************************************************************************/
int config_file_tools::item_get_ipaddr(const char* const group_name, const char* const item_name, unsigned int& value,
	const unsigned int& def_value, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	/* 按需完成，根据需要改变return的值 */
	string raw;
	int success = 0;
	success = item_get_raw(group_name, item_name, raw, group_is_case_sensitive, item_is_case_sensitive);
	if (def_value == DEFAULT_IPADDR_VALUE) {
		if (success) {
			istringstream iss(raw);
			string ip;
			unsigned int tmp;
			iss >> ip;
			success = !iss.fail();
			if (success && is_valid_ippaddr(ip, tmp))
				value = tmp;
			else {
				value = def_value;
				success = 0;
			}
		}
		else {
			value = def_value;
			success = 0;
		}
	}
	else {
		if (success) {
			istringstream iss(raw);
			string ip;
			unsigned int tmp;
			iss >> ip;
			success = !iss.fail();
			if (success && is_valid_ippaddr(ip, tmp))
				value = tmp;
			else {
				value = def_value;
				success = 1;
			}
		}
		else {
			value = def_value;
			success = 1;
		}
	}
	return success;
}

/***************************************************************************
  函数名称：
  功    能：组名/项目为string方式，其余同上
  输入参数：
  返 回 值：
  说    明：因为工具函数一般在程序初始化阶段被调用，不会在程序执行中被高频次调用，
		   因此这里直接套壳，会略微影响效率，但不影响整体性能（对高频次调用，此方法不适合）
***************************************************************************/
int config_file_tools::item_get_ipaddr(const string& group_name, const string& item_name, unsigned int& value,
	const unsigned int& def_value, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	/* 本函数已实现 */
	return this->item_get_ipaddr(group_name.c_str(), item_name.c_str(), value, def_value, group_is_case_sensitive, item_is_case_sensitive);
}