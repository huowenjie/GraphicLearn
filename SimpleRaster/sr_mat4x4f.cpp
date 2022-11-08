#include <cmath>
#include <cstdio>
#include <cstring>
#include "sr_mat4x4f.h"
#include "sr_mat3x3f.h"

//-----------------------------------------------------------------------------

SR_Mat4x4f::SR_Mat4x4f()
{
    this->m00 = this->m11 = this->m22 = this->m33 = 1.0f;
    this->m01 = this->m02 = this->m03 = 0.0f;
    this->m10 = this->m12 = this->m13 = 0.0f;
    this->m20 = this->m21 = this->m23 = 0.0f;
    this->m30 = this->m31 = this->m32 = 0.0f;
}

SR_Mat4x4f::SR_Mat4x4f(float elem)
{
    this->m00 = this->m01 = this->m02 = this->m03 = elem;
    this->m10 = this->m11 = this->m12 = this->m13 = elem;
    this->m20 = this->m21 = this->m22 = this->m23 = elem;
    this->m30 = this->m31 = this->m32 = this->m33 = elem;
}

SR_Mat4x4f::SR_Mat4x4f(float *elems)
{
    this->m00 = elems[0];
    this->m01 = elems[1];
    this->m02 = elems[2];
    this->m03 = elems[3];
    this->m10 = elems[4];
    this->m11 = elems[5];
    this->m12 = elems[6];
    this->m13 = elems[7];
    this->m20 = elems[8];
    this->m21 = elems[9];
    this->m22 = elems[10];
    this->m23 = elems[11];
    this->m30 = elems[12];
    this->m31 = elems[13];
    this->m32 = elems[14];
    this->m33 = elems[15];
}

SR_Mat4x4f::SR_Mat4x4f(float (*elems)[4])
{
    this->m00 = elems[0][0];
    this->m01 = elems[0][1];
    this->m02 = elems[0][2];
    this->m03 = elems[0][3];
    this->m10 = elems[1][0];
    this->m11 = elems[1][1];
    this->m12 = elems[1][2];
    this->m13 = elems[1][3];
    this->m20 = elems[2][0];
    this->m21 = elems[2][1];
    this->m22 = elems[2][2];
    this->m23 = elems[2][3];
    this->m30 = elems[3][0];
    this->m31 = elems[3][1];
    this->m32 = elems[3][2];
    this->m33 = elems[3][3];
}

SR_Mat4x4f::SR_Mat4x4f(const SR_Mat4x4f &mat)
{
    this->m00 = mat.m00;
    this->m01 = mat.m01;
    this->m02 = mat.m02;
    this->m03 = mat.m03;
    this->m10 = mat.m10;
    this->m11 = mat.m11;
    this->m12 = mat.m12;
    this->m13 = mat.m13;
    this->m20 = mat.m20;
    this->m21 = mat.m21;
    this->m22 = mat.m22;
    this->m23 = mat.m23;
    this->m30 = mat.m30;
    this->m31 = mat.m31;
    this->m32 = mat.m32;
    this->m33 = mat.m33;
}

SR_Mat4x4f::~SR_Mat4x4f()
{
}

//-----------------------------------------------------------------------------

SR_Mat4x4f SR_Mat4x4f::scaleMatrix(float sx, float sy, float sz)
{
    SR_Mat4x4f tmp;

    tmp.m00 = sx;
    tmp.m11 = sx;
    tmp.m22 = sx;

    return tmp;
}

SR_Mat4x4f SR_Mat4x4f::rotateZMatrix(float theta)
{
    SR_Mat4x4f tmp;

    tmp.m00 = std::cos(theta);
    tmp.m01 = -std::sin(theta);
    tmp.m10 = std::sin(theta);
    tmp.m11 = std::cos(theta);

    return tmp;
}

SR_Mat4x4f SR_Mat4x4f::rotateXMatrix(float theta)
{
    SR_Mat4x4f tmp;

    tmp.m11 = std::cos(theta);
    tmp.m12 = -std::sin(theta);
    tmp.m21 = std::sin(theta);
    tmp.m22 = std::cos(theta);

    return tmp;
}

SR_Mat4x4f SR_Mat4x4f::rotateYMatrix(float theta)
{
    SR_Mat4x4f tmp;

    tmp.m00 = std::cos(theta);
    tmp.m20 = -std::sin(theta);
    tmp.m02 = std::sin(theta);
    tmp.m22 = std::cos(theta);

    return tmp;
}

SR_Mat4x4f SR_Mat4x4f::translateMatrix(float dx, float dy, float dz)
{
    SR_Mat4x4f tmp;

    tmp.m03 = dx;
    tmp.m13 = dy;
    tmp.m23 = dz;

    return tmp;
}

SR_Mat4x4f SR_Mat4x4f::volumeMapMatrix(
    const SR_Vec3f &l,
    const SR_Vec3f &h,
    const SR_Vec3f &lp,
    const SR_Vec3f &hp
)
{
    SR_Mat4x4f tmp;

    tmp.m00 = (hp.x - lp.x) / (h.x - l.x);
    tmp.m11 = (hp.y - lp.y) / (h.y - l.y);
    tmp.m22 = (hp.z - lp.z) / (h.z - l.z);
    tmp.m03 = (lp.x * h.x - hp.x * l.x) / (h.x - l.x);
    tmp.m13 = (lp.y * h.y - hp.y * l.y) / (h.y - l.y);
    tmp.m23 = (lp.z * h.z - hp.z * l.z) / (h.z - l.z);

    return tmp;
}

SR_Mat4x4f SR_Mat4x4f::viewportMatrix(int width, int height)
{
    SR_Mat4x4f tmp;
    float nx = width;
    float ny = height;

    tmp.m00 = nx / 2.0f;
    tmp.m11 = ny / 2.0f;
    tmp.m03 = (nx - 1.0f) / 2.0f;
    tmp.m13 = (ny - 1.0f) / 2.0f;

    return tmp;
}


SR_Mat4x4f SR_Mat4x4f::cameraMatrix(const SR_Vec4f &pos, const SR_Vec4f &target)
{
    SR_Vec4f z = SR_Vec4f(0.0f, 1.0f, 0.0f, 0.0f);
    SR_Vec4f w = pos - target;
    w.normalize();

    SR_Vec4f u = z.cross(w);
    u.normalize();

    SR_Vec4f v = w.cross(u);
    v.normalize();

    SR_Mat4x4f cam;

    cam.m00 = u.x;
    cam.m01 = u.y;
    cam.m02 = u.z;
    cam.m03 = -pos.x;

    cam.m10 = v.x;
    cam.m11 = v.y;
    cam.m12 = v.z;
    cam.m13 = -pos.y;

    cam.m20 = w.x;
    cam.m21 = w.y;
    cam.m22 = w.z;
    cam.m23 = -pos.z;

    return cam;
}

SR_Mat4x4f SR_Mat4x4f::perspectiveMatrix(const SR_Vec3f &l, const SR_Vec3f &h)
{
    SR_Mat4x4f per;

    per.m33 = 0.0f;
    per.m32 = 1.0f;

    per.m00 = l.z * 2.0f / (h.x - l.x);
    per.m02 = (l.x + h.x) / (l.x - h.x);
    per.m11 = l.z * 2.0f / (h.y - l.y);
    per.m12 = (l.y + h.y) / (l.y - h.y);
    per.m22 = (l.z + h.z) / (l.z - h.z);
    per.m23 = 2.0 * l.z * h.z / (h.z - l.z);

    return per;
}

//-----------------------------------------------------------------------------

SR_Mat4x4f SR_Mat4x4f::unitMatrix()
{
    return SR_Mat4x4f();
}

SR_Mat4x4f SR_Mat4x4f::zeroMatrix()
{
    return SR_Mat4x4f(0.0f);
}

SR_Mat4x4f SR_Mat4x4f::transpose(const SR_Mat4x4f &mat)
{
    SR_Mat4x4f tmp(mat);

    tmp.m01 = mat.m10;
    tmp.m10 = mat.m01;
    tmp.m02 = mat.m20;
    tmp.m20 = mat.m02;
    tmp.m03 = mat.m30;
    tmp.m30 = mat.m03;
    tmp.m12 = mat.m21;
    tmp.m21 = mat.m12;
    tmp.m13 = mat.m31;
    tmp.m31 = mat.m13;
    tmp.m23 = mat.m32;
    tmp.m32 = mat.m23;

    return tmp;
}

SR_Mat4x4f SR_Mat4x4f::inverse(const SR_Mat4x4f &mat)
{
    // 求行列式
    float det = SR_Mat4x4f::determinant(mat);

    // 行列式为 0，不可逆，直接返回原矩阵
    if (std::abs(det) < 1e-6f) {
        return mat;
    }

    /*
     * 利用克拉默法则来求解逆矩阵
     * inverse A = adj A / det A
     * [ +c00, -c10, +c20, -c30 ]
     * [ -c01, +c11, -c21, +c31 ]
     * [ +c02, -c12, +c22, -c32 ]
     * [ -c03, +c13, -c23, +c33 ]
     */
    SR_Mat4x4f adj;
    SR_Mat3x3f tmp;

    // c00
    tmp.m00 = mat.m11; tmp.m01 = mat.m12; tmp.m02 = mat.m13;
    tmp.m10 = mat.m21; tmp.m11 = mat.m22; tmp.m12 = mat.m23;
    tmp.m20 = mat.m31; tmp.m21 = mat.m32; tmp.m22 = mat.m33;
    adj.m00 = tmp.determinant() / det;

    // c10
    tmp.m00 = mat.m01; tmp.m01 = mat.m02; tmp.m02 = mat.m03;
    tmp.m10 = mat.m21; tmp.m11 = mat.m22; tmp.m12 = mat.m23;
    tmp.m20 = mat.m31; tmp.m21 = mat.m32; tmp.m22 = mat.m33;
    adj.m01 = -(tmp.determinant()) / det;

    // c20
    tmp.m00 = mat.m01; tmp.m01 = mat.m02; tmp.m02 = mat.m03;
    tmp.m10 = mat.m11; tmp.m11 = mat.m12; tmp.m12 = mat.m13;
    tmp.m20 = mat.m31; tmp.m21 = mat.m32; tmp.m22 = mat.m33;
    adj.m02 = tmp.determinant() / det;

    // c30
    tmp.m00 = mat.m01; tmp.m01 = mat.m02; tmp.m02 = mat.m03;
    tmp.m10 = mat.m11; tmp.m11 = mat.m12; tmp.m12 = mat.m13;
    tmp.m20 = mat.m21; tmp.m21 = mat.m22; tmp.m22 = mat.m23;
    adj.m03 = -(tmp.determinant()) / det;

    // c01
    tmp.m00 = mat.m10; tmp.m01 = mat.m12; tmp.m02 = mat.m13;
    tmp.m10 = mat.m20; tmp.m11 = mat.m22; tmp.m12 = mat.m23;
    tmp.m20 = mat.m30; tmp.m21 = mat.m32; tmp.m22 = mat.m33;
    adj.m10 = -(tmp.determinant()) / det;

    // c11
    tmp.m00 = mat.m00; tmp.m01 = mat.m02; tmp.m02 = mat.m03;
    tmp.m10 = mat.m20; tmp.m11 = mat.m22; tmp.m12 = mat.m23;
    tmp.m20 = mat.m30; tmp.m21 = mat.m32; tmp.m22 = mat.m33;
    adj.m11 = tmp.determinant() / det;

    // c21
    tmp.m00 = mat.m00; tmp.m01 = mat.m02; tmp.m02 = mat.m03;
    tmp.m10 = mat.m10; tmp.m11 = mat.m12; tmp.m12 = mat.m13;
    tmp.m20 = mat.m30; tmp.m21 = mat.m32; tmp.m22 = mat.m33;
    adj.m12 = -(tmp.determinant()) / det;

    // c31
    tmp.m00 = mat.m00; tmp.m01 = mat.m02; tmp.m02 = mat.m03;
    tmp.m10 = mat.m10; tmp.m11 = mat.m12; tmp.m12 = mat.m13;
    tmp.m20 = mat.m20; tmp.m21 = mat.m22; tmp.m22 = mat.m23;
    adj.m13 = tmp.determinant() / det;

    // c02
    tmp.m00 = mat.m10; tmp.m01 = mat.m11; tmp.m02 = mat.m13;
    tmp.m10 = mat.m20; tmp.m11 = mat.m21; tmp.m12 = mat.m23;
    tmp.m20 = mat.m30; tmp.m21 = mat.m31; tmp.m22 = mat.m33;
    adj.m20 = tmp.determinant() / det;

    // c12
    tmp.m00 = mat.m00; tmp.m01 = mat.m01; tmp.m02 = mat.m03;
    tmp.m10 = mat.m20; tmp.m11 = mat.m21; tmp.m12 = mat.m23;
    tmp.m20 = mat.m30; tmp.m21 = mat.m31; tmp.m22 = mat.m33;
    adj.m21 = -(tmp.determinant()) / det;

    // c22
    tmp.m00 = mat.m00; tmp.m01 = mat.m01; tmp.m02 = mat.m03;
    tmp.m10 = mat.m10; tmp.m11 = mat.m11; tmp.m12 = mat.m13;
    tmp.m20 = mat.m30; tmp.m21 = mat.m31; tmp.m22 = mat.m33;
    adj.m22 = tmp.determinant() / det;

    // c32
    tmp.m00 = mat.m00; tmp.m01 = mat.m01; tmp.m02 = mat.m03;
    tmp.m10 = mat.m10; tmp.m11 = mat.m11; tmp.m12 = mat.m13;
    tmp.m20 = mat.m20; tmp.m21 = mat.m21; tmp.m22 = mat.m23;
    adj.m23 = -(tmp.determinant()) / det;

    // c03
    tmp.m00 = mat.m10; tmp.m01 = mat.m11; tmp.m02 = mat.m12;
    tmp.m10 = mat.m20; tmp.m11 = mat.m21; tmp.m12 = mat.m22;
    tmp.m20 = mat.m30; tmp.m21 = mat.m31; tmp.m22 = mat.m32;
    adj.m30 = -(tmp.determinant()) / det;

    // c13
    tmp.m00 = mat.m00; tmp.m01 = mat.m01; tmp.m02 = mat.m02;
    tmp.m10 = mat.m20; tmp.m11 = mat.m21; tmp.m12 = mat.m22;
    tmp.m20 = mat.m30; tmp.m21 = mat.m31; tmp.m22 = mat.m32;
    adj.m31 = tmp.determinant() / det;

    // c23
    tmp.m00 = mat.m00; tmp.m01 = mat.m01; tmp.m02 = mat.m02;
    tmp.m10 = mat.m10; tmp.m11 = mat.m11; tmp.m12 = mat.m12;
    tmp.m20 = mat.m30; tmp.m21 = mat.m31; tmp.m22 = mat.m32;
    adj.m32 = -(tmp.determinant()) / det;

    // c33
    tmp.m00 = mat.m00; tmp.m01 = mat.m01; tmp.m02 = mat.m02;
    tmp.m10 = mat.m10; tmp.m11 = mat.m11; tmp.m12 = mat.m12;
    tmp.m20 = mat.m20; tmp.m21 = mat.m21; tmp.m22 = mat.m22;
    adj.m33 = tmp.determinant() / det;

    return adj;
}

float SR_Mat4x4f::determinant(const SR_Mat4x4f &mat)
{
    float det = 0.0f;
    SR_Mat3x3f tmp;

    tmp.m00 = mat.m11; tmp.m01 = mat.m12; tmp.m02 = mat.m13;
    tmp.m10 = mat.m21; tmp.m11 = mat.m22; tmp.m12 = mat.m23;
    tmp.m20 = mat.m31; tmp.m21 = mat.m32; tmp.m22 = mat.m33;

    det = mat.m00 * tmp.determinant();

    tmp.m00 = mat.m10; tmp.m01 = mat.m12; tmp.m02 = mat.m13;
    tmp.m10 = mat.m20; tmp.m11 = mat.m22; tmp.m12 = mat.m23;
    tmp.m20 = mat.m30; tmp.m21 = mat.m32; tmp.m22 = mat.m33;

    det -= mat.m01 * tmp.determinant();

    tmp.m00 = mat.m10; tmp.m01 = mat.m11; tmp.m02 = mat.m13;
    tmp.m10 = mat.m20; tmp.m11 = mat.m21; tmp.m12 = mat.m23;
    tmp.m20 = mat.m30; tmp.m21 = mat.m31; tmp.m22 = mat.m33;

    det += mat.m02 * tmp.determinant();

    tmp.m00 = mat.m10; tmp.m01 = mat.m11; tmp.m02 = mat.m12;
    tmp.m10 = mat.m20; tmp.m11 = mat.m21; tmp.m12 = mat.m22;
    tmp.m20 = mat.m30; tmp.m21 = mat.m31; tmp.m22 = mat.m32;

    det -= mat.m03 * tmp.determinant();
    return det;
}

SR_Mat4x4f & SR_Mat4x4f::operator=(const SR_Mat4x4f &mat)
{
    if (&mat == this) {
        return *this;
    }

    this->m00 = mat.m00;
    this->m01 = mat.m01;
    this->m02 = mat.m02;
    this->m03 = mat.m03;
    this->m10 = mat.m10;
    this->m11 = mat.m11;
    this->m12 = mat.m12;
    this->m13 = mat.m13;
    this->m20 = mat.m20;
    this->m21 = mat.m21;
    this->m22 = mat.m22;
    this->m23 = mat.m23;
    this->m30 = mat.m30;
    this->m31 = mat.m31;
    this->m32 = mat.m32;
    this->m33 = mat.m33;
    return *this;
}

SR_Mat4x4f SR_Mat4x4f::operator+(const SR_Mat4x4f &mat) const
{
    SR_Mat4x4f tmp(*this);

    tmp.m00 += mat.m00;
    tmp.m01 += mat.m01;
    tmp.m02 += mat.m02;
    tmp.m03 += mat.m03;
    tmp.m10 += mat.m10;
    tmp.m11 += mat.m11;
    tmp.m12 += mat.m12;
    tmp.m13 += mat.m13;
    tmp.m20 += mat.m20;
    tmp.m21 += mat.m21;
    tmp.m22 += mat.m22;
    tmp.m23 += mat.m23;
    tmp.m30 += mat.m30;
    tmp.m31 += mat.m31;
    tmp.m32 += mat.m32;
    tmp.m33 += mat.m33;

    return tmp;
}

SR_Mat4x4f SR_Mat4x4f::operator-(const SR_Mat4x4f &mat) const
{
    SR_Mat4x4f tmp(*this);

    tmp.m00 -= mat.m00;
    tmp.m01 -= mat.m01;
    tmp.m02 -= mat.m02;
    tmp.m03 -= mat.m03;
    tmp.m10 -= mat.m10;
    tmp.m11 -= mat.m11;
    tmp.m12 -= mat.m12;
    tmp.m13 -= mat.m13;
    tmp.m20 -= mat.m20;
    tmp.m21 -= mat.m21;
    tmp.m22 -= mat.m22;
    tmp.m23 -= mat.m23;
    tmp.m30 -= mat.m30;
    tmp.m31 -= mat.m31;
    tmp.m32 -= mat.m32;
    tmp.m33 -= mat.m33;

    return tmp;
}

SR_Mat4x4f SR_Mat4x4f::operator*(float val) const
{
    SR_Mat4x4f tmp(*this);

    tmp.m00 *= val;
    tmp.m01 *= val;
    tmp.m02 *= val;
    tmp.m03 *= val;
    tmp.m10 *= val;
    tmp.m11 *= val;
    tmp.m12 *= val;
    tmp.m13 *= val;
    tmp.m20 *= val;
    tmp.m21 *= val;
    tmp.m22 *= val;
    tmp.m23 *= val;
    tmp.m30 *= val;
    tmp.m31 *= val;
    tmp.m32 *= val;
    tmp.m33 *= val;

    return tmp;
}

SR_Mat4x4f operator*(float val, const SR_Mat4x4f &mat)
{
    SR_Mat4x4f tmp(mat);

    tmp.m00 *= val;
    tmp.m01 *= val;
    tmp.m02 *= val;
    tmp.m03 *= val;
    tmp.m10 *= val;
    tmp.m11 *= val;
    tmp.m12 *= val;
    tmp.m13 *= val;
    tmp.m20 *= val;
    tmp.m21 *= val;
    tmp.m22 *= val;
    tmp.m23 *= val;
    tmp.m30 *= val;
    tmp.m31 *= val;
    tmp.m32 *= val;
    tmp.m33 *= val;

    return tmp;
}

SR_Mat4x4f SR_Mat4x4f::operator*(const SR_Mat4x4f &mat) const
{
    SR_Mat4x4f tmp(0.0f);

    tmp.m00 = m00 * mat.m00 + m01 * mat.m10 + m02 * mat.m20 + m03 * mat.m30;
    tmp.m01 = m00 * mat.m01 + m01 * mat.m11 + m02 * mat.m21 + m03 * mat.m31;
    tmp.m02 = m00 * mat.m02 + m01 * mat.m12 + m02 * mat.m22 + m03 * mat.m32;
    tmp.m03 = m00 * mat.m03 + m01 * mat.m13 + m02 * mat.m23 + m03 * mat.m33;

    tmp.m10 = m10 * mat.m00 + m11 * mat.m10 + m12 * mat.m20 + m13 * mat.m30;
    tmp.m11 = m10 * mat.m01 + m11 * mat.m11 + m12 * mat.m21 + m13 * mat.m31;
    tmp.m12 = m10 * mat.m02 + m11 * mat.m12 + m12 * mat.m22 + m13 * mat.m32;
    tmp.m13 = m10 * mat.m03 + m11 * mat.m13 + m12 * mat.m23 + m13 * mat.m33;

    tmp.m20 = m20 * mat.m00 + m21 * mat.m10 + m22 * mat.m20 + m23 * mat.m30;
    tmp.m21 = m20 * mat.m01 + m21 * mat.m11 + m22 * mat.m21 + m23 * mat.m31;
    tmp.m22 = m20 * mat.m02 + m21 * mat.m12 + m22 * mat.m22 + m23 * mat.m32;
    tmp.m23 = m20 * mat.m03 + m21 * mat.m13 + m22 * mat.m23 + m23 * mat.m33;

    tmp.m30 = m30 * mat.m00 + m31 * mat.m10 + m32 * mat.m20 + m33 * mat.m30;
    tmp.m31 = m30 * mat.m01 + m31 * mat.m11 + m32 * mat.m21 + m33 * mat.m31;
    tmp.m32 = m30 * mat.m02 + m31 * mat.m12 + m32 * mat.m22 + m33 * mat.m32;
    tmp.m33 = m30 * mat.m03 + m31 * mat.m13 + m32 * mat.m23 + m33 * mat.m33;

    return tmp;
}

SR_Vec4f SR_Mat4x4f::operator*(const SR_Vec4f &vec) const
{
    SR_Vec4f tmp;

    tmp.x = m00 * vec.x + m01 * vec.y + m02 * vec.z + m03 * vec.w;
    tmp.y = m10 * vec.x + m11 * vec.y + m12 * vec.z + m13 * vec.w;
    tmp.z = m20 * vec.x + m21 * vec.y + m22 * vec.z + m23 * vec.w;
    tmp.w = m30 * vec.x + m31 * vec.y + m32 * vec.z + m33 * vec.w;
    return tmp;
}

float *SR_Mat4x4f::operator[](int i)
{
    return matij[i];
}

SR_Mat4x4f SR_Mat4x4f::operator-() const
{
    return SR_Mat4x4f::inverse(*this);
}

SR_Mat4x4f SR_Mat4x4f::transpose() const
{
    return SR_Mat4x4f::transpose(*this);
}

float SR_Mat4x4f::determinant() const
{
    return SR_Mat4x4f::determinant(*this);
}

void SR_Mat4x4f::printValue(const char *title) const
{
    if (!title || !title[0]) {
        title = "";
    }

    std::printf(
        "%s:\n"
        "[%f, %f, %f, %f]\n"
        "[%f, %f, %f, %f]\n"
        "[%f, %f, %f, %f]\n"
        "[%f, %f, %f, %f]\n",
        title,
        m00, m01, m02, m03,
        m10, m11, m12, m13,
        m20, m21, m22, m23,
        m30, m31, m32, m33
    );
}

//-----------------------------------------------------------------------------
