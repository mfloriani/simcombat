#pragma once

#if _DX
#include "DirectX\VertexDX.h"
#elif _GL
#include "OpenGL\VertexGL.h"
#include <vector>
#endif

namespace engine
{
	class Renderer;

	class VertexBuffer
	{
	public:
		VertexBuffer();
		virtual ~VertexBuffer() = default;

	public:
		virtual bool Init(Renderer* renderer, std::vector<Vertex>& vertices) = 0;
		virtual void Draw(Renderer* renderer) = 0;
		virtual void DrawIndexed(Renderer* renderer) = 0;

	protected:
		unsigned int _numVertices;
		unsigned int _numIndices;
	};
}