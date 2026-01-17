/* 2353626 贺胡鸣 大数据 */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <cstdio>
#include "../include/class_aat.h"
//如有必要，可以加入其它头文件
using namespace std;

#if !ENABLE_LIB_COMMON_TOOLS //不使用lib才有效

/* ---------------------------------------------------------------
	 允许加入其它需要static函数（内部工具用）
   ---------------------------------------------------------------- */
   /***************************************************************************
	 函数名称：extargs_type_to_string
	 功    能：
	 输入参数：
	 返 回 值：string
	 说    明：将私有数据成员转换成const string类型便于输出
	***************************************************************************/
static string extargs_type_to_string(ST_EXTARGS_TYPE type)
{
	int index = static_cast<int>(type) - 1;

	if (index >= 0 && index < static_cast<int>(sizeof(TYPE) / sizeof(TYPE[0]))) {
		return TYPE[index];
	}
	else if (type == ST_EXTARGS_TYPE::null) {
		return "NULL";
	}
	else if (type == ST_EXTARGS_TYPE::tmax) {
		return "tmax";
	}
	else {
		return "unknown";
	}
}

/***************************************************************************
	 函数名称：getArraySize
	 功    能：
	 输入参数：
	 返 回 值：size_t
	 说    明：求数组元素个数
	***************************************************************************/
template<typename T>
static size_t getSetSize(const T* const set, T end_marker) {
	if (set == nullptr) return 0;

	size_t count = 0;
	while (set[count] != end_marker) {
		count++;
	}
	return count;
}

static int getstringsetsize(const string* const set)
{
	int count = 0;
	while (set[count] != "") {
		count++;
	}
	return count;
}

//determine whether the string is an integer
static bool isInteger(const string& str) {
	if (str.empty()) return false;

	try {
		size_t pos;
		int value = stoi(str, &pos);
		return pos == str.length();
	}
	catch (...) {
		return false;
	}
}

//determine whether the string is double
static bool isDouble(const string& str) {
	if (str.empty()) return false;

	try {
		size_t pos;
		double value = stod(str, &pos);
		return pos == str.length();
	}
	catch (...) {
		return false;
	}
}

//check whether the val is in the set
template<typename T>
static bool inset(const T val,const T* const set,T end_marker)
{
	int size = (int)getSetSize(set, end_marker);
	int result = 0;
	for (int i = 0; i < size; i++) {
		if (set[i] == val) {
			result = 1;
			break;
		}
	}
	return (result == 1) ? true : false;
}

//check whether the val is in the string set
static bool instrset(string val, string* set, string end_marker)
{
	int size = getstringsetsize(set);
	int result = 0;
	for (int i = 0; i < size; i++) {
		if (set[i] == val) {
			result = 1;
			break;
		}
	}
	return (result == 1) ? true : false;
}

//convert the ip address into the u_int digit
static bool ip_to_hex(const string& ip_str,unsigned int & result) 
{
	unsigned int a, b, c, d;

	if (sscanf(ip_str.c_str(), "%u.%u.%u.%u", &a, &b, &c, &d) != 4)
		return false;

	if (a > 255 || b > 255 || c > 255 || d > 255)
		return false;

	result = (a << 24) | (b << 16) | (c << 8) | d;
	return true;
}

//convert type
static string converttype(ST_EXTARGS_TYPE type)
{
	return TYPE[static_cast<int>(type) - 1];
}


   /***************************************************************************
	 函数名称：
	 功    能：无参初始化
	 输入参数：
	 返 回 值：
	 说    明：null
	***************************************************************************/
args_analyse_tools::args_analyse_tools()
{
	args_name = "";
}

/***************************************************************************
  函数名称：
  功    能：四个参数的初始化
  输入参数：
  返 回 值：
  说    明：boolean
 ***************************************************************************/
args_analyse_tools::args_analyse_tools(const char* name, const enum ST_EXTARGS_TYPE type, const int ext_num, const bool def)
{
	args_name = name;
	extargs_type = type;
	extargs_num = ext_num;
	extargs_bool_default = def;
	args_existed = 0;
}

/***************************************************************************
  函数名称：
  功    能：六参初始化
  输入参数：
  返 回 值：
  说    明：int_with_default、int_with_error
 ***************************************************************************/
args_analyse_tools::args_analyse_tools(const char* name, const enum ST_EXTARGS_TYPE type, const int ext_num, const int def, const int _min, const int _max)
{
	args_name = name;
	extargs_type = type;
	extargs_num = ext_num;
	extargs_int_default = def;
	extargs_int_min = _min;
	extargs_int_max = _max;
	args_existed = 0;
}

/***************************************************************************
  函数名称：
  功    能：五参初始化
  输入参数：
  返 回 值：
  说    明：int_with_set_default、int_with_set_error
 ***************************************************************************/
args_analyse_tools::args_analyse_tools(const char* name, const enum ST_EXTARGS_TYPE type, const int ext_num, const int def_of_set_pos, const int* const set)
{
	args_name = name;
	extargs_type = type;
	extargs_num = ext_num;
	int mx_pos = (int)getSetSize(set, INVALID_INT_VALUE_OF_SET); //5
	if (def_of_set_pos < 0 || def_of_set_pos >= mx_pos)
		extargs_int_default = set[0];
	else
		extargs_int_default = set[def_of_set_pos];
	extargs_int_set = (int*)set;
	args_existed = 0;
}

/***************************************************************************
  函数名称：
  功    能：四参字符串初始化
  输入参数：
  返 回 值：
  说    明：str、ipaddr_with_default、ipaddr_with_error
 ***************************************************************************/
args_analyse_tools::args_analyse_tools(const char* name, const enum ST_EXTARGS_TYPE type, const int ext_num, const string def)
{
	args_name = name;
	extargs_type = type;
	extargs_num = ext_num;
	extargs_string_default = def;
	if (type == ST_EXTARGS_TYPE::ipaddr_with_default)
		bool n1 = ip_to_hex(def, extargs_ipaddr_default);
	else if (type == ST_EXTARGS_TYPE::ipaddr_with_error)
		extargs_ipaddr_default = 0;
	args_existed = 0;
}

/***************************************************************************
  函数名称：
  功    能：五参字符串初始化
  输入参数：
  返 回 值：
  说    明：str_with_set_default、str_with_set_error
 ***************************************************************************/
args_analyse_tools::args_analyse_tools(const char* name, const enum ST_EXTARGS_TYPE type, const int ext_num, const int def_of_set_pos, const string* const set)
{
	args_name = name;
	extargs_type = type;
	extargs_num = ext_num;
	int mx_pos = getstringsetsize(set);
	if (def_of_set_pos < 0 || def_of_set_pos >= mx_pos)
		extargs_string_default = set[0];
	else
		extargs_string_default = set[def_of_set_pos];
	extargs_string_set = (string*)set;
	args_existed = 0;
}

/***************************************************************************
  函数名称：
  功    能：六参小数初始化
  输入参数：
  返 回 值：
  说    明：double_with_default、double_with_error
 ***************************************************************************/
args_analyse_tools::args_analyse_tools(const char* name, const enum ST_EXTARGS_TYPE type, const int ext_num, const double	def, const double _min, const double _max)
{
	args_name = name;
	extargs_type = type;
	extargs_num = ext_num;
	extargs_double_default = def;
	extargs_double_min = _min;
	extargs_double_max = _max;
	args_existed = 0;
}

/***************************************************************************
  函数名称：
  功    能：五参小数初始化
  输入参数：
  返 回 值：
  说    明：double_with_set_default、double_with_set_error
 ***************************************************************************/
args_analyse_tools::args_analyse_tools(const char* name, const enum ST_EXTARGS_TYPE type, const int ext_num, const int def_of_set_pos, const double* const set)
{
	args_name = name;
	extargs_type = type;
	extargs_num = ext_num;
	int mx_pos = (int)getSetSize(set, INVALID_DOUBLE_VALUE_OF_SET);
	if (def_of_set_pos < 0 || def_of_set_pos >= mx_pos)
		extargs_double_default = set[0];
	else
		extargs_double_default = set[def_of_set_pos];
	extargs_double_set = (double*)set;
	args_existed = 0;
}

/***************************************************************************
  函数名称：
  功    能：析构函数
  输入参数：
  返 回 值：
  说    明：
 ***************************************************************************/
args_analyse_tools::~args_analyse_tools()
{
}

/* ---------------------------------------------------------------
	 允许AAT中自定义成员函数的实现（private）
   ---------------------------------------------------------------- */

   /***************************************************************************
	 函数名称：
	 功    能：
	 输入参数：
	 返 回 值：
	 说    明：已实现，不要动
	***************************************************************************/
const string args_analyse_tools::get_name() const
{
	return this->args_name;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：加!!后，只能是0/1
			已实现，不要动
 ***************************************************************************/
const int args_analyse_tools::existed() const
{
	return !!this->args_existed;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：已实现，不要动
 ***************************************************************************/
const int args_analyse_tools::get_int() const
{
	return this->extargs_int_value;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：已实现，不要动
 ***************************************************************************/
const double args_analyse_tools::get_double() const
{
	return this->extargs_double_value;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：已实现，不要动
 ***************************************************************************/
const string args_analyse_tools::get_string() const
{
	return this->extargs_string_value;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：已实现，不要动
 ***************************************************************************/
const unsigned int args_analyse_tools::get_ipaddr() const
{
	return this->extargs_ipaddr_value;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：将 extargs_ipaddr_value 的值从 0x7f000001 转为 "127.0.0.1"
 ***************************************************************************/
const string args_analyse_tools::get_str_ipaddr() const
{
	unsigned int a = (extargs_ipaddr_value >> 24) & 0xFF;
	unsigned int b = (extargs_ipaddr_value >> 16) & 0xFF;
	unsigned int c = (extargs_ipaddr_value >> 8) & 0xFF;
	unsigned int d = extargs_ipaddr_value & 0xFF;

	char buffer[16];
	sprintf(buffer, "%u.%u.%u.%u", a, b, c, d);

	return string(buffer); //此句根据需要修改
}


/***************************************************************************
  函数名称：args_analyse_process
  功    能：
  输入参数：follow_up_args：是否有后续参数
			0  ：无后续参数
			1  ：有后续参数
  返 回 值：
  说    明：友元函数
***************************************************************************/
int args_analyse_process(const int argc, const char* const* const argv, args_analyse_tools* const args, const int follow_up_args)
{
	if (1) { // no fixed args
		bool visited[100] = { false };

		//initialization
		int k = 0;
		while (args[k].args_name != "") {
			if (args[k].extargs_type == ST_EXTARGS_TYPE::int_with_default || args[k].extargs_type == ST_EXTARGS_TYPE::int_with_error || args[k].extargs_type == ST_EXTARGS_TYPE::int_with_set_default || args[k].extargs_type == ST_EXTARGS_TYPE::int_with_set_error)
				args[k].extargs_int_value = args[k].extargs_int_default;
			else if (args[k].extargs_type == ST_EXTARGS_TYPE::double_with_default || args[k].extargs_type == ST_EXTARGS_TYPE::double_with_error || args[k].extargs_type == ST_EXTARGS_TYPE::double_with_set_default || args[k].extargs_type == ST_EXTARGS_TYPE::double_with_set_error)
				args[k].extargs_double_value = args[k].extargs_double_default;
			else if (args[k].extargs_type == ST_EXTARGS_TYPE::str)
				args[k].extargs_string_value = args[k].extargs_string_default;
			else if (args[k].extargs_type == ST_EXTARGS_TYPE::str_with_set_default || args[k].extargs_type == ST_EXTARGS_TYPE::str_with_set_error)
				args[k].extargs_string_value = args[k].extargs_string_default;
			else if (args[k].extargs_type == ST_EXTARGS_TYPE::ipaddr_with_default|| args[k].extargs_type == ST_EXTARGS_TYPE::ipaddr_with_error)
				args[k].extargs_ipaddr_value = args[k].extargs_ipaddr_default;
			// ...
			k++;
		}
		if (argc == 1)  // no args
			return 1;
		
		
		// args
		for (int i = 1; i < argc; i++) {
			string cur = argv[i];
			if (cur[0] == '-' && cur[1] == '-' && !visited[i]) {
				int j = 0, flag = 1;
				while (args[j].args_name != "") {
					if (args[j].args_name == cur) {
						flag = 0;
						break;
					}
					else 
						j++;
				}
				if (flag) {
					cout << "参数[" << cur << "]非法." << endl;
					return -1;
				}
	// Situation 1
				if (args[j].extargs_num == 0) {  //"--help" || "--bool" || "--t" || "--mirror"
					if (args[j].args_existed != 0) {
						cout << "参数[" << args[j].args_name << "]重复." << endl;
						return -1;
					}
					visited[i] = true;
					args[j].args_existed = 1;
					continue;
				}
				else {
					if (i==argc-1||(argv[i+1][0]=='-'&&argv[i+1][1]=='-')) {
						if (args[j].extargs_type==ST_EXTARGS_TYPE::int_with_default)
							cout << "参数[" << cur << "]的附加参数不足. (类型:int, 范围[" << args[j].extargs_int_min << ".." << args[j].extargs_int_max << "] 缺省:" << args[j].extargs_int_default << ")" << endl;
						else if(args[j].extargs_type == ST_EXTARGS_TYPE::int_with_error)
							cout << "参数[" << cur << "]的附加参数不足. (类型:int, 范围[" << args[j].extargs_int_min << ".." << args[j].extargs_int_max << "])" << endl;
						else if(args[j].extargs_type == ST_EXTARGS_TYPE::double_with_default)
							cout << "参数[" << cur << "]的附加参数不足. (类型:double, 范围[" << args[j].extargs_double_min << ".." << args[j].extargs_double_max << "] 缺省:" << args[j].extargs_double_default << ")" << endl;
						else if(args[j].extargs_type == ST_EXTARGS_TYPE::double_with_error)
							cout << "参数[" << cur << "]的附加参数不足. (类型:double, 范围[" << args[j].extargs_double_min << ".." << args[j].extargs_double_max << "])" << endl;
						else if (args[j].extargs_type == ST_EXTARGS_TYPE::int_with_set_default) {
							cout << "参数[" << cur << "]的附加参数不足. (类型:int, 可取值[";
							int intsetsize = (int)getSetSize(args[j].extargs_int_set, INVALID_INT_VALUE_OF_SET);
							for (int t = 0; t < intsetsize; t++) {
								cout << args[j].extargs_int_set[t];
								if (t != intsetsize - 1)
									cout << "/";
							}
							cout << "] 缺省:" << args[j].extargs_int_default << ")" << endl;
						}
						else if (args[j].extargs_type == ST_EXTARGS_TYPE::int_with_set_error) {
							cout << "参数[" << cur << "]的附加参数不足. (类型:int, 可取值[";
							int intsetsize = (int)getSetSize(args[j].extargs_int_set, INVALID_INT_VALUE_OF_SET);
							for (int t = 0; t < intsetsize; t++) {
								cout << args[j].extargs_int_set[t];
								if (t != intsetsize - 1)
									cout << "/";
							}
							cout << "])" << endl;
						}
						else if (args[j].extargs_type == ST_EXTARGS_TYPE::double_with_set_default) {
							cout << "参数[" << cur << "]的附加参数不足. (类型:double, 可取值[";
							int doublesetsize = (int)getSetSize(args[j].extargs_double_set, INVALID_DOUBLE_VALUE_OF_SET);
							for (int t = 0; t < doublesetsize; t++) {
								cout << args[j].extargs_double_set[t];
								if (t != doublesetsize - 1)
									cout << "/";
							}
							cout << "] 缺省:" << args[j].extargs_double_default << ")" << endl;
						}
						else if (args[j].extargs_type == ST_EXTARGS_TYPE::double_with_set_error) {
							cout << "参数[" << cur << "]的附加参数不足. (类型:double, 可取值[";
							int doublesetsize = (int)getSetSize(args[j].extargs_double_set, INVALID_DOUBLE_VALUE_OF_SET);
							for (int t = 0; t < doublesetsize; t++) {
								cout << args[j].extargs_double_set[t];
								if (t != doublesetsize - 1)
									cout << "/";
							}
							cout << "])" << endl;
						}
						else if (args[j].extargs_type == ST_EXTARGS_TYPE::str) {
							cout << "参数[" << cur << "]的附加参数不足. (类型:string";
							if (args[j].extargs_string_default == "")
								cout << ")" << endl;
							else
								cout << " 缺省:" << args[j].extargs_string_default << ")" << endl;
						}
						else if (args[j].extargs_type == ST_EXTARGS_TYPE::str_with_set_default) {
							cout << "参数[" << cur << "]的附加参数不足. (类型:string, 可取值[";
							int stringsetsize = getstringsetsize(args[j].extargs_string_set);
							for (int t = 0; t < stringsetsize; t++) {
								cout << args[j].extargs_string_set[t];
								if (t != stringsetsize - 1)
									cout << "/";
							}
							cout << "] 缺省:" << args[j].extargs_string_default << ")" << endl;
						}
						else if (args[j].extargs_type == ST_EXTARGS_TYPE::str_with_set_error) {
							cout << "参数[" << cur << "]的附加参数不足. (类型:string, 可取值[";
							int stringsetsize = getstringsetsize(args[j].extargs_string_set);
							for (int t = 0; t < stringsetsize; t++) {
								cout << args[j].extargs_string_set[t];
								if (t != stringsetsize - 1)
									cout << "/";
							}
							cout << "])" << endl;
						}
						else if (args[j].extargs_type == ST_EXTARGS_TYPE::ipaddr_with_default|| args[j].extargs_type == ST_EXTARGS_TYPE::ipaddr_with_error) {
							cout << "参数[" << cur << "]的附加参数不足. (类型:IP地址";
							if (args[j].extargs_type == ST_EXTARGS_TYPE::ipaddr_with_error)
								cout << ")" << endl;
							else
								cout << " 缺省:" << args[j].extargs_string_default << ")" << endl;
						}
						else if (cur == "--n" || cur == "--l") 
							cout << "参数[" << cur << "]的附加参数不足. (类型:int, 范围[" << args[j].extargs_int_min << ".." << args[j].extargs_int_max << "] 缺省:" << args[j].extargs_int_default << ")" << endl;
						
						//...
						return -1;
					}

					string add = argv[i + 1];
					//check whether the additional param fit the request
					if (args[j].args_existed != 0) {
						cout << "参数[" << args[j].args_name << "]重复." << endl;
						return -1;
					}
	// Situation 2
					else if (args[j].extargs_type==ST_EXTARGS_TYPE::int_with_default || args[j].extargs_type == ST_EXTARGS_TYPE ::int_with_error) {
						if (!isInteger(add)) {
							if (args[j].extargs_type == ST_EXTARGS_TYPE::int_with_default)
								cout << "参数[" << cur << "]的附加参数不是整数. (类型:int, 范围[" << args[j].extargs_int_min << ".." << args[j].extargs_int_max << "] 缺省:" << args[j].extargs_int_default << ")" << endl;
							else
								cout << "参数[" << cur << "]的附加参数不是整数. (类型:int, 范围[" << args[j].extargs_int_min << ".." << args[j].extargs_int_max << "])" << endl;
							return -1;
						}
						else {
							int int_add = stoi(add);
							if (int_add< args[j].extargs_int_min || int_add>args[j].extargs_int_max) {
								if (args[j].extargs_type == ST_EXTARGS_TYPE::int_with_default) {
									args[j].extargs_int_value = args[j].extargs_int_default;
									visited[i + 1] = true;
									args[j].args_existed = 1;
								}
								else if (args[j].extargs_type == ST_EXTARGS_TYPE::int_with_error) {
									cout << "参数[" << cur << "]的附加参数值(" << int_add << ")非法. (类型:int, 范围[" << args[j].extargs_int_min << ".." << args[j].extargs_int_max << "])" << endl;
									return -1;
								}
							}
							else {
								args[j].extargs_int_value = int_add;
								visited[i + 1] = true;
								args[j].args_existed = 1;
							}
						}
					}
	// Situation 3
					else if (args[j].extargs_type == ST_EXTARGS_TYPE::double_with_default || args[j].extargs_type == ST_EXTARGS_TYPE::double_with_error) {
						if (!isDouble(add)) {
							if (args[j].extargs_type == ST_EXTARGS_TYPE::double_with_default)
								cout << "参数[" << cur << "]的附加参数不是浮点数. (类型:double, 范围[" << args[j].extargs_double_min << ".." << args[j].extargs_double_max << "] 缺省:" << args[j].extargs_double_default << ")" << endl;
							else
								cout << "参数[" << cur << "]的附加参数不是浮点数. (类型:double, 范围[" << args[j].extargs_double_min << ".." << args[j].extargs_double_max << "])" << endl;
							return -1;
						}
						else {
							double double_add = stod(add);
							if (double_add< args[j].extargs_double_min || double_add>args[j].extargs_double_max) {
								if (args[j].extargs_type == ST_EXTARGS_TYPE::double_with_default) {
									args[j].extargs_double_value = args[j].extargs_double_default;
									visited[i + 1] = true;
									args[j].args_existed = 1;
								}
								else if (args[j].extargs_type == ST_EXTARGS_TYPE::double_with_error) {
									cout << "参数[" << cur << "]的附加参数值(" << double_add << ")非法. (类型:double, 范围[" << args[j].extargs_double_min << ".." << args[j].extargs_double_max << "])" << endl;
									return -1;
								}
							}
							else {
								args[j].extargs_double_value = double_add;
								visited[i + 1] = true;
								args[j].args_existed = 1;
							}
						}
					}
	// Situation 4
					else if (args[j].extargs_type==ST_EXTARGS_TYPE::int_with_set_default|| args[j].extargs_type == ST_EXTARGS_TYPE::int_with_set_error) {
						if (!isInteger(add)) {
							if (args[j].extargs_type == ST_EXTARGS_TYPE::int_with_set_default) {
								cout << "参数[" << cur << "]的附加参数不是整数. (类型:int, 可取值[";
								int intsetsize = (int)getSetSize(args[j].extargs_int_set, INVALID_INT_VALUE_OF_SET);
								for (int t = 0; t < intsetsize; t++) {
									cout << args[j].extargs_int_set[t];
									if (t != intsetsize - 1)
										cout << "/";
								}
								cout << "] 缺省:" << args[j].extargs_int_default << ")" << endl;
							}
							else {
								cout << "参数[" << cur << "]的附加参数不是整数. (类型:int, 可取值[";
								int intsetsize = (int)getSetSize(args[j].extargs_int_set, INVALID_INT_VALUE_OF_SET);
								for (int t = 0; t < intsetsize; t++) {
									cout << args[j].extargs_int_set[t];
									if (t != intsetsize - 1)
										cout << "/";
								}
								cout << "])" << endl;
							}
								return -1;
						}
						else {
							int int_add = stoi(add);
							if (!inset(int_add,args[j].extargs_int_set, INVALID_INT_VALUE_OF_SET)) {
								if (args[j].extargs_type == ST_EXTARGS_TYPE::int_with_set_default) {
									args[j].extargs_int_value = args[j].extargs_int_default;
									visited[i + 1] = true;
									args[j].args_existed = 1;
								}
								else if (args[j].extargs_type == ST_EXTARGS_TYPE::int_with_set_error) {
									cout << "参数[" << cur << "]的附加参数值(" << int_add << ")非法. (类型:int, 可取值[";
									int intsetsize = (int)getSetSize(args[j].extargs_int_set, INVALID_INT_VALUE_OF_SET);
									for (int t = 0; t < intsetsize; t++) {
										cout << args[j].extargs_int_set[t];
										if (t != intsetsize - 1)
											cout << "/";
									}
									cout << "])" << endl;
									return -1;
								}
							}
							else {
								args[j].extargs_int_value = int_add;
								visited[i + 1] = true;
								args[j].args_existed = 1;
							}
						}
					}
	// Situation 5
					else if (args[j].extargs_type == ST_EXTARGS_TYPE::double_with_set_default || args[j].extargs_type == ST_EXTARGS_TYPE::double_with_set_error) {
						if (!isDouble(add)) {
							if (args[j].extargs_type == ST_EXTARGS_TYPE::double_with_set_default) {
								cout << "参数[" << cur << "]的附加参数不是整数. (类型:double, 可取值[";
								int doublesetsize = (int)getSetSize(args[j].extargs_double_set, INVALID_DOUBLE_VALUE_OF_SET);
								for (int t = 0; t < doublesetsize; t++) {
									cout << args[j].extargs_double_set[t];
									if (t != doublesetsize - 1)
										cout << "/";
								}
								cout << "] 缺省:" << args[j].extargs_double_default << ")" << endl;
							}
							else {
								cout << "参数[" << cur << "]的附加参数不是整数. (类型:double, 可取值[";
								int doublesetsize = (int)getSetSize(args[j].extargs_double_set, INVALID_DOUBLE_VALUE_OF_SET);
								for (int t = 0; t < doublesetsize; t++) {
									cout << args[j].extargs_double_set[t];
									if (t != doublesetsize - 1)
										cout << "/";
								}
								cout << "])" << endl;
							}
							return -1;
						}
						else {
							double double_add = stod(add);
							if (!inset(double_add, args[j].extargs_double_set, INVALID_DOUBLE_VALUE_OF_SET)) {
								if (args[j].extargs_type == ST_EXTARGS_TYPE::double_with_set_default) {
									args[j].extargs_double_value = args[j].extargs_double_default;
									visited[i + 1] = true;
									args[j].args_existed = 1;
								}
								else if (args[j].extargs_type == ST_EXTARGS_TYPE::double_with_set_error) {
									cout << "参数[" << cur << "]的附加参数值(" << double_add << ")非法. (类型:double, 可取值[";
									int doublesetsize = (int)getSetSize(args[j].extargs_double_set, INVALID_DOUBLE_VALUE_OF_SET);
									for (int t = 0; t < doublesetsize; t++) {
										cout << args[j].extargs_double_set[t];
										if (t != doublesetsize - 1)
											cout << "/";
									}
									cout << "])" << endl;
									return -1;
								}
							}
							else {
								args[j].extargs_double_value = double_add;
								visited[i + 1] = true;
								args[j].args_existed = 1;
							}
						}
					}
	// Situation 6
					else if (args[j].extargs_type == ST_EXTARGS_TYPE::str) {
						args[j].extargs_string_value = argv[i + 1];
						args[j].args_existed = 1;
						visited[i + 1] = true;
					}
	// Situation 7
					else if (args[j].extargs_type == ST_EXTARGS_TYPE::str_with_set_default|| args[j].extargs_type == ST_EXTARGS_TYPE::str_with_set_error) {
						string str_add = argv[i + 1];
						if (!instrset(str_add, args[j].extargs_string_set, "")) {
							if (args[j].extargs_type == ST_EXTARGS_TYPE::str_with_set_default) {
								args[j].extargs_string_value = args[j].extargs_string_default;
								visited[i + 1] = true;
								args[j].args_existed = 1;
							}
							else if (args[j].extargs_type == ST_EXTARGS_TYPE::str_with_set_error) {
								cout << "参数[" << cur << "]的附加参数值(" << str_add << ")非法. (类型:string, 可取值[";
								int strsetsize = getstringsetsize(args[j].extargs_string_set);
								for (int t = 0; t < strsetsize; t++) {
									cout << args[j].extargs_string_set[t];
									if (t != strsetsize - 1)
										cout << "/";
								}
								cout << "])" << endl;
								return -1;
							}
						}
						else {
							args[j].extargs_string_value = str_add;
							visited[i + 1] = true;
							args[j].args_existed = 1;
						}
					}
	// Situation 8
					else if (args[j].extargs_type == ST_EXTARGS_TYPE::ipaddr_with_default|| args[j].extargs_type == ST_EXTARGS_TYPE::ipaddr_with_error) {
						if (!ip_to_hex(add,args[j].extargs_ipaddr_value)) {
							if (args[j].extargs_type == ST_EXTARGS_TYPE::ipaddr_with_default) {
								args[j].extargs_ipaddr_value = args[j].extargs_ipaddr_default;
								visited[i + 1] = true;
								args[j].args_existed = 1;
							}
							else if (args[j].extargs_type == ST_EXTARGS_TYPE::ipaddr_with_error) {
								cout << "参数[" << cur << "]的附加参数值(" << add << ")非法. (类型:IP地址)" << endl;
								return -1;
							}
						}
						else {
							bool n2 = ip_to_hex(add, args[j].extargs_ipaddr_value);
							visited[i + 1] = true;
							args[j].args_existed = 1;
						}
					}
					// ...
				}
			}
			else if (!visited[i]) {
				if (follow_up_args == 0) {
					cout << "参数[" << argv[i] << "]格式非法(不是--开头的有效内容)." << endl;
					return -1;
				}
				else {
					return i;
				}
			}
		}

	}
	return argc; //此句根据需要修改
}

/***************************************************************************
  函数名称：args_analyse_print
  功    能：
  输入参数：const args_analyse_tools* const args
  返 回 值：
  说    明：友元函数
***************************************************************************/
int args_analyse_print(const args_analyse_tools* const args)
{
	cout << "args_list:" << endl;
	//determine the width for each column
	size_t name_width = string("name").size(), type_width = string("type").size(), default_width = string("default").size(), exists_width = string("exists").size(), value_width = string("value").size(), range_set_width = string("range/set").size();
	int i = 0;
	while (args[i].args_name != "") {
		if (args[i].args_name.size() > name_width)
			name_width = args[i].args_name.size();
		if (converttype(args[i].extargs_type).size() > type_width)
			type_width = converttype(args[i].extargs_type).size();
		//get default width
		string defaultt;
		if (args[i].extargs_type == ST_EXTARGS_TYPE::boolean)
			defaultt = args[i].extargs_bool_default ? "true" : "false";
		else if (args[i].extargs_type == ST_EXTARGS_TYPE::int_with_default || args[i].extargs_type == ST_EXTARGS_TYPE::int_with_set_default)
			defaultt = to_string(args[i].extargs_int_default);
		else if (args[i].extargs_type == ST_EXTARGS_TYPE::double_with_default || args[i].extargs_type == ST_EXTARGS_TYPE::double_with_set_default)
			defaultt = to_string(args[i].extargs_double_default);
		else if (args[i].extargs_type == ST_EXTARGS_TYPE::str && args[i].extargs_string_default == "")
			defaultt = "/";
		else if (args[i].extargs_type == ST_EXTARGS_TYPE::str|| args[i].extargs_type == ST_EXTARGS_TYPE::str_with_set_default)
			defaultt = args[i].extargs_string_default;
		else if (args[i].extargs_type == ST_EXTARGS_TYPE::ipaddr_with_default)
			defaultt = args[i].extargs_string_default;
		else
			defaultt = "/";
		if (defaultt.size() > default_width)
			default_width = defaultt.size();

		//get value width
		string valuee;
		if (args[i].args_existed == 1) {
			if (args[i].extargs_type == ST_EXTARGS_TYPE::boolean)
				valuee = "true";
			else if (args[i].extargs_type == ST_EXTARGS_TYPE::int_with_default || args[i].extargs_type == ST_EXTARGS_TYPE::int_with_error || args[i].extargs_type == ST_EXTARGS_TYPE::int_with_set_default || args[i].extargs_type == ST_EXTARGS_TYPE::int_with_set_error)
				valuee = to_string(args[i].extargs_int_value);
			else if (args[i].extargs_type == ST_EXTARGS_TYPE::double_with_default || args[i].extargs_type == ST_EXTARGS_TYPE::double_with_error || args[i].extargs_type == ST_EXTARGS_TYPE::double_with_set_default || args[i].extargs_type == ST_EXTARGS_TYPE::double_with_set_error)
				valuee = to_string(args[i].extargs_double_value);
			else if (args[i].extargs_type == ST_EXTARGS_TYPE::str || args[i].extargs_type == ST_EXTARGS_TYPE::str_with_set_default || args[i].extargs_type == ST_EXTARGS_TYPE::str_with_set_error)
				valuee = args[i].extargs_string_value;
			else if (args[i].extargs_type == ST_EXTARGS_TYPE::ipaddr_with_default || args[i].extargs_type == ST_EXTARGS_TYPE::ipaddr_with_error)
				valuee = args[i].get_str_ipaddr();
		}
		else 
			valuee = "/";
		if (valuee.size() > value_width)
			value_width = valuee.size();

		//get the range/set width
		stringstream ss;
		if (args[i].extargs_type == ST_EXTARGS_TYPE::int_with_default || args[i].extargs_type == ST_EXTARGS_TYPE::int_with_error)
			ss << "[" << args[i].extargs_int_min << ".." << args[i].extargs_int_max << "]";
		else if (args[i].extargs_type == ST_EXTARGS_TYPE::double_with_default || args[i].extargs_type == ST_EXTARGS_TYPE::double_with_error)
			ss << "[" << fixed << setprecision(6) << args[i].extargs_double_min << ".." << fixed << setprecision(6) << args[i].extargs_double_max << "]";
		else if (args[i].extargs_type == ST_EXTARGS_TYPE::int_with_set_default || args[i].extargs_type == ST_EXTARGS_TYPE::int_with_set_error) {
			int size = (int)getSetSize(args[i].extargs_int_set, INVALID_INT_VALUE_OF_SET);
			for (int m = 0; m < size; m++) {
				ss << args[i].extargs_int_set[m];
				if (m != size - 1)
					ss << "/";
			}
		}
		else if (args[i].extargs_type == ST_EXTARGS_TYPE::double_with_set_default || args[i].extargs_type == ST_EXTARGS_TYPE::double_with_set_error) {
			int size = (int)getSetSize(args[i].extargs_double_set, INVALID_DOUBLE_VALUE_OF_SET);
			for (int m = 0; m < size; m++) {
				ss << fixed << setprecision(6) << args[i].extargs_double_set[m];
				if (m != size - 1)
					ss << "/";
			}
		}
		else if (args[i].extargs_type == ST_EXTARGS_TYPE::str_with_set_default || args[i].extargs_type == ST_EXTARGS_TYPE::str_with_set_error) {
			int size = getstringsetsize(args[i].extargs_string_set);
			for (int m = 0; m < size; m++) {
				ss << args[i].extargs_string_set[m];
				if (m != size - 1)
					ss << "/";
			}
		}
		else
			ss << "/";
		if (ss.str().size() > range_set_width)
			range_set_width = ss.str().size();

		i++;
	}
	//cout << name_width << " " << type_width <<" "<<default_width<<" "<<exists_width<<" "<<value_width<<" "<<range_set_width << endl;
	
	int sum_width = static_cast<int>(name_width + type_width + default_width + exists_width + value_width + range_set_width) + 7;

	cout << string(sum_width, '=') << endl;
	cout << left;
	cout << " "
		<< setw(static_cast<int>(name_width) + 1) << "name"
		<< setw(static_cast<int>(type_width) + 1) << "type"
		<< setw(static_cast<int>(default_width) + 1) << "default"
		<< setw(static_cast<int>(exists_width) + 1) << "exists"
		<< setw(static_cast<int>(value_width) + 1) << "value"
		<< setw(static_cast<int>(range_set_width) + 1) << "range/set"
		<< endl;
	cout << string(sum_width, '=') << endl;
	
	int p = 0;
	while (args[p].args_name != "") {
		cout << left;
		cout << " "
			<< setw(static_cast<int>(name_width) + 1) << args[p].args_name
			<< setw(static_cast<int>(type_width) + 1) << converttype(args[p].extargs_type);
		string defaultt;
		if (args[p].extargs_type == ST_EXTARGS_TYPE::boolean)
			defaultt = args[p].extargs_bool_default ? "true" : "false";
		else if (args[p].extargs_type == ST_EXTARGS_TYPE::int_with_default || args[p].extargs_type == ST_EXTARGS_TYPE::int_with_set_default)
			defaultt = to_string(args[p].extargs_int_default);
		else if (args[p].extargs_type == ST_EXTARGS_TYPE::double_with_default || args[p].extargs_type == ST_EXTARGS_TYPE::double_with_set_default)
			defaultt = to_string(args[p].extargs_double_default);
		else if (args[p].extargs_type == ST_EXTARGS_TYPE::str && args[p].extargs_string_default == "")
			defaultt = "/";
		else if (args[p].extargs_type == ST_EXTARGS_TYPE::str || args[p].extargs_type == ST_EXTARGS_TYPE::str_with_set_default)
			defaultt = args[p].extargs_string_default;
		else if (args[p].extargs_type == ST_EXTARGS_TYPE::ipaddr_with_default)
			defaultt = args[p].extargs_string_default;
		else
			defaultt = "/";
		cout << setw(static_cast<int>(default_width) + 1) << defaultt
			<< setw(static_cast<int>(exists_width) + 1) << args[p].args_existed;
		string valuee;
		if (args[p].args_existed == 1) {
			if (args[p].extargs_type == ST_EXTARGS_TYPE::boolean)
				valuee = "true";
			else if (args[p].extargs_type == ST_EXTARGS_TYPE::int_with_default || args[p].extargs_type == ST_EXTARGS_TYPE::int_with_error || args[p].extargs_type == ST_EXTARGS_TYPE::int_with_set_default || args[p].extargs_type == ST_EXTARGS_TYPE::int_with_set_error)
				valuee = to_string(args[p].extargs_int_value);
			else if (args[p].extargs_type == ST_EXTARGS_TYPE::double_with_default || args[p].extargs_type == ST_EXTARGS_TYPE::double_with_error || args[p].extargs_type == ST_EXTARGS_TYPE::double_with_set_default || args[p].extargs_type == ST_EXTARGS_TYPE::double_with_set_error)
				valuee = to_string(args[p].extargs_double_value);
			else if (args[p].extargs_type == ST_EXTARGS_TYPE::str || args[p].extargs_type == ST_EXTARGS_TYPE::str_with_set_default || args[p].extargs_type == ST_EXTARGS_TYPE::str_with_set_error)
				valuee = args[p].extargs_string_value;
			else if (args[p].extargs_type == ST_EXTARGS_TYPE::ipaddr_with_default || args[p].extargs_type == ST_EXTARGS_TYPE::ipaddr_with_error)
				valuee = args[p].get_str_ipaddr();
		}
		else
			valuee = "/";
		cout << setw(static_cast<int>(value_width) + 1) << valuee;
		stringstream ss;
		if (args[p].extargs_type == ST_EXTARGS_TYPE::int_with_default || args[p].extargs_type == ST_EXTARGS_TYPE::int_with_error)
			ss << "[" << args[p].extargs_int_min << ".." << args[p].extargs_int_max << "]";
		else if (args[p].extargs_type == ST_EXTARGS_TYPE::double_with_default || args[p].extargs_type == ST_EXTARGS_TYPE::double_with_error)
			ss << "[" << fixed << setprecision(6) << args[p].extargs_double_min << ".." << fixed << setprecision(6) << args[p].extargs_double_max << "]";
		else if (args[p].extargs_type == ST_EXTARGS_TYPE::int_with_set_default || args[p].extargs_type == ST_EXTARGS_TYPE::int_with_set_error) {
			int size = (int)getSetSize(args[p].extargs_int_set, INVALID_INT_VALUE_OF_SET);
			for (int m = 0; m < size; m++) {
				ss << args[p].extargs_int_set[m];
				if (m != size - 1)
					ss << "/";
			}
		}
		else if (args[p].extargs_type == ST_EXTARGS_TYPE::double_with_set_default || args[p].extargs_type == ST_EXTARGS_TYPE::double_with_set_error) {
			int size = (int)getSetSize(args[p].extargs_double_set, INVALID_DOUBLE_VALUE_OF_SET);
			for (int m = 0; m < size; m++) {
				ss << fixed << setprecision(6) << args[p].extargs_double_set[m];
				if (m != size - 1)
					ss << "/";
			}
		}
		else if (args[p].extargs_type==ST_EXTARGS_TYPE::str_with_set_default|| args[p].extargs_type == ST_EXTARGS_TYPE::str_with_set_error) {
			int size = getstringsetsize(args[p].extargs_string_set);
			for (int m = 0; m < size; m++) {
				ss << args[p].extargs_string_set[m];
				if (m != size - 1)
					ss << "/";
			}
		}
		else
			ss << "/";
		cout << setw(static_cast<int>(range_set_width) + 1) << ss.str() << endl;
	    p++;
	}
	cout << string(sum_width, '=') << endl;
	cout << endl;
	return 0; //此句根据需要修改
}

#endif // !ENABLE_LIB_COMMON_TOOLS