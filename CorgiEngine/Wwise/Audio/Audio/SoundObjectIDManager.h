#pragma once
#include <vector>
#include "..\..\..\Utilities\GrowingArray.h"

class CWwiseManager;

class SoundObjectIDManager
{
public:
	SoundObjectIDManager();
	~SoundObjectIDManager();

	int GetNewID();
	void RemoveID(int anID);
	void Init();
	bool CheckIfIDExists(int anID);
	void UnregisterAllObjects(CWwiseManager* aManager);

	int GetNumberOfRegisteredIDs();
	Corgi::GrowingArray<int>& GetObjectIDs();

private:
	Corgi::GrowingArray<int> myObjectIDs;
	Corgi::GrowingArray<int> myRemovedObjectIDs;
};
