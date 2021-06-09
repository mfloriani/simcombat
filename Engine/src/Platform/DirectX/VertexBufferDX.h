#pragma once

#if _DX

#include <wrl.h>
#include <d3d11.h>

#include "Platform\VertexBuffer.h"

namespace engine
{
	class Renderer;

	class VertexBufferDX : public VertexBuffer
	{
	public:
		VertexBufferDX() = default;
		virtual ~VertexBufferDX() = default;

		bool Init(Renderer* renderer, std::vector<Vertex>& vertices);
		virtual void Draw(Renderer* renderer) override;
		virtual void DrawIndexed(Renderer* renderer) override;

	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> _vertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> _indexBuffer;
	};
}
#endif