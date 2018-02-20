#include "AudioInterface.h"
#include "WwiseManager.h"


CAudioInterface* CAudioInterface::myInstance = nullptr;
CAudioInterface::CAudioInterface()
	:myWwiseManager(nullptr)
{
}

CAudioInterface::~CAudioInterface()
{
	delete myWwiseManager;
}

void CAudioInterface::Destroy()
{
	if (myInstance != nullptr)
	{
		myInstance->myIDManager.UnregisterAllObjects(myInstance->myWwiseManager);
		delete myInstance;
		myInstance = nullptr;
	}
}

bool CAudioInterface::IsInstanced() 
{
	if (myInstance == nullptr) 
	{
		return false;
	}

	return true;
}

bool CAudioInterface::Init(const char* aInitBank)
{
	myIDManager.Init();
	myWwiseManager = new CWwiseManager();
	return myWwiseManager->Init(aInitBank);
}

void CAudioInterface::Update()
{
	if (myWwiseManager)
	{
		myWwiseManager->Update();
	}
		
}

bool CAudioInterface::LoadBank(const char* aBankPath)
{
	if (myWwiseManager)
	{
		return myWwiseManager->LoadBank(aBankPath);
	}
	return false;
}

void CAudioInterface::UnLoadBank(const char* aBankPath)
{
	if (myWwiseManager)
	{
		return myWwiseManager->UnLoadBank(aBankPath);
	}
}

unsigned long CAudioInterface::PostEvent(const char* aEvent, int anObjectID)
{
	if (myWwiseManager)
	{
		return myWwiseManager->PostEvent(aEvent, anObjectID);
	}
	return 0;
}

unsigned long CAudioInterface::PostEventToAllIDs(const char * aEvent)
{
	if (myWwiseManager)
	{
		for (int i = 0; i < myIDManager.GetNumberOfRegisteredIDs(); ++i)
		{
			myWwiseManager->PostEvent(aEvent, myIDManager.GetObjectIDs()[i]);
		}
	}

	return 0;
}

void CAudioInterface::SetRTPCForAllObjects(const char* aRTPC, int aValue)
{
	if (myWwiseManager)
	{
		for (int i = 0; i < myIDManager.GetNumberOfRegisteredIDs(); ++i)
		{
			myWwiseManager->SetRTPC(aRTPC, aValue, myIDManager.GetObjectIDs()[i]);//aEvent, myIDManager.GetObjectIDs()[i]);
		}
	}
}

void CAudioInterface::SetErrorCallBack(callback_function aErrorCallback)
{
	if (myWwiseManager)
	{
		return myWwiseManager->SetErrorCallBack(aErrorCallback);
	}
}

void CAudioInterface::SetRTPC(const char* aRTPC, int aValue, int anObjectID)
{
	if (myWwiseManager)
	{
		myWwiseManager->SetRTPC(aRTPC, aValue, anObjectID);
	}
}

void CAudioInterface::SetPosition(float aX, float aY, float aZ, int aObjectID, float aDirectionX, float aDirectionY, float aDirectionZ)
{
	aDirectionX; aDirectionY; aDirectionZ;
	if (myWwiseManager)
	{
		myWwiseManager->SetPosition(aX, aY, aZ, aObjectID);
	}
}

void CAudioInterface::SetListenerPosition(float aX, float aY, float aZ, float aDirectionX, float aDirectionY, float aDirectionZ, float aUpX, float aUpY, float aUpZ)
{
	if (myWwiseManager)
	{
		myWwiseManager->SetListenerPosition(aX, aY, aZ, aDirectionX, aDirectionY, aDirectionZ, aUpX, aUpY, aUpZ);
	}
}

int CAudioInterface::RegisterObjectAtFreeID()
{
	int ID = myIDManager.GetNewID();
	if (myWwiseManager)
	{
		myWwiseManager->RegisterObject(ID);
	}
	return ID;
}

void CAudioInterface::UnRegisterObjectID(int anObjectID)
{
	myIDManager.RemoveID(anObjectID);
}