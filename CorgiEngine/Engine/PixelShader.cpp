#include "stdafx.h"
#include "PixelShader.h"




CPixelShader::CPixelShader()
{
	myShader = nullptr;
	myIsBeingFilewatched = false;
}

CPixelShader::~CPixelShader()
{
	if (myShader != nullptr)
	{
		myShader->Release();
		myShader = nullptr;
	}
}

ID3D11PixelShader * CPixelShader::GetShader() const
{
	return myShader;
}

bool CPixelShader::CreateShader(const char * aPath, ID3D11Device * aDevice, const CShaderCompiler& aShaderCompiler)
{
	myShaderCompileData.filepath = aPath;
	myShaderCompileData.devicePtr = aDevice;
	myShaderCompileData.shaderCompiler = aShaderCompiler;

	if (!myIsBeingFilewatched)
	{
		Corgi::CFileWatcher::GetInstance().WatchFileChangeWithDependencies(Corgi::ConvertCharPtrToWideString(aPath), std::bind(&CPixelShader::OnShaderFileModified, this, std::placeholders::_1));
		myIsBeingFilewatched = true;
	}

	ID3D11PixelShader* pixelShader;

	ID3D10Blob* ShaderBlob = nullptr;
	
	if (!aShaderCompiler.CompileShader(aPath, "main", "ps_5_0", ShaderBlob))
	{
		DL_ERROR(eDebugLogType::Shader, "%s%s", "Something went wrong compiling: ", aPath);
	}
	HRESULT result = aDevice->CreatePixelShader(ShaderBlob->GetBufferPointer(), ShaderBlob->GetBufferSize(), NULL, &pixelShader);

	//result = aDevice->CreatePixelShader(psData.data(), psData.size(), nullptr, &pixelShader);

	if (FAILED(result))
	{
		DL_ERROR(eDebugLogType::Resource, "%s%s%s", "Failed to load pixelShader from path: | ", aPath, " | is this file really a PixelShader?");
		return false;
	}
	myShader = pixelShader;

	ShaderBlob->Release();
	ShaderBlob = 0;

	return true;
}



void CPixelShader::OnShaderFileModified(const std::wstring& /*afile*/)
{
	DL_WRITELOG(eConsoleColor::LIGHTBLUE, eDebugLogType::Shader, "%s", "Pixel Shader modified! Trying to reload...");

	// Test compile
	ID3D10Blob* PS = nullptr;
	if (!myShaderCompileData.shaderCompiler.CompileShader(myShaderCompileData.filepath.c_str(), "main", "ps_5_0", PS))
	{
		DL_ERROR(eDebugLogType::Shader, "%s%s", "Shader modified failed! error in file: ", myShaderCompileData.filepath.c_str());
		return;
	}

	PS->Release();

	if (myShader)
	{
		myShader->Release();
		myShader = nullptr;

	}

	CreateShader(myShaderCompileData.filepath.c_str(), myShaderCompileData.devicePtr, myShaderCompileData.shaderCompiler);
	DL_WRITELOG(eConsoleColor::LIGHTBLUE, eDebugLogType::Shader, "%s", "Pixel Shader Reload Complete");
}
