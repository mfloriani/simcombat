#pragma once

#include "Constants.h"
#include <DirectXMath.h>

namespace engine
{
	class Scene;
}

class Pike
{
public:
	static void Create(engine::Scene* scene, DirectX::XMFLOAT3 position, engine::Direction direction);
};