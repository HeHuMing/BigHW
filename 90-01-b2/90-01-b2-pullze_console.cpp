/* 2353626 贺胡鸣 大数据 */
#include "90-01-b2-pullze.h"
using namespace std;

/***************************************************************************
  函数名称：draw_framework
  功    能：用伪图形画出底面框架
  输入参数：int size,int x,int y,char choice
  返 回 值：/
  说    明：/
***************************************************************************/
void draw_framework(int size, int x, int y, char choice)
{
	cct_setcursor(CURSOR_INVISIBLE);
	int flag = 1;
	if (choice == 'D' || choice == 'E' || choice == 'F' || choice == 'G')
		flag = 0;
	int step = (flag == 0 ? 2 : 8);
	cct_showstr(x, y, " ", COLOR_BLACK, COLOR_BLACK, 2 * (flag + 1));
	for (int i = 0; i < size; i++) {
		cct_showch(x + 2 * (flag + 1) + i * step, y, (char)('a' + i), COLOR_BLACK, COLOR_HWHITE, 1);
		cct_showch(x + 2 * (flag + 1) + i * step + 1, y, ' ', COLOR_BLACK, COLOR_BLACK, step - 1);
	}
	cct_showstr(x, y + 1, "╔", COLOR_HWHITE, COLOR_BLACK, 1);
	if (flag == 0) {
		cct_showstr(x + 2, y + 1, "═", COLOR_HWHITE, COLOR_BLACK, size);
		cct_showstr(x + 2 + step * size, y + 1, "╗", COLOR_HWHITE, COLOR_BLACK, 1);
	}
	else {
		cct_showstr(x + 2, y + 1, "═══╦", COLOR_HWHITE, COLOR_BLACK, size - 1);
		cct_showstr(x + 2 + step * (size - 1), y + 1, "═══╗", COLOR_HWHITE, COLOR_BLACK, 1);
	}
	if (flag == 0) {
		for (int i = 0; i < size; i++) {
			cct_showch(x - 2, y + 2 + i, (char)('A' + i), COLOR_BLACK, COLOR_HWHITE, 1);
			cct_showch(x - 1, y + 2 + i, ' ', COLOR_BLACK, COLOR_HWHITE, 1);
			cct_showstr(x, y + i + 2, "║", COLOR_HWHITE, COLOR_BLACK, 1);
			time_sleep(choice);
			for (int t = 0; t < 2 * size; t++) {
				cct_showch(x + 2 + t, y + i + 2, ' ', COLOR_HWHITE, COLOR_HWHITE, 1);
				time_sleep(choice);
			}
			cct_showstr(x + 2 + 2 * size, y + i + 2, "║", COLOR_HWHITE, COLOR_BLACK, 1);
		}
		cct_showstr(x, y + 2 + size, "╚", COLOR_HWHITE, COLOR_BLACK, 1);
		cct_showstr(x + 2, y + 2 + size, "═", COLOR_HWHITE, COLOR_BLACK, size);
		cct_showstr(x + 2 + 2 * size, y + 2 + size, "╝", COLOR_HWHITE, COLOR_BLACK, 1);
	}
	else {
		for (int i = 0; i < size; i++) {
			for (int ii = 0; ii < 4; ii++) {
				if (ii == 1) {
					cct_showch(x - 2, y + i * step / 2 + ii + 2, (char)('A' + i), COLOR_BLACK, COLOR_HWHITE, 1);
					cct_showch(x - 1, y + i * step / 2 + ii + 2, ' ', COLOR_BLACK, COLOR_HWHITE, 1);
					cct_showstr(x, y + i * step / 2 + ii + 2, "║", COLOR_HWHITE, COLOR_BLACK, 1);
				}
				else if (ii == 3) {
					cct_showch(x - 2, y + i * step / 2 + ii + 2, ' ', COLOR_BLACK, COLOR_HWHITE, 2);
					cct_showstr(x, y + i * step / 2 + ii + 2, (i == size - 1 ? "╚" : "╠"), COLOR_HWHITE, COLOR_BLACK, 1);
				}
				else {
					cct_showch(x - 2, y + i * step / 2 + ii + 2, ' ', COLOR_BLACK, COLOR_HWHITE, 2);
					cct_showstr(x, y + i * step / 2 + ii + 2, "║", COLOR_HWHITE, COLOR_BLACK, 1);
				}
				if (ii == 3 && i != size - 1) {
					for (int j = 0; j < size; j++) {
						for (int jj = 0; jj < 4; jj++) {
							if (jj == 3)
								cct_showstr(x + j * step + jj * 2 + 2, y + i * step / 2 + ii + 2, (j == size - 1 ? "╣" : "╬"), COLOR_HWHITE, COLOR_BLACK, 1);
							else
								cct_showstr(x + j * step + jj * 2 + 2, y + i * step / 2 + ii + 2, "═", COLOR_HWHITE, COLOR_BLACK, 1);
							time_sleep(choice);
						}
						time_sleep(choice);
					}
				}
				else if (ii == 3 && i == size - 1) {
					for (int j = 0; j < size; j++) {
						for (int jj = 0; jj < 4; jj++) {
							if (jj == 3)
								cct_showstr(x + j * step + jj * 2 + 2, y + i * step / 2 + ii + 2, (j == size - 1 ? "╝" : "╩"), COLOR_HWHITE, COLOR_BLACK, 1);
							else
								cct_showstr(x + j * step + jj * 2 + 2, y + i * step / 2 + ii + 2, "═", COLOR_HWHITE, COLOR_BLACK, 1);
							time_sleep(choice);
						}
						time_sleep(choice);
					}
				}
				else {
					for (int j = 0; j < size; j++) {
						for (int jj = 0; jj < 4; jj++) {
							if (jj == 3)
								cct_showstr(x + j * step + jj * 2 + 2, y + i * step / 2 + ii + 2, "║", COLOR_HWHITE, COLOR_BLACK, 1);
							else
								cct_showstr(x + j * step + jj * 2 + 2, y + i * step / 2 + ii + 2, "  ", COLOR_HWHITE, COLOR_BLACK, 1);
							time_sleep(choice);
						}
						time_sleep(choice);
					}
				}
			}
		}
	}
	cout << endl;
	cct_setcolor(COLOR_BLACK, COLOR_HWHITE);
}

/***************************************************************************
  函数名称：draw_ball
  功    能：用伪图形画出每个球的位置
  输入参数：int size,char ball[maxsize][maxsize],int x,int y,char choice
  返 回 值：/
  说    明：/
***************************************************************************/
void draw_ball(int size, char ball[maxsize][maxsize], int x, int y, char choice)
{
	cct_setcursor(CURSOR_INVISIBLE);
	int flag = 1;
	if (choice == 'D' || choice == 'E' || choice == 'F')
		flag = 0;
	if (flag == 0) {
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				if (ball[i][j] != 0)
					cct_showstr(x + 2 * j, y + i, "〇", COLOR_HBLUE, COLOR_BLACK, 1);
				time_sleep(choice);
			}
		}
		cct_gotoxy(0, y + size + 1);
	}
	else if (choice == 'G') {
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				if (ball[i][j] != 0)
					cct_showstr(x + 2 * j, y + i, "  ", COLOR_HWHITE, COLOR_HWHITE, 1);
				time_sleep(choice);
			}
		}
		cct_gotoxy(0, y + size + 1);
	}
	else if (choice == 'K') {
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				if (ball[i][j] != 0) {
					time_sleep(choice);
					cct_showstr(x + 8 * j, y + 4 * i, "      ", COLOR_HWHITE, COLOR_BLACK, 1);
					cct_showstr(x + 8 * j, y + 4 * i + 1, "      ", COLOR_HWHITE, COLOR_BLACK, 1);
					cct_showstr(x + 8 * j, y + 4 * i + 2, "      ", COLOR_HWHITE, COLOR_BLACK, 1);
				}
			}
		}
		cct_gotoxy(0, y + 4 * size);
	}
	else {
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				if (ball[i][j] != 0) {
					time_sleep(choice);
					cct_showstr(x + 8 * j, y + 4 * i, "╔═╗", COLOR_HBLUE, COLOR_BLACK, 1);
					cct_showstr(x + 8 * j, y + 4 * i + 1, "║〇║", COLOR_HBLUE, COLOR_BLACK, 1);
					cct_showstr(x + 8 * j, y + 4 * i + 2, "╚═╝", COLOR_HBLUE, COLOR_BLACK, 1);
				}
			}
		}
		cct_gotoxy(0, y + 4 * size);
	}
	cct_setcolor(COLOR_BLACK, COLOR_HWHITE);
}

/***************************************************************************
  函数名称：choiceD
  功    能：完成选项D的操作
  输入参数：/
  返 回 值：/
  说    明：/
***************************************************************************/
void choiceD()
{
	int size;
	input(&size);
	cct_cls();
	char ball[maxsize][maxsize] = { 0 };
	create_ball(size, ball);
	cct_setconsoleborder(180, 180, 180, 180);
	cct_setfontsize("新宋体", 32);
	draw_framework(size, 2, 1, 'D');
	draw_ball(size, ball, 4, 3, 'D');
}

/***************************************************************************
  函数名称：draw_col
  功    能：画f中的表格中的列
  输入参数：size,col, width, height,choice
  返 回 值：/
  说    明：/
***************************************************************************/
void draw_col(int size, int col[8][maxsize], int width, int height, char choice)
{
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
	int x = 2 * width + 6, y = 1;
	cct_gotoxy(x, y);
	if (choice == 'E' || choice == 'F' || choice == 'G') {
		cct_showstr(x, y, "╔", COLOR_HWHITE, COLOR_BLACK, 1);
		cct_showstr(x + 2, y, "═", COLOR_HWHITE, COLOR_BLACK, size);
		cct_showstr(x + 2 + 2 * size, y, "╗", COLOR_HWHITE, COLOR_BLACK, 1);
		for (int i = 0; i < height; i++) {
			cct_showstr(x, y + i + 1, "║", COLOR_HWHITE, COLOR_BLACK, 1);
			for (int j = 0; j < size; j++) {
				if (col[i][j] == 0)
					cct_showstr(x + 2 + 2 * j, y + i + 1, "  ", COLOR_HWHITE, COLOR_HWHITE, 1);
				else {
					cct_showstr(x + 2 + 2 * j, y + i + 1, " ", COLOR_HWHITE, COLOR_HWHITE, 1);
					cct_showint(x + 2 + 2 * j + 1, y + i + 1, col[i][j], COLOR_HWHITE, COLOR_BLACK, 1);
				}
				cct_showstr(x + 2 + 2 * size, y + i + 1, "║", COLOR_HWHITE, COLOR_BLACK, 1);
			}
		}
		cct_showstr(x, y + height + 1, "╠", COLOR_HWHITE, COLOR_BLACK, 1);
		cct_showstr(x + 2, y + height + 1, "═", COLOR_HWHITE, COLOR_BLACK, size);
		cct_showstr(x + 2 + 2 * size, y + height + 1, "╣", COLOR_HWHITE, COLOR_BLACK, 1);
		cct_showstr(x, y + height + 2, "║", COLOR_HWHITE, COLOR_BLACK, 1);
		cct_showstr(x + 2 + 2 * size, y + height + 2, "║", COLOR_HWHITE, COLOR_BLACK, 1);
		cct_showstr(x + 2 + 2 * size, y + height + 3, "╣", COLOR_HWHITE, COLOR_BLACK, 1);
		for (int i = 0; i < size; i++) {
			cct_showch(x + 2 + 2 * i, y + height + 2, ' ', COLOR_HWHITE, COLOR_BLACK, 1);
			cct_showch(x + 2 + 2 * i + 1, y + height + 2, (char)('a' + i), COLOR_HWHITE, COLOR_BLACK, 1);
		}
	}
	else if (choice == 'I' || choice == 'J' || choice == 'K') {
		cct_showstr(x, y, "╔", COLOR_HWHITE, COLOR_BLACK, 1);
		cct_showstr(x + 2, y, "═", COLOR_HWHITE, COLOR_BLACK, 4 * (size - 1));
		cct_showstr(x + 2 + 8 * (size - 1), y, "═══╗", COLOR_HWHITE, COLOR_BLACK, 1);
		for (int i = 0; i < height; i++) {
			cct_showstr(x, y + i + 1, "║", COLOR_HWHITE, COLOR_BLACK, 1);
			cct_showstr(x + 2, y + i + 1, " ", COLOR_HWHITE, COLOR_HWHITE, 8 * size - 2);
			cct_showstr(x + 8 * size, y + i + 1, "║", COLOR_HWHITE, COLOR_BLACK, 1);
		}
		for (int i = 0; i < height; i++)
			for (int j = 0; j < size; j++)
				if (col[i][j] != 0)
					cct_showint(x + 4 + 8 * j, y + 1 + i, col[i][j], COLOR_HWHITE, COLOR_BLACK, 1);
		cct_showstr(x, y + height + 1, "╠", COLOR_HWHITE, COLOR_BLACK, 1);
		cct_showstr(x + 2, y + height + 1, "═", COLOR_HWHITE, COLOR_BLACK, 4 * size - 1);
		cct_showstr(x + 8 * size, y + height + 1, "╣", COLOR_HWHITE, COLOR_BLACK, 1);
		cct_showstr(x, y + height + 2, "║", COLOR_HWHITE, COLOR_BLACK, 1);
		cct_showstr(x + 2, y + height + 2, " ", COLOR_HWHITE, COLOR_HWHITE, 8 * size - 2);
		cct_showstr(x + 8 * size, y + height + 2, "║", COLOR_HWHITE, COLOR_BLACK, 1);
		cct_showstr(x + 8 * size, y + height + 3, "╣", COLOR_HWHITE, COLOR_BLACK, 1);
		for (int i = 0; i < size; i++)
			cct_showch(x + 4 + 8 * i, y + height + 2, (char)('a' + i), COLOR_HWHITE, COLOR_BLACK, 1);
	}
}

/***************************************************************************
  函数名称：draw_row
  功    能：画f中的表格中的行
  输入参数：size, row, width, height,choice
  返 回 值：/
  说    明：/
***************************************************************************/
void draw_row(int size, int row[maxsize][8], int width, int height, char choice)
{
	int x = 0, y = height + 4;
	cct_gotoxy(x, y);
	cct_showstr(x, y, "╔", COLOR_HWHITE, COLOR_BLACK, 1);
	cct_showstr(x + 2, y, "═", COLOR_HWHITE, COLOR_BLACK, width);
	cct_showstr(x + 2 + 2 * width, y, "╦═╬", COLOR_HWHITE, COLOR_BLACK, 1);
	if (choice == 'E' || choice == 'F' || choice == 'G') {
		for (int i = 0; i < size; i++) {
			cct_showstr(x, y + i + 1, "║", COLOR_HWHITE, COLOR_BLACK, 1);

			int row_len = 0;
			for (int j = 0; j < 8; j++)
				if (row[i][j] != 0)
					row_len++;
			cct_showstr(x + 2, y + i + 1, " ", COLOR_HWHITE, COLOR_BLACK, 2 * (width - row_len));
			for (int j = 0; row[i][j] != 0; j++) {
				cct_showint(x + 2 + 2 * (width - row_len) + 2 * j, y + i + 1, row[i][j], COLOR_HWHITE, COLOR_BLACK, 1);
				cct_showch(x + 2 + 2 * (width - row_len) + 2 * j + 1, y + 1 + i, ' ', COLOR_HWHITE, COLOR_HWHITE, 1);
			}
			cct_showstr(x + 2 + 2 * width, y + i + 1, "║", COLOR_HWHITE, COLOR_BLACK, 1);
			cct_showch(x + 4 + 2 * width, y + i + 1, ' ', COLOR_HWHITE, COLOR_BLACK, 1);
			cct_showch(x + 5 + 2 * width, y + i + 1, (char)('A' + i), COLOR_HWHITE, COLOR_BLACK, 1);
		}
		cct_showstr(x, y + size + 1, "╚", COLOR_HWHITE, COLOR_BLACK, 1);
		cct_showstr(x + 2, y + size + 1, "═", COLOR_HWHITE, COLOR_BLACK, width);
		cct_showstr(x + 2 + 2 * width, y + size + 1, "╩═╩", COLOR_HWHITE, COLOR_BLACK, 1);
	}
	else {
		for (int i = 0; i < size; i++) {
			for (int k = 0; k < 4; k++) {
				cct_showstr(x, y + 4 * i + k + 1, "║", COLOR_HWHITE, COLOR_BLACK, 1);
				cct_showstr(x + 2, y + 4 * i + k + 1, " ", COLOR_HWHITE, COLOR_HWHITE, 2 * width);
				cct_showstr(x + 2 + 2 * width, y + 4 * i + k + 1, "║", COLOR_HWHITE, COLOR_BLACK, 1);
				cct_showstr(x + 4 + 2 * width, y + 4 * i + k + 1, " ", COLOR_HWHITE, COLOR_HWHITE, 2);
			}
		}
		cct_showstr(x, y + 4 * size, "╚", COLOR_HWHITE, COLOR_BLACK, 1);
		cct_showstr(x + 2, y + 4 * size, "═", COLOR_HWHITE, COLOR_BLACK, width);
		cct_showstr(x + 2 + 2 * width, y + 4 * size, "╩═╩", COLOR_HWHITE, COLOR_BLACK, 1);
		for (int i = 0; i < size; i++) {
			int row_len = 0;
			for (int j = 0; j < 8; j++)
				if (row[i][j] != 0)
					row_len++;
			cct_showstr(x + 2, y + 4 * i + 2, " ", COLOR_HWHITE, COLOR_HWHITE, 2 * (width - row_len));
			for (int j = 0; row[i][j] != 0; j++) {
				cct_showint(x + 2 + 2 * (width - row_len) + 2 * j, y + 4 * i + 2, row[i][j], COLOR_HWHITE, COLOR_BLACK, 1);
				cct_showch(x + 2 + 2 * (width - row_len) + 2 * j + 1, y + 4 * i + 2, ' ', COLOR_HWHITE, COLOR_HWHITE, 1);
			}
			cct_showstr(x + 2 + 2 * width, y + 4 * i + 2, "║", COLOR_HWHITE, COLOR_BLACK, 1);
			cct_showch(x + 4 + 2 * width, y + 4 * i + 2, ' ', COLOR_HWHITE, COLOR_BLACK, 1);
			cct_showch(x + 5 + 2 * width, y + 4 * i + 2, (char)('A' + i), COLOR_HWHITE, COLOR_BLACK, 1);
		}
	}
}


/***************************************************************************
  函数名称：draw_table
  功    能：画f中的表格
  输入参数：size, row, col, width, height,choice
  返 回 值：/
  说    明：/
***************************************************************************/
void draw_table(int size, char ball[maxsize][maxsize], int row[maxsize][8], int col[8][maxsize], int width, int height, char choice)
{
	draw_framework(size, 2 * width + 6, height + 3, choice);
	draw_ball(size, ball, 2 * width + 8, height + 5, choice);
	draw_col(size, col, width, height, choice);
	draw_row(size, row, width, height, choice);
	cct_gotoxy(0, height + (choice == 'E' ? 1 : 4) * size + 6);
	cct_setcolor(COLOR_BLACK, COLOR_HWHITE);
}

/***************************************************************************
  函数名称：display
  功    能：g,k中的游戏画图函数
  输入参数：int size,char ball[maxsize][maxsize],int width,int height,int cheat, int leftright[maxsize][maxsize], int show[maxsize][maxsize], char newball[maxsize][maxsize],char choice
  返 回 值：/
  说    明：
***************************************************************************/
void display(int size, char ball[maxsize][maxsize], int width, int height, int cheat, int leftright[maxsize][maxsize], int show[maxsize][maxsize], char newball[maxsize][maxsize], char choice)
{
	if (choice == 'G') {
		if (cheat == 0) {
			for (int i = 0; i < size; i++) {
				for (int j = 0; j < size; j++) {
					if (newball[i][j] != 0) {
						if (show[i][j] == 1 && leftright[i][j] == 0 && newball[i][j] == 1)
							cct_showstr(2 * width + 2 * j + 8, height + 5 + i, "○", COLOR_HBLUE, COLOR_BLACK, 1);
						else if (show[i][j] == 1 && leftright[i][j] == 1 && newball[i][j] == 2)
							cct_showstr(2 * width + 2 * j + 8, height + 5 + i, "×", COLOR_HRED, COLOR_BLACK, 1);
					}
					else
						cct_showstr(2 * width + 2 * j + 8, height + 5 + i, "  ", COLOR_HWHITE, COLOR_HWHITE, 1);
				}
			}
		}
		else {
			for (int i = 0; i < size; i++) {
				for (int j = 0; j < size; j++) {
					if (ball[i][j] == 0 && newball[i][j] == 0)
						cct_showstr(2 * width + 2 * j + 8, height + 5 + i, "  ", COLOR_HWHITE, COLOR_HWHITE, 1);
					else if (ball[i][j] != 0 && newball[i][j] == 0)
						cct_showstr(2 * width + 2 * j + 8, height + 5 + i, "○", COLOR_WHITE, COLOR_BLACK, 1);
					else if (ball[i][j] == 0 && newball[i][j] == 1)
						cct_showstr(2 * width + 2 * j + 8, height + 5 + i, "○", COLOR_HRED, COLOR_BLACK, 1);
					else if (ball[i][j] == 0 && newball[i][j] == 2)
						cct_showstr(2 * width + 2 * j + 8, height + 5 + i, "×", COLOR_HRED, COLOR_BLACK, 1);
					else if (ball[i][j] != 0 && newball[i][j] == 1)
						cct_showstr(2 * width + 2 * j + 8, height + 5 + i, "○", COLOR_HBLUE, COLOR_BLACK, 1);
					else if (ball[i][j] != 0 && newball[i][j] == 2)
						cct_showstr(2 * width + 2 * j + 8, height + 5 + i, "×", COLOR_HBLUE, COLOR_BLACK, 1);
				}
			}
		}
	}
	else if (choice == 'K') {
		if (cheat == 0) {
			for (int i = 0; i < size; i++) {
				for (int j = 0; j < size; j++) {
					if (newball[i][j] != 0) {
						if (show[i][j] == 1 && leftright[i][j] == 0 && newball[i][j] == 1) {
							cct_showstr(2 * width + 8 * j + 8, height + 4 * i + 5, "╔═╗", COLOR_HBLUE, COLOR_BLACK, 1);
							cct_showstr(2 * width + 8 * j + 8, height + 4 * i + 6, "║〇║", COLOR_HBLUE, COLOR_BLACK, 1);
							cct_showstr(2 * width + 8 * j + 8, height + 4 * i + 7, "╚═╝", COLOR_HBLUE, COLOR_BLACK, 1);
						}
						else if (show[i][j] == 1 && leftright[i][j] == 1 && newball[i][j] == 2) {
							cct_showstr(2 * width + 8 * j + 8, height + 4 * i + 5, "╔═╗", COLOR_HRED, COLOR_BLACK, 1);
							cct_showstr(2 * width + 8 * j + 8, height + 4 * i + 6, "║×║", COLOR_HRED, COLOR_BLACK, 1);
							cct_showstr(2 * width + 8 * j + 8, height + 4 * i + 7, "╚═╝", COLOR_HRED, COLOR_BLACK, 1);
						}
					}
					else {
						cct_showstr(2 * width + 8 * j + 8, height + 4 * i + 5, "      ", COLOR_HWHITE, COLOR_BLACK, 1);
						cct_showstr(2 * width + 8 * j + 8, height + 4 * i + 6, "      ", COLOR_HWHITE, COLOR_BLACK, 1);
						cct_showstr(2 * width + 8 * j + 8, height + 4 * i + 7, "      ", COLOR_HWHITE, COLOR_BLACK, 1);
					}
				}
			}
		}
		else {
			for (int i = 0; i < size; i++) {
				for (int j = 0; j < size; j++) {
					if (ball[i][j] == 0 && newball[i][j] == 0) {
						cct_showstr(2 * width + 8 * j + 8, height + 4 * i + 5, "      ", COLOR_HWHITE, COLOR_BLACK, 1);
						cct_showstr(2 * width + 8 * j + 8, height + i * 4 + 6, "      ", COLOR_HWHITE, COLOR_BLACK, 1);
						cct_showstr(2 * width + 8 * j + 8, height + i * 4 + 7, "      ", COLOR_HWHITE, COLOR_BLACK, 1);
					}
					else if (ball[i][j] != 0 && newball[i][j] == 0) {
						cct_showstr(2 * width + 8 * j + 8, height + 4 * i + 5, "╔═╗", COLOR_WHITE, COLOR_BLACK, 1);
						cct_showstr(2 * width + 8 * j + 8, height + i * 4 + 6, "║〇║", COLOR_WHITE, COLOR_BLACK, 1);
						cct_showstr(2 * width + 8 * j + 8, height + i * 4 + 7, "╚═╝", COLOR_WHITE, COLOR_BLACK, 1);
					}
					else if (ball[i][j] == 0 && newball[i][j] == 1) {
						cct_showstr(2 * width + 8 * j + 8, height + 4 * i + 5, "╔═╗", COLOR_HRED, COLOR_BLACK, 1);
						cct_showstr(2 * width + 8 * j + 8, height + i * 4 + 6, "║〇║", COLOR_HRED, COLOR_BLACK, 1);
						cct_showstr(2 * width + 8 * j + 8, height + i * 4 + 7, "╚═╝", COLOR_HRED, COLOR_BLACK, 1);
					}
					else if (ball[i][j] == 0 && newball[i][j] == 2) {
						cct_showstr(2 * width + 8 * j + 8, height + 4 * i + 5, "╔═╗", COLOR_HRED, COLOR_BLACK, 1);
						cct_showstr(2 * width + 8 * j + 8, height + i * 4 + 6, "║×║", COLOR_HRED, COLOR_BLACK, 1);
						cct_showstr(2 * width + 8 * j + 8, height + i * 4 + 7, "╚═╝", COLOR_HRED, COLOR_BLACK, 1);
					}
					else if (ball[i][j] != 0 && newball[i][j] == 1) {
						cct_showstr(2 * width + 8 * j + 8, height + 4 * i + 5, "╔═╗", COLOR_HBLUE, COLOR_BLACK, 1);
						cct_showstr(2 * width + 8 * j + 8, height + i * 4 + 6, "║〇║", COLOR_HBLUE, COLOR_BLACK, 1);
						cct_showstr(2 * width + 8 * j + 8, height + i * 4 + 7, "╚═╝", COLOR_HBLUE, COLOR_BLACK, 1);
					}
					else if (ball[i][j] != 0 && newball[i][j] == 2) {
						cct_showstr(2 * width + 8 * j + 8, height + 4 * i + 5, "╔═╗", COLOR_HBLUE, COLOR_BLACK, 1);
						cct_showstr(2 * width + 8 * j + 8, height + i * 4 + 6, "║×║", COLOR_HBLUE, COLOR_BLACK, 1);
						cct_showstr(2 * width + 8 * j + 8, height + i * 4 + 7, "╚═╝", COLOR_HBLUE, COLOR_BLACK, 1);
					}
				}
			}
		}

	}

	cct_setcolor(COLOR_BLACK, COLOR_HWHITE);
}



/***************************************************************************
  函数名称：mouse_and_keyboard
  功    能：读取鼠标和键盘的操作并显示
  输入参数：int size,int width,int height,char choice
  返 回 值：/
  说    明：/
***************************************************************************/
void mouse_and_keyboard(int size, char ball[maxsize][maxsize], int width, int height, char choice)
{
	int step = 0;
	if (choice == 'F' || choice == 'G')
		step = size;
	else
		step = 4 * size - 1;
	if (choice == 'G' || choice == 'K')
		cct_showstr(0, 0, "左键选○/右键选×,Y/y提交,Z/z作弊,Q/q结束", COLOR_BLACK, COLOR_HWHITE, 1);
	else
		cct_showstr(0, 0, "测试键盘/鼠标左键/右键，按回车退出", COLOR_BLACK, COLOR_HWHITE, 1);
	cct_gotoxy(0, height + step + 6);
	cct_enable_mouse();
	cct_setcursor(CURSOR_INVISIBLE);
	int X = 0, Y = 0;
	int ret, maction;
	int keycode1, keycode2;
	int loop = 1;
	int x_min = 8 + 2 * width, x_max = 8 + 2 * (width + step) - 1; //数据区x，y坐标的上下界
	int y_min = height + 5, y_max = height + step + 4;
	int cheat = 0;//用来表示是否打开了作弊模式，1开0闭
	int leftright[maxsize][maxsize] = { 0 };//用来记录每个格子目前是左键还是右键情况，0左1右
	int show[maxsize][maxsize] = { 0 };//用来记录每个格子目前是否需要显示颜色，0不显示1显示
	char newball[maxsize][maxsize] = { 0 };//存放所有球
	int click = 0;//用来记录是否左击或右击

	while (loop) {
		cct_setcursor(CURSOR_INVISIBLE);
		ret = cct_read_keyboard_and_mouse(X, Y, maction, keycode1, keycode2);
		if (ret == CCT_MOUSE_EVENT) {
			cct_gotoxy(2 * width + 6, step + height + 6);
			cout << "[当前光标] ";
			if (X < x_min || X > x_max || Y < y_min || Y > y_max) {
				cct_gotoxy(2 * width + 17, step + height + 6);
				cout << "位置非法";
			}
			else if ((choice == 'J' || choice == 'K') && (((X - x_min - 6) % 8 == 0) || ((X - x_min - 7) % 8 == 0) || ((Y - y_min - 3) % 4 == 0))) {   //分隔线的位置
				cct_gotoxy(2 * width + 17, step + height + 6);
				cout << "位置非法";
			}
			else { //合法位置
				if (choice == 'F' || choice == 'G') {
					int index_i = (X - (8 + 2 * width)) / 2;
					int index_j = Y - (height + 5);
					cct_gotoxy(2 * width + 17, step + height + 6);
					cout << (char)('A' + index_j) << "行" << (char)('a' + index_i) << "列              ";
				}
				else if (choice == 'J' || choice == 'K') {
					int index_i = (X - x_min) / 8;
					int index_j = (Y - y_min) / 4;
					cct_gotoxy(2 * width + 17, step + height + 6);
					cout << (char)('A' + index_j) << "行" << (char)('a' + index_i) << "列               ";
				}

				switch (maction) {  //X,Y
				case MOUSE_ONLY_MOVED:
					break;
				case MOUSE_LEFT_BUTTON_CLICK:			//按下左键
					cct_gotoxy(2 * width + 7, step + height + 6);
					cout << "读到左键";
					click = 1;
					if (choice == 'G' || choice == 'K') {
						int com_i, com_j;
						if (choice == 'G') {
							com_j = (X - (8 + 2 * width)) / 2;
							com_i = Y - (height + 5);
						}
						else {
							com_j = (X - x_min) / 8;
							com_i = (Y - y_min) / 4;
						}
						for (int i = 0; i < size; i++) {
							for (int j = 0; j < size; j++) {
								if (i == com_i && j == com_j) {
									if (show[i][j] == 0) {
										show[i][j] = 1;
										leftright[i][j] = 0;
										newball[i][j] = 1;
									}
									else if (show[i][j] == 1 && leftright[i][j] == 0) {
										show[i][j] = 0;
										newball[i][j] = 0;
									}
									else if (show[i][j] == 1 && leftright[i][j] == 1) {
										show[i][j] = 1;
										leftright[i][j] = 0;
										newball[i][j] = 1;
									}
								}
							}
						}
					}
					break;
				case MOUSE_RIGHT_BUTTON_CLICK:			//按下右键
					cct_gotoxy(2 * width + 7, step + height + 6);
					cout << "读到右键";
					click = 1;
					if (choice == 'G' || choice == 'K') {
						int com_i, com_j;
						if (choice == 'G') {
							com_j = (X - (8 + 2 * width)) / 2;
							com_i = Y - (height + 5);
						}
						else {
							com_j = (X - x_min) / 8;
							com_i = (Y - y_min) / 4;
						}
						for (int i = 0; i < size; i++) {
							for (int j = 0; j < size; j++) {
								if (i == com_i && j == com_j) {
									if (show[i][j] == 0) {
										show[i][j] = 1;
										leftright[i][j] = 1;
										newball[i][j] = 2;
									}
									else if (show[i][j] == 1 && leftright[i][j] == 0) {
										show[i][j] = 1;
										leftright[i][j] = 1;
										newball[i][j] = 2;
									}
									else if (show[i][j] == 1 && leftright[i][j] == 1) {
										show[i][j] = 0;
										newball[i][j] = 0;
									}
								}
							}
						}
					}
					break;
				}
			}
		}

		else if (ret == CCT_KEYBOARD_EVENT) {
			cct_setcursor(CURSOR_INVISIBLE);
			cct_gotoxy(2 * width + 6, step + height + 6);
			cout << "[读到键码]                      ";
			switch (keycode1) {
			case 13:	//回车键
				cct_gotoxy(2 * width + 6, step + height + 6);
				if (choice == 'F' || choice == 'J') {
					cout << "[读到回车键]                      ";
					loop = 0;
					break;
				}
				else {
					cout << "[读到回车键]                      ";
					break;
				}
			case 81:   // Q/q
			case 113:
				if (choice == 'F' || choice == 'J') {
					cct_gotoxy(2 * width + 17, step + height + 6);//其他键直接打印ASCII值
					if (keycode2)
						cout << keycode1 << '/' << keycode2 << endl;
					else
						cout << keycode1 << "/0" << endl;
					break;
				}
				else {
					cct_gotoxy(2 * width + 6, step + height + 6);
					cout << "[读到Q/q，游戏结束]        ";
					return;
					break;
				}
			case 89:   // Y/y
			case 121:
				if (choice == 'F' || choice == 'J') {
					cct_gotoxy(2 * width + 17, step + height + 6);//其他键直接打印ASCII值
					if (keycode2)
						cout << keycode1 << '/' << keycode2 << endl;
					else
						cout << keycode1 << "/0" << endl;
					break;
				}
				else {
					cct_gotoxy(2 * width + 6, step + height + 6);
					cout << "[读到提交键]        ";
					Sleep(500);
					int success = examine(size, newball, ball);
					if (success == 1) {
						cct_gotoxy(2 * width + 6, step + height + 6);
						cout << "提交错误，可用作弊模式查看        ";
					}
					else {
						cct_gotoxy(2 * width + 6, step + height + 6);
						cout << "[提交成功]        ";
						return;
					}
					break;
				}
			case 90:   // Z/z
			case 122:
				if (choice == 'F' || choice == 'J') {
					cct_gotoxy(2 * width + 17, step + height + 6);//其他键直接打印ASCII值
					if (keycode2)
						cout << keycode1 << '/' << keycode2 << endl;
					else
						cout << keycode1 << "/0" << endl;
					break;
				}
				else {
					cheat = (cheat == 0 ? 1 : 0);
					click = 1;
					cct_gotoxy(2 * width + 6, step + height + 6);
					cout << "[作弊模式" << (cheat == 1 ? "开" : "关") << "]        ";
					break;
				}
			case 224:
				switch (keycode2) {
				case KB_ARROW_UP:
					cct_gotoxy(2 * width + 6, step + height + 6);
					cout << "[读到上箭头] ";
					break;
				case KB_ARROW_DOWN:
					cct_gotoxy(2 * width + 6, step + height + 6);
					cout << "[读到下箭头] ";
					break;
				case KB_ARROW_LEFT:
					cct_gotoxy(2 * width + 6, step + height + 6);
					cout << "[读到左箭头] ";
					break;
				case KB_ARROW_RIGHT:
					cct_gotoxy(2 * width + 6, step + height + 6);
					cout << "[读到右箭头] ";
					break;
				}
				break;
			default:
				cct_gotoxy(2 * width + 17, step + height + 6);//其他键直接打印ASCII值
				if (keycode2)
					cout << keycode1 << '/' << keycode2 << endl;
				else
					cout << keycode1 << "/0" << endl;
				break;
			}
		}
		if (click == 1)
			display(size, ball, width, height, cheat, leftright, show, newball, choice);
	}
}



/***************************************************************************
  函数名称：choiceEI
  功    能：完成选项EI的操作
  输入参数：char choice
  返 回 值：/
  说    明：/
***************************************************************************/
void choiceEI(char choice)
{
	int size;
	input(&size);
	cct_cls();
	char ball[maxsize][maxsize] = { 0 };
	create_ball(size, ball);
	form(size, choice);
	int row[maxsize][8] = { 0 }, col[8][maxsize] = { 0 };
	count_ball(size, ball, row, col);
	int width = 0, height = 0;
	restrict(size, row, col, &width, &height);
	draw_table(size, ball, row, col, width, height, choice);
	if (choice == 'F' || choice == 'J' || choice == 'G' || choice == 'K')
		mouse_and_keyboard(size, ball, width, height, choice);

}



/***************************************************************************
  函数名称：choiceH
  功    能：完成选项H的操作
  输入参数：/
  返 回 值：/
  说    明：n*n的框架(有分隔线)，原样输出
***************************************************************************/
void choiceH()
{
	int size;
	input(&size);
	cct_cls();
	char ball[maxsize][maxsize] = { 0 };
	create_ball(size, ball);
	form(size, 'H');
	draw_framework(size, 2, 1, 'H');
	draw_ball(size, ball, 4, 3, 'H');
}