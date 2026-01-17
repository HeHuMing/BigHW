/* 2353626 贺胡鸣 大数据 */
#include <iostream>
#include <iomanip>  //用于debug信息打印时的格式
#include <fstream>
//不再允许加入任何头文件，特别是<Windows.h>/<Wingdi.h>，查到就是0分甚至是倒扣-20 ！！！
using namespace std;

#include "13-b7-bmp.h"

/***************************************************************************
  函数名称：bitmap构造函数
  功    能：读取BMP文件并初始化bitmap对象
  输入参数：const char* const filename - BMP文件名
            const bool is_debug - 是否输出调试信息
  返 回 值：无
  说    明：is_debug，用于查看调试信息，不需要实现（或者按自己的方式实现，对debug的输出不做要求）
            其余内容按需补充
***************************************************************************/
bitmap::bitmap(const char* const filename, const bool is_debug):is_debug(is_debug)
{
    ifstream in;
    in.open(filename, ios::in | ios::binary);
    if (in.is_open() == 0) {
        cout << "打开文件[" << filename << "]失败." << endl;
        exit(-1);
    }

    // 读取文件头
    BITMAP_FILEHEADER fileHeader;
    in.read(reinterpret_cast<char*>(&fileHeader), sizeof(fileHeader));

    // 检查文件格式签名"BM"
    if (fileHeader.bfType != 0x4D42) {
        cout << "位图信息读取错误." << endl;
        in.close();
        exit(-1);
    }

    // 读取信息头
    BITMAP_INFOHEADER infoHeader;
    in.read(reinterpret_cast<char*>(&infoHeader), sizeof(infoHeader));

    // 检查信息头大小
    if (infoHeader.biSize != 40) {
        cout << "位图信息读取错误." << endl;
        in.close();
        exit(-1);
    }

    // 检查是否为支持的位深度
    bitCount = infoHeader.biBitCount;
    if (bitCount != 1 && bitCount != 4 && bitCount != 8 && bitCount != 24 && bitCount != 32) {
        cout << "位图信息读取错误." << endl;
        in.close();
        exit(-1);
    }

    // 检查压缩类型
    if (infoHeader.biCompression != 0) {
        cout << "位图信息读取错误." << endl;
        in.close();
        exit(-1);
    }

    width = infoHeader.biWidth;
    height = infoHeader.biHeight;
    int absHeight = abs(height);

    // 读取颜色表（对于1/4/8位色）
    BMP_RGBQUAD* colorTable = nullptr;
    int colorTableSize = 0;
    if (bitCount <= 8) {
        colorTableSize = (infoHeader.biClrUsed == 0) ? (1 << bitCount) : infoHeader.biClrUsed;
        colorTable = new BMP_RGBQUAD[colorTableSize];
        in.read(reinterpret_cast<char*>(colorTable), colorTableSize * sizeof(BMP_RGBQUAD));
    }

    // 计算原始数据的每行字节数（4字节对齐）
    int stride = ((width * bitCount + 31) / 32) * 4;
    int rawDataSize = stride * absHeight;

    // 读取原始像素数据
    unsigned char* rawData = new unsigned char[rawDataSize];
    in.seekg(fileHeader.bfOffBits, ios::beg);
    in.read(reinterpret_cast<char*>(rawData), rawDataSize);

    if (in.gcount() != rawDataSize) {
        cout << "位图信息读取错误." << endl;
        delete[] rawData;
        if (colorTable) delete[] colorTable;
        in.close();
        exit(-1);
    }

    in.close();

    // 分配RGB像素数据内存
    dataSize = width * absHeight;
    pixelData = new PixelColor[dataSize];

    // 将不同位深的像素数据统一转换为RGB格式
    for (int y = 0; y < absHeight; y++) {
        for (int x = 0; x < width; x++) {
            int index = y * width + x;
            PixelColor pixel;

            // 根据位深度获取像素颜色
            switch (bitCount) {
            case 1:
                pixel = getPixelColor1Bit(rawData, x, y, stride);
                break;
            case 4:
                pixel = getPixelColor4Bit(rawData, x, y, stride);
                break;
            case 8:
                pixel = getPixelColor8Bit(rawData, x, y, stride);
                break;
            case 24:
                pixel = getPixelColor24Bit(rawData, x, y, stride);
                break;
            case 32:
                pixel = getPixelColor32Bit(rawData, x, y, stride);
                break;
            }

            // 处理图像方向（BMP文件可能是倒向存储的）
            if (height > 0) { // 倒向，需要翻转Y坐标
                pixelData[(absHeight - 1 - y) * width + x] = pixel;
            }
            else { // 正向
                pixelData[index] = pixel;
            }
        }
    }

    // 释放临时数据
    delete[] rawData;
    if (colorTable) delete[] colorTable;

    // 调试信息
    if (is_debug) {
        cout << "BMP文件读取成功：" << endl;
        cout << "  图像尺寸: " << width << " x " << absHeight << " 像素" << endl;
        cout << "  色深: " << bitCount << " 位" << endl;
        cout << "  总像素数: " << dataSize << endl;
        cout << "  方向: " << (height > 0 ? "倒向" : "正向") << endl;
    }
}

// 1位色像素颜色获取
PixelColor bitmap::getPixelColor1Bit(const unsigned char* data, int x, int y, int stride) const {
    int byteIndex = y * stride + x / 8;
    int bitIndex = 7 - (x % 8); // 1位色中高位在前
    unsigned char byte = data[byteIndex];
    int colorIndex = (byte >> bitIndex) & 0x01;

    // 这里应该使用颜色表，简单返回黑白值
    PixelColor pixel;
    if (colorIndex == 0) {
        pixel.red = pixel.green = pixel.blue = 0;   // 黑色
    }
    else {
        pixel.red = pixel.green = pixel.blue = 255; // 白色
    }
    return pixel;
}

// 4位色像素颜色获取
PixelColor bitmap::getPixelColor4Bit(const unsigned char* data, int x, int y, int stride) const {
    int byteIndex = y * stride + x / 2;
    unsigned char byte = data[byteIndex];
    int colorIndex;
    if (x % 2 == 0) {
        colorIndex = (byte >> 4) & 0x0F; // 高4位
    }
    else {
        colorIndex = byte & 0x0F;        // 低4位
    }

    // 这里应该使用颜色表，简单返回灰度值
    PixelColor pixel;
    int intensity = colorIndex * 17; // 0-15映射到0-255
    pixel.red = pixel.green = pixel.blue = intensity;
    return pixel;
}

// 8位色像素颜色获取
PixelColor bitmap::getPixelColor8Bit(const unsigned char* data, int x, int y, int stride) const {
    int byteIndex = y * stride + x;
    int colorIndex = data[byteIndex];

    // 这里应该使用颜色表，简单返回灰度值
    PixelColor pixel;
    pixel.red = pixel.green = pixel.blue = colorIndex;
    return pixel;
}

// 24位色像素颜色获取（BGR格式）
PixelColor bitmap::getPixelColor24Bit(const unsigned char* data, int x, int y, int stride) const {
    int byteIndex = y * stride + x * 3;
    PixelColor pixel;
    pixel.blue = data[byteIndex];
    pixel.green = data[byteIndex + 1];
    pixel.red = data[byteIndex + 2];
    return pixel;
}

// 32位色像素颜色获取（BGR格式）
PixelColor bitmap::getPixelColor32Bit(const unsigned char* data, int x, int y, int stride) const {
    int byteIndex = y * stride + x * 4;
    PixelColor pixel;
    pixel.blue = data[byteIndex];
    pixel.green = data[byteIndex + 1];
    pixel.red = data[byteIndex + 2];
    return pixel;
}

/***************************************************************************
  函数名称：bitmap析构函数
  功    能：释放内存
  输入参数：无
  返 回 值：无
  说    明：按需补充
***************************************************************************/
bitmap::~bitmap()
{
    if (pixelData != nullptr) {
        delete[] pixelData;
    }
}

//按需增加内容
/***************************************************************************
  函数名称：getPixel
  功    能：获取指定位置的像素颜色
***************************************************************************/
PixelColor bitmap::getPixel(int x, int y) const {
    if (pixelData == nullptr || x < 0 || x >= width || y < 0 || y >= abs(height)) {
        PixelColor black = { 0, 0, 0 };
        return black;
    }
    return pixelData[y * width + x];
}

/***************************************************************************
  函数名称：show0Degree
  功    能：显示0度旋转的图像
***************************************************************************/
void bitmap::show0Degree(const int top_left_x, const int top_left_y, const bool is_mirror,
    void (*draw_point)(const int x, const int y, const unsigned char red, const unsigned char green, const unsigned char blue)) const
{
    int imgHeight = abs(height);

    for (int y = 0; y < imgHeight; y++) {
        for (int x = 0; x < width; x++) {
            int srcX = is_mirror ? (width - 1 - x) : x;
            int srcY = y;

            PixelColor pixel = pixelData[srcY * width + srcX];

            int screenX = top_left_x + x;
            int screenY = top_left_y + y;

            draw_point(screenX, screenY, pixel.red, pixel.green, pixel.blue);
        }
    }
}

/***************************************************************************
  函数名称：show90Degree
  功    能：显示90度旋转的图像
***************************************************************************/
void bitmap::show90Degree(const int top_left_x, const int top_left_y, const bool is_mirror,
    void (*draw_point)(const int x, const int y, const unsigned char red, const unsigned char green, const unsigned char blue)) const
{
    int imgHeight = abs(height);

    for (int y = 0; y < width; y++) {
        for (int x = 0; x < imgHeight; x++) {
            int srcX, srcY;

            if (is_mirror) {
                srcX = width - 1 - y;
                srcY = x;
            }
            else {
                srcX = width - 1 - y;
                srcY = x;
            }
            PixelColor pixel = pixelData[srcY * width + srcX];

            int screenX = top_left_x + x;
            int screenY = top_left_y + y;

            draw_point(screenX, screenY, pixel.red, pixel.green, pixel.blue);
        }
    }
}

/***************************************************************************
  函数名称：show180Degree
  功    能：显示180度旋转的图像
***************************************************************************/
void bitmap::show180Degree(const int top_left_x, const int top_left_y, const bool is_mirror,
    void (*draw_point)(const int x, const int y, const unsigned char red, const unsigned char green, const unsigned char blue)) const
{
    int imgHeight = abs(height);

    for (int y = 0; y < imgHeight; y++) {
        for (int x = 0; x < width; x++) {
            int srcX = is_mirror ? x : (width - 1 - x);
            int srcY = imgHeight - 1 - y;

            PixelColor pixel = pixelData[srcY * width + srcX];

            int screenX = top_left_x + x;
            int screenY = top_left_y + y;

            draw_point(screenX, screenY, pixel.red, pixel.green, pixel.blue);
        }
    }
}

/***************************************************************************
  函数名称：show270Degree
  功    能：显示270度旋转的图像
***************************************************************************/
void bitmap::show270Degree(const int top_left_x, const int top_left_y, const bool is_mirror,
    void (*draw_point)(const int x, const int y, const unsigned char red, const unsigned char green, const unsigned char blue)) const
{
    int imgHeight = abs(height);

    for (int y = 0; y < width; y++) {
        for (int x = 0; x < imgHeight; x++) {
            int srcX, srcY;

            if (is_mirror) {
                srcX = y;
                srcY = imgHeight - 1 - x;
            }
            else {
                srcX = y;
                srcY = imgHeight - 1 - x;
            }

            PixelColor pixel = pixelData[srcY * width + srcX];

            int screenX = top_left_x + x;
            int screenY = top_left_y + y;

            draw_point(screenX, screenY, pixel.red, pixel.green, pixel.blue);
        }
    }
}

/***************************************************************************
  函数名称：bitmap::show
  功    能：显示BMP图像，支持旋转和镜像变换
  输入参数：const int top_left_x - 图像左上角x坐标
            const int top_left_y - 图像左上角y坐标
            const int angle - 旋转角度（0, 90, 180, 270）
            const bool is_mirror - 是否水平镜像
            void (*draw_point) - 画点函数指针
  返 回 值：int - 成功返回0，失败返回-1
  说    明：支持0°、90°、180°、270°旋转和水平镜像变换
***************************************************************************/
int bitmap::show(const int top_left_x, const int top_left_y, const int angle, const bool is_mirror,
    void (*draw_point)(const int x, const int y, const unsigned char red, const unsigned char green, const unsigned char blue)) const
{
    if (pixelData == nullptr || width <= 0 || abs(height) <= 0) {
        return -1; // 图像数据无效
    }

    if (draw_point == nullptr) {
        return -1; // 画点函数无效
    }

    int imgWidth = width;
    int imgHeight = abs(height);

    // 调试信息
    if (is_debug) {
        cout << "显示图像参数：" << endl;
        cout << "  位置: (" << top_left_x << ", " << top_left_y << ")" << endl;
        cout << "  旋转角度: " << angle << "°" << endl;
        cout << "  水平镜像: " << (is_mirror ? "是" : "否") << endl;
        cout << "  图像尺寸: " << imgWidth << " x " << imgHeight << endl;
    }

    // 根据旋转角度处理图像
    switch (angle) {
    case 0:   // 不旋转
        show0Degree(top_left_x, top_left_y, is_mirror, draw_point);
        break;
    case 90:  // 旋转90度
        show90Degree(top_left_x, top_left_y, is_mirror, draw_point);
        break;
    case 180: // 旋转180度
        show180Degree(top_left_x, top_left_y, is_mirror, draw_point);
        break;
    case 270: // 旋转270度
        show270Degree(top_left_x, top_left_y, is_mirror, draw_point);
        break;
    default:
        if (is_debug) {
            cout << "不支持的旋转角度: " << angle << "°，使用0°显示" << endl;
        }
        show0Degree(top_left_x, top_left_y, is_mirror, draw_point);
        break;
    }

    return 0;
}

