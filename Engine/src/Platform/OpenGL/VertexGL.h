#pragma once

#if _GL

#include <DirectXMath.h>

namespace engine
{
	typedef unsigned int Index;

	struct Vertex
	{
		DirectX::XMFLOAT3 position;
	};

	struct ModelViewProjCB
	{
		DirectX::XMFLOAT4X4 model;
		DirectX::XMFLOAT4X4 view;
		DirectX::XMFLOAT4X4 projection;
		DirectX::XMFLOAT4X4 inverseView;

	};
}

#endif
