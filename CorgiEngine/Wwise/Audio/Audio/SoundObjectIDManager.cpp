#include "SoundObjectIDManager.h"
#include "WwiseManager.h"

#define SOUND_OBJECTS_STARTSIZE 4000
#define REMOVED_SOUND_OBJECT_STARTSIZE 2000

SoundObjectIDManager::SoundObjectIDManager()
{
}


SoundObjectIDManager::~SoundObjectIDManager()
{
}


int SoundObjectIDManager::GetNewID()
{
	int ID = 0;

	if (myRemovedObjectIDs.Size() > 0)
	{
		ID = myRemovedObjectIDs.GetLast();
		myRemovedObjectIDs.RemoveCyclicAtIndex(myRemovedObjectIDs.Size() - 1);
	}
	else
	{
		ID = (int)myObjectIDs.Size() + 1;
	}

	myObjectIDs.Add(ID);
	return ID;
}

void SoundObjectIDManager::RemoveID(int anID)
{
	for (int i=0; i< myObjectIDs.Size(); i++)
	{
		if (myObjectIDs[i] == anID)
		{
			myObjectIDs.RemoveVectoralAtIndex(i);
			myRemovedObjectIDs.Add(anID);
			break;
		}
	}
}

void SoundObjectIDManager::Init()
{
	myObjectIDs.Init(SOUND_OBJECTS_STARTSIZE);
	myRemovedObjectIDs.Init(REMOVED_SOUND_OBJECT_STARTSIZE);
}

bool SoundObjectIDManager::CheckIfIDExists(int anID)
{
	for (int i = 0; i < myObjectIDs.Size(); i++)
	{
		if (myObjectIDs[i] == anID)
		{
			return true;
		}
	}
	return false;
}

void SoundObjectIDManager::UnregisterAllObjects(CWwiseManager* aManager)
{
	for (unsigned short i = 0; i < myRemovedObjectIDs.Size(); ++i)
	{
		aManager->UnRegisterObject(myRemovedObjectIDs[i]);
	}

	for (unsigned short i = 0; i < myObjectIDs.Size(); ++i)
	{
		aManager->UnRegisterObject(myObjectIDs[i]);
	}
}

int SoundObjectIDManager::GetNumberOfRegisteredIDs()
{
	return myObjectIDs.Size();
}

Corgi::GrowingArray<int>& SoundObjectIDManager::GetObjectIDs()
{
	return myObjectIDs;
}
