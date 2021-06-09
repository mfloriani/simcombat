#pragma once

#include "ECS\Component.h"

#include <DirectXMath.h>

namespace engine
{
	struct TransformComponent : public Component
	{
		TransformComponent();
		TransformComponent(DirectX::XMFLOAT3 position);
		TransformComponent(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 rotation);
		TransformComponent(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 rotation, DirectX::XMFLOAT3 scale);
		
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 rotation;
		DirectX::XMFLOAT3 scale;

		bool enabled;
	};
}