#pragma once

#if _DX
#include <wrl.h>
#include <d3d11.h>

#include "Platform\Texture.h"

namespace engine
{
	class Renderer;

	class TextureDX : public Texture
	{
	public:
		TextureDX() = default;
		virtual ~TextureDX() = default;

		virtual bool Load(Renderer* renderer, const std::string& filename) override;

		ID3D11ShaderResourceView* GetShaderResourceView() { return _textureSRV.Get(); }

	private:
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _textureSRV;
	};
}
#endif