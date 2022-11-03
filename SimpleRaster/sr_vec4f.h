#ifndef __SR_VEC4F_H__
#define __SR_VEC4F_H__

//-----------------------------------------------------------------------------
// 4 维向量或 3 维齐次坐标
//-----------------------------------------------------------------------------

class SR_Vec4f
{
public:
    SR_Vec4f();
    SR_Vec4f(float xyzw);
    SR_Vec4f(float xyz, float w);
    SR_Vec4f(float x, float y, float z, float w);
    SR_Vec4f(const SR_Vec4f &v);
    ~SR_Vec4f();

public:
    // 向量点乘
    static float dot(const SR_Vec4f &v1, const SR_Vec4f &v2);

    // 向量叉乘
    static SR_Vec4f cross(const SR_Vec4f &v1, const SR_Vec4f &v2);

    // 向量叉乘的模
    static float crossValue(const SR_Vec4f &v1, const SR_Vec4f &v2);

    // 求向量的夹角，弧度
    static float angle(const SR_Vec4f &v1, const SR_Vec4f &v2);

    // 向量归一化并返回一个新向量
    static SR_Vec4f normalize(const SR_Vec4f &v);

public:
    SR_Vec4f & operator=(const SR_Vec4f &v);

    // 向量加法和减法
    SR_Vec4f operator+(const SR_Vec4f &v) const;
    SR_Vec4f operator-(const SR_Vec4f &v) const;

    // 取反向量
    SR_Vec4f operator-() const;

    // 向量和实数相乘
    SR_Vec4f operator*(float t) const;
    friend SR_Vec4f operator*(float t, const SR_Vec4f &v);

    // 向量和实数相除
    SR_Vec4f operator/(float t) const;

    // 向量长度
    float length() const;

    // 向量点乘
    float dot(const SR_Vec4f &v) const;

    // 和另一个向量的夹角
    float angle(const SR_Vec4f &v) const;

    // 和另一个向量叉乘的值
    float crossValue(const SR_Vec4f &v) const;

    // 向量叉乘
    SR_Vec4f cross(const SR_Vec4f &v) const;

    // 将当前向量归一化
    void normalize();

    // 打印向量
    void printValue(const char *title = nullptr) const;

public:
    float x;
    float y;
    float z;
    float w;
};

#endif // __SR_VEC4F_H__
