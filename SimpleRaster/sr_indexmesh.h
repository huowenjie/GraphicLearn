#ifndef __SR_INDEX_MESH_H__
#define __SR_INDEX_MESH_H__

#include <vector>
#include "sr_vec3f.h"

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

// 索引网格
class SR_IndexMesh
{
public:
    SR_IndexMesh();
    SR_IndexMesh(
        const std::vector<SR_Vec3f> &vertexList,
        const std::vector<SR_TriangleIndexList> &indexList
    );
    ~SR_IndexMesh();

public:
    void addVertex(const SR_Vec3f &v);
    void addIndexList(const SR_TriangleIndexList &list);
    void addIndexList(int a, int b, int c);

public:
    const SR_TriangleIndexList &getIndexList(int i) const;
    const SR_Vec3f &getVertex(int i) const;

private:
    std::vector<SR_TriangleIndexList> indexArray;
    std::vector<SR_Vec3f> vertexArray;
};

//-----------------------------------------------------------------------------

#endif // __SR_INDEX_MESH_H__
