#pragma once

#if _DX
#include <d3d11.h>
//#include <ppltasks.h>
#include <fstream>
#include <vector>
#include <string>

namespace engine
{
	std::string DXGetFullPath(const std::string& filename);
	bool DXLoadCompiledShaderFile(const std::string& filename, std::vector<char>& byteCode);
	bool DXCreateVertexShader(ID3D11Device* device, ID3D11VertexShader** shader, const std::string& filename, std::vector<char>& byteCode);
	bool DXCreatePixelShader(ID3D11Device* device, ID3D11PixelShader** shader, const std::string& filename);
	bool DXCreateGeometryShader(ID3D11Device* device, ID3D11GeometryShader** shader, const std::string& filename);
	
}
#endif