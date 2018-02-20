#pragma once
#include "RenderComponent.h"
#include "CameraComponent.h"
#include "SoundComponent.h"
#include "PositionComponent.h"
#include "CollisionComponent.h"
#include "../Utilities/MemoryPool.h"
#include "..\Component\ComponentManager.h"

class CEngine;
class CComponentLoader
{
public:
	CComponentLoader(CEngine& aEngine);
	~CComponentLoader();

	CRenderComponent* GetRenderComponent(const char* aPath, ModelLoaderSpace aSpace);
	CCameraComponent * GetCameraComponent();
	//CRenderComponent* GetSkyboxRenderComponent(const char* aPath, const char* aTexture);	CCameraComponent* GetCameraComponent();
	CSoundComponent* GetSoundComponent();
	CCollisionComponent* GetCollisionComponent();

	void Update();
private:
	Corgi::CMemoryPool<CRenderComponent> myGlobalRenderComponents;
	Corgi::CMemoryPool<CRenderComponent> myLocalRenderComponents;
	Corgi::CMemoryPool<CCameraComponent> myCameraComponents;
	Corgi::CMemoryPool<CSoundComponent> mySoundComponents;
	Corgi::CMemoryPool<CCollisionComponent> myCollisionComponents;

	CComponentManager myManager;

	CEngine& myEngine;
};

