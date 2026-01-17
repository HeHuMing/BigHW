/* 2353626 贺胡鸣 大数据 */
#include <iostream>
#include "hw_check_tools.h"

using namespace std;

enum OPT_ARGS {
	OPT_ARGS_HELP = 0,
	OPT_ARGS_DEBUG,
	OPT_ARGS_CHECKNAME,
	OPT_ARGS_CHECKCFG_ONLY,
	OPT_ARGS_CFGFILE
};

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
 ***************************************************************************/
static void usage(const char* const full_procname)
{
	const int offset_len = 7;
	const char* procname = strrchr(full_procname, '\\');
	if (procname == NULL)
		procname = full_procname;
	ostringstream msg;

	msg << "Usage: " << full_procname << " --必选项 | --可选项(出现次序不限)" << endl;
	msg << endl;
	msg << "必选项：指定操作" << endl;
	msg << "   --checkname : 需要检查的项目名(配置文件的组名)" << endl;
	msg << endl;
	msg << "必选项[--checkname]的可选参数：" << endl;
	msg << "   --checkcfg_only : 仅打印配置文件检查结果" << endl;
	msg << endl;
	msg << "可选项：指定操作(任选0~n个)" << endl;
	msg << "   --help    : 打印帮助信息" << endl;
	msg << "   --debug   : 设置Debug等级(可设等级 : warn/info/debug/trace，后项均包含前项，例：info必然 包含warn信息)" << endl;
	msg << "                   fatal : 致命错误(不能设置，已打开)" << endl;
	msg << "                   error : 程序异常错误(不能设置，已打开)" << endl;
	msg << "                   warn  : 提示潜在问题，但不影响程序运行(默认项)" << endl;
	msg << "                   info  : 记录程序运行状态信息" << endl;
	msg << "                   debug : 打印程序调试过程中的详细信息(打开后会影响程序执行速度，仅建议Debug时使用)" << endl;
	msg << "                   trace : 比debug更详细的调试信息(打开后会影响程序执行速度，仅建议Debug时使用)" << endl;
	msg << "   --cfgfile : 指定配置文件" << endl;
	msg << endl;


	/* 本程序的特殊示例 */
	msg << "e.g.  :" << endl;
	msg << setw(offset_len) << ' ' << procname << " --checkname 3-b3                 : 按配置文件[3-b3]组的设定检查exe的运行结果" << endl;
	msg << setw(offset_len) << ' ' << procname << " --checkname 3-b3 --checkcfg_only : 检查配置文件[3-b3]组的设定是否正确" << endl;
	msg << setw(offset_len) << ' ' << procname << " --checkname 3-b3 --debug trace   : 按配置文件[3-b3]组的设定检查exe的运行结果，打印所有调试信息" << endl;
	msg << endl;

	cout << msg.str() << endl;
}

static void version()
{
	ostringstream msg;
	msg << endl;
	msg << "hw_check_exe_demo Version : V2025.12.23" << endl;
	cout << msg.str() << endl;
}


/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
 ***************************************************************************/
int main(int argc, char** argv)
{
	const string loglevel_define[] = { "warn", "info", "debug", "trace", "" };

	args_analyse_tools args[] = {
		args_analyse_tools("--help",		   ST_EXTARGS_TYPE::boolean, 0, false),
		args_analyse_tools("--debug",		   ST_EXTARGS_TYPE::str_with_set_default, 1, 0, loglevel_define),
		args_analyse_tools("--checkname",	   ST_EXTARGS_TYPE::str, 1, string("")),
		args_analyse_tools("--checkcfg_only", ST_EXTARGS_TYPE::boolean, 0, false),
		args_analyse_tools("--cfgfile",	   ST_EXTARGS_TYPE::str, 1, string("hw_check_exe.cfg")),
		args_analyse_tools()  //最后一个，用于结束
	};
	int cur_argc, ret = 0;

	//如果不带参数，打印参数表和使用方法并结束程序
	if (argc == 1) {
		version();
		args_analyse_print(args);
		usage(argv[0]);
		return -1;
	}

	//最后一个参数1，表示除选项参数外，还有其它参数
	if ((cur_argc = args_analyse_process(argc, argv, args, 0)) < 0) {
		//错误信息在函数中已打印
		cout << endl;
		return -1;
	}
	/*
	else {
		version();
		args_analyse_print(args);
		usage(argv[0]);
	}
	*/

	/* 对help做特殊处理 */
	if (args[OPT_ARGS_HELP].existed()) {
		//只要有 --help，其它参数都忽略，显示帮助即可
		version();
		args_analyse_print(args);
		usage(argv[0]);
		return -1; //执行完成直接退出
	}


	if (!args[OPT_ARGS_CHECKNAME].existed()) {
		cerr << "==============" << endl;
		cerr << "错误提示信息：" << endl;
		cerr << "==============" << endl;
		cerr << "参数[--checkname]必须选择" << endl;
		cerr << endl << endl;
	}
	else {
		if (args[OPT_ARGS_CHECKCFG_ONLY].existed()) {
			check_tools ct;
			ct.print_cfg(args[OPT_ARGS_CFGFILE].get_string(), args[OPT_ARGS_CHECKNAME].get_string());
			ct.open_files();
		}
		else {
			check_tools ct;
			//ct.read_db();
			ct.read_cfg(args[OPT_ARGS_CFGFILE].get_string(), args[OPT_ARGS_CHECKNAME].get_string());
			ct.open_files();
			ct.exe_run();
		}
	}

	return 0;
}
