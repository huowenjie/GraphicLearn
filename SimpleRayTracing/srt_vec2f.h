#ifndef __SRT_VEC2F_H__
#define __SRT_VEC2F_H__

//-----------------------------------------------------------------------------
// 2 维向量
//-----------------------------------------------------------------------------

class SRT_Vec2f
{
public:
    SRT_Vec2f();
    SRT_Vec2f(float xy);
    SRT_Vec2f(float x, float y);
    SRT_Vec2f(const SRT_Vec2f &v);
    ~SRT_Vec2f();

public:
    // 向量点乘
    static float dot(const SRT_Vec2f &v1, const SRT_Vec2f &v2);

    // 求向量的夹角，弧度
    static float angle(const SRT_Vec2f &v1, const SRT_Vec2f &v2);

    // 向量叉乘的模，在 2 维平面上，我们忽略向量叉乘后的方向，仅计算其模
    static float crossValue(const SRT_Vec2f &v1, const SRT_Vec2f &v2);

    // 向量归一化并返回一个新向量
    static SRT_Vec2f normalize(const SRT_Vec2f &v);

public:
    SRT_Vec2f & operator=(const SRT_Vec2f &v);

    // 向量加法和减法
    SRT_Vec2f operator+(const SRT_Vec2f &v) const;
    SRT_Vec2f operator-(const SRT_Vec2f &v) const;

    // 取反向量
    SRT_Vec2f operator-() const;

    // 向量和实数相乘
    SRT_Vec2f operator*(float t) const;
    friend SRT_Vec2f operator*(float t, const SRT_Vec2f &v);

    // 向量和实数相除
    SRT_Vec2f operator/(float t) const;

    // 向量长度
    float length() const;

    // 向量点乘
    float dot(const SRT_Vec2f &v) const;

    // 和另一个向量的夹角
    float angle(const SRT_Vec2f &v) const;

    // 和另一个向量叉乘的值
    float crossValue(const SRT_Vec2f &v) const;

    // 将当前向量归一化
    void normalize();

    // 打印向量
    void printValue(const char *title = nullptr) const;

public:
    float x;
    float y;
};

//-----------------------------------------------------------------------------

#endif // __SRT_VEC2F_H__
