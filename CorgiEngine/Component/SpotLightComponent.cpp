#include "stdafx.h"
#include "SpotLightComponent.h"
#include "../Engine/SpotLightInstance.h"
#include "Entity.h"



CSpotLightComponent::CSpotLightComponent()
{
}


CSpotLightComponent::~CSpotLightComponent()
{
}

void CSpotLightComponent::SetLightData(Vector4f aPosition, Vector4f aDirection, long long aID)
{
	if (myLightInstance != nullptr)
	{
		myLightInstance->Init(aPosition, aDirection);
		myLightInstance->SetSpotLight(aID);
	}
	else
	{
		DL_ERROR(eDebugLogType::Resource, "%s", "you need to set your SpotLightinstance to the component before setting the data!");
	}
}
void CSpotLightComponent::Init(CEntity* aParent, long long aSpotLightTypeID, Vector4f& aDirection, bool aShouldShine)
{
	CComponent::Init();
	SetParent(aParent);
	myIsInited = true;

	SetNewSpotLight(aSpotLightTypeID);
	SetDirection(aDirection);
	ToggleLight(aShouldShine);
	myComponentTag = eComponentTag::SpotLight;
}

void CSpotLightComponent::Update()
{
	assert(myIsInited && "I SHOULD USE INIT");
	if (myIsActive == false)
	{
		ToggleLight(false);
	}
	myLightInstance->SetOrientation(myOrientation);
}

void CSpotLightComponent::Release()
{
	CComponent::Release();
}

void CSpotLightComponent::FetchData()
{
	myIsActive = myParent->GetActiveStatus();
	if (myIsActive == false)
	{
		ToggleLight(false);
	}
	myOrientation = myParent->GetTransform().GetOrientationMatrix();
}

void CSpotLightComponent::SendData()
{
}

void CSpotLightComponent::SetSpotLightInstance(CSpotLightInstance * aInstance)
{
	myLightInstance = aInstance;
}

void CSpotLightComponent::SetNewSpotLight(long long aID)
{
	myLightInstance->SetSpotLight(aID);
}

void CSpotLightComponent::ToggleLight(bool aShouldShine)
{
	myLightInstance->SetIsOn(aShouldShine);
}

void CSpotLightComponent::SetDirection(Vector4f& aDirection)
{
	myLightInstance->SetDirection(aDirection);
}

void CSpotLightComponent::SetIsActive(bool isActive)
{
	myIsActive = isActive;
	myLightInstance->SetIsOn(isActive);
}

