#include "stdafx.h"
#include "GeometryShader.h"


CGeometryShader::CGeometryShader()
{
	myShader = nullptr;
}

CGeometryShader::~CGeometryShader()
{
	if (myShader != nullptr)
	{
		myShader->Release();
		myShader = nullptr;
	}
}

ID3D11GeometryShader * CGeometryShader::GetShader() const
{
	return myShader;
}

bool CGeometryShader::CreateShader(const char * aPath, ID3D11Device * aDevice, const CShaderCompiler& aShaderCompiler)
{
	myShaderCompileData.filepath =aPath;
	myShaderCompileData.devicePtr = aDevice;
	myShaderCompileData.shaderCompiler = aShaderCompiler;

	if (!myIsBeingFilewatched)
	{
		Corgi::CFileWatcher::GetInstance().WatchFileChangeWithDependencies(Corgi::ConvertCharPtrToWideString(aPath), std::bind(&CGeometryShader::OnShaderFileModified, this, std::placeholders::_1));
		myIsBeingFilewatched = true;
	}

	ID3D10Blob* ShaderBlob = nullptr;
	aShaderCompiler.CompileShader(aPath, "main", "gs_5_0", ShaderBlob);

	ID3D11GeometryShader* geometryShader;
	HRESULT result = aDevice->CreateGeometryShader(ShaderBlob->GetBufferPointer(), ShaderBlob->GetBufferSize(), NULL, &geometryShader);
	//HRESULT result = aDevice->CreateGeometryShader(gsData.data(), gsData.size(), nullptr, &geometryShader);


	if (FAILED(result))
	{
		DL_ERROR(eDebugLogType::Resource, "%s%s%s", "Failed to load geometryShader from path: | ", aPath, " | is this file really a GeometryShader?");
		return false;
	}
	myShader = geometryShader;

	ShaderBlob->Release();
	ShaderBlob = 0;
	return true;
}


void CGeometryShader::OnShaderFileModified(const std::wstring& /*afile*/)
{
	DL_WRITELOG(eConsoleColor::LIGHTBLUE, eDebugLogType::Shader, "%s", "Geometry Shader modified! Trying to reload...");

	// Test compile
	ID3D10Blob* GS = nullptr;
	if (!myShaderCompileData.shaderCompiler.CompileShader(myShaderCompileData.filepath.c_str(), "main", "gs_5_0", GS))
	{
		DL_ERROR(eDebugLogType::Shader, "%s%s", "Shader modified failed! error in file: ", myShaderCompileData.filepath.c_str());
		return;
	}

	GS->Release();

	if (myShader)
	{
		myShader->Release();
		myShader = nullptr;

	}

	CreateShader(myShaderCompileData.filepath.c_str(), myShaderCompileData.devicePtr, myShaderCompileData.shaderCompiler);

	DL_WRITELOG(eConsoleColor::LIGHTBLUE, eDebugLogType::Shader, "%s", "Geometry Shader Reload Complete");
}
