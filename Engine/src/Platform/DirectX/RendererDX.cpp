#include "pch.h"

#if _DX

#include "RendererDX.h"
#include "HelperDX.h"
#include "Constants.h"
#include "Platform\Mesh.h"
#include "TextureDX.h"

#include "ECS\Components\TransformComponent.h"
#include "ECS\Components\RenderComponent.h"

using namespace engine;
using namespace DirectX;

RendererDX::RendererDX(HWND hWnd) : _hWnd(hWnd)
{
}

RendererDX::~RendererDX()
{
}

bool RendererDX::Init()
{
	HRESULT hr = S_OK;

	if (!CreateDeviceContextResources())
		return false;


	// TODO: should rethink shader design to avoid this coupling with the Renderer
	//       what happen when I want a different shader?!
	std::vector<char> vsByteCode;
	if (!DXCreateVertexShader(_device.Get(), &_vertexShader, "QuadVS.cso", vsByteCode))
		return false;

	if (!DXCreatePixelShader(_device.Get(), &_pixelShader, "QuadPS.cso"))
		return false;

	if (!DXCreateGeometryShader(_device.Get(), &_geometryShader, "QuadGS.cso"))
		return false;

	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		//{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	hr = _device->CreateInputLayout(ied, ARRAYSIZE(ied), &vsByteCode[0], vsByteCode.size(), &_inputLayout);
	if (FAILED(hr))
	{
		ENGINE_ERROR("Error creating input layout");
		return false;
	}
	_context->IASetInputLayout(_inputLayout.Get());

	// Constant Buffer
	CD3D11_BUFFER_DESC constantBufferDesc(sizeof(ModelViewProjCB), D3D11_BIND_CONSTANT_BUFFER);
	hr = _device->CreateBuffer(
		&constantBufferDesc,
		nullptr,
		&_mvpBuffer
	);
	if (FAILED(hr))
	{
		ENGINE_ERROR("Error creating constant buffer");
		return false;
	}


	D3D11_SAMPLER_DESC SamplerDesc = {};
	SamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	SamplerDesc.AddressV = SamplerDesc.AddressU;
	SamplerDesc.AddressW = SamplerDesc.AddressU;
	SamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	hr = _device->CreateSamplerState(&SamplerDesc, &_samplerState);
	if (FAILED(hr))
	{
		ENGINE_ERROR("Error creating sampler state");
		return false;
	}

	return true;
}

bool RendererDX::CreateDeviceContextResources()
{
	HRESULT hr = S_OK;

	// This flag adds support for surfaces with a different color channel ordering
	// than the API default. It is required for compatibility with Direct2D.
	UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#if defined(_DEBUG)
	// If the project is in a debug build, enable debugging via SDK Layers with this flag.
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	// This array defines the set of DirectX hardware feature levels this app will support.
	// Note the ordering should be preserved.
	// Don't forget to declare your application's minimum required feature level in its
	// description.  All applications are assumed to support 9.1 unless otherwise stated.
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1
	};

	hr = D3D11CreateDevice(
		nullptr,					// Specify nullptr to use the default adapter.
		D3D_DRIVER_TYPE_HARDWARE,	// Create a device using the hardware graphics driver.
		0,							// Should be 0 unless the driver is D3D_DRIVER_TYPE_SOFTWARE.
		creationFlags,				// Set debug and Direct2D compatibility flags.
		featureLevels,				// List of feature levels this app can support.
		ARRAYSIZE(featureLevels),	// Size of the list above.
		D3D11_SDK_VERSION,			// Always set this to D3D11_SDK_VERSION for Microsoft Store apps.
		&_device,					// Returns the Direct3D device created.
		nullptr,			        // Returns feature level of device created.
		&_context					// Returns the device immediate context.
	);

	if (FAILED(hr))
	{
		ENGINE_ERROR("Error creating D3D11CreateDevice!");
		return false;
	}
		
	CreateWindowSizeResources();

	return true;
}

bool RendererDX::CreateWindowSizeResources()
{
	HRESULT hr = S_OK;

	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(DXGI_SWAP_CHAIN_DESC));
	desc.Windowed = TRUE; // Sets the initial state of full-screen mode.
	desc.BufferCount = 2;
	desc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.SampleDesc.Count = 1;      //multisampling setting
	desc.SampleDesc.Quality = 0;    //vendor-specific flag
	desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
	desc.OutputWindow = _hWnd;

	// Create the DXGI device object to use in other factories, such as Direct2D.
	Microsoft::WRL::ComPtr<IDXGIDevice> dxgiDevice;
	_device.As(&dxgiDevice);

	// Create swap chain.
	Microsoft::WRL::ComPtr<IDXGIAdapter> adapter;
	Microsoft::WRL::ComPtr<IDXGIFactory> factory;

	hr = dxgiDevice->GetAdapter(&adapter);

	if (SUCCEEDED(hr))
	{
		adapter->GetParent(IID_PPV_ARGS(&factory));

		hr = factory->CreateSwapChain(
			_device.Get(),
			&desc,
			&_swapChain
		);
	}

	if (!CreateBackBuffer())
	{
		ENGINE_ERROR("Error CreateBackBuffer()!");
		return false;
	}

	SetCamera();

	return true;
}

bool RendererDX::CreateBackBuffer()
{
	HRESULT hr = S_OK;

	hr = _swapChain->GetBuffer(
		0,
		__uuidof(ID3D11Texture2D),
		(void**)&_backBuffer
	);

	if (FAILED(hr))
	{
		ENGINE_ERROR("Error _swapChain->GetBuffer()!");
		return false;
	}

	hr = _device->CreateRenderTargetView(
		_backBuffer.Get(),
		nullptr,
		_renderTarget.GetAddressOf()
	);

	if (FAILED(hr))
	{
		ENGINE_ERROR("Error _device->CreateRenderTargetView()!");
		return false;
	}

	_backBuffer->GetDesc(&_bbDesc);

	// Create a depth-stencil view for use with 3D rendering if needed.
	CD3D11_TEXTURE2D_DESC depthStencilDesc(
		DXGI_FORMAT_D24_UNORM_S8_UINT,
		static_cast<UINT> (_bbDesc.Width),
		static_cast<UINT> (_bbDesc.Height),
		1, // This depth stencil view has only one texture.
		1, // Use a single mipmap level.
		D3D11_BIND_DEPTH_STENCIL
	);

	hr = _device->CreateTexture2D(
		&depthStencilDesc,
		nullptr,
		&_depthStencil
	);

	if (FAILED(hr))
	{
		ENGINE_ERROR("Error _device->CreateTexture2D()!");
		return false;
	}

	CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);

	hr = _device->CreateDepthStencilView(
		_depthStencil.Get(),
		&depthStencilViewDesc,
		&_depthStencilView
	);

	if (FAILED(hr))
	{
		ENGINE_ERROR("Error _device->CreateDepthStencilView()!");
		return false;
	}

	ZeroMemory(&_viewport, sizeof(D3D11_VIEWPORT));
	_viewport.Height = (float)_bbDesc.Height;
	_viewport.Width = (float)_bbDesc.Width;
	_viewport.MinDepth = 0;
	_viewport.MaxDepth = 1;

	_context->RSSetViewports(
		1,
		&_viewport
	);

	return true;
}

void RendererDX::ReleaseBackBuffer()
{
	// Release the render target view based on the back buffer:
	_renderTarget.Reset();

	// Release the back buffer itself:
	_backBuffer.Reset();

	// The depth stencil will need to be resized, so release it (and view):
	_depthStencilView.Reset();
	_depthStencil.Reset();

	// After releasing references to these resources, we need to call Flush() to 
	// ensure that Direct3D also releases any references it might still have to
	// the same resources - such as pipeline bindings.
	_context->Flush();
}

bool RendererDX::SetFullScreen()
{
	HRESULT hr = S_OK;

	hr = _swapChain->SetFullscreenState(TRUE, NULL);
	if (FAILED(hr))
	{
		ENGINE_ERROR("Error SetFullScreen _swapChain->SetFullscreenState()!");
		return false;
	}

	// Swap chains created with the DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL flag need to
	// call ResizeBuffers in order to realize a full-screen mode switch. Otherwise, 
	// your next call to Present will fail.

	// Before calling ResizeBuffers, you have to release all references to the back 
	// buffer device resource.
	ReleaseBackBuffer();

	// Now we can call ResizeBuffers.
	hr = _swapChain->ResizeBuffers(
		0,                   // Number of buffers. Set this to 0 to preserve the existing setting.
		0, 0,                // Width and height of the swap chain. Set to 0 to match the screen resolution.
		DXGI_FORMAT_UNKNOWN, // This tells DXGI to retain the current back buffer format.
		0
	);
	if (FAILED(hr))
	{
		ENGINE_ERROR("Error _swapChain->ResizeBuffers()!");
		return false;
	}

	// Then we can recreate the back buffer, depth buffer, and so on.
	if (!CreateBackBuffer())
	{
		ENGINE_ERROR("Error CreateBackBuffer()!");
		return false;
	}

	return true;
}

bool RendererDX::SetWindowed()
{
	HRESULT hr = S_OK;

	hr = _swapChain->SetFullscreenState(FALSE, NULL);
	if (FAILED(hr))
	{
		ENGINE_ERROR("Error SetWindowed _swapChain->SetFullscreenState()!");
		return false;
	}

	// Swap chains created with the DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL flag need to
	// call ResizeBuffers in order to realize a change to windowed mode. Otherwise, 
	// your next call to Present will fail.

	// Before calling ResizeBuffers, you have to release all references to the back 
	// buffer device resource.
	ReleaseBackBuffer();

	// Now we can call ResizeBuffers.
	hr = _swapChain->ResizeBuffers(
		0,                   // Number of buffers. Set this to 0 to preserve the existing setting.
		0, 0,                // Width and height of the swap chain. MUST be set to a non-zero value. For example, match the window size.
		DXGI_FORMAT_UNKNOWN, // This tells DXGI to retain the current back buffer format.
		0
	);
	if (FAILED(hr))
	{
		ENGINE_ERROR("Error SetWindowed _swapChain->ResizeBuffers()!");
		return false;
	}

	// Then we can recreate the back buffer, depth buffer, and so on.
	if (!CreateBackBuffer())
	{
		ENGINE_ERROR("Error SetWindowed CreateBackBuffer()!");
		return false;
	}

	return true;
}

void RendererDX::SetCamera()
{
	float aspectRatio = _viewport.Width / _viewport.Height;
	float fovAngleY = 70.0f * XM_PI / 180.0f;

	ENGINE_TRACE("New aspect ratio: {0} [w:{1},h:{2}]: ", aspectRatio, _viewport.Width, _viewport.Height);

	if (aspectRatio < 1.0f)
		fovAngleY *= 2.0f;

#if 1
	// This sample makes use of a right-handed coordinate system using row-major matrices.
	XMMATRIX perspectiveMatrix = XMMatrixPerspectiveFovRH(
		fovAngleY,
		aspectRatio,
		0.01f,
		100.0f
	);
	XMStoreFloat4x4(
		&_mvpBufferData.projection,
		XMMatrixTranspose(perspectiveMatrix)
	);
#else
	//XMMATRIX orthographicMatrix = XMMatrixOrthographicRH(10, 10, 0.1f, 100.f);
	XMMATRIX orthographicMatrix = XMMatrixOrthographicOffCenterRH(-aspectRatio, aspectRatio, -1, 1, -1.0f, 1.0f);
	XMStoreFloat4x4(
		&_mvpBufferData.projection,
		XMMatrixTranspose(orthographicMatrix)
	);
#endif

	static const XMVECTORF32 eye = { 0.0f, 0.0f, 3.0f, 0.0f };
	static const XMVECTORF32 at = { 0.0f, 0.0f, 1.0f, 0.0f };
	static const XMVECTORF32 up = { 0.0f, 1.0f, 0.0f, 0.0f };

	auto view = XMMatrixTranspose(XMMatrixLookAtRH(eye, at, up));
	XMStoreFloat4x4(&_mvpBufferData.view, view);
	XMStoreFloat4x4(&_mvpBufferData.inverseView, XMMatrixTranspose(XMMatrixInverse(nullptr, view)));
}

void RendererDX::OnResize()
{
	HRESULT hr = S_OK;

	ReleaseBackBuffer();

	// Now we can call ResizeBuffers.
	hr = _swapChain->ResizeBuffers(
		0,                   // Number of buffers. Set this to 0 to preserve the existing setting.
		0, 0,                // Width and height of the swap chain. MUST be set to a non-zero value. For example, match the window size.
		DXGI_FORMAT_UNKNOWN, // This tells DXGI to retain the current back buffer format.
		0
	);
	if (FAILED(hr))
	{
		ENGINE_ERROR("Error RendererDX::OnResize() _swapChain->ResizeBuffers()!");
		return;
	}

	// Then we can recreate the back buffer, depth buffer, and so on.
	if (!CreateBackBuffer())
	{
		ENGINE_ERROR("Error RendererDX::OnResize() CreateBackBuffer()!");
		return;
	}

	SetCamera();
}

void RendererDX::ClearScreen()
{
	ID3D11RenderTargetView* const targets[1] = { _renderTarget.Get() };
	_context->OMSetRenderTargets(1, targets, _depthStencilView.Get());
	_context->ClearRenderTargetView(_renderTarget.Get(), Colors::Black);
	_context->ClearDepthStencilView(_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}



void RendererDX::Render(RenderComponent* rc, TransformComponent* tc)
{
	auto model = XMMatrixIdentity();

	//scale
	model = XMMatrixMultiply(model, DirectX::XMMatrixScaling(tc->scale.x, tc->scale.y, tc->scale.z));

	//rotation
	model = XMMatrixMultiply(
		model,
		DirectX::XMMatrixRotationQuaternion(
			DirectX::XMQuaternionRotationRollPitchYaw(
				tc->rotation.x, tc->rotation.y, tc->rotation.z)
		)
	);

	//translation
	model = XMMatrixMultiply(model, DirectX::XMMatrixTranslation(tc->position.x, tc->position.y, tc->position.z));

	XMStoreFloat4x4(&_mvpBufferData.model, XMMatrixTranspose(model));

	_context->UpdateSubresource(
		_mvpBuffer.Get(),
		0,
		NULL,
		&_mvpBufferData,
		0,
		0
	);

	_context->VSSetShader(_vertexShader.Get(), 0, 0);
	_context->GSSetShader(_geometryShader.Get(), 0, 0);
	_context->PSSetShader(_pixelShader.Get(), 0, 0);

	_context->VSSetConstantBuffers(
		0,
		1,
		_mvpBuffer.GetAddressOf()
	);

	_context->GSSetConstantBuffers(
		0,
		1,
		_mvpBuffer.GetAddressOf()
	);

	if (rc->texture)
	{
		auto srv = static_cast<TextureDX*>(rc->texture)->GetShaderResourceView();
		_context->PSSetShaderResources(0, 1, &srv);
		
		auto sampler = _samplerState.Get();
		_context->PSSetSamplers(0, 1, &sampler);
	}

	rc->mesh->GetVertexBuffer()->Draw(this);
}

void RendererDX::SwapBuffers()
{
	_swapChain->Present(1, 0);
	//_swapChain->Present(0, 0);
}

#endif