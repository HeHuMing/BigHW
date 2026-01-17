/* 2353626 贺胡鸣 大数据 */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <sstream>
#include <string>
#include <conio.h>
#include <Windows.h>
#include "../include/class_check.h"
using namespace std;

#define TIMER_NEW_VERSION	1

#if TIMER_NEW_VERSION

//新版本定时器（CreateTimerQueueTimer函数及相关配套函数，适用于多线程）
//仅<Windows.h>即可，不需要其它头文件

#else

//旧版本定时器（timeSetEvent函数及相关配套函数，不适用多线程）
#include <mmsystem.h>
#pragma comment(lib,"Winmm.lib")

#endif


/***************************************************************************
  函数名称：
  功    能：CheckExec_Errno的输出
  输入参数：
  返 回 值：
  说    明：
 ***************************************************************************/
ostream& operator<<(ostream& out, const CheckExec_Errno& eno)
{
	switch (eno) {
	case CheckExec_Errno::ok:
		out << "正确运行";
		break;
	case CheckExec_Errno::create_timer_id_failed:
		out << "定时器创建失败";
		break;
	case CheckExec_Errno::popen_faliled:
		out << "管道方式打开失败";
		break;
	case CheckExec_Errno::start_timer_failed:
		out << "启动定时器失败";
		break;
	case CheckExec_Errno::timeout:
		out << "超时";
		break;
	case CheckExec_Errno::max_output:
		out << "超过输出上限";
		break;
	case CheckExec_Errno::killed_by_callback:
		out << "死循环";
		break;
	default:
		out << "未知的错误";
		break;
	}//end of switch
	return out;
}


/***************************************************************************
  函数名称：
  功    能：回调函数因为已经限定了类型，不能是成员函数
  输入参数：dwUser：启动定时器时传入了this指针
  返 回 值：
  说    明：设置了1秒定时器，因此每秒会触发一次，
***************************************************************************/
#if TIMER_NEW_VERSION
static void CALLBACK timeout_process(PVOID ExtParameter, BOOLEAN TimerOrWaitFired)
#else
static void CALLBACK timeout_process(UINT uTimerID, UINT uMsg, DWORD ExtParameter, DWORD dw1, DWORD dw2)
#endif
{
	st_CheckExec* my_exe = (st_CheckExec*)ExtParameter;

	/* 判断是否超时时间到（置标记，具体杀任务等操作到回调函数外面处理） */
	if (++my_exe->time_count >= my_exe->cfg_timeout) {
		my_exe->eno = CheckExec_Errno::timeout;

		/* 死机防止：
			前提：如果程序运行后没有任何输出，会导致running函数中while循环的fgetc函数阻塞（不返回），此时即使置了timeout，也无法进入running函数的while循环中，导致无法结束
					例：一个需要输入的程序，用 cmd_style = normal 方式运行，导致没有任何输入，会阻塞fgetc
			处理：在本回调函数中调用taskkill
			注意：在回调函数尽量不执行长时间操作，否则可能会带来阻塞或延迟风险，因此做一个预防性处理，只有超时后running不处理，才在本函数中taskkill
		*/
		int delta = my_exe->cfg_timeout < 5 ? my_exe->cfg_timeout * 2 : 5; //预防性处理：超时<5秒，两倍时间，>=5，加5秒
		if (my_exe->time_count >= my_exe->cfg_timeout + delta) {
			//cout << "timeout = " << my_exe->time_count << " " << my_exe->msg.str().length() << " " << my_exe->eno << endl;
			my_exe->stop(CheckExec_Errno::killed_by_callback);
		}
	}
}

/***************************************************************************
  函数名称：
  功    能：启动定时器
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
int st_CheckExec::start_timer()
{
#if TIMER_NEW_VERSION
	//	this->timer_queue = CreateTimerQueue();
	//	if (this->timer_queue == NULL) {
	//		this->eno = CheckExec_Errno::create_timer_queue_failed;
	//		return -1;
	//	}
		/* 参数解释：
		   this->timer_id  ：定时器句柄
		   NULL            ：定时器队列句柄（NULL表示和默认队列相关，this->timer_queue则表示新建队列）
		   timeout_process ：回调函数
		   this            ：回调函数的附加参数（一个任意类型的指针）
		   0               ：定时器延迟启动时间（ms）
		   1000            ：定时器间隔（ms）
		   NULL            ：标记位

		   ChangeTimerQueueTimer：改变定时器设置
		   DeleteTimerQueueTimer：删除定时器
		*/
	if (CreateTimerQueueTimer(&this->timer_id, NULL, timeout_process, this, 1000, 1000, NULL) == 0) {
		this->eno = CheckExec_Errno::create_timer_id_failed;
		return -1;
	}
#else
	/* 启动定时器（间隔1000ms，精度100ms） */
	this->timer_id = timeSetEvent(1000, 100, timeout_process, (DWORD_PTR)this, TIME_PERIODIC);
	if (timer_id == NULL) {
		this->eno = CheckExec_Errno::create_timer_id_failed;
		return -1;
	}
#endif

	/* 计时器清0 */
	this->time_count = 0;

	/* 开始时间计数 */
	QueryPerformanceFrequency(&this->time_tick);	//获得硬件支持的高精度计数器的频率
	QueryPerformanceCounter(&this->begin_time);	//获得初始硬件定时器计数

	return 0;
}

/***************************************************************************
  函数名称：
  功    能：删除定时器
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void st_CheckExec::stop_timer()
{
#if TIMER_NEW_VERSION
	if (this->timer_id != NULL) {
		DeleteTimerQueueTimer(NULL, this->timer_id, NULL);
		this->timer_id = NULL;
	}
#else
	if (this->timer_id != NULL) {
		timeKillEvent(this->timer_id);
		this->timer_id = NULL;
	}
#endif
	return;
}

/***************************************************************************
  函数名称：
  功    能：删除定时器，杀掉正在运行的任务
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
int st_CheckExec::stop(CheckExec_Errno no)
{
	GetLocalTime(&this->endSysTime);
	this->stop_timer();
	QueryPerformanceCounter(&this->end_time);//获得终止硬件定时器计数

	this->eno = no;

	/* 如果非正常结束，用 killtask 命令杀掉超时进程
		1、exe文件名为不带路径的裸文件名
		2、如果之后的 _pclose() 不返回（程序死掉），说明 taskkill 可能未成功 */
	if (this->eno != CheckExec_Errno::ok) {
		char cmd_taskkill[512];
		sprintf(cmd_taskkill, "taskkill /f /t /im %s 1>nul 2>&1", this->exec_name.c_str());
		system(cmd_taskkill);
	}

	/* 关闭文件（必须在taskkill之后，如果_plose不返回，说明task_kill可能未执行成功） */
	if (this->fp_exe) {
		_pclose(this->fp_exe);
		this->fp_exe = NULL;
	}
	return 0;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
 ***************************************************************************/
string st_CheckExec::get_full_cmd_exec() const
{
	return this->full_exec_cmd;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
 ***************************************************************************/
CheckExec_Errno st_CheckExec::get_errno() const
{
	return this->eno;
}

/***************************************************************************
  函数名称：
  功    能：获取定时器的运行时间(ms)
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
double st_CheckExec::get_running_time() const
{
	return double(this->end_time.QuadPart - this->begin_time.QuadPart) / double(time_tick.QuadPart);
}

/***************************************************************************
  函数名称：
  功    能：如果有必要，可以reset后重复运行
  输入参数：
  返 回 值：
  说    明：
 ***************************************************************************/
int st_CheckExec::reset()
{
	this->time_count = 0;
	this->fp_exe = NULL;
	this->timer_id = NULL;
	this->eno = CheckExec_Errno::ok;
	this->msg.str("");

	return 0;
}

/***************************************************************************
  函数名称：
  功    能：构造函数
  输入参数：
  返 回 值：
  说    明：
 ***************************************************************************/
st_CheckExec::st_CheckExec(const string& full_exec_cmd, const string& exec_name, int max_output_len, int timeout_second)
{
	this->full_exec_cmd = full_exec_cmd;
	this->exec_name = exec_name;
	this->max_output_len = max_output_len;
	this->cfg_timeout = timeout_second;

	this->time_count = 0;
	this->fp_exe = NULL;
	this->timer_id = NULL;
	this->eno = CheckExec_Errno::ok;
	this->msg.str("");
}

/***************************************************************************
  函数名称：
  功    能：析构函数
  输入参数：
  返 回 值：
  说    明：
 ***************************************************************************/
st_CheckExec::~st_CheckExec()
{
	if (this->timer_id != NULL)
		this->stop_timer();

	/* 如果非正常结束，生成 killtask 命令，杀掉超时进程（exe文件名一定要正确） */
	if (this->eno != CheckExec_Errno::ok) {
		char cmd_taskkill[512];
		sprintf(cmd_taskkill, "taskkill /f /t /im %s 1>nul 2>&1", this->exec_name.c_str());
		system(cmd_taskkill);
	}

	/* 关闭文件（必须在taskkill之后） */
	if (this->fp_exe) {
		_pclose(this->fp_exe);
		this->fp_exe = NULL;
	}
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
 ***************************************************************************/
int st_CheckExec::running()
{
	/* 第1步：以 r 方式打开要运行的exe文件
		r ：行结束符是\n
		rb：行结束符是\r\n */
	if ((this->fp_exe = _popen(this->full_exec_cmd.c_str(), "rb")) == NULL) {
		this->eno = CheckExec_Errno::popen_faliled;
		return -1;
	}

	/* 启动定时器 */
	if (this->start_timer() < 0) { //启动定时器(1秒)
		this->msg << "启动定时器错误" << endl;
		this->eno = CheckExec_Errno::start_timer_failed;
		return -1;
	}
	GetLocalTime(&this->startSysTime);


	/* 第2步：从fp_exe中读数据（模拟exe的输出），放入 */
	signed char ch;
	int  ch_num = 0;

	/* 循环未结束表示程序运行未结束，因此回调函数会在循环中被触发，回调函数执行完成后，仍然停留在循环中 */
	while ((ch = fgetc(this->fp_exe)) != EOF) {
		this->msg << ch; //循环输出到msg中
		++ch_num;
#if 0 //如果需要，打开后看程序输出信息
		//		cout << "num=" << ch_num << " ch=" << int(ch) << endl;
		if (ch_num % 256 == 0) { //没256个输出打印一次
			cout << ch_num << ' ';
			cout << msg.str() << endl;
		}
#endif
		/* 超时：emo已在回调函数中设置，回调函数执行完成后会执行到此处 */
		if (this->eno == CheckExec_Errno::timeout) {
			this->stop(CheckExec_Errno::timeout);
			return -1;
		}

		/* 超长 */
		if (ch_num >= this->max_output_len) {
			this->stop(CheckExec_Errno::max_output);
			return -1;
		}
	} //end of while

	/* 执行到此，有两种情况，分开处理
		1、程序已执行完成（读到了EOF），正常结束，停止定时器即可
		2、因为死循环/fgetc阻塞（没有任何输出导致fgetc不返回），这种情况能执行到此处，说明是回调函数杀的进程 */
	if (this->eno == CheckExec_Errno::killed_by_callback)
		return -1;

	/* 正常结束 */
	this->stop(CheckExec_Errno::ok);
	return 0;
	return 0;
}

// 格式化开始时间：返回 "2026-01-16 13:21:29" 格式
string st_CheckExec::getStartTimeStr() const
{
	char buf[64] = { 0 };
	sprintf(buf, "%04d-%02d-%02d %02d:%02d:%02d",
		this->startSysTime.wYear,
		this->startSysTime.wMonth,
		this->startSysTime.wDay,
		this->startSysTime.wHour,
		this->startSysTime.wMinute,
		this->startSysTime.wSecond);
	return string(buf);
}

// 格式化结束时间：返回 "2026-01-16 13:28:12" 格式
string st_CheckExec::getEndTimeStr() const
{
	char buf[64] = { 0 };
	sprintf(buf, "%04d-%02d-%02d %02d:%02d:%02d",
		this->endSysTime.wYear,
		this->endSysTime.wMonth,
		this->endSysTime.wDay,
		this->endSysTime.wHour,
		this->endSysTime.wMinute,
		this->endSysTime.wSecond);
	return string(buf);
}