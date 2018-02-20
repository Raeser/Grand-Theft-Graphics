#pragma once
#include "Component.h"

class CSoundComponent 
	: public CComponent
{
public:
	CSoundComponent();
	~CSoundComponent();

	//Returns a Playing ID
	unsigned long PostEvent(const char* aEventName);
	void Init(CEntity* aParent);

	void Update() override;
	void FetchData() override;
	void SendData() override;

	void Release() override;

	void SetRTPC(const char* aRTPC, int aValue);
private:
	int myObjectIndex;
	Matrix44 myOrientation;

	int myWwiseIndex;
	bool myHasBeenInited;

};

