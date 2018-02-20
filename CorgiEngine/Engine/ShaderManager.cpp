#include "stdafx.h"
#include "ShaderManager.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "GeometryShader.h"

#define MaxAmountOfShaders 64

CShaderManager::CShaderManager()
{
}

CShaderManager::~CShaderManager()
{
}

bool CShaderManager::Init(ID3D11Device* aDevice, ID3D11DeviceContext* aContext)
{
	myVertexShaders.Init(MaxAmountOfShaders);
	myPixelShaders.Init(MaxAmountOfShaders);
	myGeometryShaders.Init(MaxAmountOfShaders);

	myContext = aContext;
	myDevice = aDevice;
	myShaderMap.emplace("Default", -1);
	myGeometryShaders[myGeometryShaders.GetFreeItem()]; //sets shader no. 0 to nullptr wich is 100% acceptable
	myDefaultGeometryShader = 0;

	D3D11_INPUT_ELEMENT_DESC DefaultVertexShaderLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMALS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BITANGENTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "UV", 0 , DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BONES", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "WEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	myDefaultVertexShader = InternalShaderLoad("../Assets/Shaders/VS_DEFModelDefault.hlsl", eShaderType_Vertex, DefaultVertexShaderLayout, 7);
	myDefaultPixelShader = InternalShaderLoad("../Assets/Shaders/PS_DEFModelDefault.hlsl", eShaderType_Pixel, nullptr, 0);
	myDefaultAnimatedVertexShader = InternalShaderLoad("../Assets/Shaders/VS_DEFModelAnimatedDefault.hlsl", eShaderType_Vertex, DefaultVertexShaderLayout, 7);
	myDefaultAnimatedPixelShader = InternalShaderLoad("../Assets/Shaders/PS_DEFModelAnimatedDefault.hlsl", eShaderType_Pixel, nullptr, 0);
	if (!(myDefaultPixelShader == 0 && myDefaultVertexShader == 0))
	{
		return false;
	}
	return true;
}

long long CShaderManager::LoadShader(const char* aPath, D3D11_INPUT_ELEMENT_DESC* aInputLayout, int aLayoutSize)
{
	std::unordered_map<std::string, long long>::const_iterator iter = myShaderMap.find(aPath);
	if (iter == myShaderMap.end())
	{
		std::string pathToEvaluate(aPath);
		size_t lastSlash = pathToEvaluate.find_last_of('/');
		eShaderType aType = static_cast<eShaderType>(pathToEvaluate[lastSlash + 1]);
		switch (aType)
		{
		case eShaderType_Vertex:
			if (aInputLayout != nullptr)
			{
				InternalShaderLoad(aPath, eShaderType_Vertex, aInputLayout, aLayoutSize);
			}
			else
			{
				DL_ERROR(eDebugLogType::Error, "%s", "Error Creating a Vertex shader, you tried to create a vertex shader with no inputdescription attached, do you want to freeze your computer?")
			}
			break;
		case eShaderType_Pixel:
			InternalShaderLoad(aPath, eShaderType_Pixel, nullptr, 0);
			break;
		case eShaderType_Geometry:
			InternalShaderLoad(aPath, eShaderType_Geometry, nullptr, 0);
			break;
		default:
			DL_WARNING(eDebugLogType::Resource, "%s%s%s", "DID NOT LOAD SHADER!Shader does not follow naming convention | ", aPath, " | first character in the shader name is not one of the supported types!");
			aPath = "Default";
			break;
		}
		return myShaderMap[aPath];
	}
	else
	{
		DL_WARNING(eDebugLogType::Resource, "%s%s%s", "Shader Already loaded from path | ", aPath, " | this is totally fine, but it's recommended to use Get[type]Shader instead!");
		return iter->second;
	}
}

long long CShaderManager::GetShader(const char * aPath)
{
	return myShaderMap[aPath];
}

void CShaderManager::ReloadShader(const char * aPath)
{
	aPath;
}

const CVertexShader & CShaderManager::GetVertexShader(long long aID) const
{
	return myVertexShaders[aID];
}

const CPixelShader & CShaderManager::GetPixelShader(long long aID) const
{
	return myPixelShaders[aID];
}

const CGeometryShader & CShaderManager::GetGeometryShader(long long aID) const
{
	return myGeometryShaders[aID];
}

void CShaderManager::SetAsActiveShader(eShaderType aType, long long aID)
{
	switch (aType)
	{
	case eShaderType_Vertex:
		myContext->VSSetShader(myVertexShaders[aID].GetShader(), nullptr, 0);
		myContext->IASetInputLayout(myVertexShaders[aID].GetInputLayout());
		break;
	case eShaderType_Pixel:
		myContext->PSSetShader(myPixelShaders[aID].GetShader(), nullptr, 0);
		break;
	case eShaderType_Geometry:
		myContext->GSSetShader(myGeometryShaders[aID].GetShader(), nullptr, 0);
		break;
	}
}

const CShaderCompiler & CShaderManager::GetShaderCompiler() const
{
	return myShaderCompiler;
}

long long CShaderManager::InternalShaderLoad(const char * aPath, eShaderType aType, D3D11_INPUT_ELEMENT_DESC* aInputLayout, int aLayoutSize)
{
	long long shaderID = -1;

	switch (aType)
	{
	case eShaderType_Vertex:

		shaderID = myVertexShaders.GetFreeItem();
		if (myVertexShaders[shaderID].CreateShader(aPath, myDevice, aInputLayout, aLayoutSize, myShaderCompiler))
		{
			myShaderMap.emplace(aPath, shaderID);
		}
		else
		{
			myVertexShaders.ReleaseObject(shaderID);
		}
		break;
	case eShaderType_Pixel:
		shaderID = myPixelShaders.GetFreeItem();

		if (myPixelShaders[shaderID].CreateShader(aPath, myDevice, myShaderCompiler))
		{
			myShaderMap.emplace(aPath, shaderID);
		}
		else
		{
			myGeometryShaders.ReleaseObject(shaderID);
		}
		break;
	case eShaderType_Geometry:
		shaderID = myGeometryShaders.GetFreeItem();

		if (myGeometryShaders[shaderID].CreateShader(aPath, myDevice, myShaderCompiler))
		{
			myShaderMap.emplace(aPath, shaderID);
		}
		else
		{
			myGeometryShaders.ReleaseObject(shaderID);
		}
		break;
	default:
		DL_ERROR(eDebugLogType::Resource, "%s", "how the fuck did you get here?!?");
		shaderID = -1;
		myShaderMap.emplace(aPath, shaderID);
		break;
	}
	
	return shaderID;
}
