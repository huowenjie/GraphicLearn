#ifndef __SRT_VEC3F_H__
#define __SRT_VEC3F_H__

//-----------------------------------------------------------------------------
// 3 维向量
//-----------------------------------------------------------------------------

class SRT_Vec3f
{
public:
    SRT_Vec3f();
    SRT_Vec3f(float xyz);
    SRT_Vec3f(float x, float y, float z);
    SRT_Vec3f(const SRT_Vec3f &v);
    ~SRT_Vec3f();

public:
    // 向量点乘
    static float dot(const SRT_Vec3f &v1, const SRT_Vec3f &v2);

    // 向量叉乘
    static SRT_Vec3f cross(const SRT_Vec3f &v1, const SRT_Vec3f &v2);

    // 向量叉乘的模
    static float crossValue(const SRT_Vec3f &v1, const SRT_Vec3f &v2);

    // 求向量的夹角，弧度
    static float angle(const SRT_Vec3f &v1, const SRT_Vec3f &v2);

    // 向量归一化并返回一个新向量
    static SRT_Vec3f normalize(const SRT_Vec3f &v);

public:
    SRT_Vec3f & operator=(const SRT_Vec3f &v);

    // 向量加法和减法
    SRT_Vec3f operator+(const SRT_Vec3f &v) const;
    SRT_Vec3f operator-(const SRT_Vec3f &v) const;

    // 取反向量
    SRT_Vec3f operator-() const;

    // 向量和实数相乘
    SRT_Vec3f operator*(float t) const;
    friend SRT_Vec3f operator*(float t, const SRT_Vec3f &v);

    // 向量和实数相除
    SRT_Vec3f operator/(float t) const;

    // 向量长度
    float length() const;

    // 向量点乘
    float dot(const SRT_Vec3f &v) const;

    // 和另一个向量的夹角
    float angle(const SRT_Vec3f &v) const;

    // 和另一个向量叉乘的值
    float crossValue(const SRT_Vec3f &v) const;

    // 向量叉乘
    SRT_Vec3f cross(const SRT_Vec3f &v) const;

    // 将当前向量归一化
    void normalize();

    // 打印向量
    void printValue(const char *title = nullptr) const;

public:
    float x;
    float y;
    float z;
};

//-----------------------------------------------------------------------------

#endif // __SRT_VEC3F_H__
