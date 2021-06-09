#pragma once

#if _DX

#include "Platform\Renderer.h"

#include <wrl.h>
#include <wrl/client.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <directxcolors.h>
#include "VertexDX.h"

namespace engine
{
	struct RenderComponent;
	struct TransformComponent;

	class RendererDX : public Renderer
	{
	public:
		RendererDX(HWND hWnd);
		virtual ~RendererDX();

	public:
		virtual bool Init() override;
		virtual void ClearScreen() override;
		virtual void SwapBuffers() override;
		virtual void Render(RenderComponent* rc, TransformComponent* tc) override;

		bool CreateDeviceContextResources();
		bool CreateWindowSizeResources();
		bool CreateBackBuffer();
		void ReleaseBackBuffer();
		bool SetFullScreen();
		bool SetWindowed();

		void SetCamera();
		void OnResize();

	public:
		ID3D11Device* GetDevice() const { return _device.Get(); }
		ID3D11DeviceContext* GetContext() const { return _context.Get(); }

	private:
		Microsoft::WRL::ComPtr<ID3D11Device>            _device;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>     _context;
		Microsoft::WRL::ComPtr<IDXGISwapChain>          _swapChain;
		Microsoft::WRL::ComPtr<ID3D11Texture2D>         _backBuffer;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView>  _renderTarget;
		Microsoft::WRL::ComPtr<ID3D11Texture2D>         _depthStencil;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView>  _depthStencilView;
		Microsoft::WRL::ComPtr<ID3D11SamplerState>      _samplerState;

		// TODO: redesign this?!
		Microsoft::WRL::ComPtr<ID3D11InputLayout>	    _inputLayout;
		Microsoft::WRL::ComPtr<ID3D11VertexShader>	    _vertexShader;
		Microsoft::WRL::ComPtr<ID3D11GeometryShader>    _geometryShader;
		Microsoft::WRL::ComPtr<ID3D11PixelShader>	    _pixelShader;
		Microsoft::WRL::ComPtr<ID3D11Buffer>		    _mvpBuffer;

		ModelViewProjCB      _mvpBufferData{};
		D3D11_VIEWPORT       _viewport{};
		D3D11_TEXTURE2D_DESC _bbDesc{};
		HWND                 _hWnd;


	};

}

#endif