/* 2353626 贺胡鸣 大数据 */
#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <conio.h>
#include <Windows.h>
using namespace std;

#define TIMER_NEW_VERSION	1

/* 定义各种返回类型 */
enum class CheckExec_Errno {
	ok = 0,
	create_timer_id_failed,		//创建定时器ID失败
	popen_faliled,		//管道方式打开可执行文件失败
	start_timer_failed,	//启动定时器失败
	timeout,				//超时
	max_output,			//达到设定的输出上限（死循环输出，反正肯定不对了）
	killed_by_callback, //死循环（超时且fgetc阻塞）
	max
};

ostream& operator<<(ostream& out, const CheckExec_Errno& eno);

class st_CheckExec {
protected:
	/* 初始化的4个参数 */
	string full_exec_cmd;	//完整的执行命令（用于_popen）
	string exec_name;		//exe文件名（用于taskkill）
	int    cfg_timeout;		//设置的超时（秒）
	int    max_output_len;	//读取输出的最大长度

	FILE* fp_exe;
	int    time_count;

#if TIMER_NEW_VERSION
	HANDLE		timer_id; //定时器ID
#else
	MMRESULT     timer_id; //定时器ID
#endif
	LARGE_INTEGER time_tick;
	LARGE_INTEGER begin_time;
	LARGE_INTEGER end_time;

	CheckExec_Errno eno;  //错误号（不能叫errno）

	int  start_timer();
	void stop_timer();
	int  stop(CheckExec_Errno eno);
public:
	SYSTEMTIME startSysTime;  // 存储程序运行的【开始系统时间】
	SYSTEMTIME endSysTime;    // 存储程序运行的【结束系统时间】
	string getStartTimeStr() const;  // 获取格式化的开始时间字符串
	string getEndTimeStr() const;    // 获取格式化的结束时间字符串

	ostringstream msg; //存放输出
	int              running();
	double           get_running_time() const;
	string          get_full_cmd_exec() const;
	CheckExec_Errno get_errno() const;
	int              reset(); //重置，进行下次运行
	st_CheckExec(const string& full_exec_cmd, const string& exec_name, int max_output_len, int timeout_second);
	~st_CheckExec();

	/* 回调函数声明为友元 */
#if TIMER_NEW_VERSION
	friend void CALLBACK timeout_process(PVOID lpParameter, BOOLEAN TimerOrWaitFired);
#else
	friend void CALLBACK timeout_process(UINT uTimerID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2);
#endif
};

#if TIMER_NEW_VERSION
static void CALLBACK timeout_process(PVOID ExtParameter, BOOLEAN TimerOrWaitFired);
#else
static void CALLBACK timeout_process(UINT uTimerID, UINT uMsg, DWORD ExtParameter, DWORD dw1, DWORD dw2);
#endif