#pragma once

#include "ECS\Component.h"

#include <DirectXMath.h>

namespace engine
{
	struct RigidbodyComponent : public Component
	{
		RigidbodyComponent();
		RigidbodyComponent(float mass, DirectX::XMFLOAT3 vel);

		float mass;
		DirectX::XMFLOAT3 velocity;
	};
}