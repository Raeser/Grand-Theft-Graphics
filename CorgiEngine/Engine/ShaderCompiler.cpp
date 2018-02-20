#include "stdafx.h"
#include "..\Utilities\FileWatcher\FileWatcher.h"

//#define USE_LIGHTS
//#define USE_NOISE


CShaderCompiler::CShaderCompiler()
{
}


CShaderCompiler::~CShaderCompiler()
{
}


bool CShaderCompiler::FileExist(const char * filename) const
{
	std::ifstream infile(filename);
	bool isGood = infile.good();
	infile.close();
	return isGood;
}

bool CShaderCompiler::CompileShader(const char * aShader, const char * aMainFunction, const char * aTarget, ID3D10Blob *& aCodeBlob) const
{
	if (FileExist(aShader) == false)
	{
		DL_ERROR(eDebugLogType::Shader, "%s%s", "Shader not found! ", aShader);
		return false;
	}
	HRESULT result;
	ID3D10Blob *errorMessage = nullptr;

	UINT flags = 0;
#if defined( DEBUG ) || defined( _DEBUG )
	flags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION | D3DCOMPILE_PREFER_FLOW_CONTROL;
#endif

#if defined(USE_LIGHTS) && defined(USE_NOISE)
	const D3D_SHADER_MACRO dx11Defines[] = {
		{ "USE_NOISE", "true" },
		{ "USE_LIGHTS", "true" },
		{ nullptr, nullptr },
	};
	result = D3DCompileFromFile(Corgi::ConvertCharPtrToWideString(aShader).c_str(), dx11Defines, D3D_COMPILE_STANDARD_FILE_INCLUDE, aMainFunction, aTarget, flags, 0, &aCodeBlob, &errorMessage);
#elif defined(USE_LIGHTS)
	const D3D_SHADER_MACRO dx11Defines[] = {
		{ "USE_LIGHTS", "true" },
		{ nullptr, nullptr },
	};
	result = D3DCompileFromFile(Corgi::ConvertCharPtrToWideString(aShader).c_str(), dx11Defines, D3D_COMPILE_STANDARD_FILE_INCLUDE, aMainFunction, aTarget, flags, 0, &aCodeBlob, &errorMessage);
#elif defined(USE_NOISE)
	const D3D_SHADER_MACRO dx11Defines[] = {
		{ "USE_NOISE", "true" },
		{ nullptr, nullptr },
	};
	result = D3DCompileFromFile(Corgi::ConvertCharPtrToWideString(aShader).c_str(), dx11Defines, D3D_COMPILE_STANDARD_FILE_INCLUDE, aMainFunction, aTarget, flags, 0, &aCodeBlob, &errorMessage);
#else
	result = D3DCompileFromFile(Corgi::ConvertCharPtrToWideString(aShader).c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, aMainFunction, aTarget, flags, 0, &aCodeBlob, &errorMessage);

#endif

	if (FAILED(result))
	{
		DL_ERROR(eDebugLogType::Shader, "%s %s", "Failed to compile shader: ", aShader);
	}
	LPVOID voidError = NULL;
	if (errorMessage)
	{
		voidError = errorMessage->GetBufferPointer();
		const char* errorMessageChar = static_cast<const char*>(voidError);
		DL_ERROR(eDebugLogType::Shader, "%s %s", "Shader compilation error: ", errorMessageChar);
		errorMessageChar;

		return false;
	}
	return true;
}