#include "stdafx.h"
#include "PointLightComponent.h"
#include "../Engine/PointLightInstance.h"
#include "Entity.h"


CPointLightComponent::CPointLightComponent()
{
}


CPointLightComponent::~CPointLightComponent()
{
}
void CPointLightComponent::Init(const Vector3f& aPosition)
{
	myIsInited = true;
	CComponent::Init();
	myLightInstance->Init(aPosition);
	myOffset = {0,0,0};

	myComponentTag = eComponentTag::PointLight;
}

void CPointLightComponent::ToggleLight(bool aShouldShine)
{
	myLightInstance->SetIsOn(aShouldShine);
}

void CPointLightComponent::SetOffset(Vector3f aOffset)
{
	myOffset = aOffset;
}

void CPointLightComponent::SetIsActive(bool isActive)
{
	myIsActive = isActive;
	myLightInstance->SetIsOn(isActive);
}

void CPointLightComponent::Update()
{
	myLightInstance->SetPosition(myParent->GetTransform().GetPosition());
}

void CPointLightComponent::Release()
{
	//delete myLightInstance;
	//myLightInstance = nullptr;
	CComponent::Release();
}

void CPointLightComponent::FetchData()
{
}
void CPointLightComponent::SendData()
{
}

void CPointLightComponent::SetPointLightInstance(CPointLightInstance* aInstance)
{
	myLightInstance = aInstance;
}

void CPointLightComponent::SetNewPointLight(long long aID)
{
	myLightInstance->SetPointLight(aID);
}
