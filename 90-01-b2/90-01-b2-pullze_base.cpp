/* 2353626 贺胡鸣 大数据 */
#include "90-01-b2-pullze.h"
using namespace std;

/***************************************************************************
  函数名称：input
  功    能：输入区域大小
  输入参数：int* size
  返 回 值：/
  说    明：/
***************************************************************************/
void input(int* psize)
{
	cct_cls();
	while (1) {
		cout << "请输入区域大小(5/10/15) ：";
		cin >> *psize;
		if (cin.good() == 0 || (*psize != 5 && *psize != 10 && *psize != 15)) {
			cin.clear();
			cin.ignore(1024, '\n');
			continue;
		}
		else
			break;
	}
}

/***************************************************************************
  函数名称：create_ball
  功    能：随机生成球的位置,存放在一个数组ball中
  输入参数：int size,char ball[maxsize][maxsize]
  返 回 值：/
  说    明：/
***************************************************************************/
void create_ball(int size, char ball[maxsize][maxsize])
{
	srand((unsigned int)(time(0)));
	int freq = 0;
	if (size == 5)
		freq = 13;
	else if (size == 10)
		freq = 50;
	else
		freq = 113;
	for (int i = 0; i < freq; i++) {
		int a = rand() % size;
		int b = rand() % size;
		if (ball[a][b] == 0)
			ball[a][b] = 'O';
		else
			i--;
	}
}

/***************************************************************************
  函数名称：print_ball
  功    能：按照格式在不同位置打印表格
  输入参数：int size,char ball[maxsize][maxsize],int x,int y,char choice
  返 回 值：/
  说    明：/
***************************************************************************/
void print_ball(int size, char ball[maxsize][maxsize], int x, int y, char choice)
{
	cct_gotoxy(x, y);
	cout << "+";
	int n = size / 5;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < 11; j++)
			cout << "-";
		cout << "+";
	}
	cct_gotoxy(x, y + 1);
	cout << "| ";
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < 5; j++)
			cout << (char)('a' + i * 5 + j) << ' ';
		cout << "| ";
	}
	cct_gotoxy(x - 2, y + 2);
	cout << "+-+";
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < 11; j++)
			cout << "-";
		cout << "+";
	}
	cout << endl;
	int arr = 1;
	for (int i = 0; i < size; i++) {
		cct_gotoxy(x - 2, y + 2 + arr);
		cout << "|" << (char)('A' + i) << "| ";
		for (int j = 0; j < size; j++) {
			if (ball[i][j] != 0 && choice != 'C')
				cout << ball[i][j] << ' ';
			else
				cout << "  ";
			if ((j + 1) % 5 == 0)
				cout << "| ";
		}
		cout << endl;
		arr++;
		if ((i + 1) % 5 == 0) {
			cct_gotoxy(x - 2, y + 2 + arr);
			cout << "+-+";
			for (int i = 0; i < n; i++) {
				for (int j = 0; j < 11; j++)
					cout << "-";
				cout << "+";
			}
			arr++;
			cout << endl;
		}
	}
	cct_gotoxy(x - 2, y + 2 + arr);
}

/***************************************************************************
  函数名称：count_ball
  功    能：数每行每列球的个数及连续性
  输入参数：int size,char ball[maxsize][maxsize], int row[maxsize][8],int col[8][maxsize]
  返 回 值：/
  说    明：/
***************************************************************************/
void count_ball(int size, char ball[maxsize][maxsize], int row[maxsize][8], int col[8][maxsize])
{
	//行
	for (int i = 0; i < size; i++) {
		int count = 0;
		int index = 0;
		for (int j = 0; j < size; j++) {
			if (ball[i][j] == 'O')
				count++;
			else {
				if (count > 0) {
					row[i][index++] = count;
					count = 0;
				}
			}
		}
		if (count > 0)
			row[i][index] = count;
	}
	//列
	for (int j = 0; j < size; j++) {
		int count = 0;
		int index = 0;
		for (int i = 0; i < size; i++) {
			if (ball[i][j] == 'O')
				count++;
			else {
				if (count > 0) {
					col[index++][j] = count;
					count = 0;
				}
			}
		}
		if (count > 0)
			col[index][j] = count;
	}
}

/***************************************************************************
  函数名称：print_row
  功    能：打印每行球的个数及连续性
  输入参数：int size,int width,int row[maxsize][8],int x,int y
  返 回 值：/
  说    明：/
***************************************************************************/
void print_row(int size, int width, int row[maxsize][8], int x, int y)
{
	cct_showch(x, y, '-', COLOR_BLACK, COLOR_WHITE, 2 * width + 1);
	int arr = 1;
	for (int i = 0; i < size; i++) {
		int row_len = 0;
		for (int j = 0; j < 8; j++)
			if (row[i][j] != 0)
				row_len++;
		cct_gotoxy(x, y + arr);
		for (int k = 0; k < 2 * (width - row_len) + 1; k++)
			cout << " ";
		for (int j = 0; row[i][j] != 0; j++)
			cout << row[i][j] << " ";
		arr++;
		if ((i + 1) % 5 == 0) {
			cct_showch(x, y + arr, '-', COLOR_BLACK, COLOR_WHITE, 2 * width + 1);
			arr++;
		}
	}
	cct_showch(x, y - 2, '-', COLOR_BLACK, COLOR_WHITE, 2 * width + 3);
}

/***************************************************************************
  函数名称：print_col
  功    能：打印每列球的个数及连续性
  输入参数：int size,int height,int col[8][maxsize],int x,int y
  返 回 值：/
  说    明：/
***************************************************************************/
void print_col(int size, int height, int col[8][maxsize], int x, int y)
{
	cct_gotoxy(x, y);
	cout << "+";
	int n = size / 5;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < 11; j++)
			cout << "-";
		cout << "+";
	}

	for (int j = 0; j < size; ++j) {
		int zeroCount = 0;
		for (int i = 0; i < height; ++i) {
			if (col[i][j] == 0) {
				zeroCount++;
			}
		}
		int temp[8];
		int tempIndex = 0;
		for (int i = 0; i < height; ++i) {
			if (col[i][j] != 0) {
				temp[tempIndex++] = col[i][j];
			}
		}
		for (int i = 0; i < zeroCount; ++i) {
			col[i][j] = 0;
		}
		for (int i = zeroCount; i < height; ++i) {
			col[i][j] = temp[i - zeroCount];
		}
	}

	int arr = 1;
	for (int i = 0; i < height; i++) {
		cct_gotoxy(x, y + i + 1);
		cout << "| ";
		for (int j = 0; j < size; j++) {
			if (col[i][j] == 0)
				cout << "  ";
			else
				cout << col[i][j] << " ";
			if (arr % 5 == 0) {
				cout << "| ";
				arr++;
			}
			else
				arr++;
		}
	}
}

/***************************************************************************
  函数名称：print_count
  功    能：打印每行每列球的个数及连续性
  输入参数：int size,int row[maxsize][8],int col[8][maxsize],int x,int y,char choice
  返 回 值：/
  说    明：/
***************************************************************************/
void print_count(int size, char ball[maxsize][maxsize], int row[maxsize][8], int col[8][maxsize], int x, int y, char choice)
{
	int width = 0, height = 0;
	restrict(size, row, col, &width, &height);
	cct_showch(x, y, '-', COLOR_BLACK, COLOR_WHITE, 2 * width + 3);
	print_row(size, width, row, x, y + height + 3);
	print_col(size, height, col, x + 2 * width + 3, y);
	print_ball(size, ball, x + 2 * width + 3, y + height + 1, choice);
}

/***************************************************************************
  函数名称：choiceA
  功    能：完成选项A的操作
  输入参数：/
  返 回 值：/
  说    明：/
***************************************************************************/
void choiceA()
{
	int size;
	input(&size);
	cout << "初始数组：" << endl;
	char ball[maxsize][maxsize] = { 0 };
	create_ball(size, ball);
	cout << "+-";
	int x, y;
	cct_getxy(x, y);
	cct_gotoxy(x - 2, y + 1);
	cout << "|";
	print_ball(size, ball, x, y, 'A');
	cout << endl;
	end();
}

/***************************************************************************
  函数名称：choiceB
  功    能：完成选项B的操作
  输入参数：char choice
  返 回 值：/
  说    明：加入参数choice方便C直接调用B
***************************************************************************/
void choiceB(char choice)
{
	int size;
	input(&size);
	cout << "初始数组：" << endl;
	char ball[maxsize][maxsize] = { 0 };
	create_ball(size, ball);
	int row[maxsize][8] = { 0 }, col[8][maxsize] = { 0 };
	count_ball(size, ball, row, col);
	if (choice == 'C') {
		cct_setconsoleborder(189, 55, 189, 2000);
		int x, y;
		cct_getxy(x, y);
		print_count(size, ball, row, col, x, y, choice);
		choiceC(size, ball, row, col, choice);
	}
	else {
		int x, y;
		cct_getxy(x, y);
		print_count(size, ball, row, col, x, y, choice);
		cout << endl;
	}
}

/***************************************************************************
  函数名称：choiceC
  功    能：完成选项C的操作
  输入参数：int size,char ball[maxsize][maxsize],int row[maxsize][8],int col[8][maxsize],char choice
  返 回 值：/
  说    明：/
***************************************************************************/
void choiceC(int size, char ball[maxsize][maxsize], int row[maxsize][8], int col[8][maxsize], char choice)
{
	char newball[maxsize][maxsize] = { 0 };//存放所有球
	int cheat = 0;//表示是否打开了作弊模式，1开0闭
	cin.clear();
	cin.ignore(1024, '\n');
	while (1) {
		prompt();
		int x, y;
		cct_getxy(x, y);
		char game[200] = { 0 };
		cin.getline(game, 200);
		if (cin.fail() == 1) {
			return;
		}
		else if (judge(size, game, newball) == 0) {
			cout << "输入错误" << endl;
			continue;
		}
		else if (judge(size, game, newball) == 1) {
			for (int i = 0; i < size; i++) {
				for (int j = 0; j < size; j++) {
					if (i == (int)(game[0] - 'A') && j == (int)(game[1] - 'a'))
						newball[i][j] = 'O';
				}
			}
			cout << "输入" << game[0] << game[1] << "后：" << endl;
			int x = 0, y = 0;
			cct_getxy(x, y);
			print_count(size, ball, row, col, x, y, choice);
			int width = 0, height = 0;
			restrict(size, row, col, &width, &height);
			if (cheat == 0) {
				for (int i = 0; i < size; i++) {
					for (int j = 0; j < size; j++) {
						int pos_x = x + 2 * width + 2 * j + 5 + 2 * (j / 5);
						int pos_y = y + height + 4 + i + i / 5;
						if (newball[i][j] != 0)
							cct_showch(pos_x, pos_y, newball[i][j], COLOR_HYELLOW, COLOR_HBLUE, 1);
						else
							cct_showch(pos_x, pos_y, ' ', COLOR_BLACK, COLOR_BLACK, 1);
					}
				}
			}
			else {
				for (int i = 0; i < size; i++) {
					for (int j = 0; j < size; j++) {
						int pos_x = x + 2 * width + 2 * j + 5 + 2 * (j / 5);
						int pos_y = y + height + 4 + i + i / 5;
						if (newball[i][j] == ball[i][j] && ball[i][j] != 0)
							cct_showch(pos_x, pos_y, newball[i][j], COLOR_HYELLOW, COLOR_HBLUE, 1);
						else if (newball[i][j] == 0 && ball[i][j] != 0)
							cct_showch(pos_x, pos_y, ball[i][j], COLOR_BLACK, COLOR_HWHITE, 1);
						else if (newball[i][j] != 0 && ball[i][j] == 0)
							cct_showch(pos_x, pos_y, 'X', COLOR_HYELLOW, COLOR_HBLUE, 1);
						else if (newball[i][j] == 0 && ball[i][j] == 0)
							cct_showch(pos_x, pos_y, ' ', COLOR_BLACK, COLOR_HWHITE, 1);
					}
				}
			}
			cct_setcolor(COLOR_BLACK, COLOR_HWHITE);
			cct_gotoxy(x, y + height + size + 7);
		}

		else if (judge(size, game, newball) == 3) {
			for (int i = 0; i < size; i++) {
				for (int j = 0; j < size; j++) {
					if (newball[i][j] != 0 && game[0] == (char)(i + 'A') && game[1] == (char)(j + 'a')) {
						newball[i][j] = 0;
					}
				}
			}
			cout << "输入" << game[0] << game[1] << "后：" << endl;
			int x = 0, y = 0;
			cct_getxy(x, y);
			print_count(size, ball, row, col, x, y, choice);
			int width = 0, height = 0;
			restrict(size, row, col, &width, &height);
			if (cheat == 0) {
				for (int i = 0; i < size; i++) {
					for (int j = 0; j < size; j++) {
						int pos_x = x + 2 * width + 2 * j + 5 + 2 * (j / 5);
						int pos_y = y + height + 4 + i + i / 5;
						if (newball[i][j] != 0)
							cct_showch(pos_x, pos_y, newball[i][j], COLOR_HYELLOW, COLOR_HBLUE, 1);
						else
							cct_showch(pos_x, pos_y, ' ', COLOR_BLACK, COLOR_BLACK, 1);
					}
				}
			}
			else {
				for (int i = 0; i < size; i++) {
					for (int j = 0; j < size; j++) {
						int pos_x = x + 2 * width + 2 * j + 5 + 2 * (j / 5);
						int pos_y = y + height + 4 + i + i / 5;
						if (newball[i][j] == ball[i][j] && ball[i][j] != 0)
							cct_showch(pos_x, pos_y, newball[i][j], COLOR_HYELLOW, COLOR_HBLUE, 1);
						else if (newball[i][j] == 0 && ball[i][j] != 0)
							cct_showch(pos_x, pos_y, ball[i][j], COLOR_BLACK, COLOR_HWHITE, 1);
						else if (newball[i][j] != 0 && ball[i][j] == 0)
							cct_showch(pos_x, pos_y, 'X', COLOR_HYELLOW, COLOR_HBLUE, 1);
						else if (newball[i][j] == 0 && ball[i][j] == 0)
							cct_showch(pos_x, pos_y, ' ', COLOR_BLACK, COLOR_HWHITE, 1);
					}
				}
			}
			cct_setcolor(COLOR_BLACK, COLOR_HWHITE);
			cct_gotoxy(x, y + height + size + 7);
		}

		else if (judge(size, game, newball) == 2) {
			if (game[0] == 'X' || game[0] == 'x')
				return;
			else if (game[0] == 'Y' || game[0] == 'y') {
				int success = examine(size, newball, ball);
				if (success == 0) {
					cout << "提交成功，游戏结束" << endl;
					break;
				}
				else
					cout << "提交错误, 可用作弊模式查看" << endl;
			}
			else if (game[0] == 'Z' || game[0] == 'z') {
				cheat = (cheat == 0 ? 1 : 0);
				cout << "输入Z/z后：" << endl;
				int x = 0, y = 0;
				cct_getxy(x, y);
				print_count(size, ball, row, col, x, y, choice);
				int width = 0, height = 0;
				restrict(size, row, col, &width, &height);
				if (cheat == 1) {
					for (int i = 0; i < size; i++) {
						for (int j = 0; j < size; j++) {
							int pos_x = x + 2 * width + 2 * j + 5 + 2 * (j / 5);
							int pos_y = y + height + 4 + i + i / 5;
							if (newball[i][j] == ball[i][j] && ball[i][j] != 0)
								cct_showch(pos_x, pos_y, newball[i][j], COLOR_HYELLOW, COLOR_HBLUE, 1);
							else if (newball[i][j] == 0 && ball[i][j] != 0)
								cct_showch(pos_x, pos_y, ball[i][j], COLOR_BLACK, COLOR_HWHITE, 1);
							else if (newball[i][j] != 0 && ball[i][j] == 0)
								cct_showch(pos_x, pos_y, 'X', COLOR_HYELLOW, COLOR_HBLUE, 1);
							else if (newball[i][j] == 0 && ball[i][j] == 0)
								cct_showch(pos_x, pos_y, ' ', COLOR_BLACK, COLOR_HWHITE, 1);
						}
					}
				}
				else {
					for (int i = 0; i < size; i++) {
						for (int j = 0; j < size; j++) {
							int pos_x = x + 2 * width + 2 * j + 5 + 2 * (j / 5);
							int pos_y = y + height + 4 + i + i / 5;
							if (newball[i][j] != 0) {
								cct_showch(pos_x, pos_y, newball[i][j], COLOR_HYELLOW, COLOR_HBLUE, 1);
							}
							else
								cct_showch(pos_x, pos_y, ' ', COLOR_BLACK, COLOR_BLACK, 1);
						}
					}
				}
				cct_setcolor(COLOR_BLACK, COLOR_HWHITE);
				cct_gotoxy(x, y + height + size + 7);
			}
		}

		else
			continue;
	}
	cout << endl;
	end();
}