#include "stdafx.h"
#include "Component.h"

CComponent::CComponent()
{
	myParent = nullptr;
	myIsActive = false;
	myShouldRelease = false;
	myComponentTag = eComponentTag::Default;
}


CComponent::~CComponent()
{
}

void CComponent::Init()
{
	myIsActive = false;
	myShouldRelease = false;
	myComponentTag = eComponentTag::Default;
}


bool CComponent::IsActive()
{
	return myIsActive;
}

void CComponent::Release()
{
	myShouldRelease = true;
}

void CComponent::SetParent(CEntity * aParent)
{
	myParent = aParent;
	myIsActive = true;
}

void CComponent::DeInit()
{
	myIsInited = false;
	myShouldRelease = false;
	SetIsActive(false);
}

void CComponent::SetIsActive(bool isActive)
{
	myIsActive = isActive;
}

eComponentTag CComponent::GetComponentTag() const
{
	return myComponentTag;
}

bool CComponent::GetIsActive()
{
	return myIsActive;
}

bool CComponent::GetIsParentActive()
{
	if (myParent == nullptr)
	{
		DL_WARNING(eDebugLogType::Resource, "%s%s", "A components parent is nullptr. This is unexpected behavior, unless",
			" the component is a sprite belonging to the objective marker manager. Double check if you have to call this function!!");
		return false;
	}
	return myParent->GetActiveStatus();
}

bool CComponent::ShouldRelease()
{
	return myShouldRelease;
}


