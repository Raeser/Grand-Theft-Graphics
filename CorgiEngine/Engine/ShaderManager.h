#pragma once
#include <unordered_map>
#include "..\Utilities\MemoryPool.h"
#include "ShaderCompiler.h"

class CGeometryShader;
class CVertexShader;
class CPixelShader;

struct D3D11_INPUT_ELEMENT_DESC;

enum eShaderType
{
	eShaderType_UnInitialized = -1,
	eShaderType_Vertex = 'V',
	eShaderType_Pixel = 'P',
	eShaderType_Geometry = 'G',
	eShaderType_Error
};
class CShaderManager
{
public:
	CShaderManager();
	~CShaderManager();
	bool Init(ID3D11Device* aDevice, ID3D11DeviceContext* aContext);
	long long LoadShader(const char* aPath, D3D11_INPUT_ELEMENT_DESC* aInputLayout, int aLayoutSize);
	long long GetShader(const char* aPath);
	void ReloadShader(const char* aPath);
	const CVertexShader& GetVertexShader(long long aID) const;
	const CPixelShader& GetPixelShader(long long aID) const;
	const CGeometryShader& GetGeometryShader(long long aID) const;

	void SetAsActiveShader(eShaderType aType, long long aID);

	const CShaderCompiler& GetShaderCompiler() const;

private:
	
	long long InternalShaderLoad(const char* aPath, eShaderType aType, D3D11_INPUT_ELEMENT_DESC* aInputLayout, int aLayoutSize);
	Corgi::CMemoryPool<CVertexShader> myVertexShaders;
	Corgi::CMemoryPool<CPixelShader> myPixelShaders;
	Corgi::CMemoryPool<CGeometryShader> myGeometryShaders;

	CShaderCompiler myShaderCompiler;

	std::unordered_map<std::string, long long> myShaderMap;
	ID3D11Device* myDevice;
	ID3D11DeviceContext* myContext;
	long long myDefaultPixelShader;
	long long myDefaultVertexShader;
	long long myDefaultGeometryShader;
	long long myDefaultAnimatedPixelShader;
	long long myDefaultAnimatedVertexShader;


};

