#include "stdafx.h"
#include "SoundComponent.h"
#include "..\Game\WwiseWrapper.h"
#include "..\Game\GameSettings.h"


CSoundComponent::CSoundComponent() 
{
	myWwiseIndex = -1;
}


CSoundComponent::~CSoundComponent()
{
}


void CSoundComponent::Init(CEntity* aParent)
{
	assert(aParent != nullptr && "HECKing parent was null");
	SetParent(aParent);
	CComponent::Init();
	myComponentTag = eComponentTag::Sound;

	myHasBeenInited = true;

	myWwiseIndex = CWwiseWrapper::GetInstance().RegisterObjectAtFreeID();
	CWwiseWrapper::GetInstance().SetRTPC("Music_Volume", myWwiseIndex, CGameSettings::GetInstance().GetMusicVolume());
	CWwiseWrapper::GetInstance().SetRTPC("SFX_Volume", myWwiseIndex, CGameSettings::GetInstance().GetSFXVolume());
}

void CSoundComponent::Update()
{
	assert(myHasBeenInited && "I SHOULD USE INIT");
	if (myHasBeenInited)
	{
		Corgi::Vector3<float> parentPos = myParent->GetTransform().GetPosition();
		CWwiseWrapper::GetInstance().SetPosition(myWwiseIndex, parentPos);
	}
}

void CSoundComponent::FetchData()
{
}

void CSoundComponent::SendData()
{
}

void CSoundComponent::Release()
{
	if (myIsInited)
	{
		CWwiseWrapper::GetInstance().UnRegisterObjectID(myObjectIndex);
		myObjectIndex = -1;
	}
	CComponent::Release();
}

void CSoundComponent::SetRTPC(const char* aRTPC, int aValue)
{
	CWwiseWrapper::GetInstance().SetRTPC(aRTPC, myWwiseIndex, aValue);
}

unsigned long CSoundComponent::PostEvent(const char * aEventName)
{
	if (myHasBeenInited)
	{
		return CWwiseWrapper::GetInstance().PostEvent(aEventName, myWwiseIndex);
	}

	return 0;
}
