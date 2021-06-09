#include "pch.h"

#if _DX
#include "TextureDX.h"
#include "RendererDX.h"
#include "HelperDX.h"

#pragma comment( lib, "dxguid.lib")
#include "DDSTextureLoader.h"

using namespace engine;
using namespace DirectX;

bool TextureDX::Load(Renderer* renderer, const std::string& filename)
{
	auto path = DXGetFullPath(filename);
	const auto tmp = std::wstring(path.begin(), path.end());
	const wchar_t* const fn = tmp.c_str();

	auto device = static_cast<RendererDX*>(renderer)->GetDevice();

	auto hr = CreateDDSTextureFromFile(device, fn, nullptr, &_textureSRV);
	if (FAILED(hr))
	{
		ENGINE_ERROR("Error loading DX Texture {0}", filename);
		return false;
	}

	return true;
}
#endif