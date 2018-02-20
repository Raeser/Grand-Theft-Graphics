#pragma once
#include "UIObject.h"

enum eConnectedDirection
{
	Up,
	Down,
	Left,
	Right
};

class CUIConnectedObject 
	: public CUIObject
{
public:

	CUIConnectedObject();
	~CUIConnectedObject();

	virtual void InitConnectedObject();
	virtual void ConnectWithObject(CUIConnectedObject* aObject, eConnectedDirection aDirection, bool aFirstConnection = true);

	virtual void UpdateConnectedObject();

	virtual void SelectObject() { myIsSelected = true; myWasSelectedThisFrame = true; };
	virtual void DeSelectObject() { myIsSelected = false; };

	virtual void OnConnectedActivation() = 0;
	

protected:

	virtual void OnPressedUp();
	virtual void OnPressedDown();
	virtual void OnPressedLeft();
	virtual void OnPressedRight();

	Corgi::GrowingArray<CUIConnectedObject*> myConnectedObjects;
	bool myIsSelected;
	bool myWasSelectedThisFrame;

private:
	
};

