/* 2353626 贺胡鸣 大数据 */
#pragma once
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>
#include "../include/class_cft.h"
#include "../include/class_aat.h"
#include "../include/class_tc.h"
#include "../include/class_check.h"
#include "../include_mariadb_x86/mysql/mysql.h"  
using namespace std;

//学生信息
struct STUDENT {
	int idx;
	string cno;
	string name;
	string sno;
};


class check_tools {
private:
	string include;

	//database
	string db_host;
	string db_port;
	string db_name;
	string db_username;
	string db_passwd;
	string db_curr_term;
	string db_cno_list;

	string exe_style = "multi";
	string name_list = "database";
	string single_exe_dirname = "<EMPTY>";
	string multi_exe_main_dirname = "<EMPTY>";
	string multi_exe_sub_dirname = "<EMPTY>";
	string stu_exe_name = "<EMPTY>";
	string demo_exe_name = "<EMPTY>";
	string cmd_style = "normal";
	string pipe_get_input_data_exe_name = "<EMPTY>";
	string pipe_data_file = "<EMPTY>";
	string redirection_data_dirname = "<EMPTY>";
	string timeout = "1";
	string max_output_len = "1024";

	string tc_trim = "none";
	string tc_lineskip = "0";
	string tc_lineoffset = "0";
	string tc_ignoreblank = "0";
	string tc_not_ignore_linefeed = "0";
	string tc_maxdiff = "0";
	string tc_maxline = "0";
	string tc_display = "none";

	int items_num = 0;
	int items_begin = 0;
	int items_end = 0;
	vector<string> item_gname;
	vector<string> item_fname;
	vector<string> item_args;

	int warning = 1;
	ifstream demo;
	ifstream pipe_get_input_data;
	ifstream pipe_data;
	//ifstream multi_main_dirname;
	//ifstream multi_sub_dirname;

	istringstream file1, file2;
	vector<STUDENT> student;

public:
	void print_cfg(const string cfgfile, const string checkname);
	void open_files();
	void read_cfg(const string cfgfile, const string checkname);
	//void read_db();
	void exe_run();
	void single_check();
};

class group {
public:
	string groupname;
	string include;

	string exe_style = "multi";
	string name_list = "database";
	string single_exe_dirname = "<EMPTY>";
	string multi_exe_main_dirname = "<EMPTY>";
	string multi_exe_sub_dirname = "<EMPTY>";
	string stu_exe_name = "<EMPTY>";
	string demo_exe_name = "<EMPTY>";
	string cmd_style = "normal";
	string pipe_get_input_data_exe_name = "<EMPTY>";
	string pipe_data_file = "<EMPTY>";
	string redirection_data_dirname = "<EMPTY>";
	int timeout = 1;
	int max_output_len = 1024;

	string tc_trim = "none";
	int tc_lineskip = 0;
	int tc_lineoffset = 0;
	bool tc_ignoreblank = 0;
	bool tc_not_ignore_linefeed = 0;
	int tc_maxdiff = 0;
	int tc_maxline = 0;
	string tc_display = "none";

	int items_num = 0;
	int items_begin = 0;
	int items_end = 0;
	vector<string> item_gname;
	vector<string> item_fname;
	vector<string> item_args;
};


