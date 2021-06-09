#include "pch.h"
#include "ResourceManager.h"
#include "Platform\Renderer.h"

#if _DX

#include "Platform\DirectX\TextureDX.h"

using namespace DirectX;

#elif _GL

#endif

using namespace engine;



bool ResourceManager::Init(Renderer* renderer)
{
	_renderer = renderer;
	LoadPrimitives();
	return true;
}

ResourceManager::~ResourceManager()
{
	
}

Mesh* ResourceManager::GetMesh(const std::string& name)
{
	auto it = _meshes.find(name);
	if (it != _meshes.end())
	{
		return it->second.get(); // already loaded
	}
	return LoadMesh(name); // load it now
}

Texture* ResourceManager::GetTexture(const std::string& name)
{
	auto it = _textures.find(name);
	if (it != _textures.end())
	{
		return it->second.get(); // already loaded
	}
	return LoadTexture(name); // load it now
}


Mesh* ResourceManager::LoadMesh(const std::string& name)
{
	ENGINE_ERROR("ResourceManager::LoadMesh not implemented");
	//if (_meshes.find(name) == _meshes.end()) // add only when not found
	//{
	//	std::unique_ptr<Mesh> mesh = std::make_unique<Mesh>();
	//	mesh->Init(_renderer, vertices);
	//	_meshes[name] = std::move(mesh);
	//}

	//return _meshes[name].get();
	return NULL;
}

Texture* ResourceManager::LoadTexture(const std::string& name)
{
#if _DX
	auto tex = std::make_shared<TextureDX>();
	if (!tex->Load(_renderer, name))
	{
		return nullptr;
	}

	_textures.insert(std::make_pair(name, tex));

#elif _GL
	ENGINE_ERROR("ResourceManager::LoadTexture not implemented for OpenGL");
#endif

	return _textures[name].get();
}

void ResourceManager::LoadPrimitives()
{
#if _DX
	std::vector<Vertex> vertices =	{{DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f)}};
	std::unique_ptr<Mesh> mesh = std::make_unique<Mesh>();
	mesh->Init(_renderer, vertices);
	_meshes["quad"] = std::move(mesh);
#endif
}
