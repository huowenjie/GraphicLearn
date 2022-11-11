#ifndef __SR_WINDOW_H__
#define __SR_WINDOW_H__

#include "sr_indexmesh.h"
#include "sr_vec4f.h"

//-----------------------------------------------------------------------------
// 窗口上下文
//-----------------------------------------------------------------------------

// 窗口信息
class SR_WinCtx;

// 窗口上下文
class SR_Window
{
public:
    SR_Window(int width = 480, int height = 360);
    ~SR_Window();

public:

    bool initialize();
    bool terminate();

    void setUpdate(void (*update)(SR_Window &));

    // 绘制像素
    void drawPixel(const SR_Vec2f &pos, const SR_Color &color);

    // 画线
    void drawLine(const SR_Vec2f &a, const SR_Vec2f &b, const SR_Color &color);

    // 填充三角形
    void fillTriangle(
        const SR_Vec2f &a,
        const SR_Vec2f &b,
        const SR_Vec2f &c,
        const SR_Color &ca,
        const SR_Color &cb,
        const SR_Color &cc
    );

    /**
     * 对三角形网格进行齐次空间的裁剪
     * 
     * 标准齐次裁剪空间的定义如下：
     * -x + w = 0,
     *  x - w = 0,
     * -y + w = 0,
     *  y - w = 0,
     * -z + w = 0,
     *  z - w = 0
     * 
     * n 代表裁剪平面的法线，指向裁剪空间，例如近平面 near 的法线为
     * (0.0f, 0.0f, -1.0f, 1.0f)
     * 
     * a、b 和 c 分别为三角形三个顶点的信息，在透视变换之后透视除法之前，通过在
     * 齐次空间对向量进行现行插值的方法，我会分别解出边 ab 、ac 和 bc 与裁剪
     * 平面的交点位置的信息，然后将其保存在 SR_IndexMesh 中返回。
     * 
     * 裁剪主要分为四种情况：
     * 
     * 情况1、
     * 当 abc 全部在裁剪平面之外，SR_IndexMesh 会删除所有顶点信息；
     * 
     * 情况2、
     * 当 abc 完全处于裁剪平面之内，SR_IndexMesh 则保持不变；
     * 
     * 情况3、
     * abc 某一个顶点在裁剪平面外，另外两个顶点位于裁剪平面内，则需要将三角形裁成一个四边形
     * ，比如 a 点恰好位于平面外，ab 和 ac 与该裁剪平面分别相交于 m 和 n，那么四边形 mbcn
     * 则由 mbc 和 cnm 两个三角形组成，此时 SR_IndexMesh 删除处于裁剪平面外的顶点和索引表，
     * 同时新增两个裁剪后的索引表；
     * 
     * 情况4、
     * abc 两个顶点在裁剪平面外，另外一个顶点位于裁剪平面内，则裁剪的结果依旧是一个三角形
     * ，比如 bc 在外，a 在内，ab 和 ac 分别相交于 m 和 n，那么裁剪的结果即为三角形 amn，
     * SR_IndexMesh 删除处于裁剪平面外的两个顶点和索引表，重新构建三角网格。
     */
    SR_IndexMesh clipTriangle(
        const SR_Vec4f &n,
        const SR_VertexInfo &a,
        const SR_VertexInfo &b,
        const SR_VertexInfo &c
    );

    // 齐次空间上点和平面的关系，返回值的绝对值为点到平面的距离
    float pointToPlane(const SR_Vec4f &n, const SR_Vec4f &p);

    // 帧渲染
    void render();

public:
    int getWidth() const;
    int getHeight() const;

private:
    SR_WinCtx *info;
    int winWidth;
    int winHeight;

    void (*update)(SR_Window &);
};

//-----------------------------------------------------------------------------

#endif // __SR_WINDOW_H__
