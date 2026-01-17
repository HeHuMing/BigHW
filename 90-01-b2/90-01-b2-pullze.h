/* 2353626 ºØºúÃù ´óÊý¾Ý */
#pragma once

#include <iostream>
#include <iomanip>
#include <ctime>
#include <cmath>
#include <cstdio>
#include <conio.h>
#include <Windows.h>
#include "../include/cmd_console_tools.h"
#include "../include/common_menu.h"
#include "../include/common_prompts.h"

const int state_X = 0;
const int state_Y = 22;
#define maxsize 15

void end();
void time_sleep(char choice);
void form(int size, char choice);
void recover();
void prompt();
int judge(int size, char game[200], char newball[maxsize][maxsize]);
int examine(int size, char newball[maxsize][maxsize], char ball[maxsize][maxsize]);
void input(int* psize);
void create_ball(int size, char ball[maxsize][maxsize]);
void count_ball(int size, char ball[maxsize][maxsize], int row[maxsize][8], int col[8][maxsize]);
void restrict(int size, int row[maxsize][8], int col[8][maxsize], int* width, int* height);
void choiceA();
void choiceB(char choice);
void choiceC(int size, char ball[maxsize][maxsize], int row[maxsize][8], int col[8][maxsize], char choice);
void choiceD();
void choiceEI(char choice);
void choiceH();


