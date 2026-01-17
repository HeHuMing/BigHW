/* 2353626 贺胡鸣 大数据 */
#include <iostream>
#include <Windows.h>
#include <math.h>
#include "../include/cmd_hdc_tools.h"
using namespace std;

//所需的宏定义
#define PI 3.14159
//基本尺寸
const int offset_x = 400;
const int offset_y = 400;
const int size_width = 650;
const int size_height = 700;
const int win_width = 710;
const int win_height = 760;
const int rim_thickness = 4;
const int rimm_thickness = 7;
const int dx = size_width / 650;  //x方向的最小单位
const int dy = size_height / 700; //y方向的最小单位
const int eye_width = 28 * dx;
const int eye_height = 38 * dy;


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
	double Angle1 = acos(Angle1_cos) * 180 / PI;
	double Angle3_cos = (v03[0] * 0 + v03[1] * (-1)) / (L03 * 1);
	double Angle3 = acos(Angle3_cos) * 180 / PI;

	//利用叉乘调整顺逆
	Angle1 = Angle1 * v01[0] / abs(v01[0]);
	Angle3 = Angle3 * v03[0] / abs(v03[0]);

	if (1 == opt)
		hdc_arc((int)x0, (int)y0, (int)r, (int)Angle1, (int)Angle3, thinkness, RGB_value);
	else
		hdc_sector((int)x0, (int)y0, (int)r, (int)Angle1, (int)Angle3, true, thinkness, RGB_value);
}

//画平行四边形
  //逆时针给出四点坐标，人为保证是平行四边形，即对边平行
static void hdc_parallelogram(const int x1, const int y1, const int x2, const int y2, const int x3, const int y3, const int x4, const int y4, bool filled = false, const int thickness = 2, const int RGB_value = INVALID_RGB)
{
    // 画四条边
    hdc_line(x1, y1, x2, y2, thickness, RGB_value);
    hdc_line(x2, y2, x3, y3, thickness, RGB_value);
    hdc_line(x3, y3, x4, y4, thickness, RGB_value);
    hdc_line(x4, y4, x1, y1, thickness, RGB_value);

    // 填充
    if (filled) {
        if (RGB_value != INVALID_RGB)
            hdc_set_pencolor(RGB_value);

        // 计算包围框
        int min_x = x1, max_x = x1;
        int min_y = y1, max_y = y1;

        int all_x[4] = { x1, x2, x3, x4 };
        int all_y[4] = { y1, y2, y3, y4 };

        for (int i = 0; i < 4; i++) {
            if (all_x[i] < min_x) min_x = all_x[i];
            if (all_x[i] > max_x) max_x = all_x[i];
            if (all_y[i] < min_y) min_y = all_y[i];
            if (all_y[i] > max_y) max_y = all_y[i];
        }

        // 修正的扫描线填充算法
        for (int y = min_y; y <= max_y; y++) {
            int intersections[8];
            int count = 0;

            // 定义四条边
            int edges[4][4] = {
                {x1, y1, x2, y2},
                {x2, y2, x3, y3},
                {x3, y3, x4, y4},
                {x4, y4, x1, y1}
            };

            for (int i = 0; i < 4; i++) {
                int xa = edges[i][0], ya = edges[i][1];
                int xb = edges[i][2], yb = edges[i][3];

                // 跳过水平线
                if (ya == yb) continue;

                // 检查y是否在边的y范围内
                int y_min = (ya < yb) ? ya : yb;
                int y_max = (ya > yb) ? ya : yb;

                if (y >= y_min && y <= y_max) {
                    // 对于平行四边形，我们只关心真正的边界交点
                    // 排除顶点重复计算的问题
                    if (y == y_min) {
                        // 检查这个顶点是否被另一条边包含
                        bool is_shared_vertex = false;
                        for (int j = 0; j < 4; j++) {
                            if (j != i) {
                                int xc = edges[j][0], yc = edges[j][1];
                                int xd = edges[j][2], yd = edges[j][3];
                                if ((xa == xc && ya == yc) || (xa == xd && ya == yd)) {
                                    is_shared_vertex = true;
                                    break;
                                }
                            }
                        }
                        if (!is_shared_vertex) {
                            continue; // 跳过非共享的下顶点
                        }
                    }

                    double t = (double)(y - ya) / (yb - ya);
                    int x_intersect = (int)(xa + t * (xb - xa) + 0.5);
                    intersections[count++] = x_intersect;
                }
            }

            if (count < 2) continue;

            // 排序交点
            for (int i = 0; i < count - 1; i++) {
                for (int j = 0; j < count - i - 1; j++) {
                    if (intersections[j] > intersections[j + 1]) {
                        int temp = intersections[j];
                        intersections[j] = intersections[j + 1];
                        intersections[j + 1] = temp;
                    }
                }
            }

            // 关键修正：对于平行四边形，我们只需要第一个和最后一个交点
            // 这样就避免了中间的多余交点形成两个三角形
            if (count >= 2) {
                int start_x = intersections[0];
                int end_x = intersections[count - 1];
                hdc_line(start_x, y, end_x, y, 1, RGB_value);
            }
        }
    }
}

//所需的颜色
const int my_color_bup = RGB(41, 89, 70);
const int my_color_blo = RGB(66, 64, 34);
const int my_color_black = RGB(0, 0, 0);
const int my_color_white = RGB(255, 255, 255);
const int my_color_floor_edge = RGB(38, 37, 28);

const int my_color_window_glass = RGB(18, 127, 208);
const int my_color_window_frame1 = RGB(91, 91, 165);
const int my_color_window_frame2 = RGB(129, 129, 185);
const int my_color_window_edge = RGB(51, 50, 88);
const int my_color_window_light = RGB(61, 147, 211);

const int my_color_door = RGB(49, 69, 64);

const int my_color_desk_shadow = RGB(19, 47, 21);
const int my_color_desk = RGB(32, 80, 40);
const int my_color_computer = RGB(56, 56, 56);
const int my_color_computer_shadow = RGB(40, 40, 40);

const int my_color_face = RGB(172, 238, 220);
const int my_color_clothes = RGB(165, 110, 1);

const int my_color_mouth = RGB(177, 11, 11);
const int my_color_tongue = RGB(241, 31, 31);
const int my_color_eyes = RGB(248, 238, 108);
const int my_color_hole = RGB(177, 11, 11);
const int my_color_lips = RGB(118, 194, 169);
const int my_color_spot = RGB(57, 97, 95);


//函数实现
// 背景
static void COLORING_BACKGROUND(const int base_x, const int base_y)
{
	//左上角
	const int left_up_x = base_x - offset_x + (win_width - size_width) / 2;
	const int left_up_y = base_y - offset_y + (win_height - size_height) / 2;

    hdc_rectangle(left_up_x, left_up_y, size_width, size_height * 2 / 3, 0, true, 1, my_color_bup);
	hdc_rectangle(left_up_x, left_up_y + size_height * 2 / 3, size_width, size_height / 3, 0, true, 1, my_color_blo);
	
	//画地板缝隙
    hdc_line(left_up_x, left_up_y + size_height * 2 / 3, left_up_x + size_width, left_up_y + size_height * 2 / 3, rim_thickness, my_color_floor_edge);
    /* 有空画 */
}

static void COLORING_WINDOW(const int base_x, const int base_y)
{
	//左上角
	const int left_up_x = base_x - offset_x + (win_width - size_width) / 2 + rim_thickness;
	const int left_up_y = base_y - offset_y + (win_height - size_height) / 2 + rim_thickness;

	const int window_width = size_width / 7;
	const int window_height = size_height * 2 / 9;
    const int light_height = window_height * 3 / 10;

	//底色
	hdc_rectangle(left_up_x, left_up_y, window_width * 190/100, window_height * 140 / 100, 0, true, 1, my_color_window_frame1);
	hdc_rectangle(left_up_x, left_up_y, window_width * 4 / 3, window_height * 10 / 9, 0, true, 1, my_color_window_frame2);
	hdc_rectangle(left_up_x, left_up_y, window_width, window_height, 0, true, 1, my_color_window_glass);

	//描边框
	hdc_rectangle(left_up_x, left_up_y, window_width, window_height, 0, false, rim_thickness, my_color_window_edge);
	hdc_rectangle(left_up_x, left_up_y, window_width * 4 / 3, window_height * 10 / 9, 0, false, rim_thickness, my_color_window_edge);
	hdc_rectangle(left_up_x, left_up_y, window_width * 190 / 100, window_height * 140 / 100, 0, false, rim_thickness, my_color_window_edge);
	hdc_line(left_up_x + window_width, left_up_y + window_height, left_up_x + window_width * 4 / 3, left_up_y + window_height * 10 / 9, rim_thickness, my_color_window_edge);

	//画铆钉
	hdc_circle(left_up_x + window_width * 162 / 100, left_up_y + window_height / 4, 2*rim_thickness, false, rim_thickness, my_color_window_edge);
	hdc_circle(left_up_x + window_width * 162 / 100, left_up_y + window_height*3 / 4, 2*rim_thickness, false, rim_thickness, my_color_window_edge);
	hdc_circle(left_up_x + window_width * 55/100, left_up_y + window_height * 126/100, 2 * rim_thickness, false, rim_thickness, my_color_window_edge);
	hdc_circle(left_up_x + window_width * 14/10, left_up_y + window_height * 126/100, 2 * rim_thickness, false, rim_thickness, my_color_window_edge);

	//玻璃反光
    hdc_parallelogram(left_up_x+rim_thickness, left_up_y + window_height * 3 / 5, left_up_x+rim_thickness, left_up_y + window_height * 3 / 5 + light_height,
        left_up_x-rim_thickness + window_width, left_up_y+rim_thickness + light_height, left_up_x-rim_thickness + window_width, left_up_y+rim_thickness, true, rim_thickness, my_color_window_light);
}

static void COLORING_DOOR(const int base_x, const int base_y)
{
    //左上角
    const int left_up_x = base_x - offset_x + (win_width - size_width) / 2 + rim_thickness;
    const int left_up_y = base_y - offset_y + (win_height - size_height) / 2 + rim_thickness;

    const int door_width = size_width / 6;
    const int door_height = size_height / 4;
    const int x = left_up_x + size_width - door_width;
    const int y = left_up_y + door_height;
    const int distance = door_width / 10;

    hdc_line(x, left_up_y, x, y, rim_thickness, my_color_door);
    hdc_line(x, y, x + door_width - rim_thickness, y, rim_thickness, my_color_door);
    hdc_circle(x - distance, left_up_y + door_height / 3, 3 * rim_thickness / 2, false, rim_thickness * 2 / 3, my_color_door);
    hdc_circle(x - distance, left_up_y + door_height * 2 / 3, 3 * rim_thickness / 2, false, rim_thickness * 2 / 3, my_color_door);
    hdc_circle(x + door_width / 3, y - distance, 3 * rim_thickness / 2, false, rim_thickness * 2 / 3, my_color_door);
    hdc_circle(x + 2 * door_width / 3, y - distance, 3 * rim_thickness / 2, false, rim_thickness * 2 / 3, my_color_door);

}

static void COLORING_DESK(const int base_x, const int base_y)
{
    //左上角
    const int left_up_x = base_x - offset_x + (win_width - size_width) / 2 + rim_thickness;
    const int left_up_y = base_y - offset_y + (win_height - size_height) / 2 + rim_thickness;
    //左下角
    const int left_down_x = left_up_x;
    const int left_down_y = left_up_y + size_height - rim_thickness;
    const int m = left_down_x, n = left_down_y;

    const int desk_thickness = 30 * dy;

    hdc_triangle(left_down_x, left_down_y, left_down_x, left_down_y - 22*dy, left_down_x + 50*dx, left_down_y, true, rim_thickness, my_color_desk_shadow);
    hdc_triangle(left_down_x + 290 * dx, left_down_y, left_down_x + 340 * dx, left_down_y, left_down_x + 340 * dx, left_down_y - 22 * dy, true, rim_thickness, my_color_desk_shadow);
    hdc_parallelogram(left_down_x + 290 * dx, left_down_y - 30 * dy, left_down_x + 290 * dx, left_down_y, left_down_x + 360 * dx, left_down_y - 30 * dy, left_down_x + 360 * dx, left_down_y - 60 * dy, true, rim_thickness, my_color_desk_shadow);
    hdc_triangle(left_down_x + 220 * dx, left_down_y, left_down_x + 290 * dx, left_down_y, left_down_x + 290 * dx, left_down_y - 30 * dy, true, rim_thickness, my_color_desk_shadow);
    hdc_triangle(left_down_x, left_down_y - 242 * dy, left_down_x, left_down_y - 60 * dy, left_down_x + 360 * dx, left_down_y - 60 * dy, true, rim_thickness, my_color_desk);
    hdc_rectangle(left_down_x, left_down_y - 60 * dy, 220 * dx, 38 * dy, 0, true, 2, my_color_desk);
    hdc_triangle(left_down_x + 220 * dx, left_down_y, left_down_x + 220 * dx, left_down_y - 60 * dy, left_down_x + 360 * dx, left_down_y - 60 * dy, true, 2, my_color_desk);
    hdc_triangle(left_down_x, left_down_y - 22 * dy, left_down_x + 50 * dx, left_down_y, left_down_x + 50 * dx, left_down_y - 22 * dy, true, 2, my_color_desk);
    hdc_rectangle(left_down_x + 50 * dx, left_down_y - 22 * dy, 170 * dx, 22 * dy, 0, true, 2, my_color_desk);


    hdc_line(left_down_x, left_down_y - 22*dy, left_down_x + 50*dx, left_down_y, rim_thickness, my_color_black);
    hdc_line(left_down_x + 220 * dx, left_down_y, left_down_x + 360 * dx, left_down_y - 60 * dy, rim_thickness, my_color_black);
    hdc_line(left_down_x + 360 * dx, left_down_y - 60 * dy, left_down_x + 360 * dx, left_down_y - 60 * dy + desk_thickness, rim_thickness, my_color_black);
    hdc_line(left_down_x + 360 * dx, left_down_y - 60 * dy + desk_thickness, left_down_x + 290 * dx, left_down_y, rim_thickness, my_color_black);
    hdc_line(left_down_x + 340 * dx, left_down_y, left_down_x + 340 * dx, left_down_y - 22 * dy, rim_thickness, my_color_black);
    hdc_line(left_down_x, left_down_y - 242 * dy, left_down_x + 360 * dx, left_down_y - 60 * dy, rim_thickness, my_color_black);

    hdc_parallelogram(m, n - 120 * dy, m, n - 60 * dy, m + 80 * dx, n - 90 * dy, m + 80 * dx, n - 150 * dy, true, 2, my_color_computer);
    hdc_triangle(m, n - 190 * dy, m, n - 120 * dy, m + 80 * dx, n - 150 * dy, true, 2, my_color_computer);

    hdc_line(m, n - 80 * dy, m + 80 * dx, n - 110 * dy, rim_thickness, my_color_black);
    hdc_line(m + 80 * dx, n - 150 * dy, m, n - 190 * dy, rim_thickness, my_color_black);

    hdc_parallelogram(m + 80 * dx, n - 125 * dy, m + 80 * dx, n - 110 * dy, m + 130 * dx, n - 85 * dy, m + 130 * dx, n - 100 * dy, true, 2, my_color_computer);
    hdc_parallelogram(m + 130 * dx, n - 100 * dy, m + 130 * dx, n - 85 * dy, m + 200 * dx, n - 110 * dy, m + 200 * dx, n - 125 * dy, true, 2, my_color_computer);

    hdc_parallelogram(m, n - 225 * dy, m, n - 190 * dy, m + 80 * dx, n - 150 * dy, m + 80 * dx, n - 185 * dy, true, 2, my_color_computer);
    hdc_parallelogram(m + 80 * dx, n - 125 * dy, m + 130 * dx, n - 100 * dy, m + 200 * dx, n - 125 * dy, m + 150 * dx, n - 150 * dy, true, 2, my_color_computer);
    hdc_triangle(m + 80 * dx, n - 125 * dy, m + 150 * dx, n - 150 * dy, m + 80 * dx, n - 185 * dy, true, 2, my_color_computer);
    hdc_line(m, n - 225 * dy, m + 200 * dx, n - 125 * dy, rim_thickness, my_color_black);
    hdc_parallelogram(m + 80 * dx, n - 125 * dy, m + 80 * dx, n - 110 * dy, m + 130 * dx, n - 85 * dy, m + 130 * dx, n - 100 * dy, false, rim_thickness, my_color_black);
    hdc_parallelogram(m + 130 * dx, n - 100 * dy, m + 130 * dx, n - 85 * dy, m + 200 * dx, n - 110 * dy, m + 200 * dx, n - 125 * dy, false, rim_thickness, my_color_black);
    hdc_parallelogram(m, n - 120 * dy, m, n - 60 * dy, m + 80 * dx, n - 90 * dy, m + 80 * dx, n - 150 * dy, false, rim_thickness, my_color_black);
    hdc_line(m + 80 * dx, n - 150 * dy, m, n - 190 * dy, rim_thickness, my_color_black);

    hdc_line(m + 80 * dx, n - 137 * dy, m + 125 * dx, n - 115 * dy, rim_thickness, my_color_black);
    hdc_line(m + 125 * dx, n - 115 * dy, m + 180 * dx, n - 134 * dy, rim_thickness, my_color_black);
    
    hdc_line(m + 141 * dx, n - 123 * dy, m, n - 193 * dy, rim_thickness, my_color_black);
    hdc_line(m + 160 * dx, n - 130 * dy, m, n - 209 * dy, rim_thickness, my_color_black);
    /* 键盘 有空画 */

    FITTING_arc(m + 20 * dx, n - 145 * dy, m + 10 * dx, n - 141 * dy, m, n - 140 * dy, rim_thickness, my_color_computer_shadow, 2);
    hdc_parallelogram(m, n - 200 * dy, m, n - 155 * dy, m + 20 * dx, n - 145 * dy, m + 20 * dx, n - 190 * dy, true, 2, my_color_computer_shadow);
    hdc_parallelogram(m, n - 365 * dy, m, n - 200 * dy, m + 40 * dx, n - 180 * dy, m + 40 * dx, n - 345 * dy, true, 2, my_color_computer_shadow);
    hdc_parallelogram(m + 40 * dx, n - 345 * dy, m + 40 * dx, n - 180 * dy, m + 90 * dx, n - 202 * dy, m + 90 * dx, n - 367 * dy, true, 2, my_color_computer);
    hdc_triangle(m, n - 412 * dy, m, n - 365 * dy, m + 50 * dx, n - 387 * dy, true, rim_thickness, my_color_computer);
    hdc_parallelogram(m + 50 * dx, n - 387 * dy, m, n - 365 * dy, m + 40 * dx, n - 345 * dy, m + 90 * dx, n - 367 * dy, true, rim_thickness, my_color_computer);

    FITTING_arc(m + 20 * dx, n - 145 * dy, m + 10 * dx, n - 141 * dy, m, n - 140 * dy, rim_thickness, my_color_black, 1);
    hdc_line(m + 20 * dx, n - 145 * dy, m + 20 * dx, n - 190 * dy, rim_thickness, my_color_black);
    hdc_parallelogram(m, n - 365 * dy, m, n - 200 * dy, m + 40 * dx, n - 180 * dy, m + 40 * dx, n - 345 * dy, false, rim_thickness, my_color_black);
    hdc_parallelogram(m + 40 * dx, n - 345 * dy, m + 40 * dx, n - 180 * dy, m + 90 * dx, n - 202 * dy, m + 90 * dx, n - 367 * dy, false, rim_thickness, my_color_black);
    hdc_line(m, n - 412 * dy, m + 90 * dx, n - 367 * dy, rim_thickness, my_color_black);
    hdc_line(m + 40 * dx, n - 345 * dy, m, n - 290 * dy, rim_thickness, my_color_black);
    hdc_line(m, n - 320 * dy, m + 16 * dx, n - 312 * dy, rim_thickness, my_color_black);
    hdc_line(m + 16 * dx, n - 312 * dy, m + 16 * dx, n - 192 * dy, rim_thickness, my_color_black);
}


static void COLORING_CLOTHES(const int base_x, const int base_y)
{
    //左下角
    const int m = base_x - offset_x + (win_width - size_width) / 2 + rim_thickness;
    const int n = base_y - offset_y + (win_height - size_height) / 2 + rim_thickness + size_height - rim_thickness;
   
    /* 画背景色 */
    //左手
    hdc_parallelogram(m + 275 * dx, n - 160 * dy, m + 255 * dx, n - 160 * dy, m + 240 * dx, n - 122 * dy, m + 260 * dx, n - 122 * dy, true, 2, my_color_face);
    hdc_triangle(m + 240 * dx, n - 122 * dy, m + 260 * dx, n - 122 * dy, m + 256 * dx, n - 114 * dy, true, 2, my_color_face);
    
    //左袖管
    FITTING_arc(m + 275 * dx, n - 160 * dy, m + 255 * dx, n - 160 * dy, m + 242 * dx, n - 170 * dy, rim_thickness, my_color_clothes, 2);

    //衣服
    hdc_triangle(m + 342 * dx, n - 141 * dy, m + 334 * dx, n - 75 * dy, m + 256 * dx, n - 114 * dy, true, 2, my_color_clothes);
    hdc_triangle(m + 342 * dx, n - 141 * dy, m + 256 * dx, n - 114 * dy, m + 275 * dx, n - 160 * dy, true, 2, my_color_clothes);
    hdc_triangle(m + 342 * dx, n - 141 * dy, m + 275 * dx, n - 160 * dy, m + 338 * dx, n - 205 * dy, true, 2, my_color_clothes);
    hdc_triangle(m + 338 * dx, n - 205 * dy, m + 285 * dx, n - 200 * dy, m + 275 * dx, n - 160 * dy, true, 2, my_color_clothes);
    hdc_triangle(m + 275 * dx, n - 160 * dy, m + 285 * dx, n - 200 * dy, m + 265 * dx, n - 186 * dy, true, 2, my_color_clothes);
    hdc_triangle(m + 285 * dx, n - 200 * dy, m + 285 * dx, n - 235 * dy, m + 242 * dx, n - 170 * dy, true, 2, my_color_clothes);
    hdc_triangle(m + 285 * dx, n - 200 * dy, m + 338 * dx, n - 205 * dy, m + 285 * dx, n - 235 * dy, true, 2, my_color_clothes);
    FITTING_arc(m + 292 * dx, n - 215 * dy, m + 285 * dx, n - 235 * dy, m + 300 * dx, n - 250 * dy, rim_thickness, my_color_clothes, 2);
    FITTING_arc(m + 340 * dx, n - 240 * dy, m + 350 * dx, n - 235 * dy, m + 355 * dx, n - 232 * dy, rim_thickness, my_color_clothes, 2);
    FITTING_arc(m + 355 * dx, n - 232 * dy, m + 360 * dx, n - 225 * dy, m + 372 * dx, n - 146 * dy, rim_thickness, my_color_clothes, 2);
    FITTING_arc(m + 372 * dx, n - 146 * dy, m + 352 * dx, n - 142 * dy, m + 332 * dx, n - 146 * dy, rim_thickness, my_color_clothes, 2);
    hdc_triangle(m + 340 * dx, n - 240 * dy, m + 324 * dx, n - 212 * dy, m + 285 * dx, n - 235 * dy, true, 2, my_color_clothes);
    hdc_triangle(m + 340 * dx, n - 240 * dy, m + 329 * dx, n - 250 * dy, m + 315 * dx, n - 231 * dy, true, 2, my_color_clothes);

    //脖子
    hdc_triangle(m + 300 * dx, n - 280 * dy, m + 322 * dx, n - 277 * dy, m + 315 * dx, n - 231 * dy, true, 2, my_color_face);
    hdc_triangle(m + 300 * dx, n - 280 * dy, m + 322 * dx, n - 277 * dy, m + 322 * dx, n - 240 * dy, true, 2, my_color_face);
    hdc_triangle(m + 315 * dx, n - 231 * dy, m + 298 * dx, n - 242 * dy, m + 300 * dx, n - 280 * dy, true, 2, my_color_face);
    hdc_triangle(m + 315 * dx, n - 231 * dy, m + 322 * dx, n - 240 * dy, m + 322 * dx, n - 277 * dy, true, 2, my_color_face);

    //右手
    hdc_triangle(m + 342 * dx, n - 141 * dy, m + 334 * dx, n - 75 * dy, m + 348 * dx, n - 110 * dy, true, 2, my_color_face);
    hdc_triangle(m + 342 * dx, n - 141 * dy, m + 362 * dx, n - 143 * dy, m + 348 * dx, n - 110 * dy, true, 2, my_color_face);
    hdc_triangle(m + 358 * dx, n - 63 * dy, m + 334 * dx, n - 75 * dy, m + 348 * dx, n - 110 * dy, true, 2, my_color_face);
    hdc_triangle(m + 362 * dx, n - 143 * dy, m + 358 * dx, n - 63 * dy, m + 348 * dx, n - 110 * dy, true, 2, my_color_face);
    
    /* 描线 */
    //左手
    hdc_line(m + 240 * dx, n - 122 * dy, m + 255 * dx, n - 160 * dy, rim_thickness, my_color_black);
    FITTING_arc(m + 285 * dx, n - 200 * dy, m + 270 * dx, n - 150 * dy, m + 256 * dx, n - 114 * dy, rim_thickness, my_color_black, 1);
    
    //左袖管
    FITTING_arc(m + 275 * dx, n - 160 * dy, m + 255 * dx, n - 160 * dy, m + 242 * dx, n - 170 * dy, rim_thickness, my_color_black, 1);
    FITTING_arc(m + 242 * dx, n - 170 * dy, m + 260 * dx, n - 203 * dy, m + 285 * dx, n - 235 * dy, rim_thickness, my_color_black, 1);
    FITTING_arc(m + 292 * dx, n - 215 * dy, m + 285 * dx, n - 235 * dy, m + 300 * dx, n - 250 * dy, rim_thickness, my_color_black, 1);
    
    //衣领
    hdc_line(m + 292 * dx, n - 215 * dy, m + 296 * dx, n - 233 * dy, rim_thickness, my_color_black);
    hdc_line(m + 296 * dx, n - 233 * dy, m + 303 * dx, n - 215 * dy, rim_thickness, my_color_black);
    hdc_line(m + 303 * dx, n - 215 * dy, m + 315 * dx, n - 231 * dy, rim_thickness, my_color_black);
    hdc_line(m + 315 * dx, n - 231 * dy, m + 324 * dx, n - 212 * dy, rim_thickness, my_color_black);
    hdc_line(m + 300 * dx, n - 250 * dy, m + 303 * dx, n - 250 * dy, rim_thickness, my_color_black);
    hdc_line(m + 315 * dx, n - 231 * dy, m + 298 * dx, n - 242 * dy, rim_thickness, my_color_black);
    hdc_line(m + 298 * dx, n - 242 * dy, m + 303 * dx, n - 250 * dy, rim_thickness, my_color_black);
    hdc_line(m + 324 * dx, n - 212 * dy, m + 340 * dx, n - 240 * dy, rim_thickness, my_color_black);
    hdc_line(m + 300 * dx, n - 250 * dy, m + 300 * dx, n - 280 * dy, rim_thickness, my_color_black);  //left neck
    hdc_line(m + 315 * dx, n - 231 * dy, m + 329 * dx, n - 250 * dy, rim_thickness, my_color_black);
    hdc_line(m + 329 * dx, n - 250 * dy, m + 340 * dx, n - 240 * dy, rim_thickness, my_color_black);
    hdc_line(m + 322 * dx, n - 240 * dy, m + 322 * dx, n - 277 * dy, rim_thickness, my_color_black);  //right neck
    hdc_line(m + 300 * dx, n - 280 * dy, m + 322 * dx, n - 277 * dy, rim_thickness, my_color_black);  //down neck

    //右边袖管
    FITTING_arc(m + 340 * dx, n - 240 * dy, m + 350 * dx, n - 235 * dy, m + 355 * dx, n - 232 * dy, rim_thickness, my_color_black, 1);
    FITTING_arc(m + 355 * dx, n - 232 * dy, m + 360 * dx, n - 225 * dy, m + 372 * dx, n - 146 * dy, rim_thickness, my_color_black, 1);
    FITTING_arc(m + 372 * dx, n - 146 * dy, m + 352 * dx, n - 142 * dy, m + 332 * dx, n - 146 * dy, rim_thickness, my_color_black, 1);
    hdc_line(m + 332 * dx, n - 146 * dy, m + 338 * dx, n - 205 * dy, rim_thickness, my_color_black);

    //右手
    hdc_line(m + 342 * dx, n - 141 * dy, m + 334 * dx, n - 75 * dy, rim_thickness, my_color_black);
    hdc_line(m + 362 * dx, n - 143 * dy, m + 358 * dx, n - 63 * dy, rim_thickness, my_color_black);
}

static void COLORING_HEAD(const int base_x, const int base_y)
{
    //左下角
    const int m = base_x - offset_x + (win_width - size_width) / 2 + rim_thickness;
    const int n = base_y - offset_y + (win_height - size_height) / 2 + rim_thickness + size_height - rim_thickness;

    //hdc_ellipse(500, 300, eye_width, eye_height, 180, true, 2, my_color_face);

    /* 描线 */
    //mouth
    FITTING_arc(m + 300 * dx, n - 280 * dy, m + 322 * dx, n - 277 * dy, m + 348 * dx, n - 270 * dy, rim_thickness, my_color_black, 1);
    FITTING_arc(m + 348 * dx, n - 270 * dy, m + 353 * dx, n - 266 * dy, m + 358 * dx, n - 262 * dy, rim_thickness, my_color_black, 1);
    hdc_line(m + 353 * dx, n - 267 * dy, m + 346 * dx, n - 272 * dy, rim_thickness, my_color_black);
    FITTING_arc(m + 377 * dx, n - 298 * dy, m + 376 * dx, n - 276 * dy, m + 353 * dx, n - 267 * dy, rim_thickness, my_color_black, 1);
    FITTING_arc(m + 345 * dx, n - 330 * dy, m + 361 * dx, n - 320 * dy, m + 377 * dx, n - 298 * dy, rim_thickness, my_color_black, 1);
    hdc_line(m + 360 * dx, n - 321 * dy, m + 366 * dx, n - 378 * dy, rim_thickness, my_color_black);
    hdc_line(m + 308 * dx, n - 278 * dy, m + 296 * dx, n - 280 * dy, rim_thickness, my_color_black);
    hdc_line(m + 296 * dx, n - 280 * dy, m + 245 * dx, n - 292 * dy, rim_thickness, my_color_black);
    FITTING_arc(m + 245 * dx, n - 292 * dy, m + 232 * dx, n - 305 * dy, m + 242 * dx, n - 325 * dy, rim_thickness, my_color_face, 2);
    FITTING_arc(m + 245 * dx, n - 292 * dy, m + 232 * dx, n - 305 * dy, m + 242 * dx, n - 325 * dy, rim_thickness, my_color_black, 1);


    //鼻子涂色 
    FITTING_arc(m + 244 * dx, n - 300 * dy, m + 244 * dx, n - 312 * dy, m + 290 * dx, n - 372 * dy, rim_thickness, my_color_face, 2);
    FITTING_arc(m + 299 * dx, n - 291 * dy, m + 276 * dx, n - 268 * dy, m + 244 * dx, n - 300 * dy, rim_thickness, my_color_face, 2);
    hdc_triangle(m + 296 * dx, n - 322 * dy, m + 299 * dx, n - 291 * dy, m + 245 * dx, n - 292 * dy, true, 2, my_color_face);
    hdc_triangle(m + 290 * dx, n - 372 * dy, m + 360 * dx, n - 321 * dy, m + 366 * dx, n - 378 * dy, true, 2, my_color_face);
    hdc_triangle(m + 290 * dx, n - 372 * dy, m + 295 * dx, n - 284 * dy, m + 359 * dx, n - 321 * dy, true, 2, my_color_face);
    hdc_triangle(m + 359 * dx, n - 321 * dy, m + 295 * dx, n - 284 * dy, m + 353 * dx, n - 271 * dy, true, 2, my_color_face);
    FITTING_arc(m + 377 * dx, n - 298 * dy, m + 376 * dx, n - 276 * dy, m + 353 * dx, n - 267 * dy, rim_thickness, my_color_face, 2);
    hdc_triangle(m + 359 * dx, n - 317 * dy, m + 374 * dx, n - 299 * dy, m + 345 * dx, n - 277 * dy, true, 2, my_color_face);

  //mouth && tongue
    //mouth
    hdc_triangle(m + 298 * dx, n - 312 * dy, m + 315 * dx, n - 293 * dy, m + 298 * dx, n - 294 * dy, true, rim_thickness, my_color_mouth);
    hdc_triangle(m + 298 * dx, n - 312 * dy, m + 315 * dx, n - 293 * dy, m + 344 * dx, n - 309 * dy, true, rim_thickness, my_color_mouth);
    hdc_triangle(m + 344 * dx, n - 309 * dy, m + 360 * dx, n - 284 * dy, m + 319 * dx, n - 293 * dy, true, rim_thickness, my_color_mouth);
    hdc_triangle(m + 342 * dx, n - 313 * dy, m + 360 * dx, n - 284 * dy, m + 359 * dx, n - 301 * dy, true, rim_thickness, my_color_mouth);

    //tongue
    hdc_line(m + 298 * dx, n - 319 * dy, m + 328 * dx, n - 321 * dy, rim_thickness, my_color_black);
    FITTING_arc(m + 328 * dx, n - 321 * dy, m + 349 * dx, n - 312 * dy, m + 365 * dx, n - 293 * dy, rim_thickness, my_color_black, 1);
    FITTING_arc(m + 365 * dx, n - 293 * dy, m + 363 * dx, n - 281 * dy, m + 353 * dx, n - 279 * dy, rim_thickness, my_color_black, 1);
    FITTING_arc(m + 301 * dx, n - 291 * dy, m + 325 * dx, n - 287 * dy, m + 353 * dx, n - 279 * dy, rim_thickness, my_color_black, 1);
    hdc_point(m + 352 * dx, n - 279 * dy, rim_thickness, my_color_black);
    //teeth
    hdc_triangle(m + 299 * dx, n - 314 * dy, m + 344 * dx, n - 311 * dy, m + 325 * dx, n - 318 * dy, true, rim_thickness, my_color_white);
    hdc_line(m + 299 * dx, n - 314 * dy, m + 349 * dx, n - 311 * dy, rim_thickness, my_color_black);


    FITTING_arc(m + 317 * dx, n - 292 * dy, m + 327 * dx, n - 298 * dy, m + 340 * dx, n - 290 * dy, rim_thickness, my_color_tongue, 1);
    FITTING_arc(m + 340 * dx, n - 290 * dy, m + 352 * dx, n - 295 * dy, m + 362 * dx, n - 284 * dy, rim_thickness, my_color_tongue, 1);
    hdc_triangle(m + 317 * dx, n - 292 * dy, m + 340 * dx, n - 290 * dy, m + 338 * dx, n - 288 * dy, true, 4, my_color_tongue);
    hdc_triangle(m + 317 * dx, n - 292 * dy, m + 327 * dx, n - 298 * dy, m + 340 * dx, n - 290 * dy, true, rim_thickness, my_color_tongue);
    hdc_triangle(m + 338 * dx, n - 288 * dy, m + 352 * dx, n - 295 * dy, m + 360 * dx, n - 284 * dy, true, rim_thickness, my_color_tongue);
    hdc_triangle(m + 352 * dx, n - 282 * dy, m + 354 * dx, n - 284 * dy, m + 330 * dx, n - 289 * dy, true, 2, my_color_tongue);

    //鼻子描边
    FITTING_arc(m + 244 * dx, n - 300 * dy, m + 244 * dx, n - 312 * dy, m + 290 * dx, n - 372 * dy, rim_thickness, my_color_black, 1);
    FITTING_arc(m + 299 * dx, n - 291 * dy, m + 276 * dx, n - 268 * dy, m + 244 * dx, n - 300 * dy, rim_thickness, my_color_black, 1);
    FITTING_arc(m + 296 * dx, n - 322 * dy, m + 298 * dx, n - 312 * dy, m + 299 * dx, n - 291 * dy, rim_thickness, my_color_black, 1);
    FITTING_arc(m + 293 * dx, n - 322 * dy, m + 290 * dx, n - 343 * dy, m + 298 * dx, n - 358 * dy, rim_thickness, my_color_black, 1);

    //右脸描边
    FITTING_arc(m + 377 * dx, n - 298 * dy, m + 376 * dx, n - 276 * dy, m + 353 * dx, n - 267 * dy, rim_thickness, my_color_black, 1);
    FITTING_arc(m + 345 * dx, n - 330 * dy, m + 361 * dx, n - 320 * dy, m + 377 * dx, n - 298 * dy, rim_thickness, my_color_black, 1);
    hdc_line(m + 360 * dx, n - 321 * dy, m + 366 * dx, n - 378 * dy, rim_thickness, my_color_black);

   //head
    hdc_line(m + 237 * dx, n - 398 * dy, m + 270 * dx, n - 363 * dy, rim_thickness, my_color_face);
    hdc_triangle(m + 237 * dx, n - 398 * dy, m + 270 * dx, n - 363 * dy, m + 405 * dx, n - 493 * dy, true, rim_thickness, my_color_face);
    hdc_triangle(m + 288 * dx, n - 373 * dy, m + 405 * dx, n - 493 * dy, m + 427 * dx, n - 468 * dy, true, rim_thickness, my_color_face);
    hdc_triangle(m + 288 * dx, n - 373 * dy, m + 270 * dx, n - 363 * dy, m + 329 * dx, n - 442 * dy, true, 1, my_color_face);
    hdc_triangle(m + 314 * dx, n - 377 * dy, m + 217 * dx, n - 460 * dy, m + 338 * dx, n - 513 * dy, true, rim_thickness, my_color_face);
    FITTING_arc(m + 437 * dx, n - 419 * dy, m + 404 * dx, n - 382 * dy, m + 367 * dx, n - 373 * dy, rim_thickness, my_color_face, 2);
    FITTING_arc(m + 405 * dx, n - 495 * dy, m + 427 * dx, n - 472 * dy, m + 437 * dx, n - 419 * dy, rim_thickness, my_color_face, 2);
    FITTING_arc(m + 261 * dx, n - 507 * dy, m + 320 * dx, n - 520 * dy, m + 405 * dx, n - 495 * dy, rim_thickness, my_color_face, 2);
    FITTING_arc(m + 210 * dx, n - 442 * dy, m + 228 * dx, n - 483 * dy, m + 261 * dx, n - 507 * dy, rim_thickness, my_color_face, 2);
    FITTING_arc(m + 237 * dx, n - 398 * dy, m + 218 * dx, n - 415 * dy, m + 210 * dx, n - 442 * dy, rim_thickness, my_color_face, 2);
    hdc_triangle(m + 244 * dx, n - 397 * dy, m + 362 * dx, n - 381 * dy, m + 343 * dx, n - 511 * dy, true, rim_thickness, my_color_face);
    hdc_triangle(m + 294 * dx, n - 375 * dy, m + 294 * dx, n - 511 * dy, m + 360 * dx, n - 365 * dy, true, rim_thickness, my_color_face);
    hdc_triangle(m + 342 * dx, n - 378 * dy, m + 391 * dx, n - 397 * dy, m + 310 * dx, n - 513 * dy, true, rim_thickness, my_color_face);
    hdc_triangle(m + 241 * dx, n - 402 * dy, m + 238 * dx, n - 450 * dy, m + 430 * dx, n - 430 * dy, true, rim_thickness, my_color_face);
    hdc_triangle(m + 351 * dx, n - 371 * dy, m + 385 * dx, n - 393 * dy, m + 312 * dx, n - 458 * dy, true, rim_thickness, my_color_face);


    FITTING_arc(m + 437 * dx, n - 419 * dy, m + 404 * dx, n - 382 * dy, m + 367 * dx, n - 373 * dy, rim_thickness, my_color_black, 1);
    FITTING_arc(m + 405 * dx, n - 495 * dy, m + 427 * dx, n - 472 * dy, m + 437 * dx, n - 419 * dy, rim_thickness, my_color_black, 1);
    FITTING_arc(m + 261 * dx, n - 507 * dy, m + 320 * dx, n - 520 * dy, m + 405 * dx, n - 495 * dy, rim_thickness, my_color_black, 1);
    FITTING_arc(m + 210 * dx, n - 442 * dy, m + 228 * dx, n - 483 * dy, m + 261 * dx, n - 507 * dy, rim_thickness, my_color_black, 1);
    FITTING_arc(m + 237 * dx, n - 398 * dy, m + 218 * dx, n - 415 * dy, m + 210 * dx, n - 442 * dy, rim_thickness, my_color_black, 1);

    //头顶的点
    hdc_point(m + 240 * dx, n - 482 * dy, 6, my_color_spot);
    hdc_point(m + 265 * dx, n - 497 * dy, 6, my_color_spot);
    hdc_point(m + 287 * dx, n - 507 * dy, 6, my_color_spot);
    hdc_point(m + 302 * dx, n - 495 * dy, 6, my_color_spot);
    hdc_point(m + 321 * dx, n - 510 * dy, 6, my_color_spot);
    hdc_point(m + 343 * dx, n - 491 * dy, 6, my_color_spot);
    hdc_point(m + 368 * dx, n - 499 * dy, 6, my_color_spot);
    hdc_point(m + 398 * dx, n - 479 * dy, 6, my_color_spot);

    //皱纹
    FITTING_arc(m + 266 * dx, n - 478 * dy, m + 277 * dx, n - 482 * dy, m + 298 * dx, n - 486 * dy, rim_thickness, my_color_black, 1);
    FITTING_arc(m + 298 * dx, n - 486 * dy, m + 309 * dx, n - 487 * dy, m + 330 * dx, n - 480 * dy, rim_thickness, my_color_black, 1);
    FITTING_arc(m + 328 * dx, n - 480 * dy, m + 334 * dx, n - 478 * dy, m + 355 * dx, n - 468 * dy, rim_thickness, my_color_black, 1);
    FITTING_arc(m + 259 * dx, n - 458 * dy, m + 275 * dx, n - 466 * dy, m + 292 * dx, n - 471 * dy, rim_thickness, my_color_black, 1);
    FITTING_arc(m + 292 * dx, n - 471 * dy, m + 301 * dx, n - 472 * dy, m + 324 * dx, n - 466 * dy, rim_thickness, my_color_black, 1);
    FITTING_arc(m + 374 * dx, n - 445 * dy, m + 349 * dx, n - 452 * dy, m + 322 * dx, n - 465 * dy, rim_thickness, my_color_black, 1);
    hdc_line(m + 258 * dx, n - 444 * dy, m + 287 * dx, n - 457 * dy, rim_thickness, my_color_black);
    FITTING_arc(m + 287 * dx, n - 457 * dy, m + 300 * dx, n - 459 * dy, m + 314 * dx, n - 455 * dy, rim_thickness, my_color_black, 1);
    hdc_line(m + 314 * dx, n - 455 * dy, m + 342 * dx, n - 439 * dy, rim_thickness, my_color_black);

   //眼睛
    //眼球上色
    FITTING_arc(m + 262 * dx, n - 352 * dy, m + 247 * dx, n - 369 * dy, m + 243 * dx, n - 389 * dy, rim_thickness, my_color_eyes, 2);
    hdc_triangle(m + 288 * dx, n - 394 * dy, m + 295 * dx, n - 384 * dy, m + 248 * dx, n - 387 * dy, true, rim_thickness, my_color_eyes);
    hdc_triangle(m + 288 * dx, n - 385 * dy, m + 294 * dx, n - 374 * dy, m + 267 * dx, n - 367 * dy, true, rim_thickness, my_color_eyes);
    FITTING_arc(m + 323 * dx, n - 345 * dy, m + 297 * dx, n - 363 * dy, m + 289 * dx, n - 398 * dy, rim_thickness, my_color_eyes, 2);
    hdc_triangle(m + 330 * dx, n - 393 * dy, m + 322 * dx, n - 348 * dy, m + 352 * dx, n - 382 * dy, true, rim_thickness, my_color_eyes);
    hdc_triangle(m + 352 * dx, n - 382 * dy, m + 322 * dx, n - 348 * dy, m + 340 * dx, n - 355 * dy, true, rim_thickness, my_color_eyes);
    hdc_parallelogram(m + 268 * dx, n - 396 * dy, m + 268 * dx, n - 386 * dy, m + 280 * dx, n - 386 * dy, m + 280 * dx, n - 396 * dy, true, 2, my_color_hole);
    hdc_square(m + 298 * dx, n - 398 * dy, 11, 0, true, 2, my_color_hole);
    hdc_triangle(m + 298 * dx, n - 387 * dy, m + 309 * dx, n - 387 * dy, m + 310 * dx, n - 385 * dy, true, 2, my_color_hole);

    //眼球描边
    FITTING_arc(m + 262 * dx, n - 352 * dy, m + 247 * dx, n - 369 * dy, m + 243 * dx, n - 389 * dy, rim_thickness, my_color_black, 1);
    FITTING_arc(m + 323 * dx, n - 345 * dy, m + 297 * dx, n - 363 * dy, m + 289 * dx, n - 398 * dy, rim_thickness, my_color_black, 1);
    FITTING_arc(m + 354 * dx, n - 381 * dy, m + 345 * dx, n - 360 * dy, m + 323 * dx, n - 345 * dy, rim_thickness, my_color_black, 1);

    //眼皮上色
    hdc_triangle(m + 242 * dx, n - 393 * dy, m + 250 * dx, n - 419 * dy, m + 287 * dx, n - 401 * dy, true, 2, my_color_lips);
    hdc_triangle(m + 250 * dx, n - 419 * dy, m + 287 * dx, n - 401 * dy, m + 290 * dx, n - 424 * dy, true, 2, my_color_lips);
    hdc_triangle(m + 250 * dx, n - 419 * dy, m + 279 * dx, n - 434 * dy, m + 290 * dx, n - 424 * dy, true, 2, my_color_lips);
    hdc_triangle(m + 289 * dx, n - 400 * dy, m + 296 * dx, n - 420 * dy, m + 356 * dx, n - 386 * dy, true, 2, my_color_lips);
    hdc_triangle(m + 310 * dx, n - 436 * dy, m + 296 * dx, n - 420 * dy, m + 356 * dx, n - 386 * dy, true, 2, my_color_lips);
    hdc_triangle(m + 310 * dx, n - 436 * dy, m + 345 * dx, n - 416 * dy, m + 355 * dx, n - 391 * dy, true, 2, my_color_lips);

    //眼皮描边
    FITTING_arc(m + 284 * dx, n - 441 * dy, m + 266 * dx, n - 427 * dy, m + 241 * dx, n - 422 * dy, rim_thickness, my_color_black, 1);
    FITTING_arc(m + 365 * dx, n - 416 * dy, m + 330 * dx, n - 424 * dy, m + 306 * dx, n - 442 * dy, rim_thickness, my_color_black, 1);
    FITTING_arc(m + 242 * dx, n - 399 * dy, m + 244 * dx, n - 411 * dy, m + 249 * dx, n - 421 * dy, rim_thickness, my_color_black, 1);
    FITTING_arc(m + 237 * dx, n - 396 * dy, m + 244 * dx, n - 399 * dy, m + 253 * dx, n - 402 * dy, rim_thickness, my_color_black, 1);
    hdc_line(m + 237 * dx, n - 396 * dy, m + 240 * dx, n - 390 * dy, rim_thickness, my_color_black);
    FITTING_arc(m + 240 * dx, n - 390 * dy, m + 290 * dx, n - 399 * dy, m + 357 * dx, n - 382 * dy, rim_thickness, my_color_black, 1);
    FITTING_arc(m + 284 * dx, n - 435 * dy, m + 290 * dx, n - 428 * dy, m + 295 * dx, n - 419 * dy, rim_thickness, my_color_black, 1);
    FITTING_arc(m + 290 * dx, n - 405 * dy, m + 293 * dx, n - 419 * dy, m + 309 * dx, n - 437 * dy, rim_thickness, my_color_black, 1);
    hdc_line(m + 275 * dx, n - 405 * dy, m + 304 * dx, n - 405 * dy, rim_thickness, my_color_black);
    hdc_line(m + 290 * dx, n - 405 * dy, m + 286 * dx, n - 400 * dy, rim_thickness, my_color_black);
    FITTING_arc(m + 350 * dx, n - 416 * dy, m + 354 * dx, n - 406 * dy, m + 356 * dx, n - 392 * dy, rim_thickness, my_color_black, 1);
    FITTING_arc(m + 341 * dx, n - 397 * dy, m + 352 * dx, n - 394 * dy, m + 360 * dx, n - 387 * dy, rim_thickness, my_color_black, 1);
    hdc_line(m + 360 * dx, n - 387 * dy, m + 355 * dx, n - 380 * dy, rim_thickness, my_color_black);


    FITTING_arc(m + 244 * dx, n - 300 * dy, m + 244 * dx, n - 312 * dy, m + 290 * dx, n - 372 * dy, rim_thickness, my_color_black, 1);
}

static void IMAGE_EDGE(const int base_x, const int base_y)
{
	const int left_up_x = base_x - offset_x + (win_width - size_width) / 2;
	const int left_up_y = base_y - offset_y + (win_height - size_height) / 2;

	hdc_rectangle(left_up_x, left_up_y, size_width, size_height, 0, false, rim_thickness, my_color_black);
}

void hdc_draw_cartoon_2353626(const int base_x, const int base_y)
{
	hdc_cls();
	
	//上色
	COLORING_BACKGROUND(base_x, base_y);
	COLORING_WINDOW(base_x, base_y);
    COLORING_DOOR(base_x, base_y);
    COLORING_DESK(base_x, base_y);
    COLORING_CLOTHES(base_x, base_y);
    COLORING_HEAD(base_x, base_y);

	IMAGE_EDGE(base_x, base_y);

}