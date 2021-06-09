#pragma once

#include <unordered_map>
#include <string>

#include "Platform\Mesh.h"
#include "Platform\Texture.h"

namespace engine
{
	class Renderer;
	
	class ResourceManager
	{
		using MeshMap = std::unordered_map<std::string, std::unique_ptr<Mesh>>;
		using TextureMap = std::unordered_map<std::string, std::shared_ptr<Texture>>;

	public:
		~ResourceManager();

		static ResourceManager& Get()
		{
			static ResourceManager instance;
			return instance;
		}

		bool Init(Renderer* renderer);

		Mesh* GetMesh(const std::string& name);
		Texture* GetTexture(const std::string& name);
		
		

	private:
		ResourceManager() : _renderer(nullptr) {}

		Mesh* LoadMesh(const std::string& name);
		Texture* LoadTexture(const std::string& name);
		
		void LoadPrimitives();

	private:
		MeshMap	    _meshes;
		TextureMap	_textures;
		Renderer*   _renderer;

		
		
	};
}