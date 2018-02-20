#pragma once
#include "..\Utilities\GrowingArray.h"
#include "..\Utilities\MemoryPool.h"
#include "ParticleEmitterInstance.h"
struct ID3D11ShaderResourceView;
class CParticleEmitter;
class CParticleEmitterInstance;
class CDXFramework;
class CScene;
struct SParticleEmitterData;



class CParticleEmitterLoader
{
public:
	CParticleEmitterLoader();
	~CParticleEmitterLoader();
	bool Init(CDXFramework* aFramework, CScene * aScene);
	CParticleEmitterInstance* CreateInstance();
	CParticleEmitterInstance* CreateInstance(long long aParticleEmitterID);
	CParticleEmitterInstance* CreateInstance(const char* aParticleSystemFileName);
	bool IsEmitterReady(const long long& aEmitterID);
	bool SetResourceIDIfExists(long long & aIDOut, const std::string & aPath);
	void ReloadEmitter(std::wstring aPath);
	long long LoadParticleSystem(const char* aSystemConfigPath);
	bool CreateParticleSystem(CParticleEmitter& aEmitter, std::string& aParticleSystemFilePath);
	ID3D11ShaderResourceView* LoadTexture(std::string& aFilePath);
	CParticleEmitter& GetEmitter(long long aEmitterId);

	void CreateDefaultEmitter();

private:
	enum TextureEnums
	{
		eAlbedo,
		eRoughness,
		eAmbient,
		eEmissive,
		eHeight,
		eNormals,
		eShininess,
		eOpacity,
		eDisplacement,
		eLightMap,
		eMetalness
	};
	//Corgi::GrowingArray<CParticleEmitter, long long> myEmitters;
	CScene* myScene;
	CDXFramework* myFramework;
	Corgi::GrowingArray<std::string, long long> myParticlePathsAdded;
	Corgi::CMemoryPool<CParticleEmitter, long long> myParticleResources;
	Corgi::GrowingArray<bool, long long> myParticlesReadyToUse;
	long long myDefaultParticleID;
	std::string myRelativePath;
};

