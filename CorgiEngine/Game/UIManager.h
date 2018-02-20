#pragma once
#include "UIObject.h"
#include "Button.h"

class CEngine;

enum class eInputMode 
{
	Mouse,
	Keys
};

class CUIManager
{
public:
	CUIManager();
	~CUIManager();

	void Init(CEngine* aEngine);
	void AddUIObject(CUIObject& aObject);

	void Update();
	void SwitchInputState(eInputMode aInputMode);
	void Destroy();

	void SetIsActive(bool aIsActive);

	void UpdateObjectsWithMouse();
	void UpdateObjectsWithKeys();

private:
	void UpdateInputType();
	void SelectFirstConnectedObject();
	void ClearSelected();


	Corgi::GrowingArray<CUIObject*> myUIObjects;

	CEngine* myGameEngine;

	eInputMode myInputMode;
	eInputMode myPreviousInputMode;

	CUIConnectedObject* myHovoredObject;

	bool myIsActive;
};

