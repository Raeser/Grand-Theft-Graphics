#include "stdafx.h"
#include "..\Audio\Audio\AudioInterface.h"

CWwiseWrapper & CWwiseWrapper::GetInstance()
{
	static CWwiseWrapper wwise(*CAudioInterface::GetInstance());
	return wwise;
}

void CWwiseWrapper::PlaySoundAtPosition(const char * aEvent, const Corgi::Vector3<float>& aPosition)
{
	SetPosition(myWwiseObjects[myCurrentIndex], aPosition);
	PostEvent(aEvent, myWwiseObjects[myCurrentIndex]);

	myCurrentIndex++;

	if (myCurrentIndex == myWwiseObjects.Size()) 
	{
		myCurrentIndex = 0;
	}
}

CWwiseWrapper::CWwiseWrapper(CAudioInterface& aAudio) 
	: myAudioInterface(aAudio)
{
}

CWwiseWrapper::~CWwiseWrapper()
{
}

void ErrorCallback(const char* aError)
{
	OutputDebugString(aError);
}

void CWwiseWrapper::Init()
{
	myAudioInterface.CreateInstance();
	myAudioInterface.GetInstance()->Init("../Assets/Audio/Init.bnk");
	myAudioInterface.GetInstance()->SetErrorCallBack(ErrorCallback);

	if (myAudioInterface.GetInstance()->LoadBank("../Assets/Audio/Main.bnk"))
	{
		DL_SUCCESS(eDebugLogType::Resource, "%s", "AudioInterface Successfully loaded Main.bnk");
	}
	else
	{
		DL_ERROR(eDebugLogType::Resource, "%s", "AudioInterface Failed to load Main.bnk");
	}

	int id = myAudioInterface.GetInstance()->RegisterObjectAtFreeID();
	id;

	myWwiseObjects.Init(8);

	for (int i = 0; i < 4; i++)
	{
		myWwiseObjects.Add(myAudioInterface.GetInstance()->RegisterObjectAtFreeID());
	}

	myCurrentIndex = 0;
}

void CWwiseWrapper::Destroy()
{
	myAudioInterface.GetInstance()->UnLoadBank("../Assets/Audio/Main.bnk"); //Unload bnk-file and destroy AudioInterface
	myAudioInterface.GetInstance()->Destroy();
}

unsigned long CWwiseWrapper::PostEvent(const char * aEvent, int aObjectIndex)
{
	return myAudioInterface.GetInstance()->PostEvent(aEvent, aObjectIndex);
}

void CWwiseWrapper::PostNon3DEvent(const char * aEvent)
{
	myAudioInterface.GetInstance()->PostEvent(aEvent, 1);
}

void CWwiseWrapper::PostEventToAllIDs(const char * aEvent)
{
	myAudioInterface.GetInstance()->PostEventToAllIDs(aEvent);
}

int CWwiseWrapper::RegisterObjectAtFreeID()
{
	return myAudioInterface.GetInstance()->RegisterObjectAtFreeID();
}

void CWwiseWrapper::UnRegisterObjectID(int aID)
{
	myAudioInterface.GetInstance()->UnRegisterObjectID(aID);
}

void CWwiseWrapper::SetPosition(int aID, const Corgi::Vector3<float>& aPosition)
{
	myAudioInterface.GetInstance()->SetPosition(aPosition.x, aPosition.y, aPosition.z, aID);
}

void CWwiseWrapper::SetRTPC(const char* aRTPC, int aID, int aValue)
{
	myAudioInterface.GetInstance()->SetRTPC(aRTPC, aValue, aID);
}

void CWwiseWrapper::SetRTPCForAllObjects(const char* aRTPC, int aValue)
{
	myAudioInterface.GetInstance()->SetRTPCForAllObjects(aRTPC, aValue);
}
