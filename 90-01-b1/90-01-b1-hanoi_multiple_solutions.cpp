/* 2353626 贺胡鸣 大数据 */
#include "90-01-b1-hanoi.h"
using namespace std;
static int step;
static int speed;
static int stackA[10], stackB[10], stackC[10];//三根柱子上的盘子
static int topA, topB, topC;                  //三根柱子最顶部的盘子的盘号



/* ----------------------------------------------------------------------------------

     本文件功能：
    1、存放被 hanoi_main.cpp 中根据菜单返回值调用的各菜单项对应的执行函数

     本文件要求：
    1、不允许定义外部全局变量（const及#define不在限制范围内）
    2、允许定义静态全局变量（具体需要的数量不要超过文档显示，全局变量的使用准则是：少用、慎用、能不用尽量不用）
    3、静态局部变量的数量不限制，但使用准则也是：少用、慎用、能不用尽量不用
    4、按需加入系统头文件、自定义头文件、命名空间等

   ----------------------------------------------------------------------------------- */
   /***************************************************************************
     函数名称：Reset()
     功    能：初始化静态全局变量
     输入参数：无
     返 回 值：无
     说    明：在每一个操作前都需移动步数Movement初始化为1，三个栈顶和三个栈中内容初始化为0
   ***************************************************************************/
void Reset()
{
    step = 1;
    topA = topB = topC = 0;
    for (int i = 0; i < 10; i++)
        stackA[i] = stackB[i] = stackC[i] = 0;
}
/***************************************************************************
     函数名称：Time_Sleep(int speed)
     功    能：设置延时功能
     输入参数：延时功能选项speed
     返 回 值：无
     说    明：当speed为0时按回车单步演示，1延时最短，20/200延时最长
   ***************************************************************************/
void Time_Sleep(int speed)
{
    if (speed)
        Sleep(speed);
    else
        while (getchar() != '\n');
}
/***************************************************************************
     函数名称：Input(char src, char tmp, char dst, int n,int choice)
     功    能：确定汉诺塔层数和三根柱，并确定是否使用延时（做过错误处理）
     输入参数：汉诺塔层数、起始柱和目标柱，延时标志
     返 回 值：/
     说    明：/
   ***************************************************************************/
void Input(char* src, char* tmp, char* dst, int* n, int choice)
{
    while (1) {
        cout << "请输入汉诺塔的层数(1-10)：" << endl;
        cin >> *n;
        if (cin.good() == 0 || *n < 1 || *n > MAX_LAYER)
        {
            cin.clear();
            cin.ignore(1024, '\n');
            continue;
        }
        break;
    }
    while (1) {
        cout << "请输入起始柱(A-C)" << endl;
        cin >> *src;
        if (cin.good() == 0) {
            cin.clear();
            cin.ignore(1024, '\n');
            continue;
        }
        else if (*src >= 'a' && *src <= 'c')
            *src -= 32;
        else if (*src >= 'A' && *src <= 'C')
            *src = *src;
        else {
            cin.clear();
            cin.ignore(1024, '\n');
            continue;
        }
        cin.clear();
        cin.ignore(1024, '\n');
        break;
    }
    while (1) {
        cout << "请输入目标柱(A-C)" << endl;
        cin >> *dst;
        if (cin.good() == 0) {
            cin.clear();
            cin.ignore(1024, '\n');
            continue;
        }
        else if (*dst >= 'a' && *dst <= 'c')
            *dst -= 32;
        else if (*dst >= 'A' && *dst <= 'C')
            *dst = *dst;
        else {
            cin.clear();
            cin.ignore(1024, '\n');
            continue;
        }
        if (*dst == *src)
        {
            cout << "目标柱(" << *dst << ")不能与起始柱(" << *src << ")相同" << endl;
            cin.clear();
            cin.ignore(1024, '\n');
            continue;
        }
        cin.clear();
        cin.ignore(1024, '\n');
        break;
    }
    *tmp = 'B' * 3 - *src - *dst;

    if (choice == 4) {
        while (1)
        {
            cout << "请输入移动速度(0-200：0-按回车单步演示 1-200:延时1-200ms) ";
            cin >> speed;
            if (cin.good() == 0 || speed > 200 || speed < 0)
            {
                cin.clear();
                cin.ignore(1024, '\n');
                continue;
            }
            cin.clear();
            cin.ignore(1024, '\n');
            break;
        }
    }
    else if (choice == 7 || choice == 8 || choice == 9) {
        while (1)
        {
            cout << "请输入移动速度(0-10：0-按回车单步演示 1-20:延时1-20ms) ";
            cin >> speed;
            if (cin.good() == 0 || speed > 20 || speed < 0)
            {
                cin.clear();
                cin.ignore(1024, '\n');
                continue;
            }
            cin.clear();
            cin.ignore(1024, '\n');
            break;
        }
    }
}
/***************************************************************************
     函数名称：Init(int n, char src)
     功    能：初始化起始柱
     输入参数：汉诺塔层数和起始柱
     返 回 值：无
     说    明：将起始柱的对应栈中元素初始为相应盘
   ***************************************************************************/
void Init(int n, char src)
{
    if (src == 'A')
    {
        while (topA < n)
            stackA[topA++] = n - topA;
    }
    else if (src == 'B')
    {
        while (topB < n)
            stackB[topB++] = n - topB;
    }
    else if (src == 'C')
    {
        while (topC < n)
            stackC[topC++] = n - topC;
    }
}
/***************************************************************************
     函数名称：Hanoi_Stack(char src, char dst)
     功    能：实现元素在汉诺塔塔栈里的移动
     输入参数：此步移动的起始柱和目标柱
     返 回 值：无
     说    明：通过栈的出栈入栈操作实现汉诺塔的塔栈内元素的互相移动
   ***************************************************************************/
void Hanoi_Stack(char src, char dst)
{
    int element = 0;
    if (src == 'A' && topA > 0)
    {
        element = stackA[--topA];
        stackA[topA] = 0;
    }
    else if (src == 'B' && topB > 0)
    {
        element = stackB[--topB];
        stackB[topB] = 0;
    }
    else if (src == 'C' && topC > 0)
    {
        element = stackC[--topC];
        stackC[topC] = 0;
    }
    if (dst == 'A' && topA < MAX_LAYER)
        stackA[topA++] = element;
    else if (dst == 'B' && topB < MAX_LAYER)
        stackB[topB++] = element;
    else if (dst == 'C' && topC < MAX_LAYER)
        stackC[topC++] = element;
}

/***************************************************************************
     函数名称：Print_Crosswise(int x, int y)
     功    能：横向打印汉诺塔数组各元素
     输入参数：汉诺塔横向打印数组的位置坐标
     返 回 值：无
     说    明：类似于上次作业的横向打印数组，将初始位置改变即可
   ***************************************************************************/
void Print_Crosswise(int x, int y)
{
    cct_gotoxy(x, y);
    cout << "A:";
    for (int i = 0; i < MAX_LAYER; i++)
    {
        if (stackA[i])
            cout << setw(2) << stackA[i];
        else
            cout << "  ";
    }
    cout << " B:";
    for (int i = 0; i < MAX_LAYER; i++)
    {
        if (stackB[i])
            cout << setw(2) << stackB[i];
        else
            cout << "  ";
    }
    cout << " C:";
    for (int i = 0; i < MAX_LAYER; i++)
    {
        if (stackC[i])
            cout << setw(2) << stackC[i];
        else
            cout << "  ";
    }
    cout << endl;
}

/***************************************************************************
     函数名称：Print()
     功    能：横向打印汉诺塔数组各元素
     输入参数：汉诺塔横向打印数组的位置坐标
     返 回 值：无
     说    明：类似于上次作业的横向打印数组，将初始位置改变即可
   ***************************************************************************/
void Print()
{
    cout << " A:";
    for (int i = 0; i < MAX_LAYER; i++)
    {
        if (stackA[i])
            cout << setw(2) << stackA[i];
        else
            cout << "  ";
    }
    cout << " B:";
    for (int i = 0; i < MAX_LAYER; i++)
    {
        if (stackB[i])
            cout << setw(2) << stackB[i];
        else
            cout << "  ";
    }
    cout << " C:";
    for (int i = 0; i < MAX_LAYER; i++)
    {
        if (stackC[i])
            cout << setw(2) << stackC[i];
        else
            cout << "  ";
    }
    cout << endl;
}
/***************************************************************************
     函数名称：Print_Bottom(int x,int y)
     功    能：打印选项4中的底盘
     输入参数：底盘最左侧的坐标x,y
     返 回 值：无
     说    明：类似于上次作业的打印底盘，将初始位置改变即可
   ***************************************************************************/
void Print_Bottom(int x, int y)
{
    cct_gotoxy(x, y);
    for (int i = 0; i < Underpan_Distance * 2 + 7; i++)
        cout << "=";
    cout << endl;
    cct_gotoxy(x + 2, y + 1);
    for (int i = 0; i < 3; i++) {
        cout << (char)('A' + i);
        for (int j = 0; j < Underpan_Distance; j++)
            cout << " ";
    }
}
/***************************************************************************
  函数名称：Print_Vertical(char src, char dst, int init, int bottom_x, int bottom_y, int x, int y)
  功    能：纵向打印汉诺塔数组各元素
  输入参数：起始柱、目标柱、是否初始化状态、底盘的x、y坐标和打印纵向数组的x、y坐标
  返 回 值：无
  说    明：init为1是表示初始化状态，此时打印起始柱的数组内容即可
***************************************************************************/
void Print_Vertical(char src, char dst, int init, int bottom_x, int bottom_y, int x, int y)
{
    //Print_Vertical(src, dst, 1,
    //MenuItem4_Start_X+ Underpan_A_X_OFFSET-2,MenuItem4_Start_Y+ Underpan_A_Y_OFFSET-1,
    //MenuItem4_Start_X + Underpan_A_X_OFFSET-1, MenuItem4_Start_Y + Underpan_A_Y_OFFSET - 2);
    Print_Bottom(bottom_x, bottom_y);
    if (init)
    {
        for (int i = 0; i < topA; i++)
        {
            cct_gotoxy(x, y - i);
            if (stackA[i])
                cout << setw(2) << stackA[i];
        }
        for (int i = 0; i < topB; i++)
        {
            cct_gotoxy(x + Underpan_Distance + 1, y - i);
            if (stackB[i])
                cout << setw(2) << stackB[i];
        }
        for (int i = 0; i < topC; i++)
        {
            cct_gotoxy(x + (Underpan_Distance + 1) * 2, y - i);
            if (stackC[i])
                cout << setw(2) << stackC[i];
        }
    }
    else
    {
        if (src == 'A')
        {
            cct_gotoxy(x, y - topA);
            cout << "  ";
        }
        else if (src == 'B')
        {
            cct_gotoxy(x + (Underpan_Distance + 1), y - topB);
            cout << "  ";
        }
        else if (src == 'C')
        {
            cct_gotoxy(x + 2 * (Underpan_Distance + 1), y - topC);
            cout << "  ";
        }
        if (dst == 'A')
        {
            cct_gotoxy(x, y + 1 - topA);
            cout << setw(2) << stackA[topA - 1];
        }
        else if (dst == 'B')
        {
            cct_gotoxy(x + (Underpan_Distance + 1), y + 1 - topB);
            cout << setw(2) << stackB[topB - 1];
        }
        else if (dst == 'C')
        {
            cct_gotoxy(x + 2 * (Underpan_Distance + 1), y + 1 - topC);
            cout << setw(2) << stackC[topC - 1];
        }
    }
}

/***************************************************************************
  函数名称：to_be_continued
  功    能：显示提示信息
  输入参数：const char prompt[], const int pos_x = 0, const int pos_y = 22
  返 回 值：
  说    明：
***************************************************************************/
void to_be_continued(const char prompt[], const int pos_x, const int pos_y)
{
    cct_gotoxy(pos_x, pos_y);

    //因为每次都从22行0列，如果本次打印内容比上次短，后面会有残留，因此先用100个空格填充掉（未做准确判断，简单假设打印内容不超过100）
    cout << setw(100) << ' ' << '\r'; //'\r'表示回到本行开头，不换行
    cout << prompt;

    // 忽略除回车键外的所有输入（注意：_getch的回车是\r，而getchar是\n）
    while (_getch() != '\r')
        ;

    return;
}


/***************************************************************************
  函数名称：Print_Column()
  功    能：完成选项5中的预备画三个圆柱
  输入参数：无
  返 回 值：无
  说    明：无
***************************************************************************/
void Print_Column()
{
    hdc_set_pencolor(HDC_COLOR[MAX_LAYER + 1]);

    const int base_width = 23 * HDC_Base_Width;      // 水平短线长度
    const int base_height = HDC_Base_High;           // 水平短线粗细
    const int pillar_width = HDC_Base_Width;         // 垂直立柱粗细
    const int pillar_height = 10 * HDC_Base_High;    // 垂直立柱高度
    Sleep(HDC_Init_Delay);
    for (int i = 0; i < 3; i++) {
        int x_center = HDC_Start_X + (2 * i + 1) * base_width / 2 + i * HDC_Underpan_Distance;          // 塔的中心X坐标
        // (1) 绘制水平底盘
        hdc_rectangle(x_center - base_width / 2, HDC_Start_Y, base_width, base_height, 0, true, 1, HDC_COLOR[MAX_LAYER + 1]);
        Sleep(HDC_Init_Delay);
    }
    for (int i = 0; i < 3; i++) {
        int x_center = HDC_Start_X + (2 * i + 1) * base_width / 2 + i * HDC_Underpan_Distance;          // 塔的中心X坐标
        // (2) 绘制垂直立柱
        hdc_rectangle(x_center - HDC_Base_Width / 2, HDC_Start_Y - pillar_height, HDC_Base_Width, pillar_height, 0, true, 1, HDC_COLOR[MAX_LAYER + 1]);
        Sleep(HDC_Init_Delay);
    }
}
/***************************************************************************
  函数名称：Print_Plate(int n, char src)
  功    能：完成选项6中的在起始柱上画n个盘子的要求
  输入参数：汉诺塔层数和起始柱
  返 回 值：无
  说    明：无
***************************************************************************/
void Print_Plate(int n, char src)
{
    int plate_x;
    int plate_y = HDC_Base_High;
    const int base_width = 23 * HDC_Base_Width;      // 水平短线长度
    const int base_height = HDC_Base_High;           // 水平短线粗细
    const int pillar_width = HDC_Base_Width;         // 垂直立柱粗细
    const int pillar_height = 10 * HDC_Base_High;    // 垂直立柱高度
    if (src == 'A')
        plate_x = HDC_Start_X + (base_width / 2);
    else if (src == 'B')
        plate_x = HDC_Start_X + (3 * base_width / 2) + HDC_Underpan_Distance;
    else
        plate_x = HDC_Start_X + (5 * base_width / 2) + 2 * HDC_Underpan_Distance;


    for (int i = n; i > 0; i--)
    {
        int plate_width = (2 * i + 1) * HDC_Base_Width;
        hdc_rectangle(plate_x - plate_width / 2, HDC_Start_Y - (n + 1 - i) * HDC_Base_High,
            plate_width, HDC_Base_High, 0, true, 1,
            HDC_COLOR[i]);
        Sleep(HDC_Init_Delay);
    }
}

/***************************************************************************
  函数名称：Move_Plate(char src, char dst, int speed,int count)
  功    能：完成选项7，8，9中的移动盘子的要求
  输入参数：char src, char dst, int speed,int count
  返 回 值：/
  说    明：/
***************************************************************************/
void Move_Plate(char src, char dst, int speed, int count)
{
    const int base_width = 23 * HDC_Base_Width;      // 水平短线长度
    const int base_height = HDC_Base_High;           // 水平短线粗细
    const int pillar_width = HDC_Base_Width;         // 垂直立柱粗细
    const int pillar_height = 10 * HDC_Base_High;    // 垂直立柱高度
    int src_x, dst_x, dst_y, current_y, plate_width, plate_height, platenum;
    plate_height = HDC_Base_High;
    if (src == 'A') {
        src_x = HDC_Start_X + base_width / 2;
        current_y = HDC_Start_Y - (topA)*HDC_Base_High;
        plate_width = HDC_Base_Width * (2 * stackA[topA - 1] + 1);
        platenum = stackA[topA - 1];
    }
    else if (src == 'B') {
        src_x = HDC_Start_X + (3 * base_width / 2) + HDC_Underpan_Distance;
        current_y = HDC_Start_Y - (topB)*HDC_Base_High;
        plate_width = HDC_Base_Width * (2 * stackB[topB - 1] + 1);
        platenum = stackB[topB - 1];
    }
    else {
        src_x = HDC_Start_X + (5 * base_width / 2) + 2 * HDC_Underpan_Distance;
        current_y = HDC_Start_Y - (topC)*HDC_Base_High;
        plate_width = HDC_Base_Width * (2 * stackC[topC - 1] + 1);
        platenum = stackC[topC - 1];

    }

    if (dst == 'A') {
        dst_x = HDC_Start_X + base_width / 2;
        dst_y = HDC_Start_Y - (topA + 1) * HDC_Base_High;
    }
    else if (dst == 'B') {
        dst_x = HDC_Start_X + (3 * base_width / 2) + HDC_Underpan_Distance;
        dst_y = HDC_Start_Y - (topB + 1) * HDC_Base_High;
    }
    else {
        dst_x = HDC_Start_X + (5 * base_width / 2) + 2 * HDC_Underpan_Distance;
        dst_y = HDC_Start_Y - (topC + 1) * HDC_Base_High;
    }

    //上升
    int y;
    for (y = current_y; y >= HDC_Top_Y; y -= HDC_Step_Y) {
        //擦除原来盘子的位置
        hdc_rectangle(src_x - plate_width / 2, y + HDC_Base_High - HDC_Step_Y, plate_width, HDC_Step_Y, 0, true, 1, HDC_COLOR[0]);
        //补上被擦掉的柱子
        if (y + plate_height > HDC_Start_Y - pillar_height)
            hdc_rectangle(src_x - pillar_width / 2, y + plate_height - HDC_Step_Y, pillar_width, HDC_Step_Y, 0, true, 1, HDC_COLOR[11]);
        //画盘子的新位置
        if (y > HDC_Top_Y)
            hdc_rectangle(src_x - plate_width / 2, y - HDC_Step_Y, plate_width, HDC_Step_Y, 0, true, 1, HDC_COLOR[platenum]);
        else
            hdc_rectangle(src_x - plate_width / 2, HDC_Top_Y, plate_width, plate_height, 0, true, 1, HDC_COLOR[platenum]);
        //加延时
        if (count >= 9)
            Sleep(0);
        else {
            if (speed > 0)
                Sleep(speed);
            else
                while (_getch() != '\r');
        }
    }
    y += HDC_Step_Y;

    //左右平移
    if (src_x < dst_x) {
        for (int x = src_x; x <= dst_x; x += HDC_Step_X) {
            //擦除原来盘子的位置
            hdc_rectangle(x - plate_width / 2, y, HDC_Step_X, plate_height, 0, true, 1, HDC_COLOR[0]);
            //画盘子的新位置
            if (x < dst_x)
                hdc_rectangle(x - plate_width / 2 + HDC_Step_X, y, plate_width, plate_height, 0, true, 1, HDC_COLOR[platenum]);
            else
                hdc_rectangle(dst_x - plate_width / 2, y, plate_width, plate_height, 0, true, 1, HDC_COLOR[platenum]);
            //加延时
            if (count >= 9)
                Sleep(0);
            else {
                if (speed > 0)
                    Sleep(speed);
                else
                    while (_getch() != '\r');
            }
        }
    }
    else {
        for (int x = src_x; x >= dst_x; x -= HDC_Step_X) {
            //擦除原来盘子的位置
            hdc_rectangle(x + plate_width / 2 - HDC_Step_X, y, HDC_Step_X, plate_height, 0, true, 1, HDC_COLOR[0]);
            //画盘子的新位置
            if (x > dst_x)
                hdc_rectangle(x - plate_width / 2 - HDC_Step_X, y, plate_width, plate_height, 0, true, 1, HDC_COLOR[platenum]);
            else
                hdc_rectangle(dst_x - plate_width / 2, y, plate_width, plate_height, 0, true, 1, HDC_COLOR[platenum]);
            //加延时
            if (count >= 9)
                Sleep(0);
            else {
                if (speed > 0)
                    Sleep(speed);
                else
                    while (_getch() != '\r');
            }
        }
    }


    //下移
    current_y = y;
    for (y = current_y; y <= dst_y; y += HDC_Step_Y) {
        //擦除原来盘子的位置
        hdc_rectangle(dst_x - plate_width / 2, y, plate_width, HDC_Step_Y, 0, true, 1, HDC_COLOR[0]);
        //补上被擦掉的柱子
        if (y > HDC_Start_Y - pillar_height)
            hdc_rectangle(dst_x - pillar_width / 2, y, pillar_width, HDC_Step_Y, 0, true, 1, HDC_COLOR[11]);
        //画盘子的新位置
        if (y < dst_y)
            hdc_rectangle(dst_x - plate_width / 2, y + HDC_Step_Y, plate_width, plate_height, 0, true, 1, HDC_COLOR[platenum]);
        else
            hdc_rectangle(dst_x - plate_width / 2, dst_y, plate_width, plate_height, 0, true, 1, HDC_COLOR[platenum]);
        //加延时
        if (count >= 9)
            Sleep(0);
        else {
            if (speed > 0)
                Sleep(speed);
            else
                while (_getch() != '\r');
        }
    }
}

/***************************************************************************
  函数名称：Hanoi_Game(int n, char dst)
  功    能：汉诺塔游戏（选项9）的实现
  输入参数：汉诺塔层数和目标柱
  返 回 值：/
  说    明：最后通过判断目标柱的顶是否为盘1来判断游戏是否结束
***************************************************************************/
void Hanoi_Game(int n, char dst)
{
    char input_src, input_dst;
    int src_element, dst_element;
    while (1)
    {
        cct_gotoxy(MenuItem9_Start_X, MenuItem9_Start_Y + 2);
        cout << "请输入移动的柱号(命令形式：AC=A顶端的盘子移动到C，Q=退出) ：";
        input_src = getchar();
        if (input_src == '\n')
            continue;
        if (cin.good() == 0 || (input_src != 'A' && input_src != 'B' && input_src != 'C' && input_src != 'Q' && input_src != 'a' && input_src != 'b' && input_src != 'c' && input_src != 'q'))
        {
            cin.clear();
            cin.ignore(1024, '\n');
            continue;
        }
        input_dst = getchar();
        if ((input_src == 'Q' || input_src == 'q') && input_dst == '\n')
        {
            cct_gotoxy(MenuItem9_Start_X, MenuItem9_Start_Y + 3);
            cout << "游戏中止!!!!!" << endl;
            break;
        }
        if (cin.good() == 0 || (input_dst != 'A' && input_dst != 'B' && input_dst != 'C' && input_dst != 'a' && input_dst != 'b' && input_dst != 'c'))
        {
            cin.clear();
            cin.ignore(1024, '\n');
            continue;
        }

        char conti = getchar();
        if (conti != '\n')
        {
            cin.clear();
            cin.ignore(1024, '\n');
            continue;
        }
        if (input_src >= 'a' && input_src <= 'z')
            input_src -= 32;
        if (input_dst >= 'a' && input_dst <= 'z')
            input_dst -= 32;
        if (input_src == input_dst)
            continue;

        if (input_src == 'A' && topA > 0)
            src_element = stackA[topA - 1];
        else if (input_src == 'B' && topB > 0)
            src_element = stackB[topB - 1];
        else if (input_src == 'C' && topC > 0)
            src_element = stackC[topC - 1];
        else
        {
            cct_gotoxy(MenuItem9_Start_X + 60, MenuItem9_Start_Y + 2);
            cout << "   ";
            cct_gotoxy(MenuItem9_Start_X, MenuItem9_Start_Y + 3);
            cout << "源柱为空!";
            Sleep(20 * Time);
            cct_gotoxy(MenuItem9_Start_X, MenuItem9_Start_Y + 3);
            cout << "         ";
            continue;
        }
        if (input_dst == 'A' && topA > 0)
            dst_element = stackA[topA - 1];
        else if (input_dst == 'B' && topB > 0)
            dst_element = stackB[topB - 1];
        else if (input_dst == 'C' && topC > 0)
            dst_element = stackC[topC - 1];
        else
            dst_element = 11;
        if (src_element > dst_element)
        {
            cct_gotoxy(MenuItem9_Start_X + 60, MenuItem9_Start_Y + 2);
            cout << "   ";
            cct_gotoxy(MenuItem9_Start_X, MenuItem9_Start_Y + 3);
            cout << "大盘压小盘，非法移动!";
            Sleep(20 * Time);
            cct_gotoxy(MenuItem9_Start_X, MenuItem9_Start_Y + 3);
            cout << "                     ";
            continue;
        }

        cct_gotoxy(MenuItem9_Start_X, MenuItem9_Start_Y);
        cout << "第" << setw(4) << step++ << " 步( " << src_element << "#: " << input_src << "-->" << input_dst << ")  ";
        Hanoi_Stack(input_src, input_dst);
        Print_Crosswise(22, MenuItem9_Start_Y);
        Print_Vertical(input_src, input_dst, 0, MenuItem9_Start_X + Underpan_A_X_OFFSET - 2, MenuItem9_Start_Y + Underpan_A_Y_OFFSET - 1, MenuItem9_Start_X + Underpan_A_X_OFFSET - 1, MenuItem9_Start_Y + Underpan_A_Y_OFFSET - 2);
        Hanoi_Stack(input_dst, input_src);
        Move_Plate(input_src, input_dst, 10, 1);
        cct_gotoxy(MenuItem9_Start_X + 60, MenuItem9_Start_Y + 2);
        cout << "   ";
        Hanoi_Stack(input_src, input_dst);
        int end_plate;
        if (dst == 'A')
            end_plate = stackA[n - 1];
        else if (dst == 'B')
            end_plate = stackB[n - 1];
        else
            end_plate = stackC[n - 1];
        if (end_plate == 1)
        {
            cct_gotoxy(MenuItem9_Start_X, MenuItem9_Start_Y + 3);
            cout << "游戏中止!!!!!" << endl;
            break;
        }

    }
}



/***************************************************************************
  函数名称：Operation(char src, char tmp, char dst, int n, int speed, int choice)
  功    能：完成选项1、2、3、4、7、8中在递归过程中的各类打印操作
  输入参数：起始柱、中间柱、目标柱、汉诺塔层数、速度设置和菜单选项
  返 回 值：无
  说    明：无
***************************************************************************/
void Operation(char src, char tmp, char dst, int n, int speed, int choice)
{
    if (choice == 1)
        cout << n << "# " << src << "---->" << dst << endl;
    else if (choice == 2)
        cout << "第" << setw(4) << step++ << " 步(" << setw(2) << n << "#: " << src << "-->" << dst << ")" << endl;
    else if (choice == 3)
    {
        cout << "第" << setw(4) << step++ << " 步(" << setw(2) << n << "#: " << src << "-->" << dst << ")";
        Hanoi_Stack(src, dst);
        Print();
    }
    else if (choice == 4)
    {
        cct_gotoxy(MenuItem4_Start_X, MenuItem4_Start_Y);
        cout << "第" << setw(4) << step++ << " 步(" << setw(2) << n << "#: " << src << "-->" << dst << ")";
        Hanoi_Stack(src, dst);
        Print_Crosswise(22, MenuItem4_Start_Y);
        Print_Vertical(src, dst, 0, MenuItem4_Start_X + Underpan_A_X_OFFSET - 2, MenuItem4_Start_Y + Underpan_A_Y_OFFSET - 1, MenuItem4_Start_X + Underpan_A_X_OFFSET - 1, MenuItem4_Start_Y + Underpan_A_Y_OFFSET - 2);
        Time_Sleep(speed);
    }
    else if (choice == 7)
    {
        Move_Plate(src, dst, speed, 1);
    }

    else if (choice == 8)
    {
        cct_gotoxy(MenuItem8_Start_X, MenuItem8_Start_Y);
        cout << "第" << setw(4) << step++ << " 步( " << n << "#: " << src << "-->" << dst << ")  ";
        Hanoi_Stack(src, dst);
        Print_Crosswise(22, MenuItem8_Start_Y);
        Print_Vertical(src, dst, 0, MenuItem8_Start_X + Underpan_A_X_OFFSET - 2, MenuItem8_Start_Y + Underpan_A_Y_OFFSET - 1, MenuItem8_Start_X + Underpan_A_X_OFFSET - 1, MenuItem8_Start_Y + Underpan_A_Y_OFFSET - 2);
        Hanoi_Stack(dst, src);
        Move_Plate(src, dst, speed, step);
        Hanoi_Stack(src, dst);
        Time_Sleep(speed);
    }

}
/***************************************************************************
  函数名称：Hanoi(char src, char tmp, char dst, int n, int speed, int choice)
  功    能：汉诺塔递归主函数
  输入参数：起始柱、中间柱、目标柱、汉诺塔层数、速度设置和菜单选项
  返 回 值：无
  说    明：
***************************************************************************/
void Hanoi(char src, char tmp, char dst, int n, int speed, int choice)
{
    if (n == 1)
        Operation(src, tmp, dst, n, speed, choice);
    else
    {
        Hanoi(src, dst, tmp, n - 1, speed, choice);
        if (choice == 7)
            return;
        Operation(src, tmp, dst, n, speed, choice);
        Hanoi(tmp, src, dst, n - 1, speed, choice);
    }
}
/***************************************************************************
  函数名称：Choice_1()、Choice_2()、Choice_3()、Choice_4()、Choice_5()、Choice_6()、Choice_7()、Choice_8()、Choice_9()
  功    能：完成每一个菜单选项的功能
  输入参数：无
  返 回 值：无
  说    明：以下九个函数分别对应着菜单的九个操作选项，每一个函数都按照具体要求将上述函数组合而成
***************************************************************************/
void choice1()
{
    char src, tmp, dst;
    int n;
    Input(&src, &tmp, &dst, &n, 1);
    Hanoi(src, tmp, dst, n, -1, 1);
}
void choice2()
{
    char src, tmp, dst;
    int n;
    Reset();
    Input(&src, &tmp, &dst, &n, 2);
    Hanoi(src, tmp, dst, n, -1, 2);
}
void choice3()
{
    char src, tmp, dst;
    int n;
    Reset();
    Input(&src, &tmp, &dst, &n, 3);
    Init(n, src);
    Hanoi(src, tmp, dst, n, -1, 3);
}
void choice4()
{
    char src, tmp, dst;
    int n;
    Reset();
    Input(&src, &tmp, &dst, &n, 4);
    Init(n, src);
    cct_cls();
    cct_gotoxy(Status_Line_X, Status_Line_Y);
    cout << "从 " << src << " 移动到 " << dst << "，共 " << n << " 层，延时设置为 " << speed << "ms" << endl;
    cct_gotoxy(MenuItem4_Start_X, MenuItem4_Start_Y);
    cout << "初始:  ";
    Print_Crosswise(MenuItem4_Start_X + 7, MenuItem4_Start_Y);
    Print_Vertical(src, dst, 1, MenuItem4_Start_X + Underpan_A_X_OFFSET - 2, MenuItem4_Start_Y + Underpan_A_Y_OFFSET - 1, MenuItem4_Start_X + Underpan_A_X_OFFSET - 1, MenuItem4_Start_Y + Underpan_A_Y_OFFSET - 2);
    Time_Sleep(speed);
    Hanoi(src, tmp, dst, n, speed, 4);
}
void choice5()
{
    cct_cls();
    const int   win_width = 1500, win_high = 1000;	//设定屏幕宽度
    hdc_init(RGB(0, 0, 0), RGB(255, 255, 255), win_width, win_high);
    Print_Column();
}
void choice6()
{
    char src = 0, tmp = 0, dst = 0;
    int n = 0;
    Reset();
    Input(&src, &tmp, &dst, &n, 6);
    Init(n, src);
    cct_cls();
    const int   win_width = 1500, win_high = 1000;	//设定屏幕宽度
    hdc_init(RGB(0, 0, 0), RGB(255, 255, 255), win_width, win_high);
    cct_gotoxy(Status_Line_X, Status_Line_Y);
    cout << "从 " << src << " 移动到 " << dst << "，共 " << n << " 层" << endl;
    Print_Column();
    Print_Plate(n, src);
}

void choice7()
{
    char src = 0, tmp = 0, dst = 0;
    int n = 0;
    Reset();
    Input(&src, &tmp, &dst, &n, 7);
    Init(n, src);
    cct_cls();
    const int   win_width = 1500, win_high = 1000;	//设定屏幕宽度
    hdc_init(RGB(0, 0, 0), RGB(255, 255, 255), win_width, win_high);
    cct_gotoxy(Status_Line_X, Status_Line_Y);
    cout << "从 " << src << " 移动到 " << dst << "，共 " << n << " 层" << endl;
    Print_Column();
    Print_Plate(n, src);
    Sleep(Time);
    Hanoi(src, tmp, dst, n, speed, 7);
}

void choice8()
{
    char src, tmp, dst;
    int n;
    Reset();
    Input(&src, &tmp, &dst, &n, 8);
    Init(n, src);
    cct_cls();
    Sleep(200);
    const int   win_width = 1500, win_high = 1000;	//设定屏幕宽度
    hdc_init(RGB(0, 0, 0), RGB(255, 255, 255), win_width, win_high);
    cct_gotoxy(Status_Line_X, Status_Line_Y);
    cout << "从 " << src << " 移动到 " << dst << "，共 " << n << " 层，延时设置为 " << speed << "（前7步，后面自动变为0ms）" << endl;
    cct_gotoxy(MenuItem8_Start_X, MenuItem8_Start_Y);
    cout << "初始:  ";
    Print_Crosswise(MenuItem8_Start_X + 7, MenuItem8_Start_Y);
    Print_Vertical(src, dst, 1, MenuItem8_Start_X + Underpan_A_X_OFFSET - 2, MenuItem8_Start_Y + Underpan_A_Y_OFFSET - 1, MenuItem8_Start_X + Underpan_A_X_OFFSET - 1, MenuItem8_Start_Y + Underpan_A_Y_OFFSET - 2);
    Print_Column();
    Print_Plate(n, src);
    Time_Sleep(speed);
    Sleep(Time);
    Hanoi(src, tmp, dst, n, speed, 8);
}

void choice9()
{
    char src, tmp, dst;
    int n;
    Reset();
    Input(&src, &tmp, &dst, &n, 9);
    Init(n, src);
    cct_cls();
    const int   win_width = 1500, win_high = 1000;	//设定屏幕宽度
    hdc_init(RGB(0, 0, 0), RGB(255, 255, 255), win_width, win_high);
    cct_gotoxy(Status_Line_X, Status_Line_Y);
    cout << "从 " << src << " 移动到 " << dst << "，共 " << n << " 层" << endl;
    cct_gotoxy(MenuItem9_Start_X, MenuItem9_Start_Y);
    cout << "初始:  ";
    Print_Crosswise(MenuItem9_Start_X + 7, MenuItem9_Start_Y);
    Print_Vertical(src, dst, 1, MenuItem9_Start_X + Underpan_A_X_OFFSET - 2, MenuItem9_Start_Y + Underpan_A_Y_OFFSET - 1, MenuItem9_Start_X + Underpan_A_X_OFFSET - 1, MenuItem9_Start_Y + Underpan_A_Y_OFFSET - 2);
    Print_Column();
    Print_Plate(n, src);
    Hanoi_Game(n, dst);
}
