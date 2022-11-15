#ifndef __SR_FRAGMENT_H__
#define __SR_FRAGMENT_H__

#include "sr_indexmesh.h"

//-----------------------------------------------------------------------------
// 片段
//-----------------------------------------------------------------------------

class SR_Fragment
{
public:
    SR_VertexInfo vertex;
    SR_Vec3f normal;
    SR_Vec2f uv;
};

//-----------------------------------------------------------------------------

#endif // __SR_FRAGMENT_H__
