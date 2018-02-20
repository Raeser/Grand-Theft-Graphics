#include "stdafx.h"
#include "ParticleEmitterComponent.h"
#include "../Engine/ParticleEmitterInstance.h"

CParticleEmitterComponent::CParticleEmitterComponent()
{
	myIsInited = false;
}


CParticleEmitterComponent::~CParticleEmitterComponent()
{
}

void CParticleEmitterComponent::Init(CParticleEmitterInstance* aInstance, float aLifeTime)
{
	myIsInited = true;
	CComponent::Init();
	myInstance = aInstance;
	myInstance->SetOrientation(myParent->GetTransform().GetOrientationMatrix());
	myComponentTag = eComponentTag::ParticleEmitter;

	myParticleID = aInstance->GetEmitterID();
	myParticleTimer = aLifeTime;
}
void CParticleEmitterComponent::FetchData()
{
	myInstance->SetOrientation(myParent->GetTransform().GetOrientationMatrix());
}

void CParticleEmitterComponent::Update()
{
	assert(myIsInited && "I SHOULD USE INIT");

	//if (myParticleID == 0)
	//{
		myParticleTimer += Timer::GetInstance().GetDeltaTime();
		if (myParticleTimer > myParticleTimer)
		{
			myInstance->Stop();
			myShouldRelease = true;
			myParent->Release();
		}
	//}

}

void CParticleEmitterComponent::SendData()
{
}

void CParticleEmitterComponent::SetIsActive(bool aIsActive)
{
	if (myIsInited == false)
	{
		aIsActive = false;
		DL_ERROR(eDebugLogType::ResultMessages, "%s", "tried to activate a non inited particle emitter pls fix");
	}
	myIsActive = aIsActive;
	SetShouldRender(aIsActive);
	SetShouldUpdateParticles(aIsActive);
	if (aIsActive)
	{
		myInstance->Start();
	}
	else
	{
		myInstance->Stop();
	}
}

void CParticleEmitterComponent::Release()
{
	CComponent::Release();
}

void CParticleEmitterComponent::SetNewEmittor(long long aID)
{
	myInstance->SetEmitterID(aID);
}

void CParticleEmitterComponent::SetShouldRender(bool aShouldRender)
{
	myInstance->SetShouldRender(aShouldRender);
}

void CParticleEmitterComponent::SetShouldUpdateParticles(bool aShouldUpdate)
{
	myInstance->SetShouldUpdateParticles(aShouldUpdate);
}


void CParticleEmitterComponent::SetEmitterInstance(CParticleEmitterInstance * aInstance)
{
	myInstance = aInstance;
}
