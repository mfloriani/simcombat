#include "pch.h"
#include "Mesh.h"

#if _DX

#include "DirectX\VertexBufferDX.h"

#elif _GL


#endif


bool engine::Mesh::Init(Renderer* renderer, std::vector<Vertex>& vertices)
{
#if _DX
    
    _vbo = std::make_unique<VertexBufferDX>();

#elif _GL


#endif
    if (!_vbo->Init(renderer, vertices))
    {
        return false;
    }

    return true;
}
