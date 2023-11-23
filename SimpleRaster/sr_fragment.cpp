#include "sr_fragment.h"

//-----------------------------------------------------------------------------

SR_Fragment::SR_Fragment()
{
}

SR_Fragment::~SR_Fragment()
{
}

void SR_Fragment::interpolateFragCoord(
    float alpha,
    float beta,
    float gama,
    float z,
    const SR_Vec3f &a,
    const SR_Vec3f &b,
    const SR_Vec3f &c
)
{
    fragCoord = alpha * a + beta * b + gama * c;
    fragCoord = fragCoord * z;
}

void SR_Fragment::interpolateFragColor(
    float alpha,
    float beta,
    float gama,
    float z,
    const SR_Color &a,
    const SR_Color &b,
    const SR_Color &c
)
{
    fragColor = alpha * a + beta * b + gama * c;
    fragColor = fragColor * z;
}

void SR_Fragment::interpolateNormal(
    float alpha,
    float beta,
    float gama,
    float z,
    const SR_Vec3f &a,
    const SR_Vec3f &b,
    const SR_Vec3f &c
)
{
    normal = alpha * a + beta * b + gama * c;
    normal = normal * z;
}

void SR_Fragment::interpolateUV(
    float alpha,
    float beta,
    float gama,
    float z,
    const SR_Vec2f &a,
    const SR_Vec2f &b,
    const SR_Vec2f &c
)
{
    uv = alpha * a + beta * b + gama * c;
    uv = uv * z;
}

void SR_Fragment::interpolateFragGlobal(
    float alpha,
    float beta,
    float gama,
    float z,
    const SR_Vec3f &a,
    const SR_Vec3f &b,
    const SR_Vec3f &c
)
{
    fragGlobal = alpha * a + beta * b + gama * c;
    fragGlobal = fragGlobal * z;
}

void SR_Fragment::setFragCoord(const SR_Vec3f &coord)
{
    this->fragCoord = coord;
}

void SR_Fragment::setFragColor(const SR_Color &color)
{
    this->fragColor = color;
}

void SR_Fragment::setFragNormal(const SR_Vec3f &normal)
{
    this->normal = normal;
}

void SR_Fragment::setFragSurfaceNormal(const SR_Vec3f &normal)
{
    this->surfaceNormal = normal;
}

void SR_Fragment::setFragUV(const SR_Vec2f &uv)
{
    this->uv = uv;
}

void SR_Fragment::setResolution(float width, float height)
{
    this->resolution.x = width;
    this->resolution.y = height;
    this->resolution.z = 1.0f;
}

void SR_Fragment::setFragGlobal(const SR_Vec3f &global)
{
    this->fragGlobal = global;
}

SR_Vec3f SR_Fragment::getFragCoord() const
{
    return fragCoord;
}

SR_Color SR_Fragment::getFragColor() const
{
    return fragColor;
}

SR_Vec3f SR_Fragment::getFragNormal() const
{
    return normal;
}

SR_Vec3f SR_Fragment::getFragSurfaceNormal() const
{
    return surfaceNormal;
}

SR_Vec3f SR_Fragment::getFragUV() const
{
    return uv;
}

SR_Vec3f SR_Fragment::getResolution() const
{
    return resolution;
}

SR_Vec3f SR_Fragment::getFragGlobal() const
{
    return fragGlobal;
}

//-----------------------------------------------------------------------------
