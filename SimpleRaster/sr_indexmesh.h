#ifndef __SR_INDEX_MESH_H__
#define __SR_INDEX_MESH_H__

#include <vector>
#include "sr_vec4f.h"
#include "sr_color.h"

//-----------------------------------------------------------------------------
// 索引网格定义
//-----------------------------------------------------------------------------

// 三角形索引表
class SR_TriangleIndexList
{
public:
    SR_TriangleIndexList()
    {
        indexList[0] = 0;
        indexList[1] = 0;
        indexList[2] = 0;
    }

    SR_TriangleIndexList(int a, int b, int c)
    {
        indexList[0] = a;
        indexList[1] = b;
        indexList[2] = c;
    }

public:
    int indexList[3];
};

// 顶点信息
class SR_VertexInfo
{
public:
    SR_VertexInfo()
    {
    }

    SR_VertexInfo(const SR_Vec4f &v, const SR_Color &c)
    {
        vertex = v;
        color = c;
    }

public:
    SR_Vec4f vertex;
    SR_Color color;
};

// 索引网格
class SR_IndexMesh
{
public:
    SR_IndexMesh();
    ~SR_IndexMesh();

public:
    // 添加顶点
    void addVertex(const SR_VertexInfo &info);
    void addVertex(const SR_Vec4f &v);
    void addVertex(const SR_Vec4f &v, const SR_Color &c);

    // 设置顶点
    void setVertex(int i, const SR_VertexInfo &info);

    void addIndexList(const SR_TriangleIndexList &list);
    void addIndexList(int a, int b, int c);

    // 移除索引表
    void delIndexList(int i);

public:
    const SR_TriangleIndexList &getIndexList(int i) const;
    const SR_Vec4f &getVertex(int i) const;
    const SR_Color &getColor(int i) const;
    const SR_VertexInfo &getVertexInfo(int i) const;

    // 获取三角形数量
    int getTriangleCount() const;

    // 获取顶点数量
    int getVertexCount() const;

public:
    /**
     * 对三角形网格进行齐次空间的裁剪
     * 
     * 标准齐次裁剪空间的定义如下：
     * -x + lw = 0 => x + w = 0,
     *  x - rw = 0 => -x + w = 0,
     * -y + bw = 0 => y + w = 0,
     *  y - tw = 0 => -y + w = 0,
     * -z + nw = 0 => -z + w = 0,
     *  z - fw = 0 => z + w = 0
     * 
     * [l, r], [b, t], [f, n] 分别为
     * [-1, 1], [-1, 1], [-1, 1]
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
    void clipTriangle(const SR_Vec4f &n);

private:
    // 齐次空间上点和平面的关系，返回值的绝对值为点到平面的距离
    float pointToPlane(const SR_Vec4f &n, const SR_Vec4f &p);

private:
    std::vector<SR_TriangleIndexList> indexArray;
    std::vector<SR_VertexInfo> vertexArray;
};

//-----------------------------------------------------------------------------

#endif // __SR_INDEX_MESH_H__
