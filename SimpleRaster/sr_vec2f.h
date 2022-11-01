#ifndef __SR_VEC2F_H__
#define __SR_VEC2F_H__

//-----------------------------------------------------------------------------
// 2 维向量
//-----------------------------------------------------------------------------

class SR_Vec2f
{
public:
    SR_Vec2f();
    SR_Vec2f(float xy);
    SR_Vec2f(float x, float y);
    SR_Vec2f(const SR_Vec2f &v);
    ~SR_Vec2f();

public:
    // 向量点乘
    static float dot(const SR_Vec2f &v1, const SR_Vec2f &v2);

    // 求向量的夹角，弧度
    static float angle(const SR_Vec2f &v1, const SR_Vec2f &v2);

    // 向量叉乘的模，在 2 维平面上，我们忽略向量叉乘后的方向，仅计算其模
    static float crossValue(const SR_Vec2f &v1, const SR_Vec2f &v2);

    // 向量归一化并返回一个新向量
    static SR_Vec2f normalize(const SR_Vec2f &v);

public:
    SR_Vec2f & operator=(const SR_Vec2f &v);

    // 向量加法和减法
    SR_Vec2f operator+(const SR_Vec2f &v) const;
    SR_Vec2f operator-(const SR_Vec2f &v) const;

    // 取反向量
    SR_Vec2f operator-() const;

    // 向量和实数相乘
    SR_Vec2f operator*(float t) const;
    friend SR_Vec2f operator*(float t, const SR_Vec2f &v);

    // 向量和实数相除
    SR_Vec2f operator/(float t) const;

    // 向量长度
    float length() const;

    // 向量点乘
    float dot(const SR_Vec2f &v) const;

    // 和另一个向量的夹角
    float angle(const SR_Vec2f &v) const;

    // 和另一个向量叉乘的值
    float crossValue(const SR_Vec2f &v) const;

    // 将当前向量归一化
    void normalize();

    // 打印向量
    void printValue(const char *title = nullptr) const;

public:
    float x;
    float y;
};

//-----------------------------------------------------------------------------

#endif // __SR_VEC2F_H__
