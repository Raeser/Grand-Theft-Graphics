#include "stdafx.h"
#include "ComponentBuilder.h"
#include "../Engine/Engine.h"
#include "../Engine/SpriteLoader.h"
#include "../Engine/Model.h"
#include "Component_Defines.h"

#include "../Engine/PointLight.h"
#include "../Engine/SpotLight.h"
#include "../Engine/PointLightInstance.h"
#include "../Engine/SpotLightInstance.h"
#include "../Engine/LightLoader.h"

#include "../Engine/ModelLoaderWrapper.h"
#include "../Engine/ResourceManager.h"
#include "../Engine/ModelInstance.h"

CComponentBuilder::CComponentBuilder(CEngine & aEngine) :
	myEngine(aEngine)
{
}

CComponentBuilder::~CComponentBuilder()
{
}

bool CComponentBuilder::Init()
{
	myTransformComponents.Init(POSITION_COMPONENT_ARRAYSIZE);
	myRenderComponents.Init(RENDER_GLOBAL_COMPONENT_ARRAYSIZE);
	myCameraComponents.Init(CAMERA_COMPONENT_ARRAYSIZE);
	mySoundComponents.Init(SOUND_COMPONENT_ARRAYSIZE);
	myPointLightComponents.Init(POINTLIGHT_COMPONENT_ARRAYSIZE);
	mySpotLightComponents.Init(SPOTLIGHT_COMPONENT_ARRAYSIZE);

	myCollisionComponents.Init(COLLISION_COMPONENT_ARRAYSIZE);
	myHealthComponents.Init(HEALTH_COMPONENT_ARRAYSIZE);
	myParticleEmitterComponents.Init(2048);
	myLifeTimeComponents.Init(LIFETIME_COMPONENT_ARRAYSIZE);
	myTriggerComponents.Init(TRIGGER_COMPONENT_ARRAYSIZE);
	myPlayerComponenets.Init(4);
	myActorComponent.Init(128);
	myScriptComponents.Init(128);

	//for (int i = 0; i < 128; ++i)
	//{
	//	myParticleEmitterComponents[i].SetEmitterInstance(myEngine.GetParticleEmittorLoader().CreateInstance());
	//}

	for (int i = 0; i < CAMERA_COMPONENT_ARRAYSIZE; ++i)
	{
		myCameraComponents[i].SetCameraInstance(myEngine.GetGlobalCamera()); //Move Cameras To someplace better!
	}
	//Vector4f defaultPosition = POINTLIGHT_COMPONENT_DEFAULTDIRECTION;
	//for (int i = 0; i < POINTLIGHT_COMPONENT_ARRAYSIZE; ++i)
	//{
	//	myPointLightComponents[i].SetPointLightInstance(myEngine.GetLightLoader().CreatePointLightInstance(0, defaultPosition));
	//}
	//Vector4f defaultDirection = SPOTLIGHT_COMPONENT_DEFAULTDIRECTION;
	//for (int i = 0; i < SPOTLIGHT_COMPONENT_ARRAYSIZE; ++i)
	//{
	//	mySpotLightComponents[i].SetSpotLightInstance(myEngine.GetLightLoader().CreateSpotLightInstance(0, defaultPosition, defaultDirection));
	//}
	return true;
}


void CComponentBuilder::Reset()
{
	//for (CPlayerComponent& comp : myPlayerComponenets)
	//{
	//	comp.Release();
	//}
	myTransformComponents.FreeAll();
	myRenderComponents.FreeAll();
	myCameraComponents.FreeAll();
	mySoundComponents.FreeAll();
	myPointLightComponents.FreeAll();
	mySpotLightComponents.FreeAll();

	myCollisionComponents.FreeAll();
	myHealthComponents.FreeAll();
	myParticleEmitterComponents.FreeAll();
	myLifeTimeComponents.FreeAll();
	myTriggerComponents.FreeAll();
	myPlayerComponenets.FreeAll();
	myActorComponent.FreeAll();
	myScriptComponents.FreeAll();

	//Todo: 
	myEngine.GetLightLoader().Reset();
}


void CComponentBuilder::InitSpriteComponents(eScene aSpriteScene)
{
	myEngine.GetSpriteLoader().SetActiveScene(aSpriteScene);
	mySpriteComponents.Init(256);
	for (int i = 0; i < 255; ++i)
	{
		mySpriteComponents[i].SetSpriteInstance(myEngine.GetSpriteLoader().CreateSpriteInstance(-1));
	}
}



CRenderComponent * CComponentBuilder::GetRenderComponent(CEntity* aParentEntity, const char * aPath, const int& aPixelShaderID, const int& aVertextShaderID)
{
	CRenderComponent* componentToReturn;
	componentToReturn = &myRenderComponents[myRenderComponents.GetFreeItem()];
	componentToReturn->SetParent(aParentEntity);
	CModelInstance* aModelInstance = myEngine.GetResourceManager().GetModelLoaderWrapper()->CreateInstance(aPath, ModelType::Model);
	aModelInstance->SetPixelShaderID(aPixelShaderID);
	aModelInstance->SetVertexShaderID(aVertextShaderID);
	componentToReturn->SetModelInstance(aModelInstance);
	componentToReturn->SetIsActive(true);
	return componentToReturn;
}

CRenderComponent * CComponentBuilder::GetRenderComponent(CEntity* aParentEntity, const char * aPath, const char * aVSShader, const char * aPSShader, const int& aPixelShaderID, const int& aVertextShaderID)
{
	CRenderComponent* componentToReturn;
	componentToReturn = &myRenderComponents[myRenderComponents.GetFreeItem()];
	componentToReturn->SetParent(aParentEntity);
	CModelInstance* aModelInstance = myEngine.GetResourceManager().GetModelLoaderWrapper()->CreateInstance(aPath, ModelType::Model, aVSShader, aPSShader);
	aModelInstance->SetPixelShaderID(aPixelShaderID);
	aModelInstance->SetVertexShaderID(aVertextShaderID);
	componentToReturn->SetModelInstance(aModelInstance);
	componentToReturn->SetIsActive(true);
	return componentToReturn;
}


CTransformComponent * CComponentBuilder::GetTransformComponent(CEntity* aParentEntity) {
	CTransformComponent* componentToReturn = &myTransformComponents[myTransformComponents.GetFreeItem()];

	componentToReturn->Reset();
	componentToReturn->SetParent(aParentEntity);
	componentToReturn->SetIsActive(true);
	return componentToReturn;
}

CCameraComponent * CComponentBuilder::GetCameraComponent(CEntity* aParentEntity)
{
	CCameraComponent* componentToReturn = &myCameraComponents[myCameraComponents.GetFreeItem()];
	componentToReturn->SetParent(aParentEntity);
	componentToReturn->SetIsActive(true);
	return componentToReturn;
}

CSoundComponent * CComponentBuilder::GetSoundComponent(CEntity* aParentEntity)
{
	CSoundComponent* componentToReturn = &mySoundComponents[mySoundComponents.GetFreeItem()];
	componentToReturn->SetParent(aParentEntity);
	componentToReturn->SetIsActive(true);
	return componentToReturn;
}




CPointLightComponent * CComponentBuilder::GetPointLightComponent(CEntity* aParentEntity, const Vector3f& aColor, const float& aRange)
{
	SPointLightData pointLightData;
	pointLightData.myColor = aColor;
	pointLightData.myRange = aRange;

	CPointLightComponent* componentToReturn = &myPointLightComponents[myPointLightComponents.GetFreeItem()];
	componentToReturn->SetParent(aParentEntity);

	CPointLightInstance* pointLightInstance = myEngine.GetLightLoader().CreatePointLightInstance(myEngine.GetLightLoader().CreatePointLight(pointLightData));
	pointLightInstance;
	componentToReturn->SetPointLightInstance(pointLightInstance);
	componentToReturn->SetIsActive(true);
	return componentToReturn;
}

CSpotLightComponent * CComponentBuilder::GetSpotLightComponent(CEntity* aParentEntity, SSpotLightData& aSpotLightData)
{
	CSpotLightComponent* componentToReturn = &mySpotLightComponents[mySpotLightComponents.GetFreeItem()];
	componentToReturn->SetParent(aParentEntity);
	CSpotLightInstance* spotLightInstance = myEngine.GetLightLoader().CreateSpotLightInstance(myEngine.GetLightLoader().CreateSpotLight(aSpotLightData));
	componentToReturn->SetSpotLightInstance(spotLightInstance);
	componentToReturn->SetIsActive(true);
	return componentToReturn;
}

CCollisionComponent * CComponentBuilder::GetCollisionComponent(CEntity* aParentEntity)
{
	CCollisionComponent* componentToReturn = &myCollisionComponents[myCollisionComponents.GetFreeItem()];
	componentToReturn->SetParent(aParentEntity);
	componentToReturn->SetIsActive(true);
	return componentToReturn;
}


CHealthComponent * CComponentBuilder::GetHealthComponent(CEntity* aParentEntity)
{
	CHealthComponent* componentToReturn = &myHealthComponents[myHealthComponents.GetFreeItem()];
	componentToReturn->SetParent(aParentEntity);
	componentToReturn->SetIsActive(true);
	return componentToReturn;
}


CLifeTimeComponent * CComponentBuilder::GetLifeTimeComponent(CEntity * aParentEntity)
{
	CLifeTimeComponent* componentToReturn = &myLifeTimeComponents[myLifeTimeComponents.GetFreeItem()];
	componentToReturn->SetParent(aParentEntity);
	componentToReturn->SetIsActive(true);
	return componentToReturn;
}
CParticleEmitterComponent * CComponentBuilder::GetParticleEmitterComponent(CEntity * aParentEntity)
{
	CParticleEmitterComponent* componentToReturn = &myParticleEmitterComponents[myParticleEmitterComponents.GetFreeItem()];
	componentToReturn->SetParent(aParentEntity);
	return componentToReturn;
}

CSpriteComponent * CComponentBuilder::GetSpriteComponent(CEntity * aParentEntity)
{
	CSpriteComponent* componentToReturn = &mySpriteComponents[mySpriteComponents.GetFreeItem()];
	componentToReturn->SetParent(aParentEntity);
	componentToReturn->SetIsActive(true);
	return componentToReturn;
}

CTriggerComponent * CComponentBuilder::GetTriggerComponent(CEntity * aParentEntity)
{
	CTriggerComponent* componentToReturn = &myTriggerComponents[myTriggerComponents.GetFreeItem()];
	componentToReturn->SetParent(aParentEntity);
	componentToReturn->SetIsActive(true);
	return componentToReturn;
}

CPlayerComponent * CComponentBuilder::GetPlayerComponent(CEntity * aParentEntity)
{
	CPlayerComponent* componentToReturn = &myPlayerComponenets[myPlayerComponenets.GetFreeItem()];
	componentToReturn->SetParent(aParentEntity);
	componentToReturn->SetIsActive(true);
	return componentToReturn;
}

CActorComponent * CComponentBuilder::GetActorComponent(CEntity * aParentEntity)
{
	CActorComponent* componentToReturn = &myActorComponent[myActorComponent.GetFreeItem()];
	componentToReturn->SetParent(aParentEntity);
	componentToReturn->SetIsActive(true);
	return componentToReturn;
}

//TODO: Implement this code for all components later...
//
//void AddScriptComponent(asdasdasdasdasdasd)
//{
//
//	CScriptComponent* heckComp = myComponentBuilder->GetScriptComponent(&myEntities[entityID]);
//	heckComp->Init();
//	heckComp->LoadFile("../Assets/Scripts/PlayerTest.lua");
//	myEntities[entityID].AddComponent(heckComp);
//}

CScriptComponent * CComponentBuilder::GetScriptComponents(CEntity * aParentEntity)
{
	CScriptComponent* componentToReturn = &myScriptComponents[myScriptComponents.GetFreeItem()];
	componentToReturn->SetParent(aParentEntity);
	componentToReturn->SetIsActive(true);
	return componentToReturn;
}

void CComponentBuilder::Update()
{
	for (int i = 0; i < myLifeTimeComponents.Size(); ++i)
	{

		if (myLifeTimeComponents[i].ShouldRelease())
		{
			myLifeTimeComponents[i].DeInit();
			myLifeTimeComponents.ReleaseObject(i);
		}
	}

	for (int i = 0; i < myScriptComponents.Size(); ++i)
	{
		if (myScriptComponents[i].ShouldRelease())
		{
			myScriptComponents[i].DeInit();
			myScriptComponents.ReleaseObject(i);
		}
	}

	for (int i = 0; i < myActorComponent.Size(); ++i)
		{
			if (myActorComponent[i].ShouldRelease())
			{
				myActorComponent[i].DeInit();
				myActorComponent.ReleaseObject(i);
			}
	}
	for (int i = 0; i < myPlayerComponenets.Size(); ++i)
	{
		if (myPlayerComponenets[i].ShouldRelease())
		{
			myPlayerComponenets[i].DeInit();
			myPlayerComponenets.ReleaseObject(i);
		}
	}

	/*for (int i = 0; i < myInputComponents.Size(); ++i)
	{
		if (myInputComponents[i].ShouldRelease())
		{
			myInputComponents[i].DeInit();
			myInputComponents.ReleaseObject(i);
		}
	}*/


	for (int i = 0; i < myTriggerComponents.Size(); ++i)
	{

		if (myTriggerComponents[i].ShouldRelease())
		{
			myTriggerComponents[i].DeInit();
			myTriggerComponents.ReleaseObject(i);
		}
	}

	for (int i = 0; i < myCollisionComponents.Size(); ++i)
	{

			if (myCollisionComponents[i].ShouldRelease())
			{
				myCollisionComponents[i].DeInit();
				myCollisionComponents.ReleaseObject(i);

			}
		
	}


	for (int i = 0; i < myCameraComponents.Size(); ++i)
	{


			if (myCameraComponents[i].ShouldRelease())
			{
				myCameraComponents[i].DeInit();
				myCameraComponents.ReleaseObject(i);
			}
		
	}


	for (int i = 0; i < myHealthComponents.Size(); ++i)
	{

			if (myHealthComponents[i].ShouldRelease())
			{
				myHealthComponents[i].DeInit();
				myHealthComponents.ReleaseObject(i);
			}
		
	}
	

	for (int i = 0; i < myPointLightComponents.Size(); ++i)
	{

			if (myPointLightComponents[i].ShouldRelease())
			{
				myPointLightComponents[i].DeInit();
				myPointLightComponents.ReleaseObject(i);
			}
		
	}
	for (int i = 0; i < mySpotLightComponents.Size(); ++i)
	{

			if (mySpotLightComponents[i].ShouldRelease())
			{
				mySpotLightComponents[i].DeInit();
				mySpotLightComponents.ReleaseObject(i);
			}
		
	}

	for (int i = 0; i < myTransformComponents.Size(); ++i)
	{

			if (myTransformComponents[i].ShouldRelease())
			{
				myTransformComponents[i].DeInit();
				myTransformComponents.ReleaseObject(i);
			}
		
	}

	for (int i = 0; i < myRenderComponents.Size(); ++i)
	{


		if (myRenderComponents[i].ShouldRelease())
		{
			myRenderComponents[i].DeInit();
			myRenderComponents.ReleaseObject(i);
		}

	}
/*
	for (int i = 0; i < mySpriteComponents.Size(); ++i)
	{

			if (mySpriteComponents[i].ShouldRelease())
			{
				mySpriteComponents[i].DeInit();
				mySpriteComponents.ReleaseObject(i);
			}
		
	}
*/

	for (int i = 0; i < myParticleEmitterComponents.Size(); ++i)
	{

			if (myParticleEmitterComponents[i].ShouldRelease())
			{
				myParticleEmitterComponents[i].DeInit();
				myParticleEmitterComponents.ReleaseObject(i);
			}
		
	}
}
