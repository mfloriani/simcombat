#include "pch.h"

#if _DX
#include "VertexBufferDX.h"
#include "VertexDX.h"
#include "RendererDX.h"

using namespace engine;

bool VertexBufferDX::Init(Renderer* renderer, std::vector<Vertex>& vertices)
{
	auto device = static_cast<RendererDX*>(renderer)->GetDevice();
	
	D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 };
	vertexBufferData.pSysMem = vertices.data();
	vertexBufferData.SysMemPitch = 0;
	vertexBufferData.SysMemSlicePitch = 0;
	auto size = static_cast<UINT>(sizeof(Vertex) * vertices.size());
	CD3D11_BUFFER_DESC vertexBufferDesc(size, D3D11_BIND_VERTEX_BUFFER);
	HRESULT hr = device->CreateBuffer(
		&vertexBufferDesc,
		&vertexBufferData,
		&_vertexBuffer
	);
	if (FAILED(hr))
	{
		ENGINE_ERROR("Error creating vertex buffer");
		return false;
	}


	//D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
	//indexBufferData.pSysMem = indices.data();
	//indexBufferData.SysMemPitch = 0;
	//indexBufferData.SysMemSlicePitch = 0;
	//CD3D11_BUFFER_DESC indexBufferDesc(sizeof(Index) * indices.size(), D3D11_BIND_INDEX_BUFFER);
	//hr = device->CreateBuffer(
	//	&indexBufferDesc,
	//	&indexBufferData,
	//	&_indexBuffer
	//);
	//if (FAILED(hr))
	//{
	//	ENGINE_ERROR("Error creating index buffer");
	//	return false;
	//}

	_numVertices = static_cast<int>(vertices.size());
	//_numIndices = indices.size();

    return true;
}

void VertexBufferDX::Draw(Renderer* renderer)
{
	auto context = static_cast<RendererDX*>(renderer)->GetContext();

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	
	context->IASetVertexBuffers(
		0,
		1,
		_vertexBuffer.GetAddressOf(),
		&stride,
		&offset
	);

	// TODO: should not be fixed as pointlist
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	
	context->Draw(_numVertices, 0);
}

void VertexBufferDX::DrawIndexed(Renderer* renderer)
{
	// TODO: have to think about indices 
	ENGINE_WARN("VertexBufferDX::DrawIndexed not implemented");
	//auto context = static_cast<RendererDX*>(renderer)->GetContext();

	//UINT stride = sizeof(Vertex);
	//UINT offset = 0;

	//context->IASetVertexBuffers(
	//	0,
	//	1,
	//	_vertexBuffer.GetAddressOf(),
	//	&stride,
	//	&offset
	//);

	//context->IASetIndexBuffer(
	//	_indexBuffer.Get(),
	//	DXGI_FORMAT_R32_UINT,
	//	0
	//);

	//context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	////context->Draw(_numVertices, 0);
	//context->DrawIndexed(_numIndices, 0, 0);
}
#endif