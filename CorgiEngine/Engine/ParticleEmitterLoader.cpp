#include "stdafx.h"
#include "ParticleEmitterLoader.h"
#include "ParticleEmitter.h"
#include "ParticleEmitterInstance.h"
#include "Particle.h"
#include "DDSTextureLoader.h"
#include "..\Utilities\FileWatcher\FileWatcher.h"

CParticleEmitterLoader::CParticleEmitterLoader()
{
}


CParticleEmitterLoader::~CParticleEmitterLoader()
{
	myParticleResources.FreeAll();
}

bool CParticleEmitterLoader::Init(CDXFramework * aFramework, CScene * aScene)
{
	myParticleResources.Init(1024);
	myParticlePathsAdded.Init(1024);
	myParticlesReadyToUse.Init(1024);
	myScene = aScene;
	myFramework = aFramework;
	//myEmitters.Init(256);
	//LoadParticleSystem("stress.ps");
	CreateDefaultEmitter();
	myRelativePath = "../assets/ParticleSystems/";
	if (myFramework == nullptr)
	{
		return false;
	}

	return true;
}

CParticleEmitterInstance * CParticleEmitterLoader::CreateInstance()
{
	CParticleEmitterInstance* returnMe = new CParticleEmitterInstance();
	returnMe->SetEmitterID(myDefaultParticleID);
	myScene->AddParticleEmitterInstance(returnMe);
	return returnMe;
}


CParticleEmitterInstance* CParticleEmitterLoader::CreateInstance(long long aParticleEmitterID)
{
	CParticleEmitterInstance* instance = new CParticleEmitterInstance();
	myScene->AddParticleEmitterInstance(instance);
	instance->Init(aParticleEmitterID, GetEmitter(aParticleEmitterID).GetMaxAmountOfParticles());
	instance->SetIsActive(true);
	Matrix44 defaultOrientation;
	instance->SetOrientation(defaultOrientation);

	return instance;
}

CParticleEmitterInstance * CParticleEmitterLoader::CreateInstance(const char * aParticleSystemFileName)
{
	CParticleEmitterInstance* instance = new CParticleEmitterInstance();
	myScene->AddParticleEmitterInstance(instance);

	long long sysID = LoadParticleSystem(aParticleSystemFileName);
	instance->Init(sysID, GetEmitter(sysID).GetMaxAmountOfParticles());
	instance->SetIsActive(true);
	Matrix44 defaultOrientation;
	instance->SetOrientation(defaultOrientation);

	return instance;
}

bool CParticleEmitterLoader::IsEmitterReady(const long long & aEmitterID)
{
	return myParticlesReadyToUse[aEmitterID];
}

bool CParticleEmitterLoader::SetResourceIDIfExists(long long& aIDOut, const std::string& aPath)
{
	long long tempID;
	tempID = myParticlePathsAdded.Find(aPath);
	if (tempID != -1)
	{
		aIDOut = tempID;
		return true;
	}
	return false;
}
void CParticleEmitterLoader::ReloadEmitter(std::wstring aPath)
{
	std::string tempString = Corgi::ConvertWStringToString(aPath);
	Corgi::ConvertStringPathToSafeUse(tempString);
	long long tempID;
	if (SetResourceIDIfExists(tempID, tempString))
	{
		myParticlesReadyToUse[tempID] = false;

		CParticleEmitter newEmitter;
		if (CreateParticleSystem(newEmitter, tempString))
		{
			myParticleResources[tempID] = newEmitter;
		}

		DL_ERROR(eDebugLogType::Resource, "%s%s", "[ParticleEmitterLoader] - Failed to reload file: ", tempString.c_str());
		myParticlesReadyToUse[tempID] = true;
	}
	else
	{
		//Unknown Error
		assert(false);
	}
	return;
}

long long CParticleEmitterLoader::LoadParticleSystem(const char * aParticleSystemFileName)
{
	std::string particlePath = myRelativePath;
	particlePath += aParticleSystemFileName;
	Corgi::ConvertStringPathToSafeUse(particlePath);

	long long idToReturn = myDefaultParticleID;

	if (SetResourceIDIfExists(idToReturn, particlePath))
	{
		return idToReturn;
	}

	DL_WRITELOG(eConsoleColor::LIGHTBLUE,eDebugLogType::Resource, "%s%s", "[ParticleEmitterLoader] - Creating particle: ", aParticleSystemFileName);
	CParticleEmitter newEmitter;
	if (CreateParticleSystem(newEmitter, particlePath))
	{
		idToReturn = myParticleResources.GetFreeItem();
		myParticlePathsAdded.Add(particlePath);
		myParticleResources[idToReturn] = newEmitter;
		myParticlesReadyToUse.Add(true);
		DL_SUCCESS(eDebugLogType::Resource, "%s%s", "[ParticleEmitterLoader] - Particlesystem loaded: ", aParticleSystemFileName);
	}
	else
	{
		DL_ERROR(eDebugLogType::Resource, "%s%s", "[ParticleEmitterLoader] - Failed to create: ", aParticleSystemFileName);
	}
	return idToReturn;
}

bool CParticleEmitterLoader::CreateParticleSystem(CParticleEmitter& aEmitter, std::string& aParticleSystemFilePath)
{
	int textureNameSize = 0;
	SEmitter newEmitterData;
	FILE* file;
	fopen_s(&file, aParticleSystemFilePath.c_str(), "rb");

	if (file != NULL)
	{

		fread(&textureNameSize, sizeof(int), 1, file);
		std::string n(textureNameSize, '\0');
		fread(&n[0], sizeof(char), textureNameSize, file);
		fread(&newEmitterData, sizeof(SEmitter), 1, file);
		fclose(file);

		std::string path = "../Assets/Sprites/Particles/" + n;// TEMP tills relativo sökovägo;
		if (Corgi::FileExists(path.c_str()) == false)
		{
			DL_ERROR(eDebugLogType::Resource, "%s%s", "[ParticleEmitterLoader] - Failed to find file: ", path.c_str());
			return false;
		}

		aEmitter.Init(newEmitterData, myFramework->GetDevice(), myFramework->GetContext());

		auto texture = LoadTexture(path);
		if (texture == nullptr)
		{
			DL_ERROR(eDebugLogType::Resource, "%s%s", "[ParticleEmitterLoader] - Failed to crate texture: ", path.c_str());
			return false;
		}

		aEmitter.SetTexture(texture);
		Corgi::CFileWatcher::GetInstance().WatchFileChange(Corgi::ConvertCharPtrToWideString(aParticleSystemFilePath.c_str()), std::bind(&CParticleEmitterLoader::ReloadEmitter, this, std::placeholders::_1));
		return true;
	}
	DL_ERROR(eDebugLogType::Resource, "%s%s", "[ParticleEmitterLoader] - Failed to find file: ", aParticleSystemFilePath.c_str());
	return false;
}

ID3D11ShaderResourceView * CParticleEmitterLoader::LoadTexture(std::string & aFilePath)
{
	std::wstring filename;
	filename.assign(aFilePath.begin(), aFilePath.end());
	ID3D11ShaderResourceView * creation;
	HRESULT result = DirectX::CreateDDSTextureFromFile(myFramework->GetDevice(), filename.c_str(), nullptr, &creation);
	if (FAILED(result))
	{
		DL_ERROR(eDebugLogType::Resource, "%s%s", "[ParticleEmitterLoader] - LoadTexture failed to load file: ", aFilePath.c_str());
	}
	else
	{
		DL_SUCCESS(eDebugLogType::Resource, "%s%s%s", "[ParticleEmitterLoader] - Texture ", aFilePath.c_str(), " load succeded");
	}
	return creation;
}

CParticleEmitter & CParticleEmitterLoader::GetEmitter(long long aEmitterId)
{
	if (myParticleResources.IsCurrentlyUsed(aEmitterId) == false)
	{
		DL_ERROR(eDebugLogType::Resource, "%s", "[ParticleEmitterLoader] - Tried to get a emittorID that doesnt exsist");
		assert(false);
		return myParticleResources[myDefaultParticleID];
	}
	return myParticleResources[aEmitterId];
}



void CParticleEmitterLoader::CreateDefaultEmitter()
{
	SEmitter DefaultSystemData;
	CParticleEmitter defaultSystem;

	DefaultSystemData.emittertype	= eEmitterType_Rectangle;
	
	DefaultSystemData.color0 = {1,1,1,1};
	DefaultSystemData.color1 = {1,1,1,1.f};
	DefaultSystemData.color2 = {1,1,1,1.f};

	DefaultSystemData.textureColumns = 0;
	DefaultSystemData.textureRows = 0;
	DefaultSystemData.textureType = 0;
									 
	DefaultSystemData.endSizeX = 100.f;
	DefaultSystemData.endSizeY = 100.f;
	DefaultSystemData.startSizeX = 1.f;
	DefaultSystemData.startSizeY = 1.f;

	DefaultSystemData.rectSizeX = 100;
	DefaultSystemData.rectSizeZ = 100;
	
	DefaultSystemData.velocity = {3.f,8.f,1.f};
	DefaultSystemData.gravity = 1.f;
	DefaultSystemData.lifetime	= 500000.f;
	
	DefaultSystemData.emissiondelay	= 0.0000000001f;
	DefaultSystemData.looping = true;
	DefaultSystemData.maxparticles	= 9999;

	myDefaultParticleID = myParticleResources.GetFreeItem();

	defaultSystem.Init(DefaultSystemData, myFramework->GetDevice(), myFramework->GetContext());

	std::string path = "../Assets/Sprites/Particles/Smoke.dds";
	auto texture = LoadTexture(path);
	if (texture == nullptr)
	{
		return;
	}
	defaultSystem.SetTexture(texture);
	myParticlePathsAdded.Add("Default");
	myParticleResources[myDefaultParticleID] = defaultSystem;
	myParticlesReadyToUse.Add(true);
}
