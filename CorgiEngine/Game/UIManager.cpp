#include "stdafx.h"
#include "UIManager.h"

#include "InputWrapper.h"



CUIManager::CUIManager()
{
}


CUIManager::~CUIManager()
{
}

void CUIManager::Init(CEngine* aEngine)
{
	myUIObjects.Init(32);

	myGameEngine = aEngine;
	myInputMode = eInputMode::Mouse;
	myHovoredObject = nullptr;

	myIsActive = true;
}

void CUIManager::AddUIObject(CUIObject& aObject)
{
	myUIObjects.Add(&aObject);
}

void CUIManager::Update()
{
	if (myIsActive)
	{
		UpdateInputType();

		myHovoredObject = nullptr;

		if (myInputMode == eInputMode::Mouse) 
		{
			UpdateObjectsWithMouse();
		}
		else 
		{
			UpdateObjectsWithKeys();
		}
	}

}

void CUIManager::ClearSelected() 
{
	for (int i = 0; i < myUIObjects.Size(); i++)
	{
		CUIConnectedObject* connectedObject = dynamic_cast<CUIConnectedObject*>(myUIObjects[i]);

		if (connectedObject)
		{
			connectedObject->DeSelectObject();
		}
	}
}

void CUIManager::SelectFirstConnectedObject()
{
	for (int i = 0; i < myUIObjects.Size(); i++)
	{
		CUIConnectedObject* connectedObject = dynamic_cast<CUIConnectedObject*>(myUIObjects[i]);

		if (connectedObject) 
		{
			connectedObject->SelectObject();
			break;
		}
	}
}

void CUIManager::SwitchInputState(eInputMode aInputMode)
{
	switch (aInputMode)
	{
	case eInputMode::Mouse:
		myInputMode = aInputMode;
		ClearSelected();
		break;

	case eInputMode::Keys:
		myInputMode = aInputMode;

		if (myHovoredObject)
		{
			myHovoredObject->DeSelectObject();
		}

		SelectFirstConnectedObject();
		break;
	default:
		break;
	}
}

void CUIManager::Destroy()
{
	for (int i = 0; i < myUIObjects.Size(); ++i)
	{
		//myUIObjects[i]->SetIsActive(false);
		myUIObjects[i]->Destroy();
	}

	myUIObjects.DeleteAll();
}

void CUIManager::SetIsActive(bool aIsActive)
{
	for (int i = 0; i < myUIObjects.Size(); i++)
	{
		myUIObjects[i]->SetIsActive(aIsActive);
	}

	myIsActive = aIsActive;
}

void CUIManager::UpdateObjectsWithMouse()
{
	Corgi::Vector3<float> mousePos = CCursor::GetInstance().GetPosition();
	Corgi::Vector2<float> cursorPos = { mousePos.x, mousePos.y };

	for (int i = 0; i < myUIObjects.Size(); i++)
	{
		CButton* button = dynamic_cast<CButton*>(myUIObjects[i]);

		if (button)
		{
			if (button->UpdateButtonWithMouse(cursorPos))
			{
				myHovoredObject = button;
			}
		}
		else 
		{
			if (myUIObjects[i]->Update(cursorPos)) 
			{
				myHovoredObject = dynamic_cast<CUIConnectedObject*>(myUIObjects[i]);
			}
		}
	}
}

void CUIManager::UpdateObjectsWithKeys()
{
	for (int i = 0; i < myUIObjects.Size(); i++)
	{
		CUIConnectedObject* connectedObject = dynamic_cast<CUIConnectedObject*>(myUIObjects[i]);
		if (connectedObject) 
		{
			connectedObject->UpdateConnectedObject();
		}

	}
}

void CUIManager::UpdateInputType()
{
	if (CInputWrapper::GetInstance().HasMouseMoved())
	{
		if (myPreviousInputMode == eInputMode::Keys)
		{
			SwitchInputState(eInputMode::Mouse);
		}
	}
	else if (InputManager::GetInstance().CheckKeyPressed(KeyCode::Down) ||
		InputManager::GetInstance().CheckKeyPressed(KeyCode::Up) ||
		InputManager::GetInstance().CheckKeyPressed(KeyCode::Left) ||
		InputManager::GetInstance().CheckKeyPressed(KeyCode::Right))
	{
		if (myPreviousInputMode == eInputMode::Mouse)
		{
			SwitchInputState(eInputMode::Keys);
		}
	}

	myPreviousInputMode = myInputMode;
}
