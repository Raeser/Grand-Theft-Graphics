#include "stdafx.h"
#include "UIConnectedObject.h"


CUIConnectedObject::CUIConnectedObject()
{
	myIsSelected = false;
}


CUIConnectedObject::~CUIConnectedObject()
{
}

void CUIConnectedObject::InitConnectedObject()
{
	myWasSelectedThisFrame = false;
	myConnectedObjects.Init(4);

	for (int i = 0; i < 4; ++i)
	{
		myConnectedObjects.Add(nullptr);
	}

}

void CUIConnectedObject::ConnectWithObject(CUIConnectedObject* aObject, eConnectedDirection aDirection, bool aFirstConnection)
{
	assert(aObject != nullptr && "Connected object was nullptr!");
	//assert(myConnectedObjects[aDirection] == nullptr && "Already connected!");

	myConnectedObjects[aDirection] = aObject;

	bool firstconnection = aFirstConnection;
	
	if (firstconnection)
	{
		firstconnection = false;
		switch (aDirection)
		{
		case Up:
			aObject->ConnectWithObject(this, eConnectedDirection::Down, firstconnection);
				break;
		case Down:
			aObject->ConnectWithObject(this, eConnectedDirection::Up, firstconnection);
			break;
		case Left:
			aObject->ConnectWithObject(this, eConnectedDirection::Right, firstconnection);
			break;
		case Right:
			aObject->ConnectWithObject(this, eConnectedDirection::Left, firstconnection);
			break;
		default:
			break;
		}
	}

}

void CUIConnectedObject::UpdateConnectedObject()
{
	if(myIsSelected && myIsActive && myWasSelectedThisFrame != true)
	{
		if (InputManager::GetInstance().CheckKeyPressed(KeyCode::Up))
		{
			OnPressedUp();
		}
		else if (InputManager::GetInstance().CheckKeyPressed(KeyCode::Down))
		{
			OnPressedDown();
		}
		else if (InputManager::GetInstance().CheckKeyPressed(KeyCode::Left))
		{
			OnPressedLeft();
		}
		else if (InputManager::GetInstance().CheckKeyPressed(KeyCode::Right))
		{
			OnPressedRight();
		}
		else if (InputManager::GetInstance().CheckKeyPressed(KeyCode::Enter))
		{
			OnConnectedActivation();
		}
	}

	myWasSelectedThisFrame = false;
}

void CUIConnectedObject::OnPressedUp()
{
	if (myConnectedObjects[eConnectedDirection::Up])
	{
		this->DeSelectObject();
		myConnectedObjects[eConnectedDirection::Up]->SelectObject();
	}
}

void CUIConnectedObject::OnPressedDown()
{
	if (myConnectedObjects[eConnectedDirection::Down])
	{
		this->DeSelectObject();
		myConnectedObjects[eConnectedDirection::Down]->SelectObject();
	}
}

void CUIConnectedObject::OnPressedLeft()
{
	if (myConnectedObjects[eConnectedDirection::Left]) 
	{
		this->DeSelectObject();
		myConnectedObjects[eConnectedDirection::Left]->SelectObject();
	}
}

void CUIConnectedObject::OnPressedRight()
{
	if (myConnectedObjects[eConnectedDirection::Right])
	{
		this->DeSelectObject();
		myConnectedObjects[eConnectedDirection::Right]->SelectObject();
	}
}
