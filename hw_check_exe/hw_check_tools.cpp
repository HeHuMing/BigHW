/* 2353626 贺胡鸣 大数据 */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "hw_check_tools.h"
using namespace std;

void modify_dirname(string& dirname)
{
	//最后没有\的加个\

	if (dirname.empty())
		return;
	if (dirname.back() != '\\')
		dirname += "\\";
}

void modify_include(string& include)
{
	if (include.empty())
		return;
	string tmp;
	for (size_t i = 0; i < include.size(); i++) {
		if (i == 0 && include[i] == '[')
			continue;
		else if (i == include.size() - 1 && include[i] == ']')
			continue;
		else
			tmp += include[i];
	}
	include = tmp;
}

string get_current_time() {
	// 获取当前时间
	std::time_t now = std::time(nullptr);
	std::tm* local_time = std::localtime(&now);

	// 格式化时间
	char buffer[80];
	std::strftime(buffer, sizeof(buffer), "%Y-%m-%d-%H-%M-%S", local_time);

	return std::string(buffer);
}


void check_tools::print_cfg(const string cfgfile, const string checkname)
{
	config_file_tools sCfg(cfgfile, BREAK_CTYPE::Equal);
	if (sCfg.is_read_succeeded() == 0) {
		cout << "配置文件[" << checkname << "]读取不成功" << endl;
		return;
	}
	vector<string> items;
	/*
	vector<string> items, groups;
	int n = sCfg.get_all_group(groups);
	for (size_t i = 0; i < groups.size(); i++)
		cout << groups[i] << endl;
	vector<group> vec((int)groups.size());
	for (size_t i = 0; i < groups.size(); i++) {
		vec[i].groupname = groups[i];
		sCfg.item_get_raw(vec[i].groupname, "include", include);

	}
	*/
	//print 

	string groupname = "[" + checkname + "]";
	int num = sCfg.get_all_item(groupname, items);

	//for (size_t i = 0; i < items.size(); i++)
	//	cout << items[i] << endl;


	int ret = sCfg.item_get_raw(groupname, "include", include);
	int ret1 = 1;

	//递归
	if (!include.empty()) {
		modify_include(include);
		print_cfg(cfgfile, include);
	}

	int flag1 = 0; //检查需要的文件是否连续，不连续置为1
	vector<int> pipe_pos, red_pos; //记录哪个文件缺失

	//没有include的情况
	cout << endl;
	cout << "====================================================================================================" << endl;
	cout << groupname << "配置信息如下：" << endl;
	cout << "====================================================================================================" << endl;
	cout << groupname << "：" << endl;

	//求宽度
	int width = 0;
	string cmd;
	ret1 = sCfg.item_get_raw(groupname, "cmd_style", cmd);
	if (cmd == "pipe")
		width = 29;
	else if (cmd == "redirection")
		width = 25;
	else
		width = 23;

	//求exe
	string exe;
	sCfg.item_get_raw(groupname, "exe_style", exe);

	//没打印的配置参数
	if (sCfg.item_get_null("[数据库]", "db_host"))
		ret1 = sCfg.item_get_raw("[数据库]", "db_host", db_host);
	if (sCfg.item_get_null("[数据库]", "db_port"))
		ret1 = sCfg.item_get_raw("[数据库]", "db_port", db_port);
	if (sCfg.item_get_null("[数据库]", "db_name"))
		ret1 = sCfg.item_get_raw("[数据库]", "db_name", db_name);
	if (sCfg.item_get_null("[数据库]", "db_username"))
		ret1 = sCfg.item_get_raw("[数据库]", "db_username", db_username);
	if (sCfg.item_get_null("[数据库]", "db_passwd"))
		ret1 = sCfg.item_get_raw("[数据库]", "db_passwd", db_passwd);
	if (sCfg.item_get_null("[数据库]", "db_curr_term"))
		ret1 = sCfg.item_get_raw("[数据库]", "db_curr_term", db_curr_term);
	if (sCfg.item_get_null("[数据库]", "db_cno_list"))
		ret1 = sCfg.item_get_raw("[数据库]", "db_cno_list", db_cno_list);

	//第一块
	if (sCfg.item_get_null(groupname, "exe_style"))
		ret1 = sCfg.item_get_raw(groupname, "exe_style", exe_style);
	cout << "  " << left << setw(width) << "exe_style" << "= " << exe_style << endl;
	if (sCfg.item_get_null(groupname, "name_list")) 
		ret1 = sCfg.item_get_raw(groupname, "name_list", name_list);
	if (exe == "multi"||exe=="single")
		cout << "  " << left << setw(width) << "name_list" << "= " << name_list << endl;
	if (sCfg.item_get_null(groupname, "single_exe_dirname")) {
		ret1 = sCfg.item_get_raw(groupname, "single_exe_dirname", single_exe_dirname);
		modify_dirname(single_exe_dirname);
	}
	if (exe == "single")
		cout << "  " << left << setw(width) << "single_exe_dirname" << "= " << single_exe_dirname << endl;
	if (sCfg.item_get_null(groupname, "multi_exe_main_dirname")) {
		ret1 = sCfg.item_get_raw(groupname, "multi_exe_main_dirname", multi_exe_main_dirname);
		modify_dirname(multi_exe_main_dirname);
	}
	if (exe == "multi")
		cout << "  " << left << setw(width) << "multi_exe_main_dirname" << "= " << multi_exe_main_dirname << endl;
	if (sCfg.item_get_null(groupname, "multi_exe_sub_dirname")) {
		ret1 = sCfg.item_get_raw(groupname, "multi_exe_sub_dirname", multi_exe_sub_dirname);
		modify_dirname(multi_exe_sub_dirname);
	}
	if (exe == "multi")
		cout << "  " << left << setw(width) << "multi_exe_sub_dirname" << "= " << multi_exe_sub_dirname << endl;
	if (sCfg.item_get_null(groupname, "stu_exe_name")) 
		ret1 = sCfg.item_get_raw(groupname, "stu_exe_name", stu_exe_name);
	if (exe == "multi" || exe == "single")
		cout << "  " << left << setw(width) << "stu_exe_name" << "= " << stu_exe_name << endl;
	if (sCfg.item_get_null(groupname, "demo_exe_name"))
		ret1 = sCfg.item_get_raw(groupname, "demo_exe_name", demo_exe_name);
	cout << "  " << left << setw(width) << "demo_exe_name" << "= " << demo_exe_name << endl;
	if (sCfg.item_get_null(groupname, "cmd_style"))
		ret1 = sCfg.item_get_raw(groupname, "cmd_style", cmd_style);
	cout << "  " << left << setw(width) << "cmd_style" << "= " << cmd_style << endl;
	if (sCfg.item_get_null(groupname, "max_output_len"))
		ret1 = sCfg.item_get_raw(groupname, "max_output_len", max_output_len);
	cout << "  " << left << setw(width) << "max_output_len" << "= " << max_output_len << endl;
	if (sCfg.item_get_null(groupname, "timeout"))
		ret1 = sCfg.item_get_raw(groupname, "timeout", timeout);
	cout << "  " << left << setw(width) << "timeout" << "= " << timeout << endl;
	if (sCfg.item_get_null(groupname, "pipe_get_input_data_exe_name"))
		ret1 = sCfg.item_get_raw(groupname, "pipe_get_input_data_exe_name", pipe_get_input_data_exe_name);
	if (sCfg.item_get_null(groupname, "pipe_data_file"))
		ret1 = sCfg.item_get_raw(groupname, "pipe_data_file", pipe_data_file);
	if (sCfg.item_get_null(groupname, "redirection_data_dirname"))
		ret1 = sCfg.item_get_raw(groupname, "redirection_data_dirname", redirection_data_dirname);
	if (cmd_style == "pipe") {
		cout << "  " << left << setw(width) << "pipe_get_input_data_exe_name" << "= " << pipe_get_input_data_exe_name << endl;
		cout << "  " << left << setw(width) << "pipe_data_file" << "= " << pipe_data_file << endl;
	}
	else if (cmd_style == "redirection") {
		modify_dirname(redirection_data_dirname);
		cout << "  " << left << setw(width) << "redirection_data_dirname" << "= " << redirection_data_dirname << endl;
	}

	//第二块
	cout << endl;
	if (sCfg.item_get_null(groupname, "tc_trim"))
		ret1 = sCfg.item_get_raw(groupname, "tc_trim", tc_trim);
	cout << "  " << left << setw(width) << "tc_trim" << "= " << tc_trim << endl;
	if (sCfg.item_get_null(groupname, "tc_lineskip"))
		ret1 = sCfg.item_get_raw(groupname, "tc_lineskip", tc_lineskip);
	cout << "  " << left << setw(width) << "tc_lineskip" << "= " << tc_lineskip << endl;
	if (sCfg.item_get_null(groupname, "tc_lineoffset"))
		ret1 = sCfg.item_get_raw(groupname, "tc_lineoffset", tc_lineoffset);
	cout << "  " << left << setw(width) << "tc_lineoffset" << "= " << tc_lineoffset << endl;
	if (sCfg.item_get_null(groupname, "tc_ignoreblank"))
		ret1 = sCfg.item_get_raw(groupname, "tc_ignoreblank", tc_ignoreblank);
	cout << "  " << left << setw(width) << "tc_ignoreblank" << "= " << tc_ignoreblank << endl;
	if (sCfg.item_get_null(groupname, "tc_not_ignore_linefeed"))
		ret1 = sCfg.item_get_raw(groupname, "tc_not_ignore_linefeed", tc_not_ignore_linefeed);
	cout << "  " << left << setw(width) << "tc_not_ignore_linefeed" << "= " << tc_not_ignore_linefeed << endl;
	if (sCfg.item_get_null(groupname, "tc_maxdiff"))
		ret1 = sCfg.item_get_raw(groupname, "tc_maxdiff", tc_maxdiff);
	cout << "  " << left << setw(width) << "tc_maxdiff" << "= " << tc_maxdiff << endl;
	if (sCfg.item_get_null(groupname, "tc_maxline"))
		ret1 = sCfg.item_get_raw(groupname, "tc_maxline", tc_maxline);
	cout << "  " << left << setw(width) << "tc_maxline" << "= " << tc_maxline << endl;
	if (sCfg.item_get_null(groupname, "tc_display"))
		ret1 = sCfg.item_get_raw(groupname, "tc_display", tc_display);
	cout << "  " << left << setw(width) << "tc_display" << "= " << tc_display << endl;

	//第三块（如果有）
	cout << endl;
	if (sCfg.item_get_null(groupname, "items_num"))
		ret1 = sCfg.item_get_int(groupname, "items_num", items_num);
	cout << "  " << left << setw(width) << "items_num" << "= " << items_num << endl;
	if (sCfg.item_get_null(groupname, "items_begin"))
		ret1 = sCfg.item_get_int(groupname, "items_begin", items_begin);
	cout << "  " << left << setw(width) << "items_begin" << "= " << items_begin << endl;
	if (sCfg.item_get_null(groupname, "items_end"))
		ret1 = sCfg.item_get_int(groupname, "items_end", items_end);
	cout << "  " << left << setw(width) << "items_end" << "= " << items_end << endl;

	//先全求一遍
	item_gname.resize(32);
	item_fname.resize(32);
	item_args.resize(32);
	for (int i = items_begin; i <= items_end; i++) {
		string search = "item_gname_" + to_string(i);
		string item;
		if (sCfg.item_get_null(groupname, search)) {
			ret1 = sCfg.item_get_raw(groupname, search, item);
			item_gname[i - 1] = item;;
		}
	}
	for (int i = items_begin; i <= items_end; i++) {
		string search = "item_fname_" + to_string(i);
		string item;
		if (sCfg.item_get_null(groupname, search)) {
			ret1 = sCfg.item_get_raw(groupname, search, item);
			item_fname[i - 1] = item;
		}
	}
	for (int i = items_begin; i <= items_end; i++) {
		string search = "item_args_" + to_string(i);
		string item;
		if (sCfg.item_get_null(groupname, search)) {
			ret1 = sCfg.item_get_raw(groupname, search, item);
			item_args[i - 1] = item;
		}
	}
	//再按cmd_style输出
	if (cmd_style == "pipe") {
		for (int i = items_begin; i <= items_end; i++) {
			/*
			string search = "item_gname_" + to_string(i);
			string item;
			ret1 = sCfg.item_get_raw(groupname, search, item);
			cout << "  " << left << setw(width) << "item_name_" + to_string(i) << "= " << ("[" + item + "]") << endl;
			item_gname.push_back(item);
			*/
			string search = "item_gname_" + to_string(i);
			if (!item_gname[i - 1].empty())
				cout << "  " << left << setw(width) << "item_name_" + to_string(i) << "= " << ("[" + item_gname[i - 1] + "]") << endl;
			else {
				cout << "  " << left << setw(width) << "item_name_" + to_string(i) << "= " << "<EMPTY>" << endl;
				flag1 = 1;
				pipe_pos.push_back(i);
			}
		}
	}
	else if (cmd_style == "redirection") {
		for (int i = items_begin; i <= items_end; i++) {
			/*
			string search = "item_fname_" + to_string(i);
			string item;
			ret1 = sCfg.item_get_raw(groupname, search, item);
			cout << "  " << left << setw(width) << "item_name_" + to_string(i) << "= " << item << endl;
			item_fname.push_back(item);
			*/
			string search = "item_fname_" + to_string(i);
			if (!item_fname[i - 1].empty())
				cout << "  " << left << setw(width) << "item_name_" + to_string(i) << "= " << item_fname[i - 1] << endl;
			else {
				cout << "  " << left << setw(width) << "item_name_" + to_string(i) << "= " << "<EMPTY>" << endl;
				flag1 = 1;
				red_pos.push_back(i);
			}
		}
	}
	else if (cmd_style == "main_with_arguments") {
		for (int i = items_begin; i <= items_end; i++) {
			/*
			string search = "item_args_" + to_string(i);
			string item;
			ret1 = sCfg.item_get_raw(groupname, search, item);
			cout << "  " << left << setw(width) << "item_name_" + to_string(i) << "= " << item << endl;
			item_args.push_back(item);
			*/
			cout << "  " << left << setw(width) << "item_name_" + to_string(i) << "= " << item_args[i - 1] << endl;
		}
	}

	cout << "====================================================================================================" << endl << endl;


	if (flag1) {
		cerr << endl;
		if (warning) {
			cerr << "[--严重错误--] 配置文件存在下列的错误：" << endl;
			warning = 0;
		}
		if (cmd == "pipe") {
			for (size_t i = 0; i < pipe_pos.size(); i++)
				cerr << "pipe方式的配置项[item_gname_" << pipe_pos[i] << "]不存在" << endl;
		}
		else if (cmd == "redirection") {
			for (size_t i = 0; i < red_pos.size(); i++)
				cerr << "redirection方式的配置项[item_gname_" << red_pos[i] << "]不存在" << endl;
		}
		cerr << endl << endl;
	}
}



void check_tools::read_cfg(const string cfgfile, const string checkname)
{
	config_file_tools sCfg(cfgfile, BREAK_CTYPE::Equal);
	if (sCfg.is_read_succeeded() == 0) {
		cout << "配置文件[" << checkname << "]读取不成功" << endl;
		return;
	}
	vector<string> items;

	string groupname = "[" + checkname + "]";
	int num = sCfg.get_all_item(groupname, items);


	int ret = sCfg.item_get_raw(groupname, "include", include);
	int ret1 = 1;

	//递归
	if (!include.empty()) {
		modify_include(include);
		read_cfg(cfgfile, include);
	}

	int flag1 = 0; //检查需要的文件是否连续，不连续置为1
	vector<int> pipe_pos, red_pos; //记录哪个文件缺失


	//求宽度
	int width = 0;
	string cmd;
	ret1 = sCfg.item_get_raw(groupname, "cmd_style", cmd);
	if (cmd == "pipe")
		width = 29;
	else if (cmd == "redirection")
		width = 25;
	else
		width = 23;

	//求exe
	string exe;
	sCfg.item_get_raw(groupname, "exe_style", exe);

	//没打印的配置参数,databse
	if (sCfg.item_get_null("[数据库]", "db_host"))
		ret1 = sCfg.item_get_raw("[数据库]", "db_host", db_host);
	if (sCfg.item_get_null("[数据库]", "db_port"))
		ret1 = sCfg.item_get_raw("[数据库]", "db_port", db_port);
	if (sCfg.item_get_null("[数据库]", "db_name"))
		ret1 = sCfg.item_get_raw("[数据库]", "db_name", db_name);
	if (sCfg.item_get_null("[数据库]", "db_username"))
		ret1 = sCfg.item_get_raw("[数据库]", "db_username", db_username);
	if (sCfg.item_get_null("[数据库]", "db_passwd"))
		ret1 = sCfg.item_get_raw("[数据库]", "db_passwd", db_passwd);
	if (sCfg.item_get_null("[数据库]", "db_curr_term"))
		ret1 = sCfg.item_get_raw("[数据库]", "db_curr_term", db_curr_term);
	if (sCfg.item_get_null("[数据库]", "db_cno_list"))
		ret1 = sCfg.item_get_raw("[数据库]", "db_cno_list", db_cno_list);


	//第一块
	if (sCfg.item_get_null(groupname, "exe_style"))
		ret1 = sCfg.item_get_raw(groupname, "exe_style", exe_style);
	if (sCfg.item_get_null(groupname, "name_list"))
		ret1 = sCfg.item_get_raw(groupname, "name_list", name_list);
	if (sCfg.item_get_null(groupname, "single_exe_dirname")) {
		ret1 = sCfg.item_get_raw(groupname, "single_exe_dirname", single_exe_dirname);
		modify_dirname(single_exe_dirname);
	}
	if (sCfg.item_get_null(groupname, "multi_exe_main_dirname")) {
		ret1 = sCfg.item_get_raw(groupname, "multi_exe_main_dirname", multi_exe_main_dirname);
		modify_dirname(multi_exe_main_dirname);
	}
	if (sCfg.item_get_null(groupname, "multi_exe_sub_dirname")) {
		ret1 = sCfg.item_get_raw(groupname, "multi_exe_sub_dirname", multi_exe_sub_dirname);
		modify_dirname(multi_exe_sub_dirname);
	}
	if (sCfg.item_get_null(groupname, "stu_exe_name"))
		ret1 = sCfg.item_get_raw(groupname, "stu_exe_name", stu_exe_name);
	if (sCfg.item_get_null(groupname, "demo_exe_name"))
		ret1 = sCfg.item_get_raw(groupname, "demo_exe_name", demo_exe_name);
	if (sCfg.item_get_null(groupname, "cmd_style"))
		ret1 = sCfg.item_get_raw(groupname, "cmd_style", cmd_style);
	if (sCfg.item_get_null(groupname, "max_output_len"))
		ret1 = sCfg.item_get_raw(groupname, "max_output_len", max_output_len);
	if (sCfg.item_get_null(groupname, "timeout"))
		ret1 = sCfg.item_get_raw(groupname, "timeout", timeout);
	if (sCfg.item_get_null(groupname, "pipe_get_input_data_exe_name"))
		ret1 = sCfg.item_get_raw(groupname, "pipe_get_input_data_exe_name", pipe_get_input_data_exe_name);
	if (sCfg.item_get_null(groupname, "pipe_data_file"))
		ret1 = sCfg.item_get_raw(groupname, "pipe_data_file", pipe_data_file);
	if (sCfg.item_get_null(groupname, "redirection_data_dirname"))
		ret1 = sCfg.item_get_raw(groupname, "redirection_data_dirname", redirection_data_dirname);
	else if (cmd_style == "redirection") 
		modify_dirname(redirection_data_dirname);

	//第二块
	if (sCfg.item_get_null(groupname, "tc_trim"))
		ret1 = sCfg.item_get_raw(groupname, "tc_trim", tc_trim);
	if (sCfg.item_get_null(groupname, "tc_lineskip"))
		ret1 = sCfg.item_get_raw(groupname, "tc_lineskip", tc_lineskip);
	if (sCfg.item_get_null(groupname, "tc_lineoffset"))
		ret1 = sCfg.item_get_raw(groupname, "tc_lineoffset", tc_lineoffset);
	if (sCfg.item_get_null(groupname, "tc_ignoreblank"))
		ret1 = sCfg.item_get_raw(groupname, "tc_ignoreblank", tc_ignoreblank);
	if (sCfg.item_get_null(groupname, "tc_not_ignore_linefeed"))
		ret1 = sCfg.item_get_raw(groupname, "tc_not_ignore_linefeed", tc_not_ignore_linefeed);
	if (sCfg.item_get_null(groupname, "tc_maxdiff"))
		ret1 = sCfg.item_get_raw(groupname, "tc_maxdiff", tc_maxdiff);
	if (sCfg.item_get_null(groupname, "tc_maxline"))
		ret1 = sCfg.item_get_raw(groupname, "tc_maxline", tc_maxline);
	if (sCfg.item_get_null(groupname, "tc_display"))
		ret1 = sCfg.item_get_raw(groupname, "tc_display", tc_display);

	//第三块（如果有）
	if (sCfg.item_get_null(groupname, "items_num"))
		ret1 = sCfg.item_get_int(groupname, "items_num", items_num);
	if (sCfg.item_get_null(groupname, "items_begin"))
		ret1 = sCfg.item_get_int(groupname, "items_begin", items_begin);
	if (sCfg.item_get_null(groupname, "items_end"))
		ret1 = sCfg.item_get_int(groupname, "items_end", items_end);

	//先全求一遍
	item_gname.resize(32);
	item_fname.resize(32);
	item_args.resize(32);
	for (int i = items_begin; i <= items_end; i++) {
		string search = "item_gname_" + to_string(i);
		string item;
		if (sCfg.item_get_null(groupname, search)) {
			ret1 = sCfg.item_get_raw(groupname, search, item);
			item_gname[i - 1] = item;;
		}
	}
	for (int i = items_begin; i <= items_end; i++) {
		string search = "item_fname_" + to_string(i);
		string item;
		if (sCfg.item_get_null(groupname, search)) {
			ret1 = sCfg.item_get_raw(groupname, search, item);
			item_fname[i - 1] = item;
		}
	}
	for (int i = items_begin; i <= items_end; i++) {
		string search = "item_args_" + to_string(i);
		string item;
		if (sCfg.item_get_null(groupname, search)) {
			ret1 = sCfg.item_get_raw(groupname, search, item);
			item_args[i - 1] = item;
		}
	}

	if (flag1) {
		cerr << endl;
		if (warning) {
			cerr << "[--严重错误--] 配置文件存在下列的错误：" << endl;
			warning = 0;
		}
		if (cmd == "pipe") {
			for (size_t i = 0; i < pipe_pos.size(); i++)
				cerr << "pipe方式的配置项[item_gname_" << pipe_pos[i] << "]不存在" << endl;
		}
		else if (cmd == "redirection") {
			for (size_t i = 0; i < red_pos.size(); i++)
				cerr << "redirection方式的配置项[item_gname_" << red_pos[i] << "]不存在" << endl;
		}
		cerr << endl << endl;
	}
}

void check_tools::open_files()
{
	int flag = 0;
	//demo_exe_name
	demo.open(demo_exe_name, ios::in | ios::binary);
	if (demo.is_open() == 0) {
		if (warning) {
			cerr << endl << "[--严重错误--] 配置文件存在下列的错误：" << endl;
			warning = 0;
		}
		cerr << "demo_exe_name 指定的文件[" << demo_exe_name << "]不存在." << endl;
		flag = 1;
	}
	else
		demo.close();
	
	//multi
	if (exe_style == "multi") {
		//multi_exe_main_dirname
		struct stat fileinfo;  // 定义stat结构体，用来接收文件属性
		//cout << "*" << multi_exe_main_dirname.c_str() << "*" << endl;
		if (stat(multi_exe_main_dirname.c_str(), &fileinfo) != 0) {
			if (warning) {
				cerr << endl << "[--严重错误--] 配置文件存在下列的错误：" << endl;
				warning = 0;
			}
			modify_dirname(multi_exe_main_dirname);
			cerr << "multi_exe_main_dirname 指定的目录[" << multi_exe_main_dirname << "]不存在." << endl;
			flag = 1;
		}
		//multi_exe_sub_dirname
		if (multi_exe_sub_dirname!="vs-exec"&& multi_exe_sub_dirname != "vs-exec\\"&&multi_exe_sub_dirname!="dev-exec"&& multi_exe_sub_dirname != "dev-exec\\") {
			if (warning) {
				cerr << endl << "[--严重错误--] 配置文件存在下列的错误：" << endl;
				warning = 0;
			}
			modify_dirname(multi_exe_sub_dirname);
			cerr << "multi_exe_sub_dirname 指定的目录[" << multi_exe_sub_dirname << "]不存在." << endl;
			flag = 1;
		}
	}
	
	//pipe
	if (cmd_style == "pipe") {
		//pipe_get_input_data
		pipe_get_input_data.open(pipe_get_input_data_exe_name, ios::in | ios::binary);
		if (pipe_get_input_data.is_open() == 0) {
			if (warning) {
				cerr << endl << "[--严重错误--] 配置文件存在下列的错误：" << endl;
				warning = 0;
			}
			cerr << "pipe_get_input_data_exe_name 指定的文件[" << pipe_get_input_data_exe_name << "]不存在." << endl;
			flag = 1;
		}
		else
			pipe_get_input_data.close();
		//pipe_data
		pipe_data.open(pipe_data_file, ios::in | ios::binary);
		if (pipe_data.is_open() == 0) {
			if (warning) {
				cerr << endl << "[--严重错误--] 配置文件存在下列的错误：" << endl;
				warning = 0;
			}
			cerr << "pipe_data_file 指定的文件[" << pipe_data_file << "]不存在." << endl;
			flag = 1;
		}
		else {
			config_file_tools cfg(pipe_data_file, BREAK_CTYPE::Equal);
			vector<string> group;
			if (cfg.is_read_succeeded() == 0) {
				return;
			}
			cfg.get_all_group(group);
			for (size_t i = 0; i < (size_t)items_num; i++) {
				string groupname = "[" + item_gname[i] + "]";
				int flag = 1;
				for (size_t k = 0; k < group.size(); k++) {
					if (groupname == group[k])
						flag = 0;
				}
				if (flag) {
					if (warning) {
						cerr << endl << "[--严重错误--] 配置文件存在下列的错误：" << endl;
						warning = 0;
					}
					cerr << "pipe_data_file 指定的文件[" << pipe_data_file << "]中没有组[[" << item_gname[i] << "]]" << endl;
				}
			}
			
			pipe_data.close();
		}

	}
	//redirection
	if (cmd_style == "redirection") {
		struct stat fileinfo;  // 定义stat结构体，用来接收文件属性
		//cout << "*" << multi_exe_main_dirname.c_str() << "*" << endl;
		if (stat(redirection_data_dirname.c_str(), &fileinfo) != 0) {
			if (warning) {
				cerr << endl << "[--严重错误--] 配置文件存在下列的错误：" << endl;
				warning = 0;
			}
			modify_dirname(redirection_data_dirname);
			cerr << "redirection_data_dirname 指定的目录[" << redirection_data_dirname << "]不存在." << endl;
			flag = 1;
		}
		for (size_t i = 0; i < (size_t)items_num; i++) {
			modify_dirname(redirection_data_dirname);
			string test_txt = redirection_data_dirname + item_fname[i];
			ifstream in(test_txt);
			if (in.is_open() == 0) {
				if (warning) {
					cerr << endl << "[--严重错误--] 配置文件存在下列的错误：" << endl;
					warning = 0;
				}
				cerr << "redirection方式：数据文件[" << test_txt << "]不存在." << endl;
			}
			else
				in.close();
		}
	}
}

void check_tools::single_check()
{
	//out方式打开要写的xls文件
	string current_time = get_current_time();
	string xlsx = "check-result-2353626-" + current_time + "-" + exe_style + "-" + cmd_style + "-" + (name_list == "database" ? "database" : "txt") + "-" + stu_exe_name + ".xls";
	ofstream out;
	out.open(xlsx, ios::out);
	//先写头上几行内容
	out << "exe_style\t" << exe_style << endl;
	out << "cmd_style\t" << cmd_style << endl;
	out << "name_list\t" << name_list << endl;
	out << "stu_exe_name\t" << stu_exe_name << endl;
	out << "start_time\t" << current_time << endl << endl;

	if (cmd_style == "pipe") {
		out << "序号\t课号\t学号\t姓名\t正确运行\t定时器创建失败\t管道方式打开失败\t启动定时器失败\t超时\t超过输出上限\t死循环\tTC通过总数\t";
		for (size_t i = 0; i < item_gname.size(); i++) {
			out << "[" + item_gname[i] << "]";
			if (i != item_gname.size() - 1)
				out << "\t";
		}
		out << endl;
	}
	else if (cmd_style == "redirection") {
		out << "序号\t课号\t学号\t姓名\t正确运行\t定时器创建失败\t管道方式打开失败\t启动定时器失败\t超时\t超过输出上限\t死循环\tTC通过总数\t";
		for (size_t i = 0; i < item_fname.size(); i++) {
			out << item_fname[i];
			if (i != item_fname.size() - 1)
				out << "\t";
		}
		out << endl;
	}
	else if (cmd_style == "main_with_arguments") {
		out << "序号\t课号\t学号\t姓名\t正确运行\t定时器创建失败\t管道方式打开失败\t启动定时器失败\t超时\t超过输出上限\t死循环\tTC通过总数\t";
		for (size_t i = 0; i < item_args.size(); i++) {
			out << item_args[i];
			if (i != item_args.size() - 1)
				out << "\t";
		}
		out << endl;
	}

	modify_dirname(single_exe_dirname);
	modify_dirname(multi_exe_main_dirname);
	modify_dirname(multi_exe_sub_dirname);
	string demo_exe = demo_exe_name;
	for (size_t i = 0; i < student.size(); i++) { //对需要比对作业的每一个学生
		string stu_exe;
		if (exe_style == "single")
			stu_exe = single_exe_dirname + student[i].sno + "-" + student[i].cno + "-" + stu_exe_name;
		else if (exe_style == "multi")
			stu_exe = multi_exe_main_dirname + student[i].sno + "-" + student[i].cno + "\\" + multi_exe_sub_dirname + stu_exe_name;
		ifstream in(stu_exe);
		if (in.is_open() == 0) {  //该学生的信息找不到exe
			out << student[i].idx << "\t" << "=text(\"" << student[i].cno << "\", \"#\")\t" << student[i].sno << "\t" << student[i].name << "\t";
			out << "/\t/\t/\t/\t/\t/\t/\t0\t";
			for (size_t m = 0; m < (size_t)items_num; m++) {
				out << "0";
				if (m != items_num - 1)out << "\t";
			}
			out << endl;
			in.close();
			continue;
		}
		if (cmd_style == "normal") {
			st_CheckExec stu(stu_exe, "学生exe情况", stoi(max_output_len), stoi(timeout));
			int result = stu.running();
			string starttime = stu.getStartTimeStr();
			cout << stu.msg.str() << endl;
			//st_CheckExec demo(demo_exe, "demo exe情况", stoi(max_output_len), stoi(timeout));

		}
		else if (cmd_style == "pipe") {
			vector<int> result; //记录每个文件比对的结果
			for (size_t k = 0; k < size_t(8 + items_num); k++)
				result.push_back(0); //初始状态全是零
			CheckExec_Errno status;
			for (size_t j = 0; j < item_gname.size(); j++) {
				st_CheckExec stu(pipe_get_input_data_exe_name + " " + pipe_data_file + " [" + item_gname[j] + "] | " + stu_exe, "学生exe文件", stoi(max_output_len), stoi(timeout));
				stu.running();
				status = stu.get_errno();
				result[(int)status]++;
				if (status == CheckExec_Errno::ok) {  //前序没问题才进行tc
					st_CheckExec demo(pipe_get_input_data_exe_name + " " + pipe_data_file + " [" + item_gname[j] + "] | " + demo_exe, "参考exe文件", stoi(max_output_len), stoi(timeout));
					demo.running();
					istringstream iss1(stu.msg.str()), iss2(demo.msg.str());
					txt_compare tc(iss1, iss2, tc_trim, tc_display, stoi(tc_lineskip), stoi(tc_lineoffset), stoi(tc_maxdiff), stoi(tc_maxline), stoi(tc_ignoreblank), stoi(tc_not_ignore_linefeed), false);
					if (tc.compare(true) == 0) { //比对正确
						result[7]++;
						result[8 + j] = 1;
					}
				}
			}
			//该学生的全部文件都被比对完成
			out << student[i].idx << "\t" << "=text(\"" << student[i].cno << "\", \"#\")\t" << student[i].sno << "\t" << student[i].name << "\t";
			for (size_t k = 0; k < result.size(); k++) {
				out << result[k];
				if (k != result.size() - 1)
					out << "\t";
			}
			out << endl;
		}
		else if (cmd_style == "redirection") {
			vector<int> result; //记录每个文件比对的结果
			for (size_t k = 0; k < size_t(8 + items_num); k++)
				result.push_back(0); //初始状态全是零
			CheckExec_Errno status;
			modify_dirname(redirection_data_dirname);
			for (size_t j = 0; j < item_fname.size(); j++) {
				st_CheckExec stu(stu_exe + " < " + redirection_data_dirname + item_fname[j], "学生exe文件", stoi(max_output_len), stoi(timeout));
				stu.running();
				status = stu.get_errno();
				result[(int)status]++;
				if (status == CheckExec_Errno::ok) {  //前序没问题才进行tc
					st_CheckExec demo(demo_exe + " < " + redirection_data_dirname + item_fname[j], "参考exe文件", stoi(max_output_len), stoi(timeout));
					demo.running();
					istringstream iss1(stu.msg.str()), iss2(demo.msg.str());
					txt_compare tc(iss1, iss2, tc_trim, tc_display, stoi(tc_lineskip), stoi(tc_lineoffset), stoi(tc_maxdiff), stoi(tc_maxline), stoi(tc_ignoreblank), stoi(tc_not_ignore_linefeed), false);
					if (tc.compare(true) == 0) { //比对正确
						result[7]++;
						result[8 + j] = 1;
					}
				}
			}
			//该学生的全部文件都被比对完成
			out << student[i].idx << "\t" << "=text(\"" << student[i].cno << "\", \"#\")\t" << student[i].sno << "\t" << student[i].name << "\t";
			for (size_t k = 0; k < result.size(); k++) {
				out << result[k];
				if (k != result.size() - 1)
					out << "\t";
			}
			out << endl;
		}
		else if (cmd_style == "main_with_arguments") {
			vector<int> result; //记录每个文件比对的结果
			for (size_t k = 0; k < size_t(8 + items_num); k++)
				result.push_back(0); //初始状态全是零
			CheckExec_Errno status;
			for (size_t j = 0; j < item_fname.size(); j++) {
				st_CheckExec stu(stu_exe +" "+item_args[j], "学生exe文件", stoi(max_output_len), stoi(timeout));
				stu.running();
				status = stu.get_errno();
				result[(int)status]++;
				if (status == CheckExec_Errno::ok) {  //前序没问题才进行tc
					st_CheckExec demo(demo_exe + " " + item_args[j], "参考exe文件", stoi(max_output_len), stoi(timeout));
					demo.running();
					istringstream iss1(stu.msg.str()), iss2(demo.msg.str());
					txt_compare tc(iss1, iss2, tc_trim, tc_display, stoi(tc_lineskip), stoi(tc_lineoffset), stoi(tc_maxdiff), stoi(tc_maxline), stoi(tc_ignoreblank), stoi(tc_not_ignore_linefeed), false);
					if (tc.compare(true) == 0) { //比对正确
						result[7]++;
						result[8 + j] = 1;
					}
				}
			}
			//该学生的全部文件都被比对完成
			out << student[i].idx << "\t" << "=text(\"" << student[i].cno << "\", \"#\")\t" << student[i].sno << "\t" << student[i].name << "\t";
			for (size_t k = 0; k < result.size(); k++) {
				out << result[k];
				if (k != result.size() - 1)
					out << "\t";
			}
			out << endl;
		}
	}
	out << endl;
	out.close();
}

void check_tools::exe_run()
{
	string exe1, exe2; //用于比较的两个exe文件的全路径
	bool error = false;
	if (exe_style == "none") {
		exe1 = demo_exe_name;
		exe2 = exe1;
		//cout << exe1 << endl << exe2 << endl;
		if (cmd_style == "normal") {
			st_CheckExec example1(exe1, "参考exe文件", stoi(max_output_len), stoi(timeout));
			example1.running();
			string starttime = example1.getStartTimeStr();
			//st_CheckExec example2(exe2, "参考exe文件", stoi(max_output_len), stoi(timeout));
			//example2.running();
			string endtime = example1.getEndTimeStr();
			cout << starttime << "  参考答案生成情况" << endl;
			cout << "======================================================================" << endl;
			cout << "参考exe文件：" << exe2 << endl;
			cout << "测试运行情况：" << example1.get_errno() << "=" << items_num << endl;
			cout << "时间：" << starttime << " - " << endtime << endl;
			cout << "======================================================================" << endl;
		}
		else if (cmd_style == "pipe") {
			string starttime, endtime;
			CheckExec_Errno status;
			for (size_t i = 0; i < item_gname.size(); i++) {
				st_CheckExec example1(pipe_get_input_data_exe_name + " " + pipe_data_file + " [" + item_gname[i] + "] | " + exe1, "参考exe文件", stoi(max_output_len), stoi(timeout));
				example1.running();
				status = example1.get_errno();
				if (status != CheckExec_Errno::ok) error = true;
				if (i == 0) starttime = example1.getStartTimeStr();
				if (i == item_gname.size() - 1) endtime = example1.getEndTimeStr();
			}
			cout << starttime << "  参考答案生成情况" << endl;
			cout << "======================================================================" << endl;
			cout << "参考exe文件：" << exe1 << endl;
			cout << "测试运行情况：" << status << "=" << items_num << endl;
			cout << "时间：" << starttime << " - " << endtime << endl;
			cout << "======================================================================" << endl;
		}
		else if (cmd_style == "redirection") {
			string starttime, endtime;
			CheckExec_Errno status;
			modify_dirname(redirection_data_dirname);
			for (size_t i = 0; i < item_gname.size(); i++) {
				st_CheckExec example1(exe1 + " < " + redirection_data_dirname  + item_fname[i], "参考exe文件", stoi(max_output_len), stoi(timeout));
				example1.running();
				status = example1.get_errno();
				if (status != CheckExec_Errno::ok) error = true;
				if (i == 0) starttime = example1.getStartTimeStr();
				if (i == item_gname.size() - 1) endtime = example1.getEndTimeStr();
			}
			cout << starttime << "  参考答案生成情况" << endl;
			cout << "======================================================================" << endl;
			cout << "参考exe文件：" << exe1 << endl;
			cout << "测试运行情况：" << status << "=" << items_num << endl;
			cout << "时间：" << starttime << " - " << endtime << endl;
			cout << "======================================================================" << endl;
		}
		else if (cmd_style == "main_with_arguments") {
			string starttime, endtime;
			CheckExec_Errno status;
			modify_dirname(redirection_data_dirname);
			for (size_t i = 0; i < item_gname.size(); i++) {
				st_CheckExec example1(exe1 + " " + item_args[i], "参考exe文件", stoi(max_output_len), stoi(timeout));
				example1.running();
				status = example1.get_errno();
				if (status != CheckExec_Errno::ok) error = true;
				if (i == 0) starttime = example1.getStartTimeStr();
				if (i == item_gname.size() - 1) endtime = example1.getEndTimeStr();
			}
			cout << starttime << "  参考答案生成情况" << endl;
			cout << "======================================================================" << endl;
			cout << "参考exe文件：" << exe1 << endl;
			cout << "测试运行情况：" << status << "=" << items_num << endl;
			cout << "时间：" << starttime << " - " << endtime << endl;
			cout << "======================================================================" << endl;
		}
	}
	else if (exe_style == "single") {
		if (name_list == "database") {
			MYSQL* mysql;
			MYSQL_RES* result;
			MYSQL_ROW  row;

			/* 初始化 mysql 变量，失败返回NULL */
			if ((mysql = mysql_init(NULL)) == NULL) {
				cout << "mysql_init failed" << endl;
				return;
			}

			/* 连接数据库，失败返回NULL
				1、mysqld没运行
				2、没有指定名称的数据库存在 */
			if (mysql_real_connect(mysql, db_host.c_str(), db_username.c_str(), db_passwd.c_str(), db_name.c_str(), 0, NULL, 0) == NULL) {
				cout << "mysql_real_connect failed(" << mysql_error(mysql) << ")" << endl;
				return;
			}

			// 设置字符集，否则读出的字符乱码 
			mysql_set_character_set(mysql, "gbk");

			string command = "call proc_hwapp_get_stulist_from_view_student_cno('" + db_curr_term + "','" + db_cno_list + "','sno',NULL)";
			if (mysql_query(mysql, command.c_str())) {
				cout << "mysql_query failed(" << mysql_error(mysql) << ")" << endl;
				return;
			}

			// 将查询结果存储起来，出现错误则返回NULL
			//	注意：查询结果为NULL，不会返回NULL 
			if ((result = mysql_store_result(mysql)) == NULL) {
				cout << "mysql_store_result failed" << endl;
				return;
			}

			int idx = 1;
			while ((row = mysql_fetch_row(result)) != NULL) {
				cout << setiosflags(ios::left);           //输出左对齐
				// 共13项，目前只打印了5项 
				//cout << row[10];                   //row[1]是学号
				//cout << endl;
				STUDENT tmp;
				tmp.idx = idx++;
				tmp.cno = row[10];
				tmp.name = row[2];
				tmp.sno = row[1];
				student.push_back(tmp);
			}
			//cout << endl;

			// 释放result，否则会丢失内存 
			mysql_free_result(result);
		}
		else {  //从文件中读取学生信息
			student.clear(); // 清空容器，避免原有数据干扰
			ifstream ifs(name_list);
			if (!ifs.is_open())
			{
				cerr << "文件打开失败: " << name_list << endl;
				return;
			}

			string line;
			int valid_idx = 1; // 有效数据的序号，从1开始计数
			while (getline(ifs, line))
			{
				// 跳过空行
				if (line.empty()) continue;
				// 跳过以#开头的行，不读取、不计入序号、不处理
				if (line[0] == '#') continue;

				// 解析当前行的制表符分隔的三个字段 cno sno name
				stringstream ss(line);
				string cno, sno, name;
				ss >> cno >> sno >> name;

				// 核心：过滤重复行 - 三字段完全一致判定为重复数据，跳过不存入
				bool isRepeat = false;
				for (const auto& stu : student)
				{
					if (stu.cno == cno && stu.sno == sno && stu.name == name)
					{
						isRepeat = true;
						break;
					}
				}
				if (isRepeat)
					continue;

				// 封装结构体数据并存入vector
				STUDENT stu;
				stu.idx = valid_idx++;
				stu.cno = cno;
				stu.sno = sno;
				stu.name = name;
				student.push_back(stu);
			}

			ifs.close(); // 关闭文件流
		}
		/*
		for (size_t i = 0; i < student.size(); i++)
			cout <<student[i].idx<<" " << student[i].sno << " " << student[i].name << " " << student[i].cno << endl;
		*/
		single_check();
    }
	else if (exe_style == "multi") {
		if (name_list == "database") {
			MYSQL* mysql;
			MYSQL_RES* result;
			MYSQL_ROW  row;

			/* 初始化 mysql 变量，失败返回NULL */
			if ((mysql = mysql_init(NULL)) == NULL) {
				cout << "mysql_init failed" << endl;
				return;
			}

			/* 连接数据库，失败返回NULL
				1、mysqld没运行
				2、没有指定名称的数据库存在 */
			if (mysql_real_connect(mysql, db_host.c_str(), db_username.c_str(), db_passwd.c_str(), db_name.c_str(), 0, NULL, 0) == NULL) {
				cout << "mysql_real_connect failed(" << mysql_error(mysql) << ")" << endl;
				return;
			}

			// 设置字符集，否则读出的字符乱码 
			mysql_set_character_set(mysql, "gbk");

			string command = "call proc_hwapp_get_stulist_from_view_student_cno('" + db_curr_term + "','" + db_cno_list + "','sno',NULL)";
			if (mysql_query(mysql, command.c_str())) {
				cout << "mysql_query failed(" << mysql_error(mysql) << ")" << endl;
				return;
			}

			// 将查询结果存储起来，出现错误则返回NULL
			//	注意：查询结果为NULL，不会返回NULL 
			if ((result = mysql_store_result(mysql)) == NULL) {
				cout << "mysql_store_result failed" << endl;
				return;
			}

			int idx = 1;
			while ((row = mysql_fetch_row(result)) != NULL) {
				cout << setiosflags(ios::left);           //输出左对齐
				// 共13项，目前只打印了5项 
				//cout << row[10];                   //row[1]是学号
				//cout << endl;
				STUDENT tmp;
				tmp.idx = idx++;
				tmp.cno = row[10];
				tmp.name = row[2];
				tmp.sno = row[1];
				student.push_back(tmp);
			}
			//cout << endl;

			// 释放result，否则会丢失内存 
			mysql_free_result(result);
		}
		else {  //从文件中读取学生信息
			student.clear(); // 清空容器，避免原有数据干扰
			ifstream ifs(name_list);
			if (!ifs.is_open())
			{
				cerr << "文件打开失败: " << name_list << endl;
				return;
			}

			string line;
			int valid_idx = 1; // 有效数据的序号，从1开始计数
			while (getline(ifs, line))
			{
				// 跳过空行
				if (line.empty()) continue;
				// 跳过以#开头的行，不读取、不计入序号、不处理
				if (line[0] == '#') continue;

				// 解析当前行的制表符分隔的三个字段 cno sno name
				stringstream ss(line);
				string cno, sno, name;
				ss >> cno >> sno >> name;

				// 核心：过滤重复行 - 三字段完全一致判定为重复数据，跳过不存入
				bool isRepeat = false;
				for (const auto& stu : student)
				{
					if (stu.cno == cno && stu.sno == sno && stu.name == name)
					{
						isRepeat = true;
						break;
					}
				}
				if (isRepeat)
					continue;

				// 封装结构体数据并存入vector
				STUDENT stu;
				stu.idx = valid_idx++;
				stu.cno = cno;
				stu.sno = sno;
				stu.name = name;
				student.push_back(stu);
			}

			ifs.close(); // 关闭文件流
		}
		single_check();
	}
}