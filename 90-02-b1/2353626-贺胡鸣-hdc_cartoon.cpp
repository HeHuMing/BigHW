/*  1234567  计科  测试  */
#include <iostream>
#include <Windows.h>
#include <math.h>
#include "../include/cmd_hdc_tools.h"
using namespace std;

#define Pi 3.14159

//注：本图用本人自己实现的工具集才会达到更完美的效果，demo工具集作图会略有瑕疵，敬请谅解...

//基本尺寸
const int WIN_WIDTH = 800;
const int WIN_HEIGHT = 800;
const int SIZE_WIDTH = 700;
const int SIZE_HEIGHT = 766;
const int OFFSET_X = 400;
const int OFFSET_Y = 400;

//公用描边色
const int MyColor_OUTLINE = RGB(0, 0, 0); //黑


//究极利器(opt 1/2 画线/涂色  也可以1画粗线涂色,但注意此时要加一些偏移量  炒鸡好用！！！！)
	// 给出三点，用弧将该三点连接，顺时针方向排列123，半径过小时会因误差过大而作图有偏差
static void FITTING_arc(const int x1, const int y1, const int x2, const int y2, const int x3, const int y3, const int thinkness, const int RGB_value, int opt = 1)
{
	//计算圆心半径
	double A = x1 * (y2 - y3) - y1 * (x2 - x3) + x2 * y3 - x3 * y2;
	double B = (x1 * x1 + y1 * y1) * (y3 - y2) + (x2 * x2 + y2 * y2) * (y1 - y3) + (x3 * x3 + y3 * y3) * (y2 - y1);
	double C = (x1 * x1 + y1 * y1) * (x2 - x3) + (x2 * x2 + y2 * y2) * (x3 - x1) + (x3 * x3 + y3 * y3) * (x1 - x2);
	double D = (x1 * x1 + y1 * y1) * (x3 * y2 - x2 * y3) + (x2 * x2 + y2 * y2) * (x1 * y3 - x3 * y1) + (x3 * x3 + y3 * y3) * (x2 * y1 - x1 * y2);
	double x0 = -B / (2 * A);
	double y0 = -C / (2 * A);
	double r = sqrt((B * B + C * C - 4 * A * D) / (4 * A * A));

	//计算起止角度
	double v01[2] = { x1 - x0,y1 - y0 };
	double L01 = sqrt(v01[0] * v01[0] + v01[1] * v01[1]);
	double  v03[2] = { x3 - x0,y3 - y0 };
	double L03 = sqrt(v03[0] * v03[0] + v03[1] * v03[1]);
	double Angle1_cos = (v01[0] * 0 + v01[1] * (-1)) / (L01 * 1);
	double Angle1 = acos(Angle1_cos) * 180 / Pi;
	double Angle3_cos = (v03[0] * 0 + v03[1] * (-1)) / (L03 * 1);
	double Angle3 = acos(Angle3_cos) * 180 / Pi;

	//利用叉乘调整顺逆
	Angle1 = Angle1 * v01[0] / abs(v01[0]);
	Angle3 = Angle3 * v03[0] / abs(v03[0]);

	if (1 == opt)
		hdc_arc((int)x0, (int)y0, (int)r, (int)Angle1, (int)Angle3, thinkness, RGB_value);
	else
		hdc_sector((int)x0, (int)y0, (int)r, (int)Angle1, (int)Angle3, true, thinkness, RGB_value);
}


// 背景
const int MyColor_BACKGROUND_ORANGE = RGB(236, 155, 38); //橘黄底
const int MyColor_BACKGROUND_LIGHTBLUE = RGB(79, 185, 255); //浅蓝外框
const int MyColor_BACKGROUND_BLACK = RGB(0, 0, 0); //黑框线
static void DRAW_background(const int base_x, const int base_y)
{
	//左上角
	const int x = base_x - OFFSET_X + (WIN_WIDTH - SIZE_WIDTH) / 2;
	const int y = base_y - OFFSET_Y + (WIN_HEIGHT - SIZE_HEIGHT) / 2;
	//边框宽度
	int delta = min(WIN_WIDTH - SIZE_WIDTH, WIN_HEIGHT - SIZE_HEIGHT);

	hdc_rectangle(x - delta / 2, y - delta / 2, SIZE_WIDTH + delta, SIZE_HEIGHT + delta, 0, true, 1, MyColor_BACKGROUND_LIGHTBLUE);
	hdc_rectangle(x - delta / 2 + 1, y - delta / 2 + 1, SIZE_WIDTH + delta - 2, SIZE_HEIGHT + delta - 2, 0, false, 4, MyColor_BACKGROUND_BLACK);
	hdc_rectangle(x, y, SIZE_WIDTH, SIZE_HEIGHT, 0, true, 1, MyColor_BACKGROUND_ORANGE);
}


//毛发 
const int MyColor_FEATHER = RGB(245, 155, 157); //粉色
//上色 第一层
static void COLORING_feather_1(const int base_x, const int base_y)
{
	//左上角
	const int x = base_x - OFFSET_X + (WIN_WIDTH - SIZE_WIDTH) / 2;
	const int y = base_y - OFFSET_Y + (WIN_HEIGHT - SIZE_HEIGHT) / 2;

	//脸以下
	hdc_triangle(x + 459, y + 680, x + 293, y + 668, x + 399, y + 515, true, 2, MyColor_FEATHER);
	hdc_triangle(x + 243, y + 553, x + 293, y + 668, x + 399, y + 515, true, 2, MyColor_FEATHER);
	hdc_triangle(x + 243, y + 553, x + 257, y + 500, x + 399, y + 515, true, 2, MyColor_FEATHER);
	hdc_triangle(x + 397, y + 495, x + 257, y + 500, x + 399, y + 515, true, 2, MyColor_FEATHER);
	hdc_triangle(x + 397, y + 495, x + 257, y + 500, x + 260, y + 391, true, 2, MyColor_FEATHER);
	hdc_triangle(x + 245, y + 387, x + 241, y + 422, x + 253, y + 393, true, 2, MyColor_FEATHER);

	//左耳
	FITTING_arc(x + 262, y + 288, x + 209, y + 301, x + 159, y + 289, 1, MyColor_FEATHER, 2);
	FITTING_arc(x + 159, y + 289, x + 138, y + 262, x + 123, y + 222, 1, MyColor_FEATHER, 2);
	FITTING_arc(x + 124, y + 222, x + 121, y + 201, x + 126, y + 184, 1, MyColor_FEATHER, 2);
	FITTING_arc(x + 152, y + 127, x + 193, y + 106, x + 227, y + 119, 1, MyColor_FEATHER, 2);
	hdc_triangle(x + 148, y + 242, x + 125, y + 184, x + 153, y + 126, true, 2, MyColor_FEATHER);
	hdc_triangle(x + 148, y + 242, x + 198, y + 144, x + 153, y + 126, true, 2, MyColor_FEATHER);
	hdc_triangle(x + 226, y + 118, x + 198, y + 144, x + 249, y + 148, true, 2, MyColor_FEATHER);
	hdc_triangle(x + 255, y + 178, x + 198, y + 144, x + 249, y + 148, true, 2, MyColor_FEATHER);
	hdc_triangle(x + 264, y + 262, x + 198, y + 144, x + 262, y + 288, true, 2, MyColor_FEATHER);

	//头
	hdc_triangle(x + 249, y + 148, x + 469, y + 241, x + 262, y + 288, true, 2, MyColor_FEATHER);
	hdc_triangle(x + 255, y + 321, x + 469, y + 241, x + 261, y + 288, true, 2, MyColor_FEATHER);
	FITTING_arc(x + 249, y + 149, x + 340, y + 84, x + 458, y + 59, 1, MyColor_FEATHER, 2);
	hdc_circle(x + 259, y + 155, 12, true, 2, MyColor_FEATHER);
	hdc_triangle(x + 443, y + 64, x + 515, y + 38, x + 477, y + 68, true, 2, MyColor_FEATHER);
	hdc_triangle(x + 490, y + 58, x + 532, y + 48, x + 509, y + 61, true, 2, MyColor_FEATHER);
	hdc_triangle(x + 490, y + 58, x + 477, y + 68, x + 509, y + 61, true, 2, MyColor_FEATHER);
	hdc_triangle(x + 536, y + 54, x + 521, y + 65, x + 509, y + 61, true, 2, MyColor_FEATHER);
	hdc_triangle(x + 477, y + 68, x + 521, y + 65, x + 509, y + 61, true, 2, MyColor_FEATHER);
	hdc_triangle(x + 443, y + 64, x + 528, y + 60, x + 463, y + 139, true, 2, MyColor_FEATHER);
	hdc_triangle(x + 559, y + 97, x + 528, y + 60, x + 463, y + 139, true, 2, MyColor_FEATHER);
	FITTING_arc(x + 560, y + 97, x + 544, y + 84, x + 528, y + 60, 1, MyColor_BACKGROUND_ORANGE, 2);//反向擦除
	hdc_triangle(x + 558, y + 96, x + 543, y + 168, x + 458, y + 129, true, 2, MyColor_FEATHER);
	hdc_triangle(x + 432, y + 142, x + 543, y + 168, x + 550, y + 290, true, 2, MyColor_FEATHER);
	FITTING_arc(x + 550, y + 152, x + 581, y + 130, x + 620, y + 134, 1, MyColor_FEATHER, 2);
	FITTING_arc(x + 619, y + 134, x + 636, y + 176, x + 606, y + 243, 1, MyColor_FEATHER, 2);
	FITTING_arc(x + 607, y + 243, x + 584, y + 266, x + 552, y + 287, 1, MyColor_FEATHER, 2);
	hdc_circle(x + 556, y + 160, 12, true, 2, MyColor_FEATHER);
}
//上色 第二层
static void COLORING_feather_2(const int base_x, const int base_y)
{
	//左上角
	const int x = base_x - OFFSET_X + (WIN_WIDTH - SIZE_WIDTH) / 2;
	const int y = base_y - OFFSET_Y + (WIN_HEIGHT - SIZE_HEIGHT) / 2;

	//头以上
	FITTING_arc(x + 441, y + 146, x + 413, y + 121, x + 390, y + 87, 10, MyColor_FEATHER, 1);
	FITTING_arc(x + 481, y + 152, x + 466, y + 160, x + 441, y + 150, 1, MyColor_FEATHER, 2);
	FITTING_arc(x + 514, y + 91, x + 498, y + 121, x + 475, y + 154, 10, MyColor_FEATHER, 1);
	hdc_triangle(x + 510, y + 93, x + 475, y + 147, x + 466, y + 101, true, 1, MyColor_FEATHER);
	hdc_triangle(x + 442, y + 147, x + 475, y + 147, x + 466, y + 101, true, 1, MyColor_FEATHER);
	hdc_triangle(x + 442, y + 147, x + 405, y + 107, x + 466, y + 101, true, 1, MyColor_FEATHER);
	FITTING_arc(x + 524, y + 219, x + 534, y + 247, x + 540, y + 280, 5, MyColor_FEATHER, 1);

	//头以下
	hdc_circle(x + 268, y + 510, 12, true, 2, MyColor_FEATHER);
	hdc_triangle(x + 303, y + 684, x + 284, y + 646, x + 357, y + 658, true, 1, MyColor_FEATHER);
	hdc_triangle(x + 327, y + 657, x + 310, y + 694, x + 328, y + 670, true, 1, MyColor_FEATHER);
	FITTING_arc(x + 340, y + 732, x + 328, y + 705, x + 330, y + 670, 5, MyColor_FEATHER, 1);
	FITTING_arc(x + 340, y + 734, x + 333, y + 697, x + 337, y + 667, 5, MyColor_FEATHER, 1);
	hdc_line(x + 332, y + 673, x + 330, y + 704, 5, MyColor_FEATHER);
	FITTING_arc(x + 366, y + 732, x + 345, y + 717, x + 337, y + 695, 5, MyColor_FEATHER, 1);
	FITTING_arc(x + 366, y + 734, x + 351, y + 706, x + 346, y + 680, 5, MyColor_FEATHER, 1);
	hdc_triangle(x + 358, y + 727, x + 328, y + 689, x + 342, y + 676, true, 1, MyColor_FEATHER);
	hdc_line(x + 418, y + 750, x + 353, y + 706, 5, MyColor_FEATHER);
	hdc_line(x + 374, y + 723, x + 403, y + 742, 4, MyColor_FEATHER);
	FITTING_arc(x + 425, y + 715, x + 422, y + 747, x + 418, y + 754, 5, MyColor_FEATHER, 1);
	FITTING_arc(x + 422, y + 713, x + 419, y + 745, x + 415, y + 752, 5, MyColor_FEATHER, 1);
	hdc_triangle(x + 418, y + 750, x + 353, y + 706, x + 422, y + 718, true, 1, MyColor_FEATHER);
	hdc_triangle(x + 324, y + 654, x + 353, y + 706, x + 422, y + 718, true, 1, MyColor_FEATHER);
	FITTING_arc(x + 428, y + 680, x + 433, y + 716, x + 419, y + 747, 10, MyColor_FEATHER, 1);
	FITTING_arc(x + 437, y + 666, x + 445, y + 709, x + 444, y + 720, 4, MyColor_FEATHER, 1);
	hdc_line(x + 443, y + 708, x + 435, y + 670, 5, MyColor_FEATHER);
	hdc_triangle(x + 425, y + 736, x + 438, y + 669, x + 324, y + 654, true, 1, MyColor_FEATHER);
	FITTING_arc(x + 457, y + 697, x + 444, y + 677, x + 439, y + 661, 5, MyColor_FEATHER, 1);
	hdc_triangle(x + 458, y + 699, x + 454, y + 670, x + 436, y + 661, true, 2, MyColor_FEATHER);
}


//衣服
const int MyColor_CLOTHES_DARKBLUE = RGB(2, 0, 221); // 深蓝
const int MyColor_CLOTHES_WHITE = RGB(254, 254, 254); //白
const int MyColor_CLOTHES_RED = RGB(248, 50, 47); //红
const int MyColor_CLOTHES_GREY = RGB(203, 207, 210); //灰白
//上色
static void COLORING_clothes(const int base_x, const int base_y)
{
	//左上角
	const int x = base_x - OFFSET_X + (WIN_WIDTH - SIZE_WIDTH) / 2;
	const int y = base_y - OFFSET_Y + (WIN_HEIGHT - SIZE_HEIGHT) / 2;

	//上色 灰白 第一层
	hdc_circle(x + 246, y + 530, 7, true, 2, MyColor_CLOTHES_GREY);
	hdc_circle(x + 246, y + 524, 7, true, 2, MyColor_CLOTHES_GREY);
	hdc_circle(x + 241, y + 539, 8, true, 2, MyColor_CLOTHES_GREY);

	//上色 白色第一层
		//左
	hdc_triangle(x + 96, y + 639, x + 171, y + 765, x + 161, y + 613, true, 2, MyColor_CLOTHES_WHITE);
	hdc_triangle(x + 195, y + 597, x + 171, y + 765, x + 161, y + 613, true, 2, MyColor_CLOTHES_WHITE);
	hdc_triangle(x + 195, y + 597, x + 171, y + 765, x + 257, y + 765, true, 2, MyColor_CLOTHES_WHITE);
	hdc_triangle(x + 230, y + 727, x + 272, y + 653, x + 324, y + 737, true, 2, MyColor_CLOTHES_WHITE);
	hdc_triangle(x + 238, y + 541, x + 272, y + 653, x + 324, y + 737, true, 2, MyColor_CLOTHES_WHITE);
	hdc_circle(x + 204, y + 585, 11, true, 2, MyColor_CLOTHES_WHITE);
	hdc_circle(x + 231, y + 518, 10, true, 2, MyColor_CLOTHES_WHITE);
	hdc_circle(x + 228, y + 527, 8, true, 2, MyColor_CLOTHES_WHITE);
	hdc_circle(x + 228, y + 532, 8, true, 2, MyColor_CLOTHES_WHITE);
	hdc_circle(x + 228, y + 541, 9, true, 2, MyColor_CLOTHES_WHITE);
	hdc_circle(x + 236, y + 512, 7, true, 2, MyColor_CLOTHES_WHITE);
	hdc_circle(x + 242, y + 512, 7, true, 2, MyColor_CLOTHES_WHITE);
	hdc_circle(x + 250, y + 510, 7, true, 2, MyColor_CLOTHES_WHITE);
	hdc_triangle(x + 195, y + 597, x + 236, y + 708, x + 243, y + 554, true, 2, MyColor_CLOTHES_WHITE);
	hdc_triangle(x + 273, y + 652, x + 236, y + 714, x + 243, y + 554, true, 2, MyColor_CLOTHES_WHITE);
	hdc_circle(x + 230, y + 552, 10, true, 2, MyColor_CLOTHES_WHITE);
	hdc_circle(x + 225, y + 552, 10, true, 2, MyColor_CLOTHES_WHITE);
	hdc_circle(x + 222, y + 562, 10, true, 2, MyColor_CLOTHES_WHITE);
	hdc_circle(x + 212, y + 575, 10, true, 2, MyColor_CLOTHES_WHITE);
	//右
	hdc_circle(x + 416, y + 560, 10, true, 2, MyColor_CLOTHES_WHITE);
	hdc_circle(x + 412, y + 553, 10, true, 2, MyColor_CLOTHES_WHITE);
	hdc_circle(x + 409, y + 546, 8, true, 2, MyColor_CLOTHES_WHITE);
	hdc_triangle(x + 398, y + 514, x + 398, y + 542, x + 421, y + 548, true, 2, MyColor_CLOTHES_WHITE);
	hdc_circle(x + 420, y + 567, 11, true, 2, MyColor_CLOTHES_WHITE);
	hdc_circle(x + 424, y + 574, 11, true, 2, MyColor_CLOTHES_WHITE);
	hdc_circle(x + 428, y + 581, 11, true, 2, MyColor_CLOTHES_WHITE);
	hdc_circle(x + 433, y + 588, 12, true, 2, MyColor_CLOTHES_WHITE);
	hdc_circle(x + 438, y + 595, 11, true, 2, MyColor_CLOTHES_WHITE);
	hdc_circle(x + 442, y + 602, 11, true, 2, MyColor_CLOTHES_WHITE);
	hdc_circle(x + 446, y + 609, 11, true, 2, MyColor_CLOTHES_WHITE);
	hdc_circle(x + 450, y + 616, 11, true, 2, MyColor_CLOTHES_WHITE);
	hdc_circle(x + 454, y + 623, 11, true, 2, MyColor_CLOTHES_WHITE);
	hdc_circle(x + 456, y + 630, 11, true, 2, MyColor_CLOTHES_WHITE);
	hdc_circle(x + 460, y + 638, 12, true, 2, MyColor_CLOTHES_WHITE);
	hdc_circle(x + 463, y + 645, 12, true, 2, MyColor_CLOTHES_WHITE);
	hdc_circle(x + 465, y + 652, 12, true, 2, MyColor_CLOTHES_WHITE);
	hdc_circle(x + 466, y + 659, 12, true, 2, MyColor_CLOTHES_WHITE);
	hdc_circle(x + 467, y + 666, 12, true, 2, MyColor_CLOTHES_WHITE);
	hdc_circle(x + 468, y + 673, 12, true, 2, MyColor_CLOTHES_WHITE);
	hdc_circle(x + 469, y + 685, 12, true, 2, MyColor_CLOTHES_WHITE);
	hdc_triangle(x + 459, y + 610, x + 513, y + 634, x + 478, y + 723, true, 2, MyColor_CLOTHES_WHITE);
	hdc_triangle(x + 530, y + 721, x + 513, y + 634, x + 478, y + 720, true, 2, MyColor_CLOTHES_WHITE);
	hdc_triangle(x + 478, y + 765, x + 530, y + 721, x + 486, y + 722, true, 2, MyColor_CLOTHES_WHITE);
	hdc_triangle(x + 478, y + 765, x + 530, y + 721, x + 526, y + 765, true, 2, MyColor_CLOTHES_WHITE);

	// 上色 深蓝
	hdc_circle(x + 96, y + 660, 19, true, 2, MyColor_CLOTHES_DARKBLUE);
	hdc_circle(x + 129, y + 665, 20, true, 2, MyColor_CLOTHES_DARKBLUE);
	hdc_triangle(x + 33, y + 765, x + 79, y + 654, x + 79, y + 765, true, 2, MyColor_CLOTHES_DARKBLUE);
	hdc_triangle(x + 98, y + 639, x + 135, y + 644, x + 111, y + 669, true, 2, MyColor_CLOTHES_DARKBLUE);
	hdc_triangle(x + 150, y + 665, x + 150, y + 765, x + 171, y + 765, true, 2, MyColor_CLOTHES_DARKBLUE);
	hdc_rectangle(x + 80, y + 667, 69, 99, 0, true, 2, MyColor_CLOTHES_DARKBLUE);
	hdc_circle(x + 279, y + 709, 20, true, 2, MyColor_CLOTHES_DARKBLUE);
	hdc_triangle(x + 259, y + 700, x + 250, y + 730, x + 266, y + 765, true, 2, MyColor_CLOTHES_DARKBLUE);
	hdc_triangle(x + 259, y + 700, x + 275, y + 730, x + 266, y + 765, true, 2, MyColor_CLOTHES_DARKBLUE);
	hdc_triangle(x + 296, y + 695, x + 324, y + 737, x + 296, y + 765, true, 2, MyColor_CLOTHES_DARKBLUE);
	hdc_triangle(x + 337, y + 765, x + 324, y + 737, x + 296, y + 765, true, 2, MyColor_CLOTHES_DARKBLUE);
	hdc_rectangle(x + 266, y + 695, 30, 71, 0, true, 2, MyColor_CLOTHES_DARKBLUE);
	hdc_triangle(x + 478, y + 723, x + 470, y + 765, x + 454, y + 765, true, 2, MyColor_CLOTHES_DARKBLUE);
	hdc_triangle(x + 478, y + 723, x + 459, y + 723, x + 454, y + 765, true, 2, MyColor_CLOTHES_DARKBLUE);
	hdc_triangle(x + 478, y + 723, x + 459, y + 723, x + 479, y + 684, true, 2, MyColor_CLOTHES_DARKBLUE);
	hdc_circle(x + 471, y + 689, 8, true, 2, MyColor_CLOTHES_DARKBLUE);
	hdc_circle(x + 470, y + 697, 8, true, 2, MyColor_CLOTHES_DARKBLUE);
	hdc_triangle(x + 459, y + 700, x + 459, y + 723, x + 471, y + 710, true, 2, MyColor_CLOTHES_DARKBLUE);
	hdc_circle(x + 515, y + 660, 18, true, 2, MyColor_CLOTHES_DARKBLUE);
	hdc_circle(x + 517, y + 662, 19, true, 2, MyColor_CLOTHES_DARKBLUE);
	hdc_circle(x + 508, y + 654, 19, true, 2, MyColor_CLOTHES_DARKBLUE);
	hdc_triangle(x + 536, y + 663, x + 559, y + 765, x + 536, y + 765, true, 2, MyColor_CLOTHES_DARKBLUE);
	hdc_rectangle(x + 525, y + 663, 10, 103, 0, true, 2, MyColor_CLOTHES_DARKBLUE);

	//上色 白色第二层
	hdc_circle(x + 479, y + 710, 7, true, 2, MyColor_CLOTHES_WHITE);
	hdc_circle(x + 485, y + 695, 9, true, 2, MyColor_CLOTHES_WHITE);
	hdc_circle(x + 483, y + 699, 9, true, 2, MyColor_CLOTHES_WHITE);
	hdc_circle(x + 504, y + 654, 11, true, 2, MyColor_CLOTHES_WHITE);
	hdc_circle(x + 503, y + 645, 11, true, 2, MyColor_CLOTHES_WHITE);
	hdc_circle(x + 494, y + 654, 11, true, 2, MyColor_CLOTHES_WHITE);
	hdc_circle(x + 494, y + 668, 11, true, 2, MyColor_CLOTHES_WHITE);
	hdc_circle(x + 506, y + 668, 11, true, 2, MyColor_CLOTHES_WHITE);
	hdc_circle(x + 510, y + 664, 11, true, 2, MyColor_CLOTHES_WHITE);
	hdc_circle(x + 513, y + 675, 11, true, 2, MyColor_CLOTHES_WHITE);
	hdc_circle(x + 515, y + 686, 11, true, 2, MyColor_CLOTHES_WHITE);
	hdc_circle(x + 516, y + 699, 11, true, 2, MyColor_CLOTHES_WHITE);
	hdc_circle(x + 517, y + 710, 11, true, 2, MyColor_CLOTHES_WHITE);
	hdc_circle(x + 517, y + 720, 11, true, 2, MyColor_CLOTHES_WHITE);
	hdc_circle(x + 516, y + 730, 11, true, 2, MyColor_CLOTHES_WHITE);
	hdc_circle(x + 516, y + 740, 11, true, 2, MyColor_CLOTHES_WHITE);
	hdc_circle(x + 515, y + 750, 11, true, 2, MyColor_CLOTHES_WHITE);

	//上色 灰白 第二层
	hdc_triangle(x + 211, y + 560, x + 214, y + 550, x + 219, y + 581, true, 2, MyColor_CLOTHES_GREY);
	hdc_triangle(x + 248, y + 668, x + 243, y + 677, x + 243, y + 653, true, 2, MyColor_CLOTHES_GREY);

	// 上色 红色
		// 左
	hdc_triangle(x + 244, y + 740, x + 256, y + 765, x + 250, y + 730, true, 2, MyColor_CLOTHES_RED);
	hdc_triangle(x + 266, y + 765, x + 256, y + 765, x + 250, y + 730, true, 2, MyColor_CLOTHES_RED);
	hdc_circle(x + 204, y + 652, 5, true, 2, MyColor_CLOTHES_RED);
	hdc_triangle(x + 200, y + 657, x + 221, y + 702, x + 210, y + 653, true, 2, MyColor_CLOTHES_RED);
	//左上五角星
	hdc_triangle(x + 157, y + 622, x + 138, y + 629, x + 157, y + 635, true, 2, MyColor_CLOTHES_RED);
	hdc_triangle(x + 158, y + 650, x + 176, y + 630, x + 157, y + 635, true, 2, MyColor_CLOTHES_RED);
	hdc_triangle(x + 201, y + 627, x + 176, y + 630, x + 186, y + 619, true, 2, MyColor_CLOTHES_RED);
	hdc_triangle(x + 173, y + 613, x + 188, y + 607, x + 186, y + 619, true, 2, MyColor_CLOTHES_RED);
	hdc_triangle(x + 173, y + 613, x + 162, y + 612, x + 157, y + 616, true, 2, MyColor_CLOTHES_RED);
	hdc_triangle(x + 173, y + 613, x + 157, y + 622, x + 157, y + 616, true, 2, MyColor_CLOTHES_RED);
	hdc_triangle(x + 173, y + 613, x + 157, y + 622, x + 157, y + 635, true, 2, MyColor_CLOTHES_RED);
	hdc_triangle(x + 173, y + 613, x + 186, y + 619, x + 157, y + 635, true, 2, MyColor_CLOTHES_RED);
	hdc_triangle(x + 176, y + 630, x + 186, y + 619, x + 157, y + 635, true, 2, MyColor_CLOTHES_RED);
	//左下五角星
	hdc_triangle(x + 200, y + 765, x + 179, y + 759, x + 200, y + 748, true, 2, MyColor_CLOTHES_RED);
	hdc_triangle(x + 202, y + 728, x + 218, y + 741, x + 200, y + 748, true, 2, MyColor_CLOTHES_RED);
	hdc_triangle(x + 228, y + 756, x + 218, y + 741, x + 236, y + 740, true, 2, MyColor_CLOTHES_RED);
	hdc_triangle(x + 228, y + 756, x + 235, y + 765, x + 200, y + 765, true, 2, MyColor_CLOTHES_RED);
	hdc_triangle(x + 200, y + 765, x + 218, y + 741, x + 200, y + 748, true, 2, MyColor_CLOTHES_RED);
	hdc_triangle(x + 228, y + 756, x + 218, y + 741, x + 200, y + 765, true, 2, MyColor_CLOTHES_RED);
	// 右
	hdc_triangle(x + 478, y + 722, x + 470, y + 765, x + 484, y + 722, true, 2, MyColor_CLOTHES_RED);
	hdc_triangle(x + 476, y + 765, x + 470, y + 765, x + 484, y + 722, true, 2, MyColor_CLOTHES_RED);
	//右上五角星
	hdc_triangle(x + 476, y + 617, x + 499, y + 627, x + 498, y + 630, true, 1, MyColor_CLOTHES_RED);
	hdc_triangle(x + 494, y + 636, x + 475, y + 619, x + 498, y + 630, true, 2, MyColor_CLOTHES_RED);
	hdc_triangle(x + 494, y + 636, x + 475, y + 619, x + 481, y + 630, true, 2, MyColor_CLOTHES_RED);
	hdc_triangle(x + 470, y + 621, x + 475, y + 619, x + 481, y + 630, true, 1, MyColor_CLOTHES_RED);
	hdc_triangle(x + 494, y + 636, x + 482, y + 641, x + 481, y + 630, true, 2, MyColor_CLOTHES_RED);
	hdc_triangle(x + 494, y + 636, x + 507, y + 641, x + 498, y + 630, true, 2, MyColor_CLOTHES_RED);
	//右下五角星
	hdc_triangle(x + 489, y + 765, x + 494, y + 757, x + 512, y + 765, true, 2, MyColor_CLOTHES_RED);
	hdc_triangle(x + 511, y + 760, x + 494, y + 757, x + 512, y + 765, true, 2, MyColor_CLOTHES_RED);
	hdc_triangle(x + 488, y + 746, x + 494, y + 757, x + 501, y + 746, true, 2, MyColor_CLOTHES_RED);
	hdc_triangle(x + 510, y + 728, x + 513, y + 746, x + 501, y + 746, true, 2, MyColor_CLOTHES_RED);
	hdc_triangle(x + 524, y + 747, x + 513, y + 746, x + 511, y + 760, true, 2, MyColor_CLOTHES_RED);
	hdc_triangle(x + 494, y + 757, x + 513, y + 746, x + 511, y + 760, true, 2, MyColor_CLOTHES_RED);
	hdc_triangle(x + 494, y + 757, x + 513, y + 746, x + 501, y + 746, true, 2, MyColor_CLOTHES_RED);
}
//描边
static void OUTLINES_clothes(const int base_x, const int base_y)
{
	//左上角
	const int x = base_x - OFFSET_X + (WIN_WIDTH - SIZE_WIDTH) / 2;
	const int y = base_y - OFFSET_Y + (WIN_HEIGHT - SIZE_HEIGHT) / 2;

	// 描边
	//左袖肩
	hdc_line(x + 33, y + 765, x + 75, y + 658, 3, MyColor_OUTLINE);
	hdc_line(x + 75, y + 658, x + 76, y + 655, 3, MyColor_OUTLINE);
	FITTING_arc(x + 76, y + 658, x + 84, y + 645, x + 98, y + 639, 2, MyColor_OUTLINE);
	hdc_line(x + 96, y + 639, x + 195, y + 597, 3, MyColor_OUTLINE);
	hdc_line(x + 98, y + 639, x + 135, y + 644, 3, MyColor_OUTLINE);
	FITTING_arc(x + 135, y + 644, x + 146, y + 651, x + 152, y + 666, 2, MyColor_OUTLINE);
	hdc_line(x + 152, y + 666, x + 171, y + 765, 3, MyColor_OUTLINE);
	FITTING_arc(x + 75, y + 666, x + 88, y + 657, x + 106, y + 658, 2, MyColor_OUTLINE);
	hdc_line(x + 138, y + 706, x + 146, y + 700, 3, MyColor_OUTLINE);
	hdc_line(x + 146, y + 700, x + 149, y + 708, 3, MyColor_OUTLINE);
	FITTING_arc(x + 149, y + 708, x + 147, y + 734, x + 143, y + 748, 2, MyColor_OUTLINE);
	FITTING_arc(x + 143, y + 766, x + 141, y + 754, x + 143, y + 748, 2, MyColor_OUTLINE);

	// 左领子
	FITTING_arc(x + 195, y + 597, x + 192, y + 584, x + 196, y + 575, 2, MyColor_OUTLINE);
	hdc_line(x + 196, y + 575, x + 211, y + 562, 3, MyColor_OUTLINE);
	hdc_line(x + 211, y + 562, x + 220, y + 531, 3, MyColor_OUTLINE);
	FITTING_arc(x + 220, y + 531, x + 224, y + 512, x + 232, y + 506, 3, MyColor_OUTLINE);
	hdc_line(x + 232, y + 505, x + 257, y + 500, 3, MyColor_OUTLINE);
	hdc_line(x + 211, y + 562, x + 221, y + 585, 3, MyColor_OUTLINE);
	hdc_line(x + 195, y + 597, x + 231, y + 733, 3, MyColor_OUTLINE);
	hdc_line(x + 231, y + 733, x + 235, y + 739, 3, MyColor_OUTLINE);
	hdc_line(x + 235, y + 739, x + 241, y + 741, 3, MyColor_OUTLINE);
	hdc_line(x + 241, y + 741, x + 248, y + 733, 3, MyColor_OUTLINE);
	hdc_line(x + 248, y + 733, x + 259, y + 700, 3, MyColor_OUTLINE);
	FITTING_arc(x + 259, y + 700, x + 275, y + 690, x + 297, y + 695, 2, MyColor_OUTLINE);
	hdc_line(x + 297, y + 695, x + 324, y + 737, 3, MyColor_OUTLINE);
	hdc_line(x + 324, y + 737, x + 241, y + 542, 3, MyColor_OUTLINE);
	FITTING_arc(x + 241, y + 542, x + 241, y + 525, x + 255, y + 519, 2, MyColor_OUTLINE);
	FITTING_arc(x + 241, y + 542, x + 240, y + 525, x + 255, y + 519, 2, MyColor_OUTLINE);
	hdc_line(x + 324, y + 737, x + 337, y + 765, 3, MyColor_OUTLINE);
	hdc_line(x + 324, y + 737, x + 337, y + 765, 3, MyColor_OUTLINE);
	hdc_line(x + 239, y + 638, x + 248, y + 667, 2, MyColor_OUTLINE);
	hdc_line(x + 247, y + 667, x + 242, y + 679, 2, MyColor_OUTLINE);
	hdc_line(x + 242, y + 679, x + 241, y + 685, 2, MyColor_OUTLINE);
	FITTING_arc(x + 296, y + 734, x + 308, y + 748, x + 315, y + 766, 2, MyColor_OUTLINE);
	hdc_line(x + 220, y + 528, x + 218, y + 535, 2, MyColor_OUTLINE);
	hdc_line(x + 220, y + 527, x + 221, y + 535, 2, MyColor_OUTLINE);

	//右领子
	FITTING_arc(x + 422, y + 593, x + 401, y + 555, x + 399, y + 515, 3, MyColor_OUTLINE);
	FITTING_arc(x + 422, y + 593, x + 450, y + 650, x + 455, y + 765, 3, MyColor_OUTLINE);
	hdc_line(x + 454, y + 761, x + 453, y + 765, 2, MyColor_OUTLINE);//补偿
	FITTING_arc(x + 422, y + 593, x + 450, y + 650, x + 455, y + 765, 2, MyColor_OUTLINE);
	hdc_line(x + 458, y + 707, x + 464, y + 683, 2, MyColor_OUTLINE);

	hdc_triangle(x + 486, y + 699, x + 479, y + 681, x + 479, y + 690, true, 2, MyColor_OUTLINE);
	hdc_triangle(x + 466, y + 680, x + 479, y + 681, x + 479, y + 690, true, 2, MyColor_OUTLINE);
	FITTING_arc(x + 464, y + 683, x + 470, y + 679, x + 483, y + 687, 2, MyColor_OUTLINE);
	FITTING_arc(x + 483, y + 687, x + 486, y + 695, x + 488, y + 710, 2, MyColor_OUTLINE);
	FITTING_arc(x + 466, y + 682, x + 479, y + 690, x + 487, y + 711, 2, MyColor_OUTLINE);
	hdc_circle(x + 462, y + 681, 2, true, 2, MyColor_CLOTHES_WHITE);
	hdc_circle(x + 472, y + 681, 4, true, 2, MyColor_OUTLINE);

	hdc_line(x + 488 - 2, y + 710, x + 484, y + 714, 3, MyColor_OUTLINE);
	hdc_line(x + 484, y + 714, x + 480, y + 708, 3, MyColor_OUTLINE);
	hdc_line(x + 479, y + 684, x + 470, y + 708, 3, MyColor_OUTLINE);
	FITTING_arc(x + 479, y + 721, x + 472, y + 717, x + 470, y + 708, 2, MyColor_OUTLINE);
	FITTING_arc(x + 500, y + 718, x + 490, y + 722, x + 479, y + 721, 2, MyColor_OUTLINE);
	FITTING_arc(x + 505, y + 711, x + 504, y + 716, x + 500, y + 718, 2, MyColor_OUTLINE);
	FITTING_arc(x + 399, y + 515, x + 460, y + 611, x + 505, y + 714, 2, MyColor_OUTLINE);
	hdc_line(x + 500, y + 697, x + 505, y + 711, 2, MyColor_OUTLINE);//补偿
	hdc_line(x + 460, y + 611, x + 512, y + 633, 3, MyColor_OUTLINE);
	FITTING_arc(x + 512, y + 633, x + 529, y + 645, x + 540, y + 669, 2, MyColor_OUTLINE);
	hdc_line(x + 540, y + 669, x + 559, y + 765, 3, MyColor_OUTLINE);
	FITTING_arc(x + 512, y + 633, x + 529, y + 700, x + 526, y + 765, 2, MyColor_OUTLINE);
	hdc_line(x + 524, y + 761, x + 524, y + 765, 2, MyColor_OUTLINE); //补偿
	FITTING_arc(x + 544, y + 733, x + 549, y + 747, x + 552, y + 767, 2, MyColor_OUTLINE);

	//右侧马甲
	FITTING_arc(x + 478, y + 723, x + 475, y + 745, x + 470, y + 767, 2, MyColor_OUTLINE);
	FITTING_arc(x + 486, y + 723, x + 483, y + 745, x + 478, y + 767, 2, MyColor_OUTLINE);
	//右侧马甲上的五角星(下)
	hdc_line(x + 489, y + 765, x + 494, y + 757, 3, MyColor_OUTLINE);
	hdc_line(x + 494, y + 757, x + 488, y + 746, 3, MyColor_OUTLINE);
	hdc_line(x + 488, y + 746, x + 501, y + 746, 3, MyColor_OUTLINE);
	hdc_line(x + 501, y + 746, x + 510, y + 728, 3, MyColor_OUTLINE);
	hdc_line(x + 510, y + 728, x + 513, y + 746, 3, MyColor_OUTLINE);
	hdc_line(x + 513, y + 746, x + 524, y + 747, 3, MyColor_OUTLINE);
	hdc_line(x + 524, y + 747, x + 511, y + 760, 3, MyColor_OUTLINE);
	hdc_line(x + 511, y + 760, x + 513, y + 767, 3, MyColor_OUTLINE);
	//右侧马甲上的五角星(上)
	hdc_line(x + 476, y + 616, x + 475, y + 619, 3, MyColor_OUTLINE);
	hdc_line(x + 475, y + 619, x + 468, y + 621, 3, MyColor_OUTLINE);
	hdc_line(x + 468, y + 621, x + 481, y + 630, 3, MyColor_OUTLINE);
	hdc_line(x + 481, y + 630, x + 482, y + 641, 3, MyColor_OUTLINE);
	hdc_line(x + 482, y + 641, x + 494, y + 636, 3, MyColor_OUTLINE);
	hdc_line(x + 494, y + 636, x + 507, y + 641, 3, MyColor_OUTLINE);
	hdc_line(x + 507, y + 641, x + 498, y + 630, 3, MyColor_OUTLINE);
	hdc_line(x + 498, y + 630, x + 499, y + 626, 3, MyColor_OUTLINE);

	//左侧马甲
	hdc_line(x + 250, y + 730, x + 266, y + 765, 2, MyColor_OUTLINE);
	hdc_line(x + 244, y + 740, x + 255, y + 765, 2, MyColor_OUTLINE);
	FITTING_arc(x + 203, y + 663, x + 200, y + 649, x + 208, y + 645, 2, MyColor_OUTLINE);
	hdc_line(x + 203, y + 663, x + 221, y + 702, 3, MyColor_OUTLINE);
	//左侧马甲上的五角星(下)
	hdc_line(x + 200, y + 765, x + 179, y + 759, 3, MyColor_OUTLINE);
	hdc_line(x + 179, y + 759, x + 200, y + 748, 3, MyColor_OUTLINE);
	hdc_line(x + 200, y + 748, x + 202, y + 728, 3, MyColor_OUTLINE);
	hdc_line(x + 202, y + 728, x + 218, y + 741, 3, MyColor_OUTLINE);
	hdc_line(x + 218, y + 741, x + 236, y + 740, 3, MyColor_OUTLINE);
	hdc_line(x + 236, y + 740, x + 228, y + 756, 3, MyColor_OUTLINE);
	hdc_line(x + 228, y + 756, x + 237, y + 767, 3, MyColor_OUTLINE);
	//左侧马甲上的五角星(上)
	hdc_line(x + 157, y + 616, x + 157, y + 622, 3, MyColor_OUTLINE);
	hdc_line(x + 157, y + 622, x + 138, y + 629, 3, MyColor_OUTLINE);
	hdc_line(x + 138, y + 629, x + 157, y + 635, 3, MyColor_OUTLINE);
	hdc_line(x + 157, y + 635, x + 158, y + 650, 3, MyColor_OUTLINE);
	hdc_line(x + 158, y + 650, x + 176, y + 630, 3, MyColor_OUTLINE);
	hdc_line(x + 176, y + 630, x + 201, y + 627, 3, MyColor_OUTLINE);
	hdc_line(x + 201, y + 627, x + 186, y + 619, 3, MyColor_OUTLINE);
	hdc_line(x + 186, y + 619, x + 188, y + 607, 3, MyColor_OUTLINE);
	hdc_line(x + 188, y + 607, x + 173, y + 613, 3, MyColor_OUTLINE);
	hdc_line(x + 173, y + 613, x + 162, y + 612, 3, MyColor_OUTLINE);
}


//皮肤 上色
const int MyColor_SKIN = RGB(229, 200, 160);  //小麦色
static void COLORING_skin(const int base_x, const int base_y)
{
	//左上角
	const int x = base_x - OFFSET_X + (WIN_WIDTH - SIZE_WIDTH) / 2;
	const int y = base_y - OFFSET_Y + (WIN_HEIGHT - SIZE_HEIGHT) / 2;

	//上色 皮肤 小麦色
	hdc_triangle(x + 292, y + 665, x + 454, y + 673, x + 337, y + 765, true, 2, MyColor_SKIN);
	hdc_triangle(x + 465, y + 765, x + 454, y + 673, x + 337, y + 765, true, 2, MyColor_SKIN);
	hdc_circle(x + 405, y + 462, 33, true, 2, MyColor_SKIN);
	hdc_circle(x + 425, y + 459, 33, true, 2, MyColor_SKIN);
	hdc_circle(x + 445, y + 459, 33, true, 2, MyColor_SKIN);
	hdc_circle(x + 465, y + 458, 33, true, 2, MyColor_SKIN);
	hdc_circle(x + 480, y + 454, 33, true, 2, MyColor_SKIN);
	hdc_circle(x + 495, y + 446, 33, true, 2, MyColor_SKIN);
	hdc_circle(x + 506, y + 435, 33, true, 2, MyColor_SKIN);
	hdc_circle(x + 518, y + 418, 33, true, 2, MyColor_SKIN);
	hdc_circle(x + 528, y + 399, 33, true, 2, MyColor_SKIN);
	hdc_circle(x + 540, y + 379, 33, true, 2, MyColor_SKIN);
	hdc_circle(x + 552, y + 359, 33, true, 2, MyColor_SKIN);
	hdc_circle(x + 569, y + 342, 33, true, 2, MyColor_SKIN);
	hdc_circle(x + 592, y + 332, 20, true, 2, MyColor_SKIN);
	hdc_circle(x + 585, y + 324, 20, true, 2, MyColor_SKIN);
	hdc_circle(x + 570, y + 316, 20, true, 2, MyColor_SKIN);
	hdc_circle(x + 555, y + 312, 20, true, 2, MyColor_SKIN);
	hdc_circle(x + 540, y + 305, 20, true, 2, MyColor_SKIN);
	hdc_circle(x + 526, y + 295, 20, true, 2, MyColor_SKIN);
	hdc_circle(x + 514, y + 285, 20, true, 2, MyColor_SKIN);
	hdc_circle(x + 508, y + 275, 20, true, 2, MyColor_SKIN);
	hdc_circle(x + 500, y + 265, 20, true, 2, MyColor_SKIN);
	hdc_circle(x + 509, y + 239, 10, true, 2, MyColor_SKIN);
	hdc_circle(x + 356, y + 257, 12, true, 2, MyColor_SKIN);
	hdc_circle(x + 392, y + 462, 30, true, 2, MyColor_SKIN);
	hdc_circle(x + 374, y + 454, 30, true, 2, MyColor_SKIN);
	hdc_circle(x + 358, y + 444, 30, true, 2, MyColor_SKIN);
	hdc_circle(x + 344, y + 431, 30, true, 2, MyColor_SKIN);
	hdc_circle(x + 330, y + 418, 30, true, 2, MyColor_SKIN);
	hdc_circle(x + 316, y + 405, 30, true, 2, MyColor_SKIN);
	hdc_circle(x + 302, y + 393, 30, true, 2, MyColor_SKIN);
	hdc_circle(x + 288, y + 381, 30, true, 2, MyColor_SKIN);
	hdc_circle(x + 274, y + 369, 30, true, 2, MyColor_SKIN);
	hdc_circle(x + 261, y + 359, 30, true, 2, MyColor_SKIN);
	hdc_circle(x + 262, y + 349, 30, true, 2, MyColor_SKIN);
	hdc_triangle(x + 498, y + 296, x + 254, y + 317, x + 344, y + 269, true, 2, MyColor_SKIN);
	hdc_triangle(x + 498, y + 296, x + 254, y + 317, x + 290, y + 336, true, 2, MyColor_SKIN);
	hdc_triangle(x + 498, y + 296, x + 461, y + 160, x + 290, y + 336, true, 2, MyColor_SKIN);
	hdc_circle(x + 365, y + 251, 10, true, 2, MyColor_SKIN);
	hdc_circle(x + 375, y + 245, 10, true, 2, MyColor_SKIN);
	hdc_circle(x + 387, y + 242, 10, true, 2, MyColor_SKIN);
	hdc_circle(x + 495, y + 242, 15, true, 2, MyColor_SKIN);
	hdc_circle(x + 486, y + 238, 15, true, 2, MyColor_SKIN);
	hdc_circle(x + 353, y + 186, 50, true, 2, MyColor_SKIN);
	hdc_circle(x + 351, y + 180, 50, true, 2, MyColor_SKIN);
	hdc_circle(x + 363, y + 162, 52, true, 2, MyColor_SKIN);
	hdc_circle(x + 372, y + 108, 21, true, 2, MyColor_SKIN);
	hdc_circle(x + 362, y + 116, 21, true, 2, MyColor_SKIN);
	hdc_circle(x + 353, y + 129, 25, true, 2, MyColor_SKIN);
	hdc_triangle(x + 372, y + 235, x + 393, y + 104, x + 495, y + 224, true, 2, MyColor_SKIN);
	hdc_triangle(x + 406, y + 117, x + 521, y + 91, x + 466, y + 204, true, 2, MyColor_SKIN);
	hdc_circle(x + 527, y + 94, 9, true, 2, MyColor_SKIN);
	hdc_circle(x + 498, y + 197, 25, true, 2, MyColor_SKIN);
	hdc_circle(x + 513, y + 161, 23, true, 2, MyColor_SKIN);
	hdc_circle(x + 500, y + 177, 33, true, 2, MyColor_SKIN);
	hdc_circle(x + 518, y + 147, 20, true, 2, MyColor_SKIN);
	hdc_circle(x + 520, y + 129, 18, true, 2, MyColor_SKIN);
	hdc_circle(x + 518, y + 111, 20, true, 2, MyColor_SKIN);
	hdc_triangle(x + 544, y + 168, x + 525, y + 218, x + 541, y + 277, true, 2, MyColor_SKIN);
	hdc_circle(x + 582, y + 185, 36, true, 2, MyColor_SKIN);
	hdc_triangle(x + 603, y + 146, x + 577, y + 147, x + 618, y + 173, true, 2, MyColor_SKIN);
	hdc_circle(x + 558, y + 172, 12, true, 2, MyColor_SKIN);
	hdc_triangle(x + 544, y + 168, x + 585, y + 242, x + 541, y + 277, true, 2, MyColor_SKIN);
	hdc_triangle(x + 564, y + 205, x + 585, y + 242, x + 606, y + 221, true, 2, MyColor_SKIN);
	hdc_circle(x + 587, y + 195, 31, true, 2, MyColor_SKIN);
	hdc_circle(x + 215, y + 237, 45, true, 2, MyColor_SKIN);
	hdc_circle(x + 198, y + 159, 33, true, 2, MyColor_SKIN);
	hdc_triangle(x + 254, y + 180, x + 258, y + 226, x + 227, y + 140, true, 2, MyColor_SKIN);
	FITTING_arc(x + 178, y + 271, x + 149, y + 204, x + 173, y + 138, 1, MyColor_SKIN, 2);
	hdc_triangle(x + 232, y + 153, x + 250, y + 208, x + 218, y + 182, true, 2, MyColor_SKIN);
	hdc_triangle(x + 260, y + 263, x + 258, y + 231, x + 239, y + 277, true, 2, MyColor_SKIN);
	hdc_circle(x + 195, y + 255, 22, true, 2, MyColor_SKIN);
}


//眼睛 
const int MyColor_EYES_WHITE = RGB(255, 255, 255); //眼白
const int MyColor_EYES_BLACK = RGB(2, 2, 2); //眼珠
//上色
static void COLORING_eyes(const int base_x, const int base_y)
{
	//左上角
	const int x = base_x - OFFSET_X + (WIN_WIDTH - SIZE_WIDTH) / 2;
	const int y = base_y - OFFSET_Y + (WIN_HEIGHT - SIZE_HEIGHT) / 2;

	//左眼
		//眼白
	FITTING_arc(x + 340, y + 185, x + 387, y + 186, x + 426, y + 199, 9, MyColor_EYES_WHITE, 1);
	FITTING_arc(x + 338, y + 189, x + 354, y + 160, x + 369, y + 145, 8, MyColor_EYES_WHITE, 1);
	FITTING_arc(x + 365, y + 151, x + 374, y + 141, x + 391, y + 142, 8, MyColor_EYES_WHITE, 1);
	hdc_line(x + 390, y + 142, x + 418, y + 157, 8, MyColor_EYES_WHITE);
	FITTING_arc(x + 414, y + 154, x + 433, y + 170, x + 437, y + 184, 8, MyColor_EYES_WHITE, 1);
	FITTING_arc(x + 437, y + 184, x + 433, y + 193, x + 422, y + 198, 8, MyColor_EYES_WHITE, 1);
	hdc_circle(x + 360, y + 171, 15, true, 2, MyColor_EYES_WHITE);
	hdc_circle(x + 377, y + 155, 15, true, 2, MyColor_EYES_WHITE);
	hdc_circle(x + 422, y + 183, 15, true, 2, MyColor_EYES_WHITE);
	hdc_circle(x + 412, y + 176, 15, true, 2, MyColor_EYES_WHITE);
	hdc_circle(x + 402, y + 170, 15, true, 2, MyColor_EYES_WHITE);
	hdc_circle(x + 392, y + 162, 15, true, 2, MyColor_EYES_WHITE);
	hdc_circle(x + 388, y + 174, 15, true, 2, MyColor_EYES_WHITE);
	//眼珠
	hdc_circle(x + 413, y + 177, 16, true, 2, MyColor_EYES_BLACK);
	hdc_circle(x + 400, y + 172, 2, true, 2, MyColor_EYES_WHITE);
	hdc_triangle(x + 397, y + 169, x + 402, y + 172, x + 396, y + 175, true, 2, MyColor_EYES_WHITE);

	//右眼
		//眼白
	FITTING_arc(x + 470, y + 184, x + 490, y + 159, x + 524, y + 141, 7, MyColor_EYES_WHITE, 1);
	FITTING_arc(x + 524, y + 142, x + 525, y + 144, x + 527, y + 156, 8, MyColor_EYES_WHITE, 1);
	hdc_line(x + 528, y + 154, x + 525, y + 185, 6, MyColor_EYES_WHITE);
	FITTING_arc(x + 524, y + 186, x + 510, y + 194, x + 496, y + 197, 8, MyColor_EYES_WHITE, 1);
	FITTING_arc(x + 497, y + 197, x + 481, y + 193, x + 470, y + 185, 8, MyColor_EYES_WHITE, 1);
	hdc_triangle(x + 498, y + 150, x + 498, y + 195, x + 468, y + 187, true, 2, MyColor_EYES_WHITE);
	hdc_triangle(x + 498, y + 150, x + 498, y + 195, x + 522, y + 144, true, 2, MyColor_EYES_WHITE);
	hdc_triangle(x + 524, y + 188, x + 498, y + 195, x + 524, y + 144, true, 2, MyColor_EYES_WHITE);
	//眼珠
	hdc_ellipse(x + 508, y + 175, 12, 13, 0, true, 2, MyColor_EYES_BLACK);
	hdc_circle(x + 497, y + 171, 2, true, 2, MyColor_EYES_WHITE);
}
//描边
static void OUTLINES_eyes(const int base_x, const int base_y)
{
	//左上角
	const int x = base_x - OFFSET_X + (WIN_WIDTH - SIZE_WIDTH) / 2;
	const int y = base_y - OFFSET_Y + (WIN_HEIGHT - SIZE_HEIGHT) / 2;

	//眼睛
		//外廓
	FITTING_arc(x + 374, y + 236, x + 353, y + 239, x + 333, y + 235, 2, MyColor_OUTLINE);
	FITTING_arc(x + 333, y + 235, x + 304, y + 204, x + 309, y + 151, 2, MyColor_OUTLINE);
	FITTING_arc(x + 309, y + 151, x + 333, y + 115, x + 359, y + 91, 2, MyColor_OUTLINE);
	FITTING_arc(x + 359, y + 91, x + 374, y + 86, x + 390, y + 93, 2, MyColor_OUTLINE);
	FITTING_arc(x + 441, y + 151, x + 413, y + 126, x + 390, y + 93, 2, MyColor_OUTLINE);
	FITTING_arc(x + 481, y + 153, x + 466, y + 161, x + 441, y + 151, 2, MyColor_OUTLINE);
	FITTING_arc(x + 519, y + 91, x + 503, y + 121, x + 480, y + 153, 2, MyColor_OUTLINE);
	FITTING_arc(x + 519, y + 91, x + 531, y + 86, x + 537, y + 93, 2, MyColor_OUTLINE);
	FITTING_arc(x + 537, y + 93, x + 540, y + 126, x + 536, y + 171, 2, MyColor_OUTLINE);
	FITTING_arc(x + 536, y + 171, x + 528, y + 199, x + 515, y + 219, 2, MyColor_OUTLINE);
	FITTING_arc(x + 515, y + 219, x + 506, y + 224, x + 493, y + 223, 2, MyColor_OUTLINE);
	//内部
	FITTING_arc(x + 418, y + 220, x + 451, y + 208, x + 483, y + 220, 2, MyColor_OUTLINE);
	FITTING_arc(x + 418, y + 220, x + 447, y + 201, x + 474, y + 212, 2, MyColor_OUTLINE);
	FITTING_arc(x + 467, y + 205, x + 465, y + 193, x + 468, y + 181, 2, MyColor_OUTLINE);
	FITTING_arc(x + 474, y + 158, x + 474, y + 169, x + 468, y + 181, 2, MyColor_OUTLINE);
	hdc_line(x + 459, y + 159, x + 458, y + 169, 2, MyColor_OUTLINE);
	hdc_line(x + 464, y + 160, x + 464, y + 165, 2, MyColor_OUTLINE);

	//左眼
	FITTING_arc(x + 349, y + 131, x + 369, y + 129, x + 394, y + 139, 2, MyColor_OUTLINE);
	hdc_line(x + 394, y + 139, x + 433, y + 162, 3, MyColor_OUTLINE);
	hdc_line(x + 433, y + 162, x + 445, y + 171, 2, MyColor_OUTLINE);  //眉毛可改进
	hdc_line(x + 445, y + 171, x + 449, y + 170, 3, MyColor_OUTLINE);
	FITTING_arc(x + 334, y + 187, x + 350, y + 160, x + 365, y + 145, 2, MyColor_OUTLINE);
	FITTING_arc(x + 365, y + 145, x + 374, y + 137, x + 391, y + 138, 2, MyColor_OUTLINE);
	FITTING_arc(x + 334, y + 187, x + 350, y + 160, x + 365, y + 145, 2, MyColor_OUTLINE);
	FITTING_arc(x + 418, y + 153, x + 437, y + 170, x + 441, y + 181, 2, MyColor_OUTLINE);
	FITTING_arc(x + 441, y + 181, x + 437, y + 193, x + 426, y + 202, 2, MyColor_OUTLINE);
	FITTING_arc(x + 332, y + 188, x + 387, y + 189, x + 426, y + 202, 2, MyColor_OUTLINE);
	hdc_line(x + 332, y + 188, x + 394, y + 198, 2, MyColor_OUTLINE);
	FITTING_arc(x + 332, y + 188, x + 387, y + 189, x + 426, y + 202, 2, MyColor_OUTLINE);
	FITTING_arc(x + 442, y + 196, x + 389, y + 207, x + 332, y + 188, 2, MyColor_OUTLINE);
	FITTING_arc(x + 382, y + 213, x + 356, y + 208, x + 332, y + 188, 2, MyColor_OUTLINE);

	//右眼
	FITTING_arc(x + 468, y + 180, x + 490, y + 155, x + 513, y + 140, 2, MyColor_OUTLINE);
	hdc_line(x + 513, y + 140, x + 524, y + 134, 2, MyColor_OUTLINE);
	hdc_line(x + 524, y + 134, x + 532, y + 133, 2, MyColor_OUTLINE);
	FITTING_arc(x + 524, y + 136, x + 531, y + 148, x + 532, y + 160, 2, MyColor_OUTLINE);
	FITTING_arc(x + 531, y + 160, x + 525, y + 190, x + 507, y + 206, 2, MyColor_OUTLINE);
	FITTING_arc(x + 525, y + 190, x + 510, y + 198, x + 497, y + 201, 2, MyColor_OUTLINE);
	FITTING_arc(x + 497, y + 201, x + 481, y + 197, x + 466, y + 188, 2, MyColor_OUTLINE);
	FITTING_arc(x + 479, y + 201, x + 473, y + 198, x + 466, y + 188, 2, MyColor_OUTLINE);
}


// 鼻子描边
static void OUTLINES_nose(const int base_x, const int base_y)
{
	//左上角
	const int x = base_x - OFFSET_X + (WIN_WIDTH - SIZE_WIDTH) / 2;
	const int y = base_y - OFFSET_Y + (WIN_HEIGHT - SIZE_HEIGHT) / 2;

	//鼻子
		//外廓
	FITTING_arc(x + 393, y + 270, x + 377, y + 276, x + 357, y + 274, 2, MyColor_OUTLINE);
	FITTING_arc(x + 357, y + 274, x + 345, y + 265, x + 353, y + 245, 2, MyColor_OUTLINE);
	FITTING_arc(x + 353, y + 245, x + 369, y + 237, x + 388, y + 231, 2, MyColor_OUTLINE);
	hdc_line(x + 388, y + 231, x + 425, y + 219, 2, MyColor_OUTLINE);
	FITTING_arc(x + 425, y + 219, x + 447, y + 213, x + 465, y + 216, 2, MyColor_OUTLINE);
	hdc_line(x + 465, y + 216, x + 511, y + 229, 2, MyColor_OUTLINE);
	FITTING_arc(x + 511, y + 229, x + 521, y + 241, x + 516, y + 250, 2, MyColor_OUTLINE);
	FITTING_arc(x + 516, y + 250, x + 504, y + 255, x + 486, y + 254, 2, MyColor_OUTLINE);
	//内部
	FITTING_arc(x + 415, y + 248, x + 397, y + 261, x + 378, y + 263, 2, MyColor_OUTLINE);
	FITTING_arc(x + 378, y + 263, x + 371, y + 259, x + 376, y + 252, 2, MyColor_OUTLINE);
	FITTING_arc(x + 376, y + 252, x + 406, y + 246, x + 435, y + 259, 2, MyColor_OUTLINE);
	hdc_line(x + 472, y + 254, x + 505, y + 239, 2, MyColor_OUTLINE);
	FITTING_arc(x + 505, y + 239, x + 512, y + 243, x + 509, y + 249, 2, MyColor_OUTLINE);
	FITTING_arc(x + 509, y + 249, x + 497, y + 251, x + 482, y + 250, 2, MyColor_OUTLINE);
	FITTING_arc(x + 458, y + 215, x + 466, y + 223, x + 465, y + 237, 2, MyColor_OUTLINE);
	hdc_line(x + 465, y + 237, x + 463, y + 256, 2, MyColor_OUTLINE);
	hdc_line(x + 450, y + 259, x + 463, y + 256, 2, MyColor_OUTLINE);
}


//牙龈
const int MyColor_GUMS = RGB(208, 167, 138); //浅棕色
//下牙龈 上色
static void COLORING_gums_low(const int base_x, const int base_y)
{
	//左上角
	const int x = base_x - OFFSET_X + (WIN_WIDTH - SIZE_WIDTH) / 2;
	const int y = base_y - OFFSET_Y + (WIN_HEIGHT - SIZE_HEIGHT) / 2;

	//下牙龈
	FITTING_arc(x + 533, y + 399, x + 553, y + 382, x + 585, y + 361, 10, MyColor_GUMS, 1);
	FITTING_arc(x + 554, y + 385, x + 533, y + 401, x + 506, y + 417, 8, MyColor_GUMS, 1);
	FITTING_arc(x + 508, y + 415, x + 457, y + 428, x + 397, y + 424, 10, MyColor_GUMS, 1);
	FITTING_arc(x + 397, y + 425, x + 369, y + 418, x + 345, y + 409, 10, MyColor_GUMS, 1);
	hdc_line(x + 345, y + 409, x + 301, y + 381, 10, MyColor_GUMS);
	FITTING_arc(x + 309, y + 384, x + 281, y + 368, x + 270, y + 343, 8, MyColor_GUMS, 1);
	FITTING_arc(x + 599, y + 331, x + 598, y + 347, x + 581, y + 366, 8, MyColor_GUMS, 1);
	FITTING_arc(x + 595, y + 331, x + 594, y + 347, x + 577, y + 366, 8, MyColor_GUMS, 1);
	hdc_triangle(x + 549, y + 379, x + 288, y + 367, x + 442, y + 430, true, 2, MyColor_GUMS);
	hdc_triangle(x + 549, y + 379, x + 509, y + 412, x + 442, y + 426, true, 2, MyColor_GUMS);
	hdc_triangle(x + 333, y + 403, x + 286, y + 367, x + 442, y + 426, true, 2, MyColor_GUMS);
	hdc_line(x + 364, y + 409, x + 424, y + 417, 10, MyColor_GUMS);
}
//上牙龈 上色
static void COLORING_gums_high(const int base_x, const int base_y)
{
	//左上角
	const int x = base_x - OFFSET_X + (WIN_WIDTH - SIZE_WIDTH) / 2;
	const int y = base_y - OFFSET_Y + (WIN_HEIGHT - SIZE_HEIGHT) / 2;

	//上牙龈
	FITTING_arc(x + 353, y + 327, x + 332, y + 327, x + 317, y + 319, 6, MyColor_GUMS, 1);
	FITTING_arc(x + 374, y + 306, x + 350, y + 317, x + 317, y + 323, 6, MyColor_GUMS, 1);
	FITTING_arc(x + 353, y + 325, x + 377, y + 325, x + 399, y + 330, 10, MyColor_GUMS, 1);
	FITTING_arc(x + 415, y + 329, x + 409, y + 332, x + 397, y + 330, 10, MyColor_GUMS, 1);
	hdc_line(x + 415, y + 327, x + 452, y + 329, 10, MyColor_GUMS);
	FITTING_arc(x + 484, y + 325, x + 470, y + 328, x + 449, y + 330, 10, MyColor_GUMS, 1);
	FITTING_arc(x + 522, y + 324, x + 503, y + 328, x + 484, y + 325, 10, MyColor_GUMS, 1);
	hdc_line(x + 516, y + 326, x + 531, y + 325, 7, MyColor_GUMS);
	FITTING_arc(x + 567, y + 321, x + 550, y + 324, x + 528, y + 323, 10, MyColor_GUMS, 1);
	hdc_line(x + 567, y + 323, x + 586, y + 323, 6, MyColor_GUMS);
	FITTING_arc(x + 427, y + 297, x + 406, y + 303, x + 377, y + 306, 10, MyColor_GUMS, 1);
	FITTING_arc(x + 427, y + 295, x + 454, y + 294, x + 478, y + 298, 10, MyColor_GUMS, 1);
	hdc_line(x + 475, y + 297, x + 525, y + 313, 10, MyColor_GUMS);
	FITTING_arc(x + 525, y + 313, x + 554, y + 312, x + 582, y + 320, 10, MyColor_GUMS, 1);
	hdc_circle(x + 583, y + 322, 4, true, 2, MyColor_GUMS);
	hdc_line(x + 512, y + 316, x + 559, y + 315, 10, MyColor_GUMS);
	hdc_triangle(x + 536, y + 324, x + 458, y + 290, x + 330, y + 324, true, 2, MyColor_GUMS);
	hdc_line(x + 352, y + 318, x + 379, y + 309, 10, MyColor_GUMS);
	hdc_circle(x + 383, y + 310, 10, true, 2, MyColor_GUMS);
	hdc_circle(x + 404, y + 324, 10, true, 2, MyColor_GUMS);
}
//描边
static void OUTLINES_gums(const int base_x, const int base_y)
{
	//左上角
	const int x = base_x - OFFSET_X + (WIN_WIDTH - SIZE_WIDTH) / 2;
	const int y = base_y - OFFSET_Y + (WIN_HEIGHT - SIZE_HEIGHT) / 2;

	//牙龈
	FITTING_arc(x + 353, y + 330, x + 332, y + 330, x + 315, y + 320, 2, MyColor_OUTLINE);
	hdc_line(x + 353, y + 330, x + 355, y + 323, 2, MyColor_OUTLINE);
	FITTING_arc(x + 353, y + 330, x + 377, y + 330, x + 399, y + 335, 2, MyColor_OUTLINE);
	FITTING_arc(x + 415, y + 334, x + 409, y + 337, x + 399, y + 335, 2, MyColor_OUTLINE);
	hdc_line(x + 415, y + 334, x + 414, y + 319, 2, MyColor_OUTLINE);
	hdc_line(x + 415, y + 334, x + 421, y + 332, 2, MyColor_OUTLINE);
	FITTING_arc(x + 470, y + 332, x + 449, y + 334, x + 421, y + 332, 2, MyColor_OUTLINE);
	hdc_line(x + 470, y + 332, x + 484, y + 328, 2, MyColor_OUTLINE);
	hdc_line(x + 484, y + 328, x + 483, y + 316, 2, MyColor_OUTLINE);
	FITTING_arc(x + 522, y + 329, x + 503, y + 333, x + 484, y + 328, 2, MyColor_OUTLINE);
	hdc_line(x + 522, y + 329, x + 512, y + 314, 2, MyColor_OUTLINE);
	hdc_line(x + 522, y + 329, x + 528, y + 328, 2, MyColor_OUTLINE);
	hdc_line(x + 528, y + 328, x + 520, y + 317, 2, MyColor_OUTLINE);
	FITTING_arc(x + 567, y + 326, x + 550, y + 329, x + 528, y + 328, 2, MyColor_OUTLINE);
	hdc_line(x + 567, y + 326, x + 585, y + 326, 2, MyColor_OUTLINE);
	hdc_line(x + 585, y + 326, x + 588, y + 322, 2, MyColor_OUTLINE);
	hdc_line(x + 588, y + 322, x + 587, y + 318, 2, MyColor_OUTLINE);
	FITTING_arc(x + 600, y + 328, x + 600, y + 336, x + 599, y + 342, 2, MyColor_OUTLINE);
	FITTING_arc(x + 599, y + 342, x + 592, y + 351, x + 579, y + 361, 2, MyColor_OUTLINE);
	FITTING_arc(x + 572, y + 364, x + 542, y + 385, x + 507, y + 398, 2, MyColor_OUTLINE);
	FITTING_arc(x + 507, y + 398, x + 484, y + 402, x + 462, y + 403, 2, MyColor_OUTLINE);
	FITTING_arc(x + 462, y + 402, x + 428, y + 402, x + 391, y + 399, 2, MyColor_OUTLINE);
	FITTING_arc(x + 391, y + 399, x + 364, y + 397, x + 338, y + 389, 2, MyColor_OUTLINE);
	FITTING_arc(x + 300, y + 373, x + 280, y + 361, x + 273, y + 342, 2, MyColor_OUTLINE);
	hdc_line(x + 299, y + 372, x + 319, y + 379, 2, MyColor_OUTLINE);
	hdc_line(x + 567, y + 326, x + 559, y + 315, 2, MyColor_OUTLINE);
	hdc_line(x + 401, y + 297, x + 399, y + 305, 2, MyColor_OUTLINE);
	FITTING_arc(x + 434, y + 304, x + 439, y + 310, x + 442, y + 318, 2, MyColor_OUTLINE);
	hdc_line(x + 458, y + 286, x + 457, y + 308, 2, MyColor_OUTLINE);
	hdc_line(x + 495, y + 296, x + 496, y + 307, 2, MyColor_OUTLINE);
	hdc_line(x + 320, y + 380, x + 301, y + 378, 2, MyColor_OUTLINE);
	hdc_line(x + 329, y + 391, x + 326, y + 396, 2, MyColor_OUTLINE);
	hdc_line(x + 397, y + 399, x + 394, y + 408, 2, MyColor_OUTLINE);
	hdc_line(x + 411, y + 400, x + 412, y + 407, 2, MyColor_OUTLINE);
	hdc_line(x + 452, y + 400, x + 442, y + 414, 2, MyColor_OUTLINE);
	hdc_line(x + 465, y + 400, x + 465, y + 407, 2, MyColor_OUTLINE);
	hdc_line(x + 496, y + 399, x + 497, y + 405, 2, MyColor_OUTLINE);
}


//牙齿
const int MyColor_TEETH = RGB(32, 217, 238); //天蓝色
//上色
static void COLORING_teeth(const int base_x, const int base_y)
{
	//左上角
	const int x = base_x - OFFSET_X + (WIN_WIDTH - SIZE_WIDTH) / 2;
	const int y = base_y - OFFSET_Y + (WIN_HEIGHT - SIZE_HEIGHT) / 2;

	//左侧边缘
	FITTING_arc(x + 295, y + 367, x + 302, y + 359, x + 315, y + 355, 9, MyColor_TEETH);
	hdc_line(x + 295, y + 367, x + 322, y + 376, 9, MyColor_TEETH);
	hdc_line(x + 298, y + 362, x + 325, y + 371, 12, MyColor_TEETH);
	FITTING_arc(x + 323, y + 376, x + 313, y + 349, x + 321, y + 321, 10, MyColor_TEETH);
	hdc_line(x + 332, y + 389, x + 317, y + 371, 8, MyColor_TEETH);
	hdc_line(x + 333, y + 389, x + 344, y + 369, 8, MyColor_TEETH);

	//右侧边缘
	FITTING_arc(x + 583, y + 326, x + 582, y + 345, x + 574, y + 363, 6, MyColor_TEETH);

	//下侧边缘
	FITTING_arc(x + 391, y + 394, x + 364, y + 392, x + 338, y + 384, 10, MyColor_TEETH);
	FITTING_arc(x + 462, y + 397, x + 428, y + 398, x + 391, y + 394, 10, MyColor_TEETH);
	FITTING_arc(x + 507, y + 393, x + 484, y + 397, x + 462, y + 397, 10, MyColor_TEETH);
	FITTING_arc(x + 572, y + 359, x + 542, y + 380, x + 507, y + 393, 10, MyColor_TEETH);

	//填涂
	hdc_triangle(x + 316, y + 325, x + 583, y + 327, x + 319, y + 364, true, 2, MyColor_TEETH);
	hdc_triangle(x + 576, y + 358, x + 583, y + 327, x + 319, y + 364, true, 2, MyColor_TEETH);
	hdc_triangle(x + 576, y + 358, x + 331, y + 382, x + 319, y + 364, true, 2, MyColor_TEETH);
	hdc_triangle(x + 576, y + 358, x + 331, y + 382, x + 467, y + 396, true, 2, MyColor_TEETH);
	hdc_triangle(x + 576, y + 358, x + 526, y + 386, x + 467, y + 396, true, 2, MyColor_TEETH);
}
//描边
static void OUTLINES_teeth(const int base_x, const int base_y)
{
	//左上角
	const int x = base_x - OFFSET_X + (WIN_WIDTH - SIZE_WIDTH) / 2;
	const int y = base_y - OFFSET_Y + (WIN_HEIGHT - SIZE_HEIGHT) / 2;

	//牙
	FITTING_arc(x + 288, y + 367, x + 297, y + 357, x + 308, y + 351, 2, MyColor_OUTLINE);
	FITTING_arc(x + 318, y + 376, x + 308, y + 349, x + 316, y + 321, 2, MyColor_OUTLINE);
	hdc_line(x + 333, y + 394, x + 317, y + 375, 2, MyColor_OUTLINE);
	FITTING_arc(x + 344, y + 333, x + 350, y + 353, x + 344, y + 377, 2, MyColor_OUTLINE);
	hdc_line(x + 333, y + 394, x + 344, y + 375, 2, MyColor_OUTLINE);
	FITTING_arc(x + 369, y + 379, x + 353, y + 360, x + 351, y + 332, 2, MyColor_OUTLINE);
	FITTING_arc(x + 412, y + 365, x + 397, y + 377, x + 369, y + 379, 2, MyColor_OUTLINE);
	hdc_line(x + 412, y + 365, x + 413, y + 342, 2, MyColor_OUTLINE);
	hdc_line(x + 413, y + 342, x + 409, y + 337, 2, MyColor_OUTLINE);
	hdc_line(x + 413, y + 342, x + 417, y + 335, 2, MyColor_OUTLINE);
	hdc_line(x + 393, y + 392, x + 396, y + 376, 2, MyColor_OUTLINE);
	hdc_line(x + 393, y + 392, x + 388, y + 398, 2, MyColor_OUTLINE);
	hdc_line(x + 393, y + 392, x + 397, y + 400, 2, MyColor_OUTLINE);
	FITTING_arc(x + 429, y + 374, x + 419, y + 371, x + 412, y + 365, 2, MyColor_OUTLINE);
	hdc_line(x + 429, y + 374, x + 465, y + 373, 2, MyColor_OUTLINE);
	FITTING_arc(x + 479, y + 367, x + 474, y + 371, x + 465, y + 373, 2, MyColor_OUTLINE);
	FITTING_arc(x + 484, y + 333, x + 483, y + 349, x + 479, y + 367, 2, MyColor_OUTLINE);
	hdc_line(x + 484, y + 333, x + 478, y + 329, 2, MyColor_OUTLINE);
	hdc_line(x + 484, y + 333, x + 488, y + 329, 2, MyColor_OUTLINE);
	FITTING_arc(x + 507, y + 371, x + 491, y + 371, x + 479, y + 367, 2, MyColor_OUTLINE);
	FITTING_arc(x + 535, y + 365, x + 521, y + 369, x + 506, y + 371, 2, MyColor_OUTLINE);
	FITTING_arc(x + 530, y + 329, x + 537, y + 347, x + 535, y + 365, 2, MyColor_OUTLINE);
	FITTING_arc(x + 565, y + 327, x + 564, y + 342, x + 558, y + 353, 2, MyColor_OUTLINE);
	FITTING_arc(x + 558, y + 353, x + 550, y + 360, x + 535, y + 365, 2, MyColor_OUTLINE);
	FITTING_arc(x + 574, y + 364, x + 567, y + 354, x + 563, y + 345, 2, MyColor_OUTLINE);
	FITTING_arc(x + 587, y + 326, x + 586, y + 345, x + 576, y + 365, 2, MyColor_OUTLINE);
	hdc_line(x + 537, y + 381, x + 532, y + 388, 2, MyColor_OUTLINE);
	hdc_line(x + 537, y + 381, x + 549, y + 379, 2, MyColor_OUTLINE);
	hdc_line(x + 537, y + 381, x + 535, y + 364, 2, MyColor_OUTLINE);
	hdc_line(x + 474, y + 394, x + 463, y + 401, 2, MyColor_OUTLINE);
	FITTING_arc(x + 500, y + 399, x + 486, y + 398, x + 474, y + 394, 2, MyColor_OUTLINE);
	FITTING_arc(x + 476, y + 371, x + 477, y + 381, x + 475, y + 395, 2, MyColor_OUTLINE);
	hdc_line(x + 477, y + 373, x + 480, y + 367, 2, MyColor_OUTLINE);

	//牙内纹路
	hdc_line(x + 292, y + 367, x + 300, y + 361, 2, MyColor_OUTLINE);
	FITTING_arc(x + 326, y + 348, x + 323, y + 338, x + 323, y + 326, 2, MyColor_OUTLINE);
	hdc_line(x + 329, y + 344, x + 333, y + 352, 2, MyColor_OUTLINE);
	hdc_line(x + 357, y + 331, x + 360, y + 342, 2, MyColor_OUTLINE);
	hdc_line(x + 397, y + 335, x + 399, y + 343, 2, MyColor_OUTLINE);
	hdc_line(x + 345, y + 392, x + 349, y + 385, 2, MyColor_OUTLINE);
	hdc_line(x + 466, y + 332, x + 466, y + 341, 2, MyColor_OUTLINE);
	hdc_line(x + 433, y + 400, x + 434, y + 395, 2, MyColor_OUTLINE);
	hdc_line(x + 508, y + 371, x + 509, y + 362, 2, MyColor_OUTLINE);
	hdc_line(x + 514, y + 370, x + 514, y + 366, 2, MyColor_OUTLINE);
	hdc_line(x + 553, y + 329, x + 552, y + 336, 2, MyColor_OUTLINE);
	hdc_line(x + 577, y + 327, x + 577, y + 337, 2, MyColor_OUTLINE);
	hdc_line(x + 582, y + 327, x + 582, y + 333, 2, MyColor_OUTLINE);
	hdc_line(x + 524, y + 393, x + 525, y + 381, 2, MyColor_OUTLINE);
	hdc_line(x + 529, y + 390, x + 529, y + 378, 2, MyColor_OUTLINE);
}


//嘴 描边
static void OUTLINES_mouse(const int base_x, const int base_y)
{
	//左上角
	const int x = base_x - OFFSET_X + (WIN_WIDTH - SIZE_WIDTH) / 2;
	const int y = base_y - OFFSET_Y + (WIN_HEIGHT - SIZE_HEIGHT) / 2;

	//嘴
		//外廓
	hdc_line(x + 344, y + 268, x + 256, y + 316, 2, MyColor_OUTLINE);
	FITTING_arc(x + 253, y + 393, x + 230, y + 359, x + 257, y + 317, 2, MyColor_OUTLINE);
	FITTING_arc(x + 253, y + 393, x + 288, y + 421, x + 338, y + 468, 2, MyColor_OUTLINE);
	hdc_line(x + 329, y + 459, x + 339, y + 468, 2, MyColor_OUTLINE);//补偿
	FITTING_arc(x + 404, y + 498, x + 372, y + 488, x + 338, y + 468, 2, MyColor_OUTLINE);
	FITTING_arc(x + 403, y + 497, x + 442, y + 492, x + 447, y + 492, 2, MyColor_OUTLINE);
	FITTING_arc(x + 403, y + 497, x + 442, y + 492, x + 447, y + 492, 2, MyColor_OUTLINE);
	FITTING_arc(x + 545, y + 438, x + 526, y + 465, x + 447, y + 492, 2, MyColor_OUTLINE);
	hdc_line(x + 544, y + 439, x + 546, y + 434, 2, MyColor_OUTLINE);//补偿
	FITTING_arc(x + 545, y + 438, x + 558, y + 413, x + 578, y + 381, 2, MyColor_OUTLINE);
	FITTING_arc(x + 614, y + 336, x + 604, y + 353, x + 578, y + 381, 2, MyColor_OUTLINE);
	FITTING_arc(x + 594, y + 306, x + 609, y + 319, x + 614, y + 336, 2, MyColor_OUTLINE);
	FITTING_arc(x + 553, y + 290, x + 576, y + 297, x + 594, y + 306, 2, MyColor_OUTLINE);
	FITTING_arc(x + 553, y + 290, x + 532, y + 273, x + 516, y + 252, 2, MyColor_OUTLINE);
	//内部
	FITTING_arc(x + 579, y + 381, x + 560, y + 405, x + 539, y + 425, 2, MyColor_OUTLINE);
	FITTING_arc(x + 578, y + 381, x + 542, y + 408, x + 496, y + 432, 2, MyColor_OUTLINE);
	FITTING_arc(x + 389, y + 449, x + 463, y + 432, x + 527, y + 435, 3, MyColor_OUTLINE);
	hdc_line(x + 420, y + 437, x + 393, y + 437, 3, MyColor_OUTLINE);
	hdc_line(x + 393, y + 437, x + 369, y + 430, 2, MyColor_OUTLINE);
	FITTING_arc(x + 279, y + 395, x + 318, y + 418, x + 347, y + 439, 2, MyColor_OUTLINE);
	FITTING_arc(x + 279, y + 395, x + 262, y + 387, x + 246, y + 373, 2, MyColor_OUTLINE);
	FITTING_arc(x + 287, y + 388, x + 317, y + 403, x + 340, y + 418, 2, MyColor_OUTLINE);
	FITTING_arc(x + 287, y + 388, x + 266, y + 377, x + 249, y + 363, 2, MyColor_OUTLINE);
	FITTING_arc(x + 249, y + 363, x + 244, y + 347, x + 252, y + 334, 2, MyColor_OUTLINE);
	FITTING_arc(x + 251, y + 335, x + 292, y + 314, x + 343, y + 298, 2, MyColor_OUTLINE);
	FITTING_arc(x + 295, y + 306, x + 315, y + 299, x + 333, y + 296, 2, MyColor_OUTLINE);
	hdc_line(x + 376, y + 290, x + 423, y + 275, 2, MyColor_OUTLINE);
	FITTING_arc(x + 423, y + 276, x + 454, y + 269, x + 485, y + 274, 2, MyColor_OUTLINE);
	FITTING_arc(x + 552, y + 290, x + 521, y + 286, x + 485, y + 275, 2, MyColor_OUTLINE);
	hdc_line(x + 452, y + 448, x + 450, y + 457, 2, MyColor_OUTLINE);
	hdc_line(x + 461, y + 448, x + 461, y + 455, 2, MyColor_OUTLINE);
	hdc_line(x + 468, y + 447, x + 472, y + 466, 2, MyColor_OUTLINE);
	hdc_line(x + 477, y + 446, x + 480, y + 451, 2, MyColor_OUTLINE);
	FITTING_arc(x + 481, y + 447, x + 463, y + 451, x + 442, y + 449, 1, MyColor_OUTLINE);
	hdc_line(x + 356, y + 446, x + 365, y + 450, 2, MyColor_OUTLINE);
	hdc_line(x + 326, y + 286, x + 330, y + 291, 2, MyColor_OUTLINE);
	hdc_line(x + 337, y + 281, x + 342, y + 287, 2, MyColor_OUTLINE);
	hdc_line(x + 384, y + 281, x + 419, y + 269, 2, MyColor_OUTLINE);
	FITTING_arc(x + 385, y + 267, x + 376, y + 268, x + 368, y + 266, 2, MyColor_OUTLINE);
	FITTING_arc(x + 368, y + 266, x + 364, y + 262, x + 369, y + 250, 2, MyColor_OUTLINE);
	hdc_line(x + 479, y + 272, x + 470, y + 264, 2, MyColor_OUTLINE);
	FITTING_arc(x + 554, y + 289, x + 523, y + 280, x + 498, y + 271, 2, MyColor_OUTLINE);
	FITTING_arc(x + 409, y + 241, x + 429, y + 245, x + 444, y + 254, 2, MyColor_OUTLINE);
	hdc_line(x + 435, y + 239, x + 444, y + 247, 2, MyColor_OUTLINE);
	hdc_line(x + 512, y + 264, x + 508, y + 269, 2, MyColor_OUTLINE);
	hdc_line(x + 519, y + 269, x + 518, y + 271, 2, MyColor_OUTLINE);
	//离牙最近的一层轮廓.
	FITTING_arc(x + 316, y + 321, x + 293, y + 334, x + 260, y + 345, 2, MyColor_OUTLINE);
	FITTING_arc(x + 316, y + 321, x + 343, y + 306, x + 378, y + 301, 2, MyColor_OUTLINE);
	FITTING_arc(x + 427, y + 290, x + 406, y + 296, x + 376, y + 301, 2, MyColor_OUTLINE);
	FITTING_arc(x + 427, y + 290, x + 454, y + 285, x + 478, y + 289, 2, MyColor_OUTLINE);
	hdc_line(x + 476, y + 288, x + 525, y + 304, 2, MyColor_OUTLINE);
	FITTING_arc(x + 525, y + 304, x + 570, y + 309, x + 603, y + 331, 2, MyColor_OUTLINE);
	FITTING_arc(x + 602, y + 331, x + 602, y + 347, x + 585, y + 366, 2, MyColor_OUTLINE);
	FITTING_arc(x + 533, y + 404, x + 553, y + 387, x + 585, y + 366, 2, MyColor_OUTLINE);
	FITTING_arc(x + 533, y + 404, x + 508, y + 419, x + 457, y + 432, 2, MyColor_OUTLINE);
	FITTING_arc(x + 457, y + 432, x + 369, y + 423, x + 301, y + 386, 2, MyColor_OUTLINE);
	FITTING_arc(x + 301, y + 386, x + 277, y + 368, x + 266, y + 344, 2, MyColor_OUTLINE);
	FITTING_arc(x + 376, y + 302, x + 350, y + 314, x + 315, y + 320, 2, MyColor_OUTLINE);
}


//脖子 描边
static void OUTLINES_neck(const int base_x, const int base_y)
{
	//左上角
	const int x = base_x - OFFSET_X + (WIN_WIDTH - SIZE_WIDTH) / 2;
	const int y = base_y - OFFSET_Y + (WIN_HEIGHT - SIZE_HEIGHT) / 2;

	// 脖子
	FITTING_arc(x + 257, y + 519, x + 253, y + 537, x + 245, y + 552, 3, MyColor_OUTLINE);
	hdc_line(x + 255, y + 519, x + 259, y + 421, 3, MyColor_OUTLINE);
	hdc_line(x + 399, y + 515, x + 400, y + 497, 3, MyColor_OUTLINE);

	//左侧毛
	hdc_line(x + 245, y + 387, x + 241, y + 422, 2, MyColor_OUTLINE);
	hdc_line(x + 241, y + 422, x + 253, y + 393, 2, MyColor_OUTLINE);
	hdc_line(x + 253, y + 393, x + 267, y + 442, 3, MyColor_OUTLINE);
	hdc_line(x + 267, y + 442, x + 269, y + 419, 3, MyColor_OUTLINE);
	hdc_line(x + 259, y + 397, x + 291, y + 467, 2, MyColor_OUTLINE);
	hdc_line(x + 291, y + 467, x + 288, y + 421, 2, MyColor_OUTLINE);
	hdc_line(x + 290, y + 439, x + 304, y + 462, 2, MyColor_OUTLINE);
	hdc_line(x + 304, y + 462, x + 303, y + 443, 2, MyColor_OUTLINE);
	hdc_line(x + 299, y + 429, x + 310, y + 459, 2, MyColor_OUTLINE);
	hdc_line(x + 310, y + 459, x + 311, y + 441, 2, MyColor_OUTLINE);

	//胸毛
	hdc_line(x + 303, y + 686, x + 318, y + 678, 3, MyColor_OUTLINE);
	hdc_line(x + 327, y + 657, x + 310, y + 694, 3, MyColor_OUTLINE);
	hdc_line(x + 310, y + 694, x + 328, y + 670, 3, MyColor_OUTLINE);
	FITTING_arc(x + 342, y + 736, x + 326, y + 705, x + 328, y + 670, 3, MyColor_OUTLINE);
	FITTING_arc(x + 342, y + 736, x + 335, y + 697, x + 339, y + 667, 2, MyColor_OUTLINE);
	FITTING_arc(x + 368, y + 734, x + 345, y + 719, x + 335, y + 697, 3, MyColor_OUTLINE);
	FITTING_arc(x + 368, y + 734, x + 353, y + 706, x + 348, y + 680, 2, MyColor_OUTLINE);
	FITTING_arc(x + 401, y + 744, x + 378, y + 729, x + 353, y + 706, 3, MyColor_OUTLINE);//
	FITTING_arc(x + 401, y + 743, x + 411, y + 748, x + 420, y + 754, 3, MyColor_OUTLINE);//
	FITTING_arc(x + 427, y + 715, x + 424, y + 747, x + 420, y + 754, 2, MyColor_OUTLINE);
	FITTING_arc(x + 433, y + 680, x + 438, y + 716, x + 424, y + 747, 3, MyColor_OUTLINE);
	FITTING_arc(x + 433, y + 680, x + 442, y + 709, x + 441, y + 735, 2, MyColor_OUTLINE);
	FITTING_arc(x + 439, y + 666, x + 447, y + 709, x + 441, y + 735, 3, MyColor_OUTLINE);
	FITTING_arc(x + 438, y + 654, x + 449, y + 666, x + 457, y + 682, 2, MyColor_OUTLINE);
	hdc_line(x + 444, y + 682, x + 458, y + 702, 3, MyColor_OUTLINE);
	FITTING_arc(x + 268, y + 607, x + 293, y + 605, x + 322, y + 608, 2, MyColor_OUTLINE);
	FITTING_arc(x + 391, y + 612, x + 413, y + 610, x + 434, y + 613, 2, MyColor_OUTLINE);
	hdc_line(x + 423, y + 765, x + 419, y + 752, 3, MyColor_OUTLINE);

}


//头 描边
static void OUTLINES_head(const int base_x, const int base_y)
{
	//左上角
	const int x = base_x - OFFSET_X + (WIN_WIDTH - SIZE_WIDTH) / 2;
	const int y = base_y - OFFSET_Y + (WIN_HEIGHT - SIZE_HEIGHT) / 2;

	//头左侧
	FITTING_arc(x + 249, y + 148, x + 261, y + 225, x + 257, y + 317, 2, MyColor_OUTLINE);
	hdc_line(x + 256, y + 310, x + 253, y + 317, 2, MyColor_OUTLINE);//补偿
	//左耳
		//外廓
	FITTING_arc(x + 262, y + 288, x + 209, y + 301, x + 159, y + 289, 2, MyColor_OUTLINE);
	FITTING_arc(x + 159, y + 289, x + 138, y + 262, x + 123, y + 222, 2, MyColor_OUTLINE);
	FITTING_arc(x + 123, y + 222, x + 120, y + 201, x + 125, y + 184, 2, MyColor_OUTLINE);
	hdc_line(x + 125, y + 184, x + 152, y + 125, 2, MyColor_OUTLINE);
	FITTING_arc(x + 152, y + 126, x + 193, y + 105, x + 227, y + 118, 2, MyColor_OUTLINE);
	hdc_line(x + 226, y + 117, x + 249, y + 148, 2, MyColor_OUTLINE);
	//内部
	FITTING_arc(x + 264, y + 262, x + 216, y + 284, x + 177, y + 271, 2, MyColor_OUTLINE);
	FITTING_arc(x + 177, y + 271, x + 148, y + 204, x + 172, y + 138, 2, MyColor_OUTLINE);
	FITTING_arc(x + 172, y + 138, x + 201, y + 125, x + 227, y + 137, 2, MyColor_OUTLINE);
	hdc_line(x + 225, y + 136, x + 255, y + 178, 2, MyColor_OUTLINE);
	FITTING_arc(x + 261, y + 224, x + 247, y + 248, x + 224, y + 266, 2, MyColor_OUTLINE);
	FITTING_arc(x + 211, y + 251, x + 203, y + 197, x + 247, y + 171, 2, MyColor_OUTLINE);
	FITTING_arc(x + 159, y + 190, x + 181, y + 163, x + 225, y + 175, 2, MyColor_OUTLINE);
	FITTING_arc(x + 162, y + 134, x + 200, y + 118, x + 234, y + 137, 2, MyColor_OUTLINE);
	hdc_line(x + 170, y + 138, x + 151, y + 154, 2, MyColor_OUTLINE);
	FITTING_arc(x + 151, y + 244, x + 143, y + 224, x + 139, y + 199, 2, MyColor_OUTLINE);
	FITTING_arc(x + 254, y + 285, x + 240, y + 291, x + 222, y + 293, 2, MyColor_OUTLINE);
	hdc_line(x + 156, y + 255, x + 161, y + 261, 2, MyColor_OUTLINE);

	//头上侧
	FITTING_arc(x + 249, y + 148, x + 340, y + 83, x + 458, y + 58, 2, MyColor_OUTLINE);
	hdc_line(x + 443, y + 64, x + 515, y + 38, 3, MyColor_OUTLINE);
	hdc_line(x + 515, y + 38, x + 477, y + 68, 3, MyColor_OUTLINE);
	hdc_line(x + 490, y + 58, x + 532, y + 48, 3, MyColor_OUTLINE);
	hdc_line(x + 532, y + 48, x + 509, y + 61, 3, MyColor_OUTLINE);
	hdc_line(x + 509, y + 61, x + 536, y + 54, 3, MyColor_OUTLINE);
	hdc_line(x + 536, y + 54, x + 521, y + 65, 3, MyColor_OUTLINE);
	FITTING_arc(x + 560, y + 98, x + 544, y + 84, x + 528, y + 60, 2, MyColor_OUTLINE);

	//头右侧
	FITTING_arc(x + 560, y + 103, x + 545, y + 168, x + 526, y + 219, 2, MyColor_OUTLINE);
	hdc_line(x + 526, y + 212, x + 522, y + 217, 2, MyColor_OUTLINE);//补偿
	FITTING_arc(x + 526, y + 219, x + 536, y + 247, x + 542, y + 280, 2, MyColor_OUTLINE);
	//右耳
		//外廓
	FITTING_arc(x + 550, y + 151, x + 581, y + 129, x + 619, y + 132, 2, MyColor_OUTLINE);
	FITTING_arc(x + 618, y + 132, x + 637, y + 176, x + 607, y + 243, 2, MyColor_OUTLINE);
	FITTING_arc(x + 607, y + 243, x + 584, y + 266, x + 552, y + 287, 2, MyColor_OUTLINE);
	//内部
	FITTING_arc(x + 545, y + 168, x + 579, y + 147, x + 612, y + 147, 2, MyColor_OUTLINE);
	FITTING_arc(x + 604, y + 147, x + 620, y + 179, x + 599, y + 230, 2, MyColor_OUTLINE);
	FITTING_arc(x + 599, y + 230, x + 575, y + 254, x + 540, y + 280, 2, MyColor_OUTLINE);
	FITTING_arc(x + 540, y + 182, x + 579, y + 168, x + 607, y + 175, 2, MyColor_OUTLINE);
	FITTING_arc(x + 579, y + 168, x + 589, y + 198, x + 571, y + 233, 2, MyColor_OUTLINE);
	FITTING_arc(x + 569, y + 246, x + 551, y + 253, x + 536, y + 247, 2, MyColor_OUTLINE);
	FITTING_arc(x + 556, y + 154, x + 603, y + 136, x + 621, y + 143, 2, MyColor_OUTLINE);
	FITTING_arc(x + 622, y + 165, x + 625, y + 181, x + 622, y + 200, 2, MyColor_OUTLINE);
}


//阴影
const int MyColor_SHADOW = RGB(7, 5, 8); //黑色阴影
const int MyColor_SHADOW_FEATHER = RGB(212, 123, 125);//皮毛上的浅色阴影
//第一层
static void COLORING_shadow_1(const int base_x, const int base_y)
{
	//左上角
	const int x = base_x - OFFSET_X + (WIN_WIDTH - SIZE_WIDTH) / 2;
	const int y = base_y - OFFSET_Y + (WIN_HEIGHT - SIZE_HEIGHT) / 2;

	//脖子
	FITTING_arc(x + 398, y + 519, x + 351, y + 494, x + 320, y + 449, 2, MyColor_SHADOW_FEATHER);
	FITTING_arc(x + 396, y + 515, x + 351, y + 490, x + 320, y + 445, 10, MyColor_SHADOW_FEATHER);
	FITTING_arc(x + 396, y + 510, x + 351, y + 485, x + 320, y + 440, 10, MyColor_SHADOW_FEATHER);
	FITTING_arc(x + 396, y + 505, x + 351, y + 480, x + 320, y + 435, 10, MyColor_SHADOW_FEATHER);
	FITTING_arc(x + 396, y + 500, x + 351, y + 475, x + 320, y + 430, 10, MyColor_SHADOW_FEATHER);
	hdc_line(x + 396, y + 519, x + 398, y + 496, 2, MyColor_SHADOW_FEATHER);
}
//第二层
static void COLORING_shadow_2(const int base_x, const int base_y)
{
	//左上角
	const int x = base_x - OFFSET_X + (WIN_WIDTH - SIZE_WIDTH) / 2;
	const int y = base_y - OFFSET_Y + (WIN_HEIGHT - SIZE_HEIGHT) / 2;

	// 脸左下毛发
	hdc_triangle(x + 267, y + 442, x + 269, y + 419, x + 253, y + 394, true, 1, MyColor_SHADOW);
	hdc_triangle(x + 259, y + 397, x + 269, y + 419, x + 253, y + 394, true, 1, MyColor_SHADOW);
	hdc_triangle(x + 290, y + 439, x + 304, y + 462, x + 303, y + 443, true, 1, MyColor_SHADOW);
	hdc_triangle(x + 290, y + 439, x + 299, y + 430, x + 303, y + 443, true, 1, MyColor_SHADOW);
	hdc_triangle(x + 290, y + 439, x + 299, y + 430, x + 290, y + 421, true, 1, MyColor_SHADOW);

	// 嘴内
		//左侧
	FITTING_arc(x + 304, y + 368, x + 284, y + 361, x + 277, y + 343, 8, MyColor_SHADOW);
	FITTING_arc(x + 316, y + 326, x + 293, y + 338, x + 277, y + 343, 8, MyColor_SHADOW);
	hdc_triangle(x + 277, y + 348, x + 316, y + 326, x + 304, y + 373, true, 1, MyColor_SHADOW);
	//上侧
	FITTING_arc(x + 316, y + 326, x + 343, y + 311, x + 377, y + 306, 10, MyColor_SHADOW);
	FITTING_arc(x + 427, y + 294, x + 454, y + 289, x + 478, y + 293, 8, MyColor_SHADOW);
	hdc_line(x + 475, y + 293, x + 525, y + 309, 10, MyColor_SHADOW);
	FITTING_arc(x + 525, y + 308, x + 570, y + 313, x + 600, y + 333, 8, MyColor_SHADOW);
	//右侧
	FITTING_arc(x + 595, y + 342, x + 588, y + 351, x + 575, y + 358, 8, MyColor_SHADOW);
	hdc_circle(x + 588, y + 336, 10, true, 2, MyColor_SHADOW);
}
//第三层
static void COLORING_shadow_3(const int base_x, const int base_y)
{
	//左上角
	const int x = base_x - OFFSET_X + (WIN_WIDTH - SIZE_WIDTH) / 2;
	const int y = base_y - OFFSET_Y + (WIN_HEIGHT - SIZE_HEIGHT) / 2;

	//牙缝
	hdc_triangle(x + 410, y + 337, x + 413, y + 342, x + 418, y + 335, true, 1, MyColor_SHADOW);
	hdc_triangle(x + 343, y + 330, x + 350, y + 330, x + 350, y + 349, true, 1, MyColor_SHADOW);
	hdc_line(x + 350, y + 330, x + 352, y + 332, 4, MyColor_SHADOW);
	hdc_triangle(x + 478, y + 329, x + 488, y + 329, x + 484, y + 333, true, 1, MyColor_SHADOW);
	hdc_triangle(x + 389, y + 398, x + 397, y + 399, x + 393, y + 392, true, 1, MyColor_SHADOW);
	hdc_triangle(x + 464, y + 401, x + 489, y + 399, x + 474, y + 394, true, 1, MyColor_SHADOW);
	hdc_triangle(x + 533, y + 388, x + 537, y + 382, x + 549, y + 379, true, 1, MyColor_SHADOW);
	hdc_triangle(x + 476, y + 369, x + 480, y + 364, x + 476, y + 376, true, 1, MyColor_SHADOW);

	//鼻孔
		//左
	hdc_circle(x + 378, y + 256, 5, true, 2, MyColor_SHADOW);
	hdc_circle(x + 382, y + 256, 7, true, 2, MyColor_SHADOW);
	hdc_circle(x + 386, y + 254, 7, true, 2, MyColor_SHADOW);
	hdc_circle(x + 390, y + 254, 7, true, 2, MyColor_SHADOW);
	hdc_circle(x + 394, y + 254, 7, true, 2, MyColor_SHADOW);
	hdc_circle(x + 398, y + 253, 6, true, 2, MyColor_SHADOW);
	hdc_circle(x + 402, y + 252, 5, true, 2, MyColor_SHADOW);
	hdc_circle(x + 408, y + 249, 4, true, 2, MyColor_SHADOW);
	//右
	hdc_circle(x + 506, y + 244, 4, true, 2, MyColor_SHADOW);
	hdc_triangle(x + 482, y + 250, x + 505, y + 239, x + 507, y + 247, true, 1, MyColor_SHADOW);

	//领子 
	hdc_triangle(x + 237, y + 532, x + 246, y + 544, x + 244, y + 548, true, 2, MyColor_SHADOW);
	hdc_triangle(x + 242, y + 546, x + 251, y + 538, x + 244, y + 548, true, 2, MyColor_SHADOW);

	//锁骨
	hdc_triangle(x + 267, y + 605, x + 270, y + 610, x + 320, y + 608, true, 2, MyColor_SHADOW_FEATHER);
	hdc_triangle(x + 431, y + 610, x + 434, y + 615, x + 392, y + 611, true, 2, MyColor_SHADOW_FEATHER);

	//胸毛
	FITTING_arc(x + 388, y + 724, x + 366, y + 708, x + 349, y + 683, 4, MyColor_SHADOW_FEATHER);
	FITTING_arc(x + 388, y + 724, x + 384, y + 706, x + 388, y + 688, 4, MyColor_SHADOW_FEATHER);
	FITTING_arc(x + 418, y + 752, x + 396, y + 720, x + 388, y + 688, 4, MyColor_SHADOW_FEATHER);

	FITTING_arc(x + 390, y + 732, x + 368, y + 716, x + 349, y + 691, 10, MyColor_SHADOW_FEATHER);
	hdc_triangle(x + 401, y + 742, x + 390, y + 734, x + 385, y + 688, true, 2, MyColor_SHADOW_FEATHER);
	hdc_triangle(x + 400, y + 733, x + 399, y + 742, x + 418, y + 752, true, 2, MyColor_SHADOW_FEATHER);
	hdc_circle(x + 345, y + 695, 5, true, 2, MyColor_FEATHER);

	//左耳
	hdc_triangle(x + 255, y + 269, x + 260, y + 263, x + 252, y + 290, true, 2, MyColor_SHADOW);
	hdc_triangle(x + 258, y + 288, x + 260, y + 263, x + 252, y + 290, true, 2, MyColor_SHADOW);
	hdc_triangle(x + 254, y + 177, x + 248, y + 148, x + 243, y + 160, true, 2, MyColor_SHADOW);
	hdc_triangle(x + 243, y + 140, x + 248, y + 148, x + 243, y + 160, true, 2, MyColor_SHADOW);
}


//一些补充细节
const int MyColor_DETAILS_WHITE = RGB(235, 234, 242); //牙齿闪光白
static void COLORING_details(const int base_x, const int base_y)
{
	//左上角
	const int x = base_x - OFFSET_X + (WIN_WIDTH - SIZE_WIDTH) / 2;
	const int y = base_y - OFFSET_Y + (WIN_HEIGHT - SIZE_HEIGHT) / 2;

	//牙齿闪光十字
		//左一
	hdc_triangle(x + 366, y + 350, x + 370, y + 350, x + 368, y + 290, true, 1, MyColor_DETAILS_WHITE);
	hdc_triangle(x + 366, y + 350, x + 370, y + 350, x + 368, y + 409, true, 1, MyColor_DETAILS_WHITE);
	hdc_triangle(x + 368, y + 348, x + 368, y + 352, x + 310, y + 350, true, 1, MyColor_DETAILS_WHITE);
	hdc_triangle(x + 368, y + 348, x + 368, y + 352, x + 422, y + 351, true, 1, MyColor_DETAILS_WHITE);
	//左二
	hdc_triangle(x + 409, y + 392, x + 417, y + 392, x + 412, y + 370, true, 1, MyColor_DETAILS_WHITE);
	hdc_triangle(x + 410, y + 392, x + 416, y + 392, x + 412, y + 418, true, 1, MyColor_DETAILS_WHITE);
	hdc_triangle(x + 414, y + 390, x + 414, y + 394, x + 396, y + 393, true, 1, MyColor_DETAILS_WHITE);
	hdc_triangle(x + 414, y + 390, x + 414, y + 394, x + 435, y + 393, true, 1, MyColor_DETAILS_WHITE);
	//左三
	hdc_triangle(x + 466, y + 357, x + 472, y + 357, x + 469, y + 332, true, 1, MyColor_DETAILS_WHITE);
	hdc_triangle(x + 466, y + 357, x + 472, y + 357, x + 469, y + 380, true, 1, MyColor_DETAILS_WHITE);
	hdc_triangle(x + 468, y + 355, x + 468, y + 359, x + 442, y + 357, true, 1, MyColor_DETAILS_WHITE);
	hdc_triangle(x + 468, y + 355, x + 468, y + 359, x + 493, y + 357, true, 1, MyColor_DETAILS_WHITE);
	//左四
	hdc_triangle(x + 521, y + 364, x + 530, y + 364, x + 526, y + 336, true, 1, MyColor_DETAILS_WHITE);
	hdc_triangle(x + 520, y + 364, x + 529, y + 364, x + 526, y + 385, true, 1, MyColor_DETAILS_WHITE);
	hdc_triangle(x + 525, y + 361, x + 525, y + 366, x + 488, y + 363, true, 1, MyColor_DETAILS_WHITE);
	hdc_triangle(x + 525, y + 361, x + 525, y + 366, x + 545, y + 363, true, 1, MyColor_DETAILS_WHITE);

	//牙齿反光三角
	hdc_triangle(x + 322, y + 379, x + 320, y + 327 + 15, x + 335, y + 393, true, 1, MyColor_DETAILS_WHITE);
	FITTING_arc(x + 322, y + 379, x + 318, y + 354, x + 320, y + 324 + 15, 4, MyColor_DETAILS_WHITE);
	FITTING_arc(x + 335, y + 393, x + 327, y + 367, x + 320, y + 324 + 15, 2, MyColor_DETAILS_WHITE);
	hdc_triangle(x + 377, y + 397, x + 384, y + 380, x + 386, y + 397, true, 1, MyColor_DETAILS_WHITE);
	hdc_triangle(x + 394, y + 350, x + 404, y + 342, x + 397, y + 377, true, 1, MyColor_DETAILS_WHITE);
	FITTING_arc(x + 404, y + 344, x + 402, y + 360, x + 397, y + 377, 2, MyColor_DETAILS_WHITE);
	hdc_triangle(x + 569, y + 354, x + 576, y + 340, x + 575, y + 363, true, 1, MyColor_DETAILS_WHITE);
	hdc_triangle(x + 580, y + 355, x + 576, y + 340, x + 575, y + 363, true, 1, MyColor_DETAILS_WHITE);
	hdc_triangle(x + 548, y + 378, x + 550, y + 359, x + 560, y + 372, true, 1, MyColor_DETAILS_WHITE);
	hdc_triangle(x + 450, y + 400, x + 464, y + 399, x + 461, y + 372, true, 1, MyColor_DETAILS_WHITE);
	hdc_triangle(x + 424, y + 372, x + 425, y + 351, x + 436, y + 374, true, 1, MyColor_DETAILS_WHITE);
	hdc_triangle(x + 488, y + 369, x + 495, y + 348, x + 503, y + 371, true, 1, MyColor_DETAILS_WHITE);
	hdc_triangle(x + 490, y + 385, x + 496, y + 393, x + 493, y + 372, true, 1, MyColor_DETAILS_WHITE);
	hdc_triangle(x + 497, y + 371, x + 496, y + 393, x + 492, y + 372, true, 1, MyColor_DETAILS_WHITE);
	hdc_triangle(x + 457, y + 372, x + 469, y + 353, x + 470, y + 371, true, 1, MyColor_DETAILS_WHITE);
	hdc_triangle(x + 546, y + 361, x + 545, y + 347, x + 559, y + 348, true, 1, MyColor_DETAILS_WHITE);
	hdc_triangle(x + 550, y + 359, x + 553, y + 340, x + 562, y + 346, true, 1, MyColor_DETAILS_WHITE);
}


void hdc_draw_cartoon_1234567(const int base_x, const int base_y)
{
	hdc_cls();

	//背景
	DRAW_background(base_x, base_y);

	//上色 (需按一定顺序才能正确的完美涂色)
	COLORING_feather_1(base_x, base_y);
	COLORING_shadow_1(base_x, base_y);
	COLORING_skin(base_x, base_y);
	COLORING_eyes(base_x, base_y);
	COLORING_clothes(base_x, base_y);
	COLORING_feather_2(base_x, base_y);
	COLORING_gums_low(base_x, base_y);
	COLORING_shadow_2(base_x, base_y);
	COLORING_teeth(base_x, base_y);
	COLORING_shadow_3(base_x, base_y);
	COLORING_gums_high(base_x, base_y);
	COLORING_details(base_x, base_y);

	//描边
	OUTLINES_clothes(base_x, base_y);
	OUTLINES_head(base_x, base_y);
	OUTLINES_eyes(base_x, base_y);
	OUTLINES_nose(base_x, base_y);
	OUTLINES_mouse(base_x, base_y);
	OUTLINES_gums(base_x, base_y);
	OUTLINES_teeth(base_x, base_y);
	OUTLINES_neck(base_x, base_y);

}