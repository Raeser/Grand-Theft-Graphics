#include "stdafx.h"
#include "LightLoader.h"
#include "DDSTextureLoader.h"
#include "EnvironmentLightInstance.h"
#include "EnvironmentLight.h"
#include "PointLight.h"
#include "PointLightInstance.h"
#include "SpotLight.h"
#include "SpotLightInstance.h"


CLightLoader::CLightLoader()
{
}


CLightLoader::~CLightLoader()
{
	myFramework = nullptr;
	for (CEnvironmentLight& light : myEnvironmentLights)
	{
		light.Release();
	}
}

bool CLightLoader::Init(CDXFramework* aFrameWork, CScene* aScene)
{
	myEnvironmentLights.Init(4);
	myCubemapPaths.Init(128);
	myPointLights.Init(256);
	mySpotLights.Init(256);
	SSpotLightData defaultSpotLight;
	CreateSpotLight(defaultSpotLight);
	SPointLightData defaultPointLight;
	CreatePointLight(defaultPointLight);
	myFramework = aFrameWork;
	myScene = aScene;
	return true;
}

void CLightLoader::Reset()
{
	//myEnvironmentLights.RemoveAll();
	//myCubemapPaths.RemoveAll();
	myPointLights.RemoveAll();
	mySpotLights.RemoveAll();
}

CEnvironmentLightInstance * CLightLoader::CreateEnvironmentLightInstance()
{
	CEnvironmentLightInstance* returnMe = new CEnvironmentLightInstance();

	if (!myScene->AddEnvironmentLightInstance(returnMe))
	{
		DL_ERROR(eDebugLogType::Resource, "%s", "Error creating empty environmentlight in GlobalScene");
		assert(false && "Error creating empty environmentlight in GlobalScene");
		return nullptr;
	}

	return returnMe;
}

CEnvironmentLightInstance * CLightLoader::CreateEnvironmentLightInstance(long long aLightID, Vector4f aDirection, Vector4f aColor)
{
	CEnvironmentLightInstance* returnMe = new CEnvironmentLightInstance();
	returnMe->Init(aDirection, aColor);
	returnMe->SetEnvironmentLight(aLightID);


	if (!myScene->AddEnvironmentLightInstance(returnMe))
	{
		DL_ERROR(eDebugLogType::Resource, "%s", "Error creating environmentlight using ID in GlobalScene");
		assert(false && "Error creating environmentlight using ID in GlobalScene");
		return nullptr;
	}
	DL_WRITELOG(eConsoleColor::LIGHTBLUE, eDebugLogType::Engine, "%s", "[LightLoader] - EnvironmentLightInstance Created");
	return returnMe;
}

CEnvironmentLightInstance * CLightLoader::CreateEnvironmentLightInstance(const char * aCubeMapPath, Vector4f aDirection, Vector4f aColor)
{
	CEnvironmentLightInstance* returnMe = new CEnvironmentLightInstance();
	returnMe->Init(aDirection, aColor);
	returnMe->SetEnvironmentLight(CreateEnvironmentLight(aCubeMapPath));


	if (!myScene->AddEnvironmentLightInstance(returnMe))
	{
		DL_ERROR(eDebugLogType::Resource, "%s", "Error creating environmentlight with new Light in GlobalScene");
		assert(false && "Error creating environmentlight with new Light in GlobalScene");
		return nullptr;
	}

	DL_WRITELOG(eConsoleColor::LIGHTBLUE, eDebugLogType::Engine, "%s", "[LightLoader] - EnvironmentLightInstance Created");
	return returnMe;
}

CPointLightInstance * CLightLoader::CreatePointLightInstance(long long aID, Vector3f& aPos)
{
	Vector4f aPosition;
	aPosition.x = aPos.x;
	aPosition.y = aPos.y;
	aPosition.z = aPos.z;
	aPosition.w = 0.f;

	CPointLightInstance* createdPointLightInstance = new CPointLightInstance();
	createdPointLightInstance->Init(aPosition);
	createdPointLightInstance->SetPointLight(aID);
	createdPointLightInstance->SetIsOn(true);

	if (!myScene->AddPointLightInstance(createdPointLightInstance))
	{
		DL_ERROR(eDebugLogType::Resource, "%s", "Error creating pointLightInstance with new PointLight in GlobalScene");
		assert(false && "Error creating pointLightInstance with new PointLight in GlobalScene");
		return nullptr;
	}
	DL_WRITELOG(eConsoleColor::LIGHTBLUE, eDebugLogType::Engine, "%s", "[LightLoader] - PointLightInstance Created");
	return createdPointLightInstance;
}

CPointLightInstance * CLightLoader::CreatePointLightInstance(long long aID, Vector4f& aPos)
{
	CPointLightInstance* createdPointLightInstance = new CPointLightInstance();
	createdPointLightInstance->Init(aPos);
	createdPointLightInstance->SetPointLight(aID);
	createdPointLightInstance->SetIsOn(true);

		if (!myScene->AddPointLightInstance(createdPointLightInstance))
		{
			DL_ERROR(eDebugLogType::Resource, "%s", "Error creating pointLightInstance with new PointLight in GlobalScene");
			assert(false && "Error creating pointLightInstance with new PointLight in GlobalScene");
			return nullptr;
		}
		DL_WRITELOG(eConsoleColor::LIGHTBLUE, eDebugLogType::Engine, "%s", "[LightLoader] - PointLightInstance Created");
	return createdPointLightInstance;
}

CPointLightInstance * CLightLoader::CreatePointLightInstance(long long aID)
{
	CPointLightInstance* createdPointLightInstance = new CPointLightInstance();
	createdPointLightInstance->Init({0.f, 0.f, 0.f, 0.f});
	createdPointLightInstance->SetPointLight(aID);
	createdPointLightInstance->SetIsOn(false);

	if (!myScene->AddPointLightInstance(createdPointLightInstance))
	{
		DL_ERROR(eDebugLogType::Resource, "%s", "Error creating pointLightInstance with new PointLight in GlobalScene");
		assert(false && "Error creating pointLightInstance with new PointLight in GlobalScene");
		return nullptr;
	}
	DL_WRITELOG(eConsoleColor::LIGHTBLUE, eDebugLogType::Engine, "%s", "[LightLoader] - PointLightInstance Created");
	return createdPointLightInstance;
}


CSpotLightInstance * CLightLoader::CreateSpotLightInstance(long long aID, Vector3f & aPos, Vector3f& aDir)
{
	CSpotLightInstance* createdSpotLightInstance = new CSpotLightInstance();
	createdSpotLightInstance->Init({ aPos.x,  aPos.y,  aPos.z, 0.f }, { aDir.x, aDir.y, aDir.z, 1.f });
	createdSpotLightInstance->SetSpotLight(aID);
	createdSpotLightInstance->SetIsOn(true);

	if (!myScene->AddSpotLightInstance(createdSpotLightInstance))
	{
		DL_ERROR(eDebugLogType::Resource, "%s", "Error creating pointLightInstance with new PointLight in GlobalScene");
		assert(false && "Error creating pointLightInstance with new PointLight in GlobalScene");
		return nullptr;
	}
	DL_WRITELOG(eConsoleColor::LIGHTBLUE, eDebugLogType::Engine, "%s", "[LightLoader] - SpotLightInstance Created");
	return createdSpotLightInstance;
}

CSpotLightInstance * CLightLoader::CreateSpotLightInstance(long long aID)
{
	CSpotLightInstance* createdSpotLightInstance = new CSpotLightInstance();
	createdSpotLightInstance->Init({ 0.f,  0.f,  0.f, 0.f }, { 0.f , 0.f , 0.f , 1.f });
	createdSpotLightInstance->SetSpotLight(aID);
	createdSpotLightInstance->SetIsOn(true);

	if (!myScene->AddSpotLightInstance(createdSpotLightInstance))
	{
		DL_ERROR(eDebugLogType::Resource, "%s", "Error creating pointLightInstance with new PointLight in GlobalScene");
		assert(false && "Error creating pointLightInstance with new PointLight in GlobalScene");
		return nullptr;
	}
	DL_WRITELOG(eConsoleColor::LIGHTBLUE, eDebugLogType::Engine, "%s", "[LightLoader] - SpotLightInstance Created");
	return createdSpotLightInstance;
}

CSpotLightInstance * CLightLoader::CreateSpotLightInstance(long long aID, Vector4f & aPos, Vector4f& aDir)
{
	CSpotLightInstance* createdSpotLightInstance = new CSpotLightInstance();
	createdSpotLightInstance->Init(aPos, aDir);
	createdSpotLightInstance->SetSpotLight(aID);
	createdSpotLightInstance->SetIsOn(true);

	if (!myScene->AddSpotLightInstance(createdSpotLightInstance))
	{
		DL_ERROR(eDebugLogType::Resource, "%s", "Error creating pointLightInstance with new PointLight in GlobalScene");
		assert(false && "Error creating pointLightInstance with new PointLight in GlobalScene");
		return nullptr;
	}
	DL_WRITELOG(eConsoleColor::LIGHTBLUE, eDebugLogType::Engine, "%s", "[LightLoader] - SpotLightInstance Created");
	return createdSpotLightInstance;
}

long long CLightLoader::CreateEnvironmentLight(const char * aCubeMapPath)
{
	ID3D11Buffer* directionalLightBuffer;

	HRESULT result;

	D3D11_BUFFER_DESC bufferDescription = {};
	bufferDescription.ByteWidth = sizeof(SDirectionLightBufferData);
	bufferDescription.Usage = D3D11_USAGE_DYNAMIC;
	bufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	result = myFramework->GetDevice()->CreateBuffer(&bufferDescription, nullptr, &directionalLightBuffer);

	if (FAILED(result))
	{
		DL_ERROR(eDebugLogType::Resource, "%s", "EnvironmentLight failed to create buffer");
		return false;
	}

	//Load CubeMap
	std::wstring filename;
	std::string cubeMapPath = aCubeMapPath;
	filename = L"";
	filename.assign(cubeMapPath.begin(), cubeMapPath.end());
	ID3D11ShaderResourceView* cubeMap;
	



	result = DirectX::CreateDDSTextureFromFile(myFramework->GetDevice(), filename.c_str(), nullptr, &cubeMap);
	if (FAILED(result))
	{
		DL_ERROR(eDebugLogType::Resource, "%s", "CubeMap failed to load");
		if (result == D3D11_ERROR_FILE_NOT_FOUND)
		{
			DL_ERROR(eDebugLogType::Resource, "%s", "CubeMap File was not found");
		}
	}
	else
	{
		DL_SUCCESS(eDebugLogType::Resource, "%s", "CubeMap load succeded");
	}
	//End load cubemap


	SEnvironmentLightData myData;
	myData.myDirectionalLightBuffer = directionalLightBuffer;
	myData.myCubeMap = cubeMap;

	CEnvironmentLight createdLight;	
	createdLight.Init(myData);
	myEnvironmentLights.Add(createdLight);
	myCubemapPaths.Add(aCubeMapPath);

	DL_WRITELOG(eConsoleColor::LIGHTBLUE, eDebugLogType::Engine, "%s", "[LightLoader] - EnvironmentLight Created");
	return myEnvironmentLights.Size() - 1;
}

long long CLightLoader::CreatePointLight(const SPointLightData & aData)
{
	CPointLight createdPointLight;
	createdPointLight.Init(aData);
#ifndef _RETAIL
	if (aData.myColor.x > 1 || aData.myColor.y > 1 || aData.myColor.z > 1)
	{
		DL_ERROR(eDebugLogType::Resource, "%s", "PointLightColor was over maximum value(value should be between 0 - 1)");
		return 0;
	}
	if (aData.myColor.x < 0 || aData.myColor.y < 0 || aData.myColor.z < 0)
	{
		DL_ERROR(eDebugLogType::Resource, "%s", "PointLightColor was under minimum value(value should be between 0 - 1)");
		return 0;
	}
#endif
	myPointLights.Add(createdPointLight);

	DL_WRITELOG(eConsoleColor::LIGHTBLUE, eDebugLogType::Engine, "%s", "[LightLoader] - PointLight Created");
	return myPointLights.Size() - 1;
}

long long CLightLoader::CreateSpotLight(const SSpotLightData & aData)
{
	CSpotLight createdSpotLight;
	createdSpotLight.Init(aData);
#ifndef _RETAIL
	if (aData.myColor.x > 1 || aData.myColor.y > 1 || aData.myColor.z > 1)
	{
		DL_ERROR(eDebugLogType::Resource, "%s", "PointLightColor was over maximum value(value should be between 0 - 1)");
		return 0;
	}
	if (aData.myColor.x < 0 || aData.myColor.y < 0 || aData.myColor.z < 0)
	{
		DL_ERROR(eDebugLogType::Resource, "%s", "PointLightColor was under minimum value(value should be between 0 - 1)");
		return 0;
	}
#endif
	mySpotLights.Add(createdSpotLight);

	DL_WRITELOG(eConsoleColor::LIGHTBLUE, eDebugLogType::Engine, "%s", "[LightLoader] - SpotLight Created");
	return mySpotLights.Size() - 1;
}


void CLightLoader::AssignFrameWork(CDXFramework * aFrameWork)
{
	myFramework = aFrameWork;
}


void CLightLoader::LoadLights()
{
	//LoadSpotLights();
	LoadPointLights();
}

CEnvironmentLight & CLightLoader::GetEnvironmentLight(long long aID)
{
	return myEnvironmentLights[aID];
}

CPointLight & CLightLoader::GetPointLight(long long aID)
{
	return myPointLights[aID];
}

CSpotLight & CLightLoader::GetSpotLight(long long aID)
{
	return mySpotLights[aID];
}

void CLightLoader::LoadPointLights()
{
	SPointLightData data;
	data.myColor = { 1, 0.01f, 0.01f };
	data.myRange = 30.f;

	CreatePointLight(data);

	data.myColor = { 0.01f, 1.f, 0.01f };
	data.myRange = 15.f;

	CreatePointLight(data);

	data.myColor = { 0.01f, 0.01f, 1.f };
	data.myRange = 10.f;

	CreatePointLight(data);

	data.myColor = { 228 / 255.f, 39 / 255.f, 39 / 255.f };
	data.myRange = 2.03999901f;

	CreatePointLight(data);
}
