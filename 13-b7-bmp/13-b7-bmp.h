/* 2353626 贺胡鸣 大数据 */
#pragma once

//不允许加入任何头文件，特别是<Windows.h>/<Wingdi.h>，查到就是0分甚至是倒扣-20 ！！！

//关于BMP的微软官方文档：
// https://learn.microsoft.com/zh-cn/windows/win32/api/wingdi/ns-wingdi-bitmap

//自行查阅相关资料，了解下面这几个预编译命令的作用，看能否给你的作业带来方便！！！
//#pragma pack(show) //以警告信息的形式显示当前字节对齐的值
//#pragma pack(push) //将当前字节对齐值压入编译器栈的栈顶
//#pragma pack(push, 4) //将当前字节对齐值压入编译器栈的栈顶，然后再将4设置当前值
//#pragma pack(pop)  //将编译器栈栈顶的字节对齐值弹出并设置为当前值
//#pragma pack() //不带参数是恢复默认值

//允许定义其它需要的结构体（类）、常量、常变量等
#pragma pack(push, 1)
//文件头，14字节
struct BITMAP_FILEHEADER {          // 内容              偏移量       大小（字节）
    unsigned short bfType;          // 文件类型，"BM"     0x00         2  
    unsigned int   bfSize;          // 文件大小           0x02         4
    unsigned short bfReserved1;     // 保留字段,0         0x06         2
    unsigned short bfReserved2;     // 保留字段,0         0x08         2
    unsigned int   bfOffBits;       // 像素数据偏移量     0x0A         4             从文件开始到像素数据开始的字节数，即bmBits指针所指向的数据在文件中的位置
};

//信息头，40字节通常
struct BITMAP_INFOHEADER {          //内容               偏移量       大小（字节）
    unsigned int   biSize;          // 信息头大小         0x0E         4             通常是40字节
    int            biWidth;         // 图像宽度           0x12         4
    int            biHeight;        // 图像高度           0x16         4             正值表示像素数据自下而上存储，负值表示自上而下
    unsigned short biPlanes;        // 颜色平面数         0x1A         2             必须为1
    unsigned short biBitCount;      // 每像素位数         0x1C         2             决定颜色深度（需支持1、4、8、24、32）
    unsigned int   biCompression;   // 压缩类型           0x1E         4             通常为0（不压缩）
    unsigned int   biSizeImage;     // 图像数据大小       0x22         4             像素数据区域的大小,>=0
    int            biXPelsPerMeter; // 水平分辨率         0x26         4
    int            biYPelsPerMeter; // 垂直分辨率         0x3A         4
    unsigned int   biClrUsed;       // 使用的颜色数       0x3E         4
    unsigned int   biClrImportant;  // 重要颜色数         0x42         4
};

// RGB颜色表项 biBitCount==1/4/8时使用，即像素数据存储的不是实际颜色，而是调色板数组的索引
// 调色板的大小是 （2^biBitCount)*4 字节
struct BMP_RGBQUAD {                // 调色板每个条目是一个4字节的结构体，蓝，绿，红，以及保留字节
    unsigned char rgbBlue;
    unsigned char rgbGreen;
    unsigned char rgbRed;
    unsigned char rgbReserved;
};

// 像素颜色信息结构
// 从 bfOffBits 开始，每个扫描行（即每一行像素）必须是4的倍数，不够的话需要用0对齐
struct PixelColor {                 // 计算方式： stride = floor((biWidth * biBitCount + 31)/32) * 4
    unsigned char red;              // 存储顺序： 从图像的左下角开始存储（如果biHeight为正），逐行向上
    unsigned char green;
    unsigned char blue;
};
#pragma pack(pop)

class bitmap {
private:
    const bool is_debug;
    /* 自行添加你需要的数据成员及成员函数，不限制数量 */
    int width;                    // 图像宽度
    int height;                   // 图像高度（正数表示倒向，负数表示正向）
    unsigned short bitCount;      // 每像素位数
    PixelColor* pixelData;        // 统一存储为RGB格式的像素数据
    int dataSize;                 // 像素数据大小（以像素为单位）

    PixelColor getPixelColor1Bit(const unsigned char* data, int x, int y, int stride) const;
    PixelColor getPixelColor4Bit(const unsigned char* data, int x, int y, int stride) const;
    PixelColor getPixelColor8Bit(const unsigned char* data, int x, int y, int stride) const;
    PixelColor getPixelColor24Bit(const unsigned char* data, int x, int y, int stride) const;
    PixelColor getPixelColor32Bit(const unsigned char* data, int x, int y, int stride) const;

    PixelColor getPixel(int x, int y) const;

    void show0Degree(const int top_left_x, const int top_left_y, const bool is_mirror,
        void (*draw_point)(const int x, const int y, const unsigned char red, const unsigned char green, const unsigned char blue)) const;

    void show90Degree(const int top_left_x, const int top_left_y, const bool is_mirror,
        void (*draw_point)(const int x, const int y, const unsigned char red, const unsigned char green, const unsigned char blue)) const;

    void show180Degree(const int top_left_x, const int top_left_y, const bool is_mirror,
        void (*draw_point)(const int x, const int y, const unsigned char red, const unsigned char green, const unsigned char blue)) const;

    void show270Degree(const int top_left_x, const int top_left_y, const bool is_mirror,
        void (*draw_point)(const int x, const int y, const unsigned char red, const unsigned char green, const unsigned char blue)) const;

public:
    /* 公有部分不允许添加任何内容 */
    bitmap(const char* const filename, const bool is_debug);
    ~bitmap();
    int show(const int top_left_x, const int top_left_y, const int angle, const bool is_mirror,
        void (*draw_point)(const int x, const int y, const unsigned char red, const unsigned char green, const unsigned char blue)) const;
};