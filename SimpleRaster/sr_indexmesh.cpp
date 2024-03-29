#include "sr_indexmesh.h"

//-----------------------------------------------------------------------------

SR_IndexMesh::SR_IndexMesh()
{
}

SR_IndexMesh::~SR_IndexMesh()
{
}

void SR_IndexMesh::addVertex(const SR_VertexInfo &info)
{
    vertexArray.push_back(info);
}

void SR_IndexMesh::addVertex(const SR_Vec4f &v)
{
    SR_VertexInfo info;
    info.vertex = v;
    vertexArray.push_back(info);
}

void SR_IndexMesh::addVertex(const SR_Vec4f &v, const SR_Color &c)
{
    SR_VertexInfo info;

    info.vertex = v;
    info.color = c;
    vertexArray.push_back(info);
}

void SR_IndexMesh::addVertex(const SR_Vec4f &v, const SR_Color &c, const SR_Vec3f &g)
{
    SR_VertexInfo info;

    info.vertex = v;
    info.color = c;
    info.global = g;
    vertexArray.push_back(info);
}

void SR_IndexMesh::setVertex(int i, const SR_VertexInfo &info)
{
    vertexArray[i] = info;
}

void SR_IndexMesh::addIndexList(const SR_TriangleIndexList &list)
{
    indexArray.push_back(list);
}

void SR_IndexMesh::addIndexList(int a, int b, int c)
{
    indexArray.push_back(SR_TriangleIndexList(a, b, c));
}

void SR_IndexMesh::addIndexList(int a, int b, int c, const SR_Vec3f &n)
{
    indexArray.push_back(SR_TriangleIndexList(a, b, c, n));
}

void SR_IndexMesh::setIndexList(int i, const SR_TriangleIndexList &list)
{
    indexArray[i] = list;
}

void SR_IndexMesh::delIndexList(int i)
{
    indexArray.erase(indexArray.begin() + i);
}

const SR_TriangleIndexList & SR_IndexMesh::getIndexList(int i) const
{
    return indexArray[i];
}

const SR_Vec4f &SR_IndexMesh::getVertex(int i) const
{
    return vertexArray[i].vertex;
}

const SR_Color &SR_IndexMesh::getColor(int i) const
{
    return vertexArray[i].color;
}

const SR_VertexInfo &SR_IndexMesh::getVertexInfo(int i) const
{
    return vertexArray[i];
}

int SR_IndexMesh::getTriangleCount() const
{
    return (int)indexArray.size();
}

int SR_IndexMesh::getVertexCount() const
{
    return (int)vertexArray.size();
}

#if 0
void SR_IndexMesh::clipTriangle(const SR_Vec4f &n)
{
    int num = indexArray.size();
    int pos = 0;

    for (int i = 0; i < num; i++) {
        const SR_TriangleIndexList &list = indexArray[pos];

        const SR_VertexInfo &a = vertexArray[list.indexList[0]];
        const SR_VertexInfo &b = vertexArray[list.indexList[1]];
        const SR_VertexInfo &c = vertexArray[list.indexList[2]];

        float da = pointToPlane(n, a.vertex);
        float db = pointToPlane(n, b.vertex);
        float dc = pointToPlane(n, c.vertex);

        if (da > 0.0f && db > 0.0f && dc > 0.0f) {
            // 全部在平面外，不渲染
            delIndexList(pos);
        } else if (
            (da <= 0.0f && db  > 0.0f && dc  > 0.0f) ||
            (da  > 0.0f && db <= 0.0f && dc  > 0.0f) ||
            (da  > 0.0f && db  > 0.0f && dc <= 0.0f)
        ) {
            // 三角形三个点全部按照顺时针方向考虑
            // 有两个点在裁剪平面外则裁剪为一个三角形
            // 首先找到在裁剪平面内的点 v0
            SR_Vec4f v0; float d0 = 0.0f; SR_Color c0; SR_Vec3f g0;
            SR_Vec4f v1; float d1 = 0.0f; SR_Color c1; SR_Vec3f g1;
            SR_Vec4f v2; float d2 = 0.0f; SR_Color c2; SR_Vec3f g2;

            if (da <= 0.0) {
                v0 = a.vertex; d0 = da; c0 = a.color; g0 = a.global;
                v1 = b.vertex; d1 = db; c1 = b.color; g1 = b.global;
                v2 = c.vertex; d2 = dc; c2 = c.color; g2 = c.global;
            } else if (db <= 0.0f) {
                v0 = b.vertex; d0 = db; c0 = b.color; g0 = b.global;
                v1 = c.vertex; d1 = dc; c1 = c.color; g1 = c.global;
                v2 = a.vertex; d2 = da; c2 = a.color; g2 = a.global;
            } else {
                v0 = c.vertex; d0 = dc; c0 = c.color; g0 = c.global;
                v1 = a.vertex; d1 = da; c1 = a.color; g1 = a.global;
                v2 = b.vertex; d2 = db; c2 = b.color; g2 = b.global;
            }

            // p = a * tb + b * ta 且 tb = 1.0 - ta
            float ta = d0 / (d0 - d1);
            float tb = d0 / (d0 - d2);

            // 在齐次空间下通过线性插值求交点 insA、insB
            // 求出交点后，新的三角形为 v0-insA-insB
            addVertex(v0, c0, g0);
            addVertex(SR_Vec4f::lerp(v0, v1, ta), SR_Color::lerp(c0, c1, ta), SR_Vec3f::lerp(g0, g1, ta));
            addVertex(SR_Vec4f::lerp(v0, v2, tb), SR_Color::lerp(c0, c2, tb), SR_Vec3f::lerp(g0, g2, tb));

            int end = (int)vertexArray.size() - 1;
            addIndexList(end - 2, end - 1, end, list.normal);

            // 删除原有的三角形索引表
            delIndexList(pos);
        } else if (
            (da  > 0.0f && db <= 0.0f && dc <= 0.0f) ||
            (da <= 0.0f && db  > 0.0f && dc <= 0.0f) ||
            (da <= 0.0f && db <= 0.0f && dc  > 0.0f)
        ) {
            // 三角形三个点全部按照顺时针方向考虑
            // 有一个点在裁剪平面外则裁剪为一个由两个三角形组成的四边形
            // 首先找到在裁剪平面外的点 v0
            SR_Vec4f v0; float d0 = 0.0f; SR_Color c0; SR_Vec3f g0;
            SR_Vec4f v1; float d1 = 0.0f; SR_Color c1; SR_Vec3f g1;
            SR_Vec4f v2; float d2 = 0.0f; SR_Color c2; SR_Vec3f g2;

            if (da > 0.0) {
                v0 = a.vertex; d0 = da; c0 = a.color; g0 = a.global;
                v1 = b.vertex; d1 = db; c1 = b.color; g1 = b.global;
                v2 = c.vertex; d2 = dc; c2 = c.color; g2 = c.global;
            } else if (db > 0.0f) {
                v0 = b.vertex; d0 = db; c0 = b.color; g0 = b.global;
                v1 = c.vertex; d1 = dc; c1 = c.color; g1 = c.global;
                v2 = a.vertex; d2 = da; c2 = a.color; g2 = a.global;
            } else {
                v0 = c.vertex; d0 = dc; c0 = c.color; g0 = c.global;
                v1 = a.vertex; d1 = da; c1 = a.color; g1 = a.global;
                v2 = b.vertex; d2 = db; c2 = b.color; g2 = b.global;
            }

            // 三角形 abc a在外部，那么 ab ac 和裁剪平面的交点分别为 insA，insB
            // 那么原有的三角形被拆分为 insA-b-c 和 c-insB-insA
            float ta = d0 / (d0 - d1);
            float tb = d0 / (d0 - d2);

            // 分别是 insA - b - c - insB
            addVertex(SR_Vec4f::lerp(v0, v1, ta), SR_Color::lerp(c0, c1, ta), SR_Vec3f::lerp(g0, g1, ta));
            addVertex(v1, c1, g1);
            addVertex(v2, c2, g2);
            addVertex(SR_Vec4f::lerp(v0, v2, tb), SR_Color::lerp(c0, c2, tb), SR_Vec3f::lerp(g0, g2, tb));

            // 添加两个新的三角形索引表
            int end = (int)vertexArray.size() - 1;
            addIndexList(end - 3, end - 2, end - 1, list.normal);
            addIndexList(end - 1, end, end - 3, list.normal);

            // 删除原有的三角形索引表
            delIndexList(pos);
        } else {
            // 剩下就是全部顶点在裁剪平面内的情况，不做任何改变，列表索引 + 1
            pos++;
        }
    }
}
#endif

void SR_IndexMesh::clipTriangle(const SR_Vec4f &n, const SR_Vec4f &q)
{
    int num = indexArray.size();
    int pos = 0;

    for (int i = 0; i < num; i++) {
        const SR_TriangleIndexList &list = indexArray[pos];

        const SR_VertexInfo &a = vertexArray[list.indexList[0]];
        const SR_VertexInfo &b = vertexArray[list.indexList[1]];
        const SR_VertexInfo &c = vertexArray[list.indexList[2]];

        SR_Vec4f pa = a.vertex;
        SR_Vec4f pb = b.vertex;
        SR_Vec4f pc = c.vertex;
        pa.homogenDivide();
        pb.homogenDivide();
        pc.homogenDivide();

        float da = pointToPlane(n, q, pa);
        float db = pointToPlane(n, q, pb);
        float dc = pointToPlane(n, q, pc);

        if (da < 0.0f && db < 0.0f && dc < 0.0f) {
            // 全部在平面外，不渲染
            delIndexList(pos);
        } else if (
            (da > 0.0f && db < 0.0f && dc < 0.0f) ||
            (da < 0.0f && db > 0.0f && dc < 0.0f) ||
            (da < 0.0f && db < 0.0f && dc > 0.0f)
        ) {
            // 三角形三个点全部按照顺时针方向考虑
            // 有两个点在裁剪平面外则裁剪为一个三角形
            // 首先找到在裁剪平面内的点 v0
            SR_Vec4f v0; float d0 = 0.0f; SR_Color c0; SR_Vec3f g0;
            SR_Vec4f v1; float d1 = 0.0f; SR_Color c1; SR_Vec3f g1;
            SR_Vec4f v2; float d2 = 0.0f; SR_Color c2; SR_Vec3f g2;

            if (da <= 0.0) {
                v0 = a.vertex; d0 = da; c0 = a.color; g0 = a.global;
                v1 = b.vertex; d1 = db; c1 = b.color; g1 = b.global;
                v2 = c.vertex; d2 = dc; c2 = c.color; g2 = c.global;
            } else if (db <= 0.0f) {
                v0 = b.vertex; d0 = db; c0 = b.color; g0 = b.global;
                v1 = c.vertex; d1 = dc; c1 = c.color; g1 = c.global;
                v2 = a.vertex; d2 = da; c2 = a.color; g2 = a.global;
            } else {
                v0 = c.vertex; d0 = dc; c0 = c.color; g0 = c.global;
                v1 = a.vertex; d1 = da; c1 = a.color; g1 = a.global;
                v2 = b.vertex; d2 = db; c2 = b.color; g2 = b.global;
            }

            // p = a * tb + b * ta 且 tb = 1.0 - ta
            float ta = d0 / (d0 - d1);
            float tb = d0 / (d0 - d2);

            // 在齐次空间下通过线性插值求交点 insA、insB
            // 求出交点后，新的三角形为 v0-insA-insB
            addVertex(v0, c0, g0);
            addVertex(SR_Vec4f::lerp(v0, v1, ta), SR_Color::lerp(c0, c1, ta), SR_Vec3f::lerp(g0, g1, ta));
            addVertex(SR_Vec4f::lerp(v0, v2, tb), SR_Color::lerp(c0, c2, tb), SR_Vec3f::lerp(g0, g2, tb));

            int end = (int)vertexArray.size() - 1;
            addIndexList(end - 2, end - 1, end, list.normal);

            // 删除原有的三角形索引表
            delIndexList(pos);
        } else if (
            (da < 0.0f && db > 0.0f && dc > 0.0f) ||
            (da > 0.0f && db < 0.0f && dc > 0.0f) ||
            (da > 0.0f && db > 0.0f && dc < 0.0f)
        ) {
            // 三角形三个点全部按照顺时针方向考虑
            // 有一个点在裁剪平面外则裁剪为一个由两个三角形组成的四边形
            // 首先找到在裁剪平面外的点 v0
            SR_Vec4f v0; float d0 = 0.0f; SR_Color c0; SR_Vec3f g0;
            SR_Vec4f v1; float d1 = 0.0f; SR_Color c1; SR_Vec3f g1;
            SR_Vec4f v2; float d2 = 0.0f; SR_Color c2; SR_Vec3f g2;

            if (da > 0.0) {
                v0 = a.vertex; d0 = da; c0 = a.color; g0 = a.global;
                v1 = b.vertex; d1 = db; c1 = b.color; g1 = b.global;
                v2 = c.vertex; d2 = dc; c2 = c.color; g2 = c.global;
            } else if (db > 0.0f) {
                v0 = b.vertex; d0 = db; c0 = b.color; g0 = b.global;
                v1 = c.vertex; d1 = dc; c1 = c.color; g1 = c.global;
                v2 = a.vertex; d2 = da; c2 = a.color; g2 = a.global;
            } else {
                v0 = c.vertex; d0 = dc; c0 = c.color; g0 = c.global;
                v1 = a.vertex; d1 = da; c1 = a.color; g1 = a.global;
                v2 = b.vertex; d2 = db; c2 = b.color; g2 = b.global;
            }

            // 三角形 abc a在外部，那么 ab ac 和裁剪平面的交点分别为 insA，insB
            // 那么原有的三角形被拆分为 insA-b-c 和 c-insB-insA
            float ta = d0 / (d0 - d1);
            float tb = d0 / (d0 - d2);

            // 分别是 insA - b - c - insB
            addVertex(SR_Vec4f::lerp(v0, v1, ta), SR_Color::lerp(c0, c1, ta), SR_Vec3f::lerp(g0, g1, ta));
            addVertex(v1, c1, g1);
            addVertex(v2, c2, g2);
            addVertex(SR_Vec4f::lerp(v0, v2, tb), SR_Color::lerp(c0, c2, tb), SR_Vec3f::lerp(g0, g2, tb));

            // 添加两个新的三角形索引表
            int end = (int)vertexArray.size() - 1;
            addIndexList(end - 3, end - 2, end - 1, list.normal);
            addIndexList(end - 1, end, end - 3, list.normal);

            // 删除原有的三角形索引表
            delIndexList(pos);
        } else {
            // 剩下就是全部顶点在裁剪平面内的情况，不做任何改变，列表索引 + 1
            pos++;
        }
    }
}

void SR_IndexMesh::clipTriangle()
{
    // 在齐次坐标空间裁剪
    // 分别在 near far left right top bottom 平面对顶点进行裁剪
    clipTriangle(SR_Vec4f(0.0f, 0.0f, -1.0f, 1.0f), SR_Vec4f(0.0f, 0.0f, 1.0f, 1.0f));
    clipTriangle(SR_Vec4f(0.0f, 0.0f, 1.0f, 1.0f), SR_Vec4f(0.0f, 0.0f, -1.0f, 1.0f));
    clipTriangle(SR_Vec4f(1.0f, 0.0f, 0.0f, 1.0f), SR_Vec4f(-1.0f, 0.0f, 0.0f, 1.0f));
    clipTriangle(SR_Vec4f(-1.0f, 0.0f, 0.0f, 1.0f), SR_Vec4f(1.0f, 0.0f, 0.0f, 1.0f));
    clipTriangle(SR_Vec4f(0.0f, -1.0f, 0.0f, 1.0f), SR_Vec4f(0.0f, 1.0f, 0.0f, 1.0f));
    clipTriangle(SR_Vec4f(0.0f, 1.0f, 0.0f, 1.0f), SR_Vec4f(0.0f, -1.0f, 0.0f, 1.0f));
}

#if 0
float SR_IndexMesh::pointToPlane(const SR_Vec4f &n, const SR_Vec4f &p)
{
    return n.x * p.x + n.y * p.y + n.z * p.z + n.w * p.w;
}
#endif

float SR_IndexMesh::pointToPlane(
    const SR_Vec4f &n, const SR_Vec4f &q, const SR_Vec4f &p)
{
    SR_Vec4f t = p - q;
    return n.x * t.x + n.y * t.y + n.z * t.z + n.w * t.w;
}

//-----------------------------------------------------------------------------
