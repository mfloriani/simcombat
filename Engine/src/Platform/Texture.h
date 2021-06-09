#pragma once

#include <string>

namespace engine
{
	class Renderer;

	class Texture
	{
	public:
		Texture() = default;
		virtual ~Texture() = default;

		virtual bool Load(Renderer* renderer, const std::string& filename) = 0;
	};
}