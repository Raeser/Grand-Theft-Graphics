#include "stdafx.h"
#include "LifeTimeComponent.h"


CLifeTimeComponent::CLifeTimeComponent()
{
	myTime = 0;
}


CLifeTimeComponent::~CLifeTimeComponent()
{
}

void CLifeTimeComponent::Init(float aLifeTime)
{
	CComponent::Init();
	myComponentTag = eComponentTag::Timer;
	myTime = aLifeTime;
}

void CLifeTimeComponent::Update()
{
	myTime -= Timer::GetInstance().GetDeltaTime();
}

void CLifeTimeComponent::FetchData()
{
}

void CLifeTimeComponent::SendData()
{
	if (myTime <= 0)
	{
		myShouldRelease = true;
		myParent->Release();
	}
}

void CLifeTimeComponent::Release()
{
	CComponent::Release();
}

float CLifeTimeComponent::GetTime() const
{
	return myTime;
}
