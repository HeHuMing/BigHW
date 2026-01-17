/*  1234567  计科  测试  */
#include <conio.h>  
#include <math.h>
using namespace std;

#include "../include/cmd_hdc_tools.h"

//参考点坐标（左侧下方线的中心）
const int reference_center_x = 150;
const int reference_center_y = 650;
//参考线半长
const double radius = sqrt(5) * 50;
// 2\1\根号5-三角形的最小角 - 偏差角；底部参考直线与竖直的夹角.
const double reference_angle = 26.565 - 0.05 * 13;
// 两参考线的x向偏差，正的在右侧
const int reference_cross_x = 500;
// 两参考线的y向偏差，正的在下侧
const int reference_cross_y = -50;
// 马鞍面的两侧边与两参考线的高差，正为上
const int reference_height_above = 400;
// 边的等分数
const int N = 15;

#define ANGLE_RESTRICT  1  // 转角限制

/*
	数学曲线名称：直纹面 - 双曲抛物面 - 马鞍面 （参考线，即投影线）.
	操作方式：可扭曲 按 a , 趋平；按 d , 趋竖；按 c , 退出.（若将转角限制改为0，即可放开，体验长按动画）.
	注：受电脑性能导致的画线速度的影响，在点阵字体下长按可能导致内部线的延时产生，想要体验极致的丝滑，请移步新宋体！
 */
void hdc_draw_Mathematical_curve_1234567()
{
	hdc_cls();

	// 八个基准点的坐标
	double left_lower_x1, left_lower_y1, left_lower_x2, left_lower_y2;
	double right_lower_x1, right_lower_y1, right_lower_x2, right_lower_y2;
	double left_high_x1, left_high_y1, left_high_x2, left_high_y2;
	double right_high_x1, right_high_y1, right_high_x2, right_high_y2;

	// 计算时需要用到的变量
	double x1, x2, y1, y2;
	double my_angle = 0;
	char ch;

	while (1)
	{
		hdc_cls();
		cct_showstr(2, 1, "<可扭曲的马鞍面>", 7, 0);
		cct_showstr(2, 2, "[按 a 趋平；按 d 趋竖；按 c 退出] ", 7, 0);

		// 底部投影
		left_lower_x1 = reference_center_x - radius * sin(reference_angle + my_angle);
		left_lower_y1 = reference_center_y - radius * cos(reference_angle + my_angle);
		left_lower_x2 = reference_center_x + radius * sin(reference_angle + my_angle);
		left_lower_y2 = reference_center_y + radius * cos(reference_angle + my_angle);
		right_lower_x1 = left_lower_x1 + reference_cross_x;
		right_lower_y1 = left_lower_y1 + reference_cross_y;
		right_lower_x2 = left_lower_x2 + reference_cross_x;
		right_lower_y2 = left_lower_y2 + reference_cross_y;

		hdc_line((int)left_lower_x1, (int)left_lower_y1, (int)left_lower_x2, (int)left_lower_y2, 1);
		hdc_line((int)right_lower_x1, (int)right_lower_y1, (int)right_lower_x2, (int)right_lower_y2, 1);
		hdc_line((int)left_lower_x1, (int)left_lower_y1, (int)right_lower_x1, (int)right_lower_y1, 1);
		hdc_line((int)left_lower_x2, (int)left_lower_y2, (int)right_lower_x2, (int)right_lower_y2, 1);

		// 顶部马鞍面
		left_high_x1 = left_lower_x1;
		left_high_y1 = left_lower_y1 - reference_height_above;
		left_high_x2 = left_lower_x2;
		left_high_y2 = left_lower_y2 - reference_height_above;
		right_high_x1 = right_lower_x1;
		right_high_y1 = right_lower_y2 - reference_height_above;
		right_high_x2 = right_lower_x2;
		right_high_y2 = right_lower_y1 - reference_height_above;

		// 可见粗线
		hdc_line((int)left_high_x1, (int)left_high_y1, (int)left_high_x2, (int)left_high_y2, 3);
		hdc_line((int)right_high_x1, (int)right_high_y1, (int)right_high_x2, (int)right_high_y2, 3 - ANGLE_RESTRICT);
		hdc_line((int)left_high_x2, (int)left_high_y2, (int)right_high_x2, (int)right_high_y2, 3);
		// 远处细线
		hdc_line((int)left_high_x1, (int)left_high_y1, (int)right_high_x1, (int)right_high_y1, 1);

		// 内部细线
		for (int i = 1; i < N; ++i)
		{
			x1 = left_high_x1 + i * (left_high_x2 - left_high_x1) / N;
			y1 = left_high_y1 + i * (left_high_y2 - left_high_y1) / N;
			x2 = right_high_x1 + i * (right_high_x2 - right_high_x1) / N;
			y2 = right_high_y1 + i * (right_high_y2 - right_high_y1) / N;
			hdc_line((int)x1, (int)y1, (int)x2, (int)y2, 1);

			x1 = left_high_x2 + i * (right_high_x2 - left_high_x2) / N;
			y1 = left_high_y2 + i * (right_high_y2 - left_high_y2) / N;
			x2 = left_high_x1 + i * (right_high_x1 - left_high_x1) / N;
			y2 = left_high_y1 + i * (right_high_y1 - left_high_y1) / N;
			hdc_line((int)x1, (int)y1, (int)x2, (int)y2, 1);
		}

		// 扭曲该面
		int loop = 1;
		while (loop)
		{
			ch = _getch();
			switch (ch)
			{
			case 'a':
#if ANGLE_RESTRICT
				if (my_angle >= 0.05 * 6) // 向左最大转角，转角限制改为0可放开限制，体验长按动画
					continue;
#endif
				my_angle += 0.05;
				loop = 0;
				break;
			case 'd':
#if ANGLE_RESTRICT
				if (my_angle <= -0.05 * 9) // 向右最大转角，转角限制改为0可放开限制，体验长按动画
					continue;
#endif
				my_angle -= 0.05;
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
