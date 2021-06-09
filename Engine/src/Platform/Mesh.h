#pragma once

#include <memory>
#include <vector>

#if _DX
#include "DirectX\VertexDX.h"
#elif _GL

#endif

#include "VertexBuffer.h"


namespace engine
{
	class Renderer;

	class Mesh
	{
	public:
		Mesh() = default;
		~Mesh() = default;

		bool Init(Renderer* renderer, std::vector<Vertex>& vertices);
		
		VertexBuffer* GetVertexBuffer() { return _vbo.get(); }

	private:
		std::unique_ptr<VertexBuffer> _vbo;		
	};
}