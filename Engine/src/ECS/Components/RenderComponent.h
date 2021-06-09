#pragma once

#include "ECS\Component.h"

namespace engine
{
	class Mesh;
	class Texture;

	struct RenderComponent : public Component
	{
		RenderComponent(Mesh* mesh, Texture* texture);

		Mesh*    mesh;
		Texture* texture;
	};
}