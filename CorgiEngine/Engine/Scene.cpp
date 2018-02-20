#include "stdafx.h"
#include "EnvironmentLightInstance.h"
#include "PointLightInstance.h"
#include "SpotLightInstance.h"
#include "ParticleEmitterInstance.h"
#include "SpriteInstance.h"
#include "ModelLoaderWrapper.h"
#include "AnimationManager.h"
#include "AnimationInstance.h"
#define CSCENE_MODELINSTANCES_ARRAYSIZE 100000
#define CSCENE_POINTLIGHTINSTANCES_ARRAYSIZE 10000
#define CSCENE_SPOTLIGHTINSTANCES_ARRAYSIZE 4096
#define CSCENE_EMITTERINSTANCES_ARRAYSIZE 256
#define CSCENE_SPRITEINSTANCES_ARRAYSIZE 1024


CScene::CScene(CAnimationManager& aAnimationManager,CResourceManager& aResourceManagerRef) :
	myAnimationManager (aAnimationManager),
myResourceManagerRef(aResourceManagerRef)
{
}


CScene::~CScene()
{
}

bool CScene::Init()
{
	myModelInstances.Init(CSCENE_MODELINSTANCES_ARRAYSIZE);
	myPointLightInstances.Init(CSCENE_POINTLIGHTINSTANCES_ARRAYSIZE);
	mySpotLightInstances.Init(CSCENE_SPOTLIGHTINSTANCES_ARRAYSIZE);
	myEmitters.Init(CSCENE_EMITTERINSTANCES_ARRAYSIZE);
	mySpriteInstances.Init(CSCENE_SPRITEINSTANCES_ARRAYSIZE);
	return true;
}

void CScene::Update()
{
	for (CModelInstance* inst : myModelInstances)
	{
		if (inst->myIsStatic && inst->myIsAnimationInitialized == false && myResourceManagerRef.GetModelLoaderWrapper()->IsModelReadyToUse(inst->GetModelId()) && myResourceManagerRef.GetModelLoaderWrapper()->GetModel(inst->GetModelId()).IsAnimated() && inst->myAnimationName == "")
		{
			inst->Animate(myResourceManagerRef.GetModelLoaderWrapper()->GetModel(inst->GetModelId()).GetAnimationID(), true);
		}
		if (inst->myAnimateThisObject)
		{
			if (!inst->myAnimationInstance)
			{
				CAnimationInstance* animinst = myAnimationManager.GetAnimationInstance();
				inst->SetAnimationInstance(animinst, 0);
				if (inst->myIsStatic)
				{
					animinst->SetAnimationTime(Corgi::RandomFloat(0, 1000));
				}
			}
			inst->myAnimationInstance->SetAnimation(inst->myAnimationName);
		}
	}
}

bool CScene::AddModelInstance(CModelInstance * aModelInstance)
{
	myModelInstances.Add(aModelInstance);
	if (!aModelInstance)
	{
		return false;
	}
	return true;
}

bool CScene::AddEnvironmentLightInstance(CEnvironmentLightInstance * aEnvironmentLightInstance)
{
	myEnvironmentLightInstance = aEnvironmentLightInstance;
	if (!myEnvironmentLightInstance)
	{
		return false;
	}
	return true;
}

bool CScene::AddPointLightInstance(CPointLightInstance * aPointLightInstance)
{
	if (!aPointLightInstance)
	{
		return false;
	}
	myPointLightInstances.Add(aPointLightInstance);
	return true;
}

bool CScene::AddSpotLightInstance(CSpotLightInstance * aSpotLightInstance)
{
	if (!aSpotLightInstance)
	{
		return false;
	}
	mySpotLightInstances.Add(aSpotLightInstance);
	return true;
}

bool CScene::AddParticleEmitterInstance(CParticleEmitterInstance * aParticleEmitterInstance)
{
	myEmitters.Add(aParticleEmitterInstance);
	return true;
}

bool CScene::AddSpriteInstance(CSpriteInstance * aSpriteInstance)
{
	mySpriteInstances.Add(aSpriteInstance);
	return true;
}

void CScene::AddSkybox(CModelInstance * aSkybox)
{
	mySkybox = aSkybox;
}

void CScene::GetSkybox(Corgi::GrowingArray<CModelInstance*>& aModelsContainer)
{
	if (mySkybox != nullptr)
	{
		aModelsContainer.Add(mySkybox);
	}
}


CEnvironmentLightInstance * CScene::GetEnvironmentLight()
{
	return myEnvironmentLightInstance;
}
void CScene::Cull(Corgi::GrowingArray<CModelInstance*>& aModelsContainer)
{
	long long aID;
	for (int i = 0; i < myModelInstances.Size(); ++i)
	{
		if (myModelInstances[i]->GetShouldRender() == false)
		{
			continue;
		}
		
		aID = myModelInstances[i]->GetModelId();
		if (aID == -1)
		{
			continue;
		}

		if (myResourceManagerRef.GetModelLoaderWrapper()->IsModelReadyToUse(aID) == false)
		{
			continue;
		}

		aModelsContainer.Add(myModelInstances[i]);	
	}
}

void CScene::CullPointLightInstances(Corgi::GrowingArray<CPointLightInstance*>& aPointLightInstanceContainer)
{
	for (int i = 0; i < myPointLightInstances.Size(); ++i)
	{
		if (myPointLightInstances[i]->GetID() != -1 && myPointLightInstances[i]->GetShouldShine() == true)
		{
			aPointLightInstanceContainer.Add(myPointLightInstances[i]);
		}
	}
#ifdef _DEBUGGRAPHICS
	DL_WRITELOG(eConsoleColor::GRAY, eDebugLogType::Render, "%s%i", "CULLED:", amount);
	//std::cout << "CULLED: "<< amount <<std::endl;
#endif
}

void CScene::CullSpotLightInstances(Corgi::GrowingArray<CSpotLightInstance*>& aSpotLightInstanceContainer)
{
	for (int i = 0; i < mySpotLightInstances.Size(); ++i)
	{
		if (mySpotLightInstances[i]->GetID() != -1 && mySpotLightInstances[i]->GetShouldShine() == true)
		{
			aSpotLightInstanceContainer.Add(mySpotLightInstances[i]);
		}
	}
#ifdef _DEBUGGRAPHICS
	DL_WRITELOG(eConsoleColor::GRAY, eDebugLogType::Render, "%s%i", "CULLED:", amount);
	//std::cout << "CULLED: "<< amount <<std::endl;
#endif
}

void CScene::CullParticleEmitterInstances(Corgi::GrowingArray<CParticleEmitterInstance*>& aParticleEmitterInstanceContainer)
{
	for (int i = 0; i < myEmitters.Size(); ++i)
	{
		if (myEmitters[i]->GetEmitterID() != -1 && myEmitters[i]->GetIsActive() == true)
		{
 			aParticleEmitterInstanceContainer.Add(myEmitters[i]);
		}
	}
}

void CScene::CullSpriteInstances(Corgi::GrowingArray<CSpriteInstance*>& aSpriteInstanceContainer)
{
	int loopDistance = mySpriteInstances.Size();
	for (int i = 0; i < loopDistance; ++i)
	{
		if (mySpriteInstances[i]->GetShouldRemove())
		{
			delete mySpriteInstances[i];
			mySpriteInstances[i] = nullptr;
			mySpriteInstances.RemoveCyclicAtIndex(i);
			--loopDistance;
			--i;
		}
		else if (mySpriteInstances[i]->GetSpriteID() != -1 && mySpriteInstances[i]->GetShouldRender() == true)
		{
			aSpriteInstanceContainer.Add(mySpriteInstances[i]);
		}
	}
}

void CScene::Clean()
{
	myModelInstances.DeleteAll();
	myPointLightInstances.DeleteAll();
	mySpotLightInstances.DeleteAll();
	myEmitters.DeleteAll();
	mySpriteInstances.DeleteAll();
	//myAnimationManager.Reset();
}
