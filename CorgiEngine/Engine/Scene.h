#pragma once
#include "..\Utilities\GrowingArray.h"

class CModelInstance;
class CPointLightInstance;
class CEnvironmentLightInstance;
class CSpotLightInstance;
class CParticleEmitterInstance;
class CSpriteInstance;
class CResourceManager;

class CAnimationManager;
class CScene
{
public:
	CScene(CAnimationManager& aAnimationManager,CResourceManager& aResourceManagerRef);
	~CScene();

	bool Init();
	void Update();

	bool AddModelInstance(CModelInstance* aModelInstance);
	bool AddEnvironmentLightInstance(CEnvironmentLightInstance* aEnvironmentLightInstance);
	bool AddPointLightInstance(CPointLightInstance* aPointLightInstance);
	bool AddSpotLightInstance(CSpotLightInstance* aSpotLightInstance);
	bool AddParticleEmitterInstance(CParticleEmitterInstance* aParticleEmitterInstance);
	bool AddSpriteInstance(CSpriteInstance* aSpriteInstance);
	void AddSkybox(CModelInstance* aSkybox);
	void GetSkybox(Corgi::GrowingArray<CModelInstance*>& aModelsContainer);
	CEnvironmentLightInstance* GetEnvironmentLight();
	void Cull(Corgi::GrowingArray<CModelInstance*>& aModelsContainer);
	void CullPointLightInstances(Corgi::GrowingArray<CPointLightInstance*>& aPointLightInstanceContainer);
	void CullSpotLightInstances(Corgi::GrowingArray<CSpotLightInstance*>& aSpotLightInstanceContainer);
	void CullParticleEmitterInstances(Corgi::GrowingArray<CParticleEmitterInstance*>& aParticleEmitterInstanceContainer);
	void CullSpriteInstances(Corgi::GrowingArray<CSpriteInstance*>& aSpriteInstanceContainer);
	void Clean();
private:
	CAnimationManager& myAnimationManager;
	Corgi::GrowingArray<CModelInstance*> myModelInstances;
	CModelInstance* mySkybox;
	Corgi::GrowingArray<CPointLightInstance*>myPointLightInstances;
	Corgi::GrowingArray<CSpotLightInstance*>mySpotLightInstances;
	Corgi::GrowingArray<CParticleEmitterInstance*> myEmitters;
	Corgi::GrowingArray<CSpriteInstance*> mySpriteInstances;
	CResourceManager& myResourceManagerRef;
	CEnvironmentLightInstance* myEnvironmentLightInstance;
};

