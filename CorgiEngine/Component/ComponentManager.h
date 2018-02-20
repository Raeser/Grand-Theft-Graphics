#pragma once
#include "..\Utilities\GrowingArray.h"

class CComponent;

class CComponentManager
{
public:
	CComponentManager();
	~CComponentManager();

	void Init();
	void AddComponent(CComponent* aComponent);

	void Lock();

	void UnlockAndFillBuffer();


	void FetchDataPass();
	void Update();
	void SendDataPass();

private:
	Corgi::GrowingArray<CComponent*> myComponents;
	Corgi::GrowingArray<CComponent*> myComponentsToAddNextLoop;
	bool myIsLocked; 
};

