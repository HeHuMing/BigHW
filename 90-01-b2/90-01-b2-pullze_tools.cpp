/* 2353626 贺胡鸣 大数据 */
#include "90-01-b2-pullze.h"
using namespace std;

/***************************************************************************
  函数名称：menu
  功    能：菜单的显示与选择
  输入参数：/
  返 回 值：char选择
  说    明：/
***************************************************************************/
char menu()
{
	cct_cls();
	cout << "---------------------------------------------------------" << endl;
	cout << " A.内部数组，原样输出" << endl;
	cout << " B.内部数组，生成提示行列并输出" << endl;
	cout << " C.内部数组，游戏版" << endl;
	cout << " --------------------------------------------------------" << endl;
	cout << " D.n*n的框架(无分隔线)，原样输出" << endl;
	cout << " E.n*n的框架(无分隔线)，含提示行列" << endl;
	cout << " F.n*n的框架(无分隔线)，显示初始状态，鼠标移动可显示坐标" << endl;
	cout << " G.cmd图形界面完整版(无分隔线)" << endl;
	cout << " --------------------------------------------------------" << endl;
	cout << " H.n*n的框架(有分隔线)，原样输出" << endl;
	cout << " I.n*n的框架(有分隔线)，含提示行列" << endl;
	cout << " J.n*n的框架(有分隔线)，显示初始状态，鼠标移动可显示坐标" << endl;
	cout << " K.cmd图形界面完整版(有分隔线)" << endl;
	cout << " --------------------------------------------------------" << endl;
	cout << " Q.退出" << endl;
	cout << "---------------------------------------------------------" << endl;
	cout << "[请选择:] ";
	char choice;
	while (1) {
		choice = _getch();
		if (choice >= 'A' && choice <= 'K' || choice == 'Q') {
			putchar(choice);
			break;
		}
		else if (choice >= 'a' && choice <= 'k' || choice == 'q') {
			putchar(choice);
			choice -= 32;
			break;
		}
		else
			continue;
	}
	cout << endl;
	return choice;
}

/***************************************************************************
  函数名称：end
  功    能：输入end表示本小题结束
  输入参数：/
  返 回 值：/
  说    明：/
***************************************************************************/
void end()
{
	int x, y;
	cct_getxy(x, y);
	while (1) {
		cct_gotoxy(x, y);
		cout << "本小题结束，请输入End继续...";
		char e = 0, n = 0, d = 0;
		int i = 1;
		while (1) {
			char tmp = _getch();
			if (tmp == '\r')
				break;
			putchar(tmp);
			if (i == 1)
				e = tmp;
			else if (i == 2)
				n = tmp;
			else if (i == 3)
				d = tmp;
			i++;
		}
		if ((e == 'e' || e == 'E') && (n == 'n' || n == 'N') && (d == 'd' || d == 'D'))
			break;
		else {
			cct_gotoxy(x + 28, y);
			cout << "    ";
			cct_gotoxy(x, y + 1);
			cout << "输入错误，请重新输入" << endl;
			continue;
		}
	}
}

/***************************************************************************
  函数名称：time_sleep
  功    能：加延时
  输入参数：char choice
  返 回 值：/
  说    明：/
***************************************************************************/
void time_sleep(char choice)
{
	if (choice == 'D' || choice == 'H')
		Sleep(1);
	else
		return;
}

/***************************************************************************
  函数名称：form
  功    能：设置初始窗口大小、缓冲区大小、字体及大小格式
  输入参数：size,choice
  返 回 值：/
  说    明：/
***************************************************************************/
void form(int size, char choice)
{
	if (choice == 'E' || choice == 'F' || choice == 'G') {
		if (size == 5) {
			cct_setfontsize("新宋体", 36);
			cct_setconsoleborder(55, 24, 55, 24);
		}
		else if (size == 10) {
			cct_setfontsize("新宋体", 36);
			cct_setconsoleborder(61, 24, 61, 24);
		}
		else {
			cct_setfontsize("新宋体", 24);
			cct_setconsoleborder(85, 50, 85, 50);
		}
	}
	else {
		if (size == 15)
			cct_setfontsize("新宋体", 12);
		else
			cct_setfontsize("新宋体", 16);
		cct_setconsoleborder(180, 180, 180, 180);
	}
}

/***************************************************************************
  函数名称：recover
  功    能：恢复初始窗口大小、缓冲区大小、字体及大小
  输入参数：/
  返 回 值：/
  说    明：/
***************************************************************************/
void recover()
{
	cct_setconsoleborder(188, 200, 188, 55);
	cct_setfontsize("新宋体", 16);
}

/***************************************************************************
  函数名称：restrict
  功    能：求表格的宽度和高度
  输入参数：int size,int row[maxsize][8],int col[8][maxsize],int width, int height
  返 回 值：/
  说    明：/
***************************************************************************/
void restrict(int size, int row[maxsize][8], int col[8][maxsize], int* width, int* height)
{
	int tmp = 0;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < 8; j++) {
			if (row[i][j] != 0)
				tmp++;
		}
		*width = tmp > *width ? tmp : *width;
		tmp = 0;
	}
	tmp = 0;
	for (int j = 0; j < size; j++) {
		for (int i = 0; i < 8; i++) {
			if (col[i][j] != 0)
				tmp++;
		}
		*height = tmp > *height ? tmp : *height;
		tmp = 0;
	}
}

/***************************************************************************
  函数名称：prompt
  功    能：游戏小题的输入提示
  输入参数：/
  返 回 值：/
  说    明：/
***************************************************************************/
void prompt()
{
	cout << endl;
	cout << "命令形式：Aa=等价于图形游戏中鼠标左键选择Aa位(区分大小写)" << endl;
	cout << "             不需要支持图形界面的右键打叉，再次输入Aa相当于清除" << endl;
	cout << "          X/x=退出(新行仅有X/x，不分大小写)" << endl;
	cout << "          Y/y=提交(新行仅有Y/y，不分大小写)" << endl;
	cout << "          Z/z=作弊(新行仅有Z/z，不分大小写)" << endl;
	cout << "          Ctrl+c 强制退出" << endl;
	cout << "请输入 : ";
}

/***************************************************************************
  函数名称：judge
  功    能：游戏小题的输入判断
  输入参数：int size,char game[200],char newball[maxsize][maxsize]
  返 回 值：/
  说    明：flag=1:未标记，需要画球；flag=2:x,y,z特殊操作；flag=3:已标记，需要擦球; flag=0,输入错误
***************************************************************************/
int judge(int size, char game[200], char newball[maxsize][maxsize])
{
	int flag = 0;
	if (game[0] >= 'A' && game[0] <= 'A' + size - 1) {
		if (game[1] >= 'a' && game[1] <= 'a' + size - 1) {
			int flag1 = 0;
			for (int i = 0; i < size; i++) {
				for (int j = 0; j < size; j++) {
					if (newball[i][j] != 0 && game[0] == (char)(i + 'A') && game[1] == (char)(j + 'a'))
						flag1 = 1;
				}
			}
			if (flag1)
				flag = 3;
			else
				flag = 1;
		}
		else
			flag = 0;
	}
	else if (game[0] == 'X' || game[0] == 'Y' || game[0] == 'Z' || game[0] == 'x' || game[0] == 'y' || game[0] == 'z') {
		if (game[1] != 0)
			flag = 0;
		else
			flag = 2;
	}
	else
		flag = 0;
	return flag;
}

/***************************************************************************
  函数名称：examine
  功    能：游戏小题提交成功的判断
  输入参数：int size,char newball[15][15],char ball[maxsize][maxsize]
  返 回 值：0/1
  说    明：可能会存在多解
***************************************************************************/
int examine(int size, char newball[maxsize][maxsize], char ball[maxsize][maxsize])
{
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (newball[i][j] == 1)
				newball[i][j] = 'O';
		}
	}
	int newrow[maxsize][8] = { 0 }, newcol[8][maxsize] = { 0 };
	count_ball(size, newball, newrow, newcol);
	int row[maxsize][8] = { 0 }, col[8][maxsize] = { 0 };
	count_ball(size, ball, row, col);
	int result = 0;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < 8; j++) {
			if (row[i][j] != newrow[i][j])
				result = 1;
		}
	}
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < size; j++) {
			if (col[i][j] != newcol[i][j])
				result = 1;
		}
	}
	return result;
}