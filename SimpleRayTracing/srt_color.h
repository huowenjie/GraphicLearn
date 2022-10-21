#ifndef __SRT_COLOR_H__
#define __SRT_COLOR_H__

//-----------------------------------------------------------------------------
// 颜色
//-----------------------------------------------------------------------------

class SRT_Color
{
public:
    // 默认状态下，a 分量为 1.0f
    SRT_Color();

    // 将 32 位整数转换为 Color 类型
    SRT_Color(unsigned int rgba);
    SRT_Color(float rgb);
    SRT_Color(float r, float g, float b);
    SRT_Color(float r, float g, float b, float a);
    SRT_Color(const SRT_Color &c);
    ~SRT_Color();

public:
    static SRT_Color fromUintRGB(unsigned int rgb);

public:
    SRT_Color & operator=(const SRT_Color &c);

    // 颜色加法和减法
    SRT_Color operator+(const SRT_Color &c) const;
    SRT_Color operator-(const SRT_Color &c) const;

    // 颜色调制
    SRT_Color operator*(float t) const;
    SRT_Color operator*(const SRT_Color &c) const;
    friend SRT_Color operator*(float t, const SRT_Color &c);

    // 转换为整数
    unsigned int toUintRGB() const;

    // 设置颜色
    void setRGB(float rgb);
    void setRGB(float r, float g, float b);

    // 打印颜色
    void printValue(const char *title = nullptr) const;

public:
    float r;
    float g;
    float b;
    float a;
};

//-----------------------------------------------------------------------------

#endif // __SRT_COLOR_H__
