#include "stdafx.h"
#include "VertexShader.h"

CVertexShader::CVertexShader()
{
	myInputLayout = nullptr;
	myShader = nullptr;
	myIsBeingFilewatched = false;
}

CVertexShader::~CVertexShader()
{
	if (myShader != nullptr)
	{
		myShader->Release();
		myShader = nullptr;
	}
}

ID3D11VertexShader * CVertexShader::GetShader() const
{
	return myShader;
}

bool CVertexShader::CreateShader(const char* aPath, ID3D11Device* aDevice, D3D11_INPUT_ELEMENT_DESC* aInputLayout, int aLayoutSize, const CShaderCompiler& aShaderCompiler)
{
	if (!myIsBeingFilewatched)
	{
		myShaderCompileData.filePath = aPath;
		myShaderCompileData.devicePtr = aDevice;

		if (myShaderCompileData.inputLayoutDesc)
		{
			delete myShaderCompileData.inputLayoutDesc;
			myShaderCompileData.inputLayoutDesc = nullptr;
		}
		myShaderCompileData.inputLayoutDesc = new D3D11_INPUT_ELEMENT_DESC[aLayoutSize];
		for (int i = 0; i < aLayoutSize; ++i)
		{
			myShaderCompileData.inputLayoutDesc[i] = aInputLayout[i];
		}

		myShaderCompileData.layoutSize = aLayoutSize;
		myShaderCompileData.shaderCompiler = aShaderCompiler;

		Corgi::CFileWatcher::GetInstance().WatchFileChangeWithDependencies(Corgi::ConvertCharPtrToWideString(aPath), std::bind(&CVertexShader::OnShaderFileModified, this, std::placeholders::_1));
		myIsBeingFilewatched = true;
	}

	HRESULT result;

	ID3D10Blob* ShaderBlob = nullptr;
	if (!aShaderCompiler.CompileShader(aPath, "main", "vs_5_0", ShaderBlob))
	{
		DL_ERROR(eDebugLogType::Shader, "%s%s", "Something went wrong compiling: ", aPath);
	}

	ID3D11VertexShader* vertexShader = nullptr;
	result = aDevice->CreateVertexShader(ShaderBlob->GetBufferPointer(), ShaderBlob->GetBufferSize(), nullptr, &vertexShader);
	//result = aDevice->CreateVertexShader(vsData.data(), vsData.size(), nullptr, &vertexShader);
	if (FAILED(result))
	{
		DL_ERROR(eDebugLogType::Resource, "%s%s%s", "failed to load vertexShader from path: | ", aPath, " | is this file really a VertexShader?");
		return false;
	}

	ID3D11InputLayout* inputLayout = nullptr;
	//result = aDevice->CreateInputLayout(aInputLayout, aLayoutSize, vsData.data(), vsData.size(), &inputLayout);
	void* bufferptr = ShaderBlob->GetBufferPointer();
	result = aDevice->CreateInputLayout(aInputLayout, aLayoutSize, bufferptr, ShaderBlob->GetBufferSize(), &inputLayout);
	//End creation of Layout
	if (FAILED(result))
	{
		DL_ERROR(eDebugLogType::Resource, "%s%s%s", "InputLayout failed to load! vShader was: | ", aPath, " | Please Check your input in this vertex shader!");
		return false;
	}

	if (myInputLayout)
	{
		myInputLayout->Release();
		myInputLayout = nullptr;
	}
	myInputLayout = inputLayout;
	myShader = vertexShader;

	ShaderBlob->Release();
	ShaderBlob = 0;

	return true;
}

void CVertexShader::OnShaderFileModified(const std::wstring& /*afile*/)
{
	DL_WRITELOG(eConsoleColor::LIGHTBLUE, eDebugLogType::Shader, "%s", "Vertex Shader modified! Trying to reload...");

	// Test compile
	ID3D10Blob* VS = nullptr;
	if (!myShaderCompileData.shaderCompiler.CompileShader(myShaderCompileData.filePath.c_str(), "main", "vs_5_0", VS))
	{
		DL_ERROR(eDebugLogType::Shader, "%s%s", "Shader modified failed! error in file: ", myShaderCompileData.filePath.c_str());
		return;
	}

	VS->Release();

	if (myShader)
	{
		myShader->Release();
		myShader = nullptr;

	}

	CreateShader(myShaderCompileData.filePath.c_str(), myShaderCompileData.devicePtr, myShaderCompileData.inputLayoutDesc, myShaderCompileData.layoutSize, myShaderCompileData.shaderCompiler);

	DL_WRITELOG(eConsoleColor::LIGHTBLUE, eDebugLogType::Shader, "%s", "Vertex Shader Reload Complete");
}

ID3D11InputLayout * CVertexShader::GetInputLayout() const
{
	return myInputLayout;
}
