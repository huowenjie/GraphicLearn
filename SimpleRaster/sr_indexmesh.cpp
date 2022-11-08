#include "sr_indexmesh.h"

//-----------------------------------------------------------------------------

SR_IndexMesh::SR_IndexMesh()
{
}

SR_IndexMesh::SR_IndexMesh(
    const std::vector<SR_Vec3f> &vertexList,
    const std::vector<SR_TriangleIndexList> &indexList
)
{
    this->indexArray = indexList;
    this->vertexArray = vertexList;
}

SR_IndexMesh::~SR_IndexMesh()
{
}

void SR_IndexMesh::addVertex(const SR_Vec3f &v)
{
    this->vertexArray.push_back(v);
}

void SR_IndexMesh::addIndexList(const SR_TriangleIndexList &list)
{
    this->indexArray.push_back(list);
}

void SR_IndexMesh::addIndexList(int a, int b, int c)
{
    this->indexArray.push_back(SR_TriangleIndexList(a, b, c));
}

const SR_TriangleIndexList &SR_IndexMesh::getIndexList(int i) const
{
    return this->indexArray[i];
}

const SR_Vec3f &SR_IndexMesh::getVertex(int i) const
{
    return this->vertexArray[i];
}

//-----------------------------------------------------------------------------
