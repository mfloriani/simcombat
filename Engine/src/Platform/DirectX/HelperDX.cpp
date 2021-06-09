#include "pch.h"
#if _DX
#include "HelperDX.h"


std::string engine::DXGetFullPath(const std::string& filename)
{
	char tmpPath[MAX_PATH];
	GetModuleFileNameA(NULL, tmpPath, MAX_PATH);
	std::string path(tmpPath);
	path = path.substr(0, 1 + path.find_last_of('\\'));
	path += filename;
	return path;
}

bool engine::DXLoadCompiledShaderFile(const std::string& filename, std::vector<char>& byteCode)
{
	byteCode.clear();

	char tmpPath[MAX_PATH];
	GetModuleFileNameA(NULL, tmpPath, MAX_PATH);
	std::string path(tmpPath);
	path = path.substr(0, 1 + path.find_last_of('\\'));

	path += filename;
	ENGINE_TRACE("Loading [{0}]", path);

	std::ifstream fin(path, std::ios::binary);
	if (!fin)
	{
		//ENGINE_TRACE("{0}", path);
		//ENGINE_TRACE("{0}", __FILE__);
		char errmsg[256];
		strerror_s(errmsg, 256, errno);
		ENGINE_ERROR("Error loading compiled shader file {0}: {1}", filename, errmsg);
		return false;
	}

	fin.seekg(0, fin.end);
	size_t len = fin.tellg();
	fin.seekg(0, fin.beg);

	byteCode.resize(len);
	fin.read(byteCode.data(), len);

	//std::vector<unsigned char> byteCode(std::istreambuf_iterator<char>(fin), {});
	
	fin.close();

	return true;

}

bool engine::DXCreateVertexShader(ID3D11Device* device, ID3D11VertexShader** shader, const std::string& filename, std::vector<char>& byteCode)
{
	if (!DXLoadCompiledShaderFile(filename, byteCode))
	{
		return false;
	}
	
	HRESULT hr = device->CreateVertexShader(
		&byteCode[0],
		byteCode.size(),
		nullptr,
		shader
	);

	if (FAILED(hr))
	{
		ENGINE_ERROR("Error creating vertex shader!");
		return false;
	}

	return true;
}

bool engine::DXCreatePixelShader(ID3D11Device* device, ID3D11PixelShader** shader, const std::string& filename)
{
	std::vector<char> byteCode;
	if (!DXLoadCompiledShaderFile(filename, byteCode))
		return false;

	HRESULT hr = device->CreatePixelShader(
		&byteCode[0],
		byteCode.size(),
		nullptr,
		shader
	);

	if (FAILED(hr))
	{
		ENGINE_ERROR("Error creating pixel shader!");
		return false;
	}

	return true;
}

bool engine::DXCreateGeometryShader(ID3D11Device* device, ID3D11GeometryShader** shader, const std::string& filename)
{
	std::vector<char> byteCode;
	if (!DXLoadCompiledShaderFile(filename, byteCode))
		return false;

	HRESULT hr = device->CreateGeometryShader(
		&byteCode[0],
		byteCode.size(),
		nullptr,
		shader
	);

	if (FAILED(hr))
	{
		ENGINE_ERROR("Error creating geometry shader!");
		return false;
	}

	return true;
}

#endif