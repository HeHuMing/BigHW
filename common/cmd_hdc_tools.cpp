/* 2353626 贺胡鸣 大数据 */

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <Windows.h>
#include "../include/cmd_hdc_tools.h"
using namespace std;

#if !HDC_SERIES_BY_TEACHER	//未定义的情况下才打开条件编译

/* 允许添加需要的头文件 */
#include <cmath>

extern "C" WINBASEAPI HWND WINAPI GetConsoleWindow(); //VS中此处会有波浪线，不影响正常使用

/* 已定义的静态全局变量 */
static HWND hWnd = GetConsoleWindow();
static HDC hdc = NULL;
static const double PI = 3.14159;
static int _BgColor_, _FgColor_, _Width_, _High;
static int _Delay_ms;

/* 允许适度使用静态全局变量，但仅限static，即外部不可见 */
static const int MAX_EDGES = 200;
static const int MAX_INTERSECTIONS = 20;
static const int maxHeight = 2003;

struct point {
	int x, y;
};

/* 此处允许添加自定义函数，但仅限static，即外部不可见 */
/***************************************************************************
  函数名称：getAngle
  功    能：求两点间角度
  输入参数：int angle, int angle0 = 0, bool isClockwise = 1
  返 回 值：int
  说    明：分为顺时针逆时针考虑
***************************************************************************/
static int getAngle(int angle, int angle0 = 0, bool isClockwise = 1) {
	while (angle < 0)
		angle += 360;

	if (isClockwise)
		return (angle0 + angle) % 360;
	else
		return (angle0 - angle + 360) % 360;
}

/***************************************************************************
  函数名称：getDistance
  功    能：求两点间距离
  输入参数：double x1, double y1, double x2, double y2
  返 回 值：double
  说    明：
***************************************************************************/
static double getDistance(double x1, double y1, double x2, double y2)
{
	return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

/***************************************************************************
  函数名称：normalize_angle
  功    能：把角度调整到0-360°的范围内
  输入参数：const int angle
  返 回 值：double
  说    明：
***************************************************************************/
double normalize_angle(const int angle) {
	double normalized = fmod(angle, 360.0);
	if (normalized < 0) {
		normalized += 360.0;
	}
	return normalized;
}

/***************************************************************************
  函数名称：in_ellipse
  功    能：判断这个点是否在椭圆内部
  输入参数：int x,int y,int x0,int y0,int a,int b,double sint,double cost
  返 回 值：bool
  说    明：
***************************************************************************/
bool in_ellipse(int x, int y, int x0, int y0, int a, int b, double sint, double cost)
{
	double t1 = (x - x0) * cost + (y - y0) * sint, t2 = -(x - x0) * sint + (y - y0) * cost;
	if ((t1 * t1) / (a * a) + (t2 * t2) / (b * b) <= 1)
		return true;
	return false;
}


static inline void hdc_base_point(const int x, const int y);
static inline void hdc_base_line(const int x1, const int y1, const int x2, const int y2);
/***************************************************************************
  函数名称：fillsector
  功    能：用扫描水平线的方式填充扇形内部
  输入参数：int N, point p[], int RGB_value
  返 回 值：/
  说    明：
***************************************************************************/
static void fillsector(int N, point p[], int RGB_value)
{
	if (RGB_value != INVALID_RGB)
		hdc_set_pencolor(RGB_value);
	int MAXY = 0, MINY = INT_MAX;
	for (int i = 0; i < N; i++) {
		if (p[i].y > MAXY)
			MAXY = p[i].y;
		if (p[i].y < MINY)
			MINY = p[i].y;
	}
	struct NET {
		int ymax;
		double x;
		double dx;
		NET* next;
	}*pNET[maxHeight], * pAET = new(nothrow) NET;
	if (!pAET)
		return;
	for (int i = 0; i < maxHeight; i++)
	{
		pNET[i] = new(nothrow) NET;
		if (!pNET[i]) {
			delete pAET;
			for (int j = 0; j < i; j++)
				delete pNET[j];
			return;
		}
		pNET[i]->next = NULL;
	}
	pAET->next = NULL;

	for (int i = 0; i < N; i++) {
		if (p[i].y < p[(i - 1 + N) % N].y) {
			NET* newEdge = new(nothrow) NET;
			if (!newEdge) {
				delete pAET;
				for (int j = 0; j < maxHeight; j++)
					delete pNET[j];
				return;
			}
			newEdge->ymax = p[(i - 1 + N) % N].y;
			newEdge->x = p[i].x;
			newEdge->dx = double(p[(i - 1 + N) % N].x - p[i].x) / (p[(i - 1 + N) % N].y - p[i].y);
			newEdge->next = pNET[p[i].y - MINY]->next;
			pNET[p[i].y - MINY]->next = newEdge;
		}
		if (p[i].y < p[(i + 1) % N].y) {
			NET* newEdge = new(nothrow) NET;
			if (!newEdge) {
				delete pAET;
				for (int j = 0; j < maxHeight; j++)
					delete pNET[j];
				return;
			}
			newEdge->ymax = p[(i + 1) % N].y;
			newEdge->x = p[i].x;
			newEdge->dx = double(p[(i + 1) % N].x - p[i].x) / (p[(i + 1) % N].y - p[i].y);
			newEdge->next = pNET[p[i].y - MINY]->next;
			pNET[p[i].y - MINY]->next = newEdge;
		}
	}

	for (int i = 0; i <= MAXY - MINY; i++) {
		NET* p = pAET;
		while (p->next) {
			while (p->next->ymax == i + MINY) {
				NET* temp = p->next;
				p->next = p->next->next;
				delete temp;
				if (p->next == NULL)
					break;
			}
			if (p->next == NULL)
				break;
			p->next->x += p->next->dx;
			p = p->next;
		}
		for (p = pAET; p && p->next; p = p->next) {
			NET* q = pAET;
			while (q != p) {
				if (q->next->x > p->next->x)
					break;
				q = q->next;
			}
			if (q != p) {
				NET* temp = p->next;
				p->next = p->next->next;
				temp->next = q->next;
				q->next = temp;
			}
		}

		p = pNET[i];
		while (p->next) {
			NET* temp = p->next;
			p->next = p->next->next;
			NET* q = pAET;
			while (q->next) {
				if (q->next->x > temp->x)
					break;
				q = q->next;
			}
			temp->next = q->next;
			q->next = temp;
		}

		p = pAET->next;
		while (p && p->next) {
			hdc_base_line((int)p->x, i + MINY, (int)p->next->x, i + MINY);
			p = p->next->next;
		}
	}
	for (int i = 0; i < maxHeight; i++) {
		delete pNET[i];
	}
	while (pAET->next) {
		NET* temp = pAET->next;
		pAET->next = pAET->next->next;
		delete temp;
	}
	delete pAET;
}

/* 下面给出了几个基本函数的完整实现，不要改动 */
/***************************************************************************
  函数名称：
  功    能：初始化
  输入参数：const int bgcolor：背景色
			const int fgcolor：前景色
			const int width  ：屏幕宽度（点阵）
			const int high   ：屏幕高度（点阵）
  返 回 值：
  说    明：
***************************************************************************/
void hdc_init(const int bgcolor, const int fgcolor, const int width, const int high)
{
	/* 先释放，防止不release而再次init（hdc_release可重入） */
	hdc_release();

	/* 窗口init后，用一个静态全局量记录，后续hdc_cls()会用到 */
	_BgColor_ = bgcolor;
	_FgColor_ = fgcolor;
	_Width_ = width;
	_High = high;

	hdc = GetDC(hWnd);

	cct_setcursor(CCT_CURSOR_INVISIBLE);
	cct_setcolor(bgcolor, fgcolor);
	cct_setfontsize("点阵字体", 16);
	cct_setconsoleborder(width / 8 + !!(width % 8), high / 16 + !!(high % 16)); //将点阵的宽度及高度转换为特定字体的行列数，!!的含义：如果不是8/16的倍数，行列多+1
	cct_cls();
}

/***************************************************************************
  函数名称：
  功    能：释放画图资源
  输入参数：
  返 回 值：
  说    明：可重入
***************************************************************************/
void hdc_release()
{
	if (hdc) {
		ReleaseDC(hWnd, hdc);
		hdc = NULL;
		cct_setcursor(CCT_CURSOR_VISIBLE_NORMAL);
	}
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void hdc_set_delay(int ms)
{
	if (ms > 0)
		_Delay_ms = ms;
	else
		_Delay_ms = 0;
}

/***************************************************************************
  函数名称：
  功    能：设置画笔颜色，传入RGB值
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void hdc_set_pencolor(const int RGB_value)
{
	SelectObject(hdc, GetStockObject(DC_PEN));
	SetDCPenColor(hdc, RGB_value);
}

/***************************************************************************
  函数名称：
  功    能：设置画笔颜色，传入RGB三色，值0-255
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void hdc_set_pencolor(const unsigned char red, const unsigned char green, const unsigned char blue)
{
	hdc_set_pencolor(RGB(red, green, blue));
}

/***************************************************************************
  函数名称：
  功    能：清除屏幕上现有的图形
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void hdc_cls()
{
	/* 发现一定要换一种颜色初始化才能清除像素点，找到更简便方法的同学可以通知我 */
	hdc_init(_BgColor_, (_FgColor_ + 1) % 16, _Width_, _High);

	hdc_init(_BgColor_, _FgColor_, _Width_, _High);

	/* 部分机器上运行demo时，会出现hdc_cls()后第一根线有缺线的情况，加延时即可
	   如果部分机器运行还有问题，调高此延时值 */
	Sleep(30);
}

/***************************************************************************
  函数名称：
  功    能：在(x,y)位置处用指定颜色画出一个像素点
  输入参数：const int x：x坐标，左上角为(0,0)
			const int y：y坐标，左上角为(0,0)
  返 回 值：
  说    明：颜色直接用当前设定，可以在调用处指定
***************************************************************************/
static inline void hdc_base_point(const int x, const int y)
{
	MoveToEx(hdc, x - 1, y - 1, NULL);
	LineTo(hdc, x, y);
	if (_Delay_ms > 0)
		Sleep(_Delay_ms);
}

/***************************************************************************
  函数名称：
  功    能：在(x1,y1)-(x2,y2)之间画出一个像素点的连线
  输入参数：const int x1：起点x坐标，左上角为(0,0)
			const int y1：起点y坐标，左上角为(0,0)
			const int x2：终点y坐标，左上角为(0,0)
			const int y2：终点y坐标，左上角为(0,0)
  返 回 值：
  说    明：颜色直接用当前设定
***************************************************************************/
static inline void hdc_base_line(const int x1, const int y1, const int x2, const int y2)
{
	MoveToEx(hdc, x1, y1, NULL);
	LineTo(hdc, x2, y2);
	if (_Delay_ms > 0)
		Sleep(_Delay_ms);
}

/***************************************************************************
  函数名称：
  功    能：在(x,y)位置处用指定颜色画一个指定粗细的点(用画实心圆来模拟)
  输入参数：const int x			：x坐标，左上角为(0,0)
			const int y			：y坐标，左上角为(0,0)
			const int thickness	：点的粗细，下限位1，上限不限(如过大会导致不完全填充)（有缺省值）
			const int RGB_value	：点的颜色（有缺省值）
  返 回 值：
  说    明：改进运行效率
***************************************************************************/
void hdc_point(const int x, const int y, const int thickness, const int RGB_value)
{
	int tn = thickness;
	if (tn < 1)
		tn = 1;		//处理下限
	/* 不卡点直径的上限，但是单上限超过一定大小是，画出的圆部分位置未实心 */

	const int tn_end = (tn <= 1) ? 1 : tn / 2;

	int angle, level;
	int old_x1 = INT_MAX, old_y1 = INT_MAX, x1, y1;
	int count = 0;

	if (RGB_value != INVALID_RGB)
		hdc_set_pencolor(RGB_value);

	/* 用画圆的方式模拟粗点 */
	for (level = 1; level <= tn_end; level++) {
		for (angle = 0; angle <= 360; angle++) {
			/* 注意，系统的坐标轴，0°在圆的最下方 */
			x1 = x + (int)(level * sin(angle * PI / 180));
			y1 = y - (int)(level * cos(angle * PI / 180));

			/* 当半径很小时，角度变化不会引起int型的x1/y1变化，因此加判断语句，避免重复画同一像素点 */
			if (x1 != old_x1 || y1 != old_y1) {
				old_x1 = x1;
				old_y1 = y1;
				hdc_base_point(x1, y1);
				++count;
			}
		}
	}
#if 0
	/* 放开此注释，可以看到一个粗点用了几个像素点组成 */
	cct_gotoxy(0, 41);
	printf("count=%d           ", count);
	getchar();
#endif
}

/* -------------------------------------------------------------------------
	给出下列函数的实现，函数名及参数表不准改动
	1、不需要调用系统的其他有关图形操作的函数
	2、下列函数的实现都基于hdc_base_point/hdc_base_line函数的组合
	3、想一想，是不是所有函数都需要给出独立的实现过程？应该先实现哪些函数？
	4、填充是实现中最复杂的部分
	5、系统的角度与函数参数中的角度含义相差180°
   ------------------------------------------------------------------------- */

   /***************************************************************************
	 函数名称：hdc_line
	 功    能：画线段
	 输入参数：const int x1		：起点的x
			   const int y1		：起点的y
			   const int x2		：终点的x
			   const int y2		：终点的y
			   const int thickness	：线段的粗细（有缺省值）
			   const int RGB_value	：线段的颜色（有缺省值）
	 返 回 值：
	 说    明：本函数可以用基于hdc_point的组合（速度慢）
					 也可以用hdc_base_point/hdc_base_line的组合（速度快）
   ***************************************************************************/
void hdc_line(const int x1, const int y1, const int x2, const int y2, const int thickness, const int RGB_value)
{
	if (RGB_value != INVALID_RGB)
		hdc_set_pencolor(RGB_value);
	if (thickness <= 1)
		hdc_base_line(x1, y1, x2, y2);
	else {
		double tn = (thickness == 0) ? 0.5 : (double)thickness / 2;
		double dis = getDistance(x1, y1, x2, y2);
		double angle = atan2(y2 - y1, x2 - x1);
		double nowx, nowy, prex, prey;
		for (int x = int(x1 - tn); x <= x1 + tn; x++) {
			for (int y = int(y1 - tn); y <= y1 + tn; y++) {
				if (getDistance(x, y, x1, y1) <= tn)
					hdc_base_point(x, y);
			}
		}
		prex = x1;
		prey = y1;
		for (int s = 0; s <= dis; s++) {
			nowx = x1 + s * cos(angle);
			nowy = y1 + s * sin(angle);
			for (int x = int(nowx - tn); x <= int(nowx + tn); x++) {
				for (int y = (int)(nowy - tn); y <= (int)(nowy + tn); y++) {
					if (getDistance(x, y, nowx, nowy) <= tn && getDistance(x, y, prex, prey) > tn)
						hdc_base_point(x, y);
				}
			}
			prex = nowx;
			prey = nowy;
		}
	}
}

/***************************************************************************
  函数名称：hdc_triangle
  功    能：给出三点的坐标，画一个三角形
  输入参数：const int x1		：第1个点的x
			const int y1		：第1个点的y
			const int x2		：第2个点的x
			const int y2		：第2个点的y
			const int x3		：第3个点的x
			const int y3		：第3个点的y
			bool filled			：是否需要填充（有缺省值）
			const int thickness	：边的粗细（有缺省值）
			const int RGB_value	：颜色（有缺省值）
  返 回 值：
  说    明：不判断三点是否共线，如果共线，划出一条直线即可
***************************************************************************/
void hdc_triangle(const int x1, const int y1, const int x2, const int y2, const int x3, const int y3, bool filled, const int thickness, const int RGB_value)
{
	if (RGB_value != INVALID_RGB)
		hdc_set_pencolor(RGB_value);

	//先画三条边
	hdc_line(x1, y1, x2, y2, thickness, RGB_value);
	hdc_line(x1, y1, x3, y3, thickness, RGB_value);
	hdc_line(x2, y2, x3, y3, thickness, RGB_value);
	hdc_point(x1, y1, 2, RGB_value);
	hdc_point(x2, y2, 2, RGB_value);
	hdc_point(x3, y3, 2, RGB_value);

	//内部填充，用hdc_base_line
	if (filled) {
		// 计算包围框
		int min_x = min(min(x1, x2), x3);
		int max_x = max(max(x1, x2), x3);
		int min_y = min(min(y1, y2), y3);
		int max_y = max(max(y1, y2), y3);

		// 扫描每条水平线
		for (int y = min_y; y <= max_y; y++) {
			int intersections[6]; // 最多6个交点
			int count = 0;

			// 检查三条边，避免重复计算顶点
			int edges[3][4] = {
				{x1, y1, x2, y2},
				{x2, y2, x3, y3},
				{x3, y3, x1, y1}
			};

			for (int i = 0; i < 3; i++) {
				int xa = edges[i][0], ya = edges[i][1];
				int xb = edges[i][2], yb = edges[i][3];

				// 跳过水平线
				if (ya == yb) continue;

				// 检查y是否在边的y范围内（包含一个端点，排除另一个）
				if ((y >= min(ya, yb)) && (y <= max(ya, yb))) {
					// 确保不重复计算下顶点（只包含上顶点）
					if (y == min(ya, yb) && ya != yb) {
						// 这是边的下顶点，跳过避免重复
						continue;
					}

					double t = (double)(y - ya) / (yb - ya);
					int x_intersect = (int)(xa + t * (xb - xa) + 0.5); // 四舍五入

					intersections[count++] = x_intersect;
				}
			}

			if (count == 0) continue; // 如果没有交点，跳过

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

			// 填充每对交点之间的区域
			for (int i = 0; i < count; i += 2) {
				if (i + 1 < count) {
					int start_x = intersections[i];
					int end_x = intersections[i + 1];
					if (start_x <= end_x) {
						hdc_base_line(start_x, y, end_x, y);
					}
				}
			}
		}
	} //if(filled)
}

/***************************************************************************
  函数名称：hdc_rectangle
  功    能：给出左上角坐标及宽度、高度，画出一个长方形
  输入参数：const int left_up_x			：左上角x
			const int left_up_y			：左上角y
			const int width				：宽度
			const int high				：高度
			const int rotation_angles	：以左上角为支点，与x轴的旋转倾角（向下转为正）
			bool filled					：是否需要填充（有缺省值）
			const int thickness			：边的粗细（有缺省值）
			const int RGB_value			：颜色（有缺省值）
  返 回 值：
  说    明：
***************************************************************************/
void hdc_rectangle(const int left_up_x, const int left_up_y, const int width, const int high, const int rotation_angles, const bool filled, const int thickness, const int RGB_value)
{
	//确定另外三个顶点的坐标
	const int rotation_angle = getAngle(rotation_angles, 0, true);
	double theta = rotation_angles * PI / 180.0; //弧度制
	int x1, y1, x2, y2, x3, y3, x4, y4;
	x1 = left_up_x;
	y1 = left_up_y;
	x2 = (int)(x1 + width * cos(theta) + 0.5);
	y2 = (int)(y1 + width * sin(theta) + 0.5);
	x3 = (int)(x2 - high * sin(theta) + 0.5);
	y3 = (int)(y2 + high * cos(theta) + 0.5);
	x4 = (int)(x1 - high * sin(theta) + 0.5);
	y4 = (int)(y1 + high * cos(theta) + 0.5);

	//画边框
	if (RGB_value != INVALID_RGB)
		hdc_set_pencolor(RGB_value);
	int t = thickness;
	hdc_line(x1, y1, x2, y2, t, RGB_value);
	hdc_line(x2, y2, x3, y3, t, RGB_value);
	hdc_line(x3, y3, x4, y4, t, RGB_value);
	hdc_line(x4, y4, x1, y1, t, RGB_value);

	//填充
	if (filled) {
		if (RGB_value != INVALID_RGB)
			hdc_set_pencolor(RGB_value);

		// 计算包围框
		int min_x = x1;
		int max_x = x1;
		int min_y = y1;
		int max_y = y1;

		// 找到最小和最大的x、y坐标
		int all_x[4] = { x1, x2, x3, x4 };
		int all_y[4] = { y1, y2, y3, y4 };

		for (int i = 0; i < 4; i++) {
			if (all_x[i] < min_x) min_x = all_x[i];
			if (all_x[i] > max_x) max_x = all_x[i];
			if (all_y[i] < min_y) min_y = all_y[i];
			if (all_y[i] > max_y) max_y = all_y[i];
		}

		// 扫描每条水平线
		for (int y = min_y; y <= max_y; y++) {
			int intersections[8]; // 最多8个交点（四边形）
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

				// 检查y是否在边的y范围内（包含一个端点，排除另一个）
				if ((y >= (ya < yb ? ya : yb)) && (y <= (ya > yb ? ya : yb))) {
					// 确保不重复计算下顶点（只包含上顶点）
					if (y == (ya < yb ? ya : yb) && ya != yb) {
						// 这是边的下顶点，跳过避免重复
						continue;
					}

					double t = (double)(y - ya) / (yb - ya);
					int x_intersect = (int)(xa + t * (xb - xa) + 0.5); // 四舍五入

					intersections[count++] = x_intersect;
				}
			}

			if (count == 0) continue; // 如果没有交点，跳过

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

			// 填充每对交点之间的区域
			for (int i = 0; i < count; i += 2) {
				if (i + 1 < count) {
					int start_x = intersections[i];
					int end_x = intersections[i + 1];
					if (start_x <= end_x) {
						hdc_base_line(start_x, y, end_x, y);
					}
				}
			}
		}
	} // if(filled)
}

/***************************************************************************
  函数名称：hdc_square
  功    能：给出左上角坐标及边长，画出一个长方形
  输入参数：const int left_up_x			：左上角x
			const int left_up_y			：左上角y
			const int length			：边长
			const int rotation_angles	：以左上角为支点，与x轴的旋转倾角（向下转为正）（有缺省值）
			const bool filled			：是否需要填充（有缺省值）
			const int thickness			：边的粗细（有缺省值）
			const int RGB_value			：颜色（有缺省值）
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void hdc_square(const int left_up_x, const int left_up_y, const int length, const int rotation_angles, const bool filled, const int thickness, const int RGB_value)
{
	hdc_rectangle(left_up_x, left_up_y, length, length, rotation_angles, filled, thickness, RGB_value);
}

/***************************************************************************
  函数名称：hdc_arc
  功    能：按照给出的参数画一段圆弧
  输入参数：const int point_x		：圆心的x
			const int point_y		：圆心的y
			const int radius		：半径
			const int angle_begin	：起始角度（以圆的最上方为0°，顺时针为正，注意：与系统坐标差了180°!!!）（有缺省值）
			const int angle_end		：结束角度（同上）（有缺省值）
			const int thickness		：粗细（有缺省值）
			const int RGB_value		：颜色（有缺省值）
  返 回 值：
  说    明：
***************************************************************************/
void hdc_arc(const int point_x, const int point_y, const int radius, const int angle_begin, const int angle_end, const int thickness, const int RGB_value)
{
	if (radius <= 0) return;

	/* 角度处理 */
	int angle_begins = getAngle(angle_begin, 0, true);
	int angle_ends = getAngle(angle_end, 0, true);
	double start_angle = normalize_angle(angle_begins);
	double end_angle = normalize_angle(angle_ends);
	if (end_angle <= start_angle)
		end_angle += 360;

	/* 画弧线 */
	if (RGB_value != INVALID_RGB)
		hdc_set_pencolor(RGB_value);
	double current_angle = start_angle;
	while (1) {
		if (current_angle <= end_angle) {
			double theta = current_angle * PI / 180.0;
			int current_x = (int)(point_x + radius * sin(theta));
			int current_y = (int)(point_y - radius * cos(theta));
			hdc_point(current_x, current_y, thickness, RGB_value);
			current_angle+=0.1;
		}
		else
			break;
	} //while(1)
}

/***************************************************************************
  函数名称：hdc_sector
  功    能：按照给出的参数画一个扇形
  输入参数：const int point_x		：圆心的x
			const int point_y		：圆心的y
			const int radius		：半径
			const int angle_begin	：起始角度（以圆的最上方为0°，顺时针为正，注意：与系统坐标差了180°!!!）（有缺省值）
			const int angle_end		：结束角度（同上）（有缺省值）
			const bool filled		：是否需要填充（有缺省值）
			const int thickness		：粗细（有缺省值）
			const int RGB_value		：颜色（有缺省值）
  输入参数：
  返 回 值：
  说    明：当起始/结束角度差值为360的倍数时，不画两条边
***************************************************************************/
void hdc_sector(const int point_x, const int point_y, const int radius, const int angle_begin, const int angle_end, const bool filled, const int thickness, const int RGB_value)
{
	int x1, y1, x2, y2;
	if (RGB_value != INVALID_RGB)
		hdc_set_pencolor(RGB_value);
	hdc_arc(point_x, point_y, radius, angle_begin, angle_end, thickness, RGB_value);
	if ((angle_end - angle_begin) % 360) {
		x1 = point_x - (int)(radius * sin(getAngle(angle_begin, 180) * PI / 180));
		y1 = point_y + (int)(radius * cos(getAngle(angle_begin, 180) * PI / 180));
		x2 = point_x - (int)(radius * sin(getAngle(angle_end, 180) * PI / 180));
		y2 = point_y + (int)(radius * cos(getAngle(angle_end, 180) * PI / 180));
		hdc_line(point_x, point_y, x1, y1, thickness, RGB_value);
		hdc_line(point_x, point_y, x2, y2, thickness, RGB_value);
	}

	if (filled) {
		point p[370];
		int count = 0;
		p[count].x = point_x - (int)(radius * sin(getAngle(angle_begin, 180) * PI / 180));
		p[count].y = point_y + (int)(radius * cos(getAngle(angle_begin, 180) * PI / 180));
		count++;
		for (int angle = angle_begin + 1; (angle - angle_end) % 360; angle++, count++) {
			p[count].x = point_x - (int)(radius * sin(getAngle(angle, 180) * PI / 180));
			p[count].y = point_y + (int)(radius * cos(getAngle(angle, 180) * PI / 180));
		}
		if ((angle_begin - angle_end) % 360 != 0) {
			p[count].x = point_x - (int)(radius * sin(getAngle(angle_end, 180) * PI / 180));
			p[count].y = point_y + (int)(radius * cos(getAngle(angle_end, 180) * PI / 180));
			count++;
			p[count].x = point_x;
			p[count].y = point_y;
			count++;
		}
		fillsector(count, p, RGB_value);
	} //if（filled）
}

/***************************************************************************
  函数名称：hdc_circle
  功    能：按照给出的参数画一个圆
  输入参数：const int point_x		：圆心的x
			const int point_y		：圆心的y
			const int radius		：半径
			const bool filled		：是否需要填充（有缺省值）
			const int thickness		：粗细（有缺省值）
			const int RGB_value		：颜色（有缺省值）
  返 回 值：
  说    明：
***************************************************************************/
void hdc_circle(const int point_x, const int point_y, const int radius, const bool filled, const int thickness, const int RGB_value)
{
	if (radius <= 0) return;

	hdc_sector(point_x, point_y, radius, 0, 0, filled, thickness, RGB_value);
}

/***************************************************************************
  函数名称：hdc_ellipse
  功    能：按照给出的参数画一个椭圆
  输入参数：const int point_x			：圆心的x
			const int point_y			：圆心的y
			const int radius_a			：平行于X轴的半径
			const int radius_b			：平行于Y轴的半径
			const int rotation_angles	：以圆心为支点，与x轴的旋转倾角（向下转为正）（有缺省值）
			const bool filled			：是否需要填充（有缺省值）
			const int thickness			：粗细（有缺省值）
			const int RGB_value			：颜色（有缺省值）
  返 回 值：
  说    明：
***************************************************************************/
void hdc_ellipse(const int point_x, const int point_y, const int radius_a, const int radius_b, const int rotation_angles, const bool filled, const int thickness, const int RGB_value)
{
	if (radius_a <= 0 || radius_b < 0) return;

	/* 角度处理 */
	double start_angle = normalize_angle(rotation_angles);
	double start_theta = start_angle * PI / 180.0;

	/* 画椭圆边框 */
	if (RGB_value != INVALID_RGB)
		hdc_set_pencolor(RGB_value);
	double current_angle = start_angle;
	while (1) {
		if (current_angle <= start_angle+360) {
			double theta = current_angle * PI / 180.0;
			int current_x = (int)(point_x + radius_a * cos(theta) * cos(start_theta) - radius_b * sin(theta) * sin(start_theta));
			int current_y = (int)(point_y + radius_a * cos(theta) * sin(start_theta) + radius_b * sin(theta) * cos(start_theta));
			hdc_point(current_x, current_y, thickness, RGB_value);
			current_angle += 0.1;
		}
		else
			break;
	} //while(1)

	/* 填充 */
	if (filled) {
		// 求出最高点和最低点的y坐标
		double sint = sin(start_theta), cost = cos(start_theta);
		int mx = radius_a > radius_b ? radius_a : radius_b;
		double tmp = sqrt(radius_a * radius_a * sint * sint + radius_b * radius_b * cost * cost);
		int y_max = (int)(point_y + tmp), y_min = (int)(point_y - tmp);
		int h = y_max;
		while (h >= y_min) {
			// 对于每一个h，求出水平线与椭圆边界的交点
			if (rotation_angles % 180 == 0 && (h == y_max || h == y_min)) {
				h--;
				continue;
			}
			int flag = 0;
			int xl = 0, xr = 0;
			for (int i = point_x - mx; i <= point_x + mx; i++) {
				if (in_ellipse(i, h, point_x, point_y, radius_a, radius_b, sint, cost) && flag == 0) {
					flag = 1;
					xl = i;
				}
				else if (in_ellipse(i, h, point_x, point_y, radius_a, radius_b, sint, cost) && flag == 1)
					xr = i;
			}
			if (xl <= xr)
				hdc_base_line(xl, h, xr, h);
			h--;
		} //while(h>=y_min)
	} //if(filled)
}

#endif !HDC_SERIES_BY_TEACHER