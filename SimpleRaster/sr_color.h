#ifndef __SR_COLOR_H__
#define __SR_COLOR_H__

//-----------------------------------------------------------------------------
// 颜色
//-----------------------------------------------------------------------------

class SR_Color
{
public:
    // 默认状态下，a 分量为 1.0f
    SR_Color();

    // 将 32 位整数转换为 Color 类型
    SR_Color(unsigned int rgba);
    SR_Color(float rgb);
    SR_Color(float r, float g, float b);
    SR_Color(float r, float g, float b, float a);
    SR_Color(const SR_Color &c);
    ~SR_Color();

public:
    static SR_Color fromUintRGB(unsigned int rgb);

public:
    SR_Color & operator=(const SR_Color &c);

    // 颜色加法和减法
    SR_Color operator+(const SR_Color &c) const;
    SR_Color operator-(const SR_Color &c) const;

    // 颜色调制
    SR_Color operator*(float t) const;
    SR_Color operator*(const SR_Color &c) const;
    friend SR_Color operator*(float t, const SR_Color &c);

    // 转换为整数
    unsigned int toUintRGB() const;

    // 设置颜色
    void setRGB(float rgb);
    void setRGB(float r, float g, float b);

    // 限制颜色范围
    void clamp();

    // 打印颜色
    void printValue(const char *title = nullptr) const;

public:
    float r;
    float g;
    float b;
    float a;
};

//-----------------------------------------------------------------------------

#endif // __SR_COLOR_H__
