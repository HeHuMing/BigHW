/* 2353626 贺胡鸣 大数据 */
#include <conio.h>  
#include <math.h>
using namespace std;

#include "../include/cmd_hdc_tools.h"

//PI值
const double PI = 3.1415926;
//参考点坐标
const int center_x = 400;
const int center_y = 300;
//初始规模大小
const double scale = 25;
//最大最小变化比率
const int max_k = 5;
const int min_k = 1;

void hdc_draw_Mathematical_curve_2353626()
{
	hdc_cls();

	// 计算时需要用到的变量
	char ch;
	int k = 2;

	while (1)
	{
		hdc_cls();
		cct_showstr(2, 1, "<蝴蝶曲线>", 7, 0);
		cct_showstr(2, 2, "[按 a 变大；按 d 变小；按 c 退出] ", 7, 0);

		for (double theta = 0; theta <= 24 * PI; theta += 0.001) {
			double r = exp(cos(theta)) - 2 * cos(4 * theta) + pow(sin(theta / 12), 5);

			if (k >= min_k && k <= max_k) {
				r *= k;
			}
			else if (k < min_k)
				k = min_k;
			else if (k > max_k)
				k = max_k;

			int x = center_x + (int)(r * cos(theta) * scale);
			int y = center_y + (int)(r * sin(theta) * scale);

			int segment = (int)(theta / (2 * PI)) % 6;
			int color;
			switch (segment) {
			case 0: color = 0xFF0000; break; // 红
			case 1: color = 0xFF7F00; break; // 橙
			case 2: color = 0xFFFF00; break; // 黄
			case 3: color = 0x00FF00; break; // 绿
			case 4: color = 0x0000FF; break; // 蓝
			case 5: color = 0x8B00FF; break; // 紫
			default: color = 0xFFFFFF;
			}

			hdc_point(x, y, 1, color);
		}

		// 调节蝴蝶大小
		int loop = 1;
		while (loop)
		{
			ch = _getch();
			switch (ch)
			{
			case 'a':  //变大
				k++;
				loop = 0;
				break;
			case 'd':  //变小
				k--;
				loop = 0;
				break;
			case 'c':  //按C退出
				return;
			default:
				continue;
				break;
			}
		}
	}
}