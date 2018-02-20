#include "stdafx.h"
#include "..\Engine\Sprite.h"


CButton::CButton() 
{
}


CButton::~CButton()
{
}

void CButton::Init(CEngine * aEngine)
{
	assert(aEngine != nullptr && "Engine was nullptr!");

	myGameEngine = aEngine;
	myState = eButtonState::Normal;
	myPreviousState = myState;
}

void CButton::InitButton(std::function<void()> aFunction, const Corgi::Vector3<float>& aPos, const SButtonSprites & aButtonSprites, float aWidth, float aHeight)
{
	InitConnectedObject();
	myButtonSprites = aButtonSprites;
	myFunction = aFunction;
	myPosition = aPos;

	CreateSpriteInstance(myButtonSprites.myNormalSprite.c_str());
	//mySpriteInstance->SetPosition0To1(myPosition);

	myHovoredSpriteIndex = myGameEngine->GetSpriteLoader().CreateSprite(aButtonSprites.myHovoredSprite.c_str());

	myAABB.Init(aWidth, aHeight, { myPosition.x, myPosition.y });

	//long long dotIndex = myGameEngine->GetSpriteLoader().CreateSprite("../Assets/Sprites/dot.dds");
	//myDotSprite = myGameEngine->GetSpriteLoader().CreateSpriteInstance(dotIndex);
	//myDotSprite->SetPosition0To1(aPos);
}

bool CButton::Update(const Corgi::Vector2<float>& aMousePos)
{
	//mySpriteInstance->SetPosition0To1(myPosition);
	UpdateButtonWithMouse(aMousePos);

	return true;
}

bool CButton::UpdateButtonWithMouse(const Vector2f & aMousePos)
{
	bool isColliding = false;

	if (myAABB.IsCollidingWithPoint(aMousePos)) 
	{
		isColliding = true;
		myState = eButtonState::Hovered;

		if (InputManager::GetInstance().CheckKeyPressed(KeyCode::MouseLeft)) 
		{
			OnRelease();
		}

	}
	else 
	{
		myState = eButtonState::Normal;
	}

	if (myPreviousState != myState)
	{
		SwitchState();
	}

	myPreviousState = myState;

	return isColliding;
}

void CButton::SetIsActive(const bool& aIsActive)
{
	myIsActive = aIsActive;
	mySpriteInstance->SetShouldRender(aIsActive);
}

void CButton::SelectObject()
{
	myState = eButtonState::Hovered;
	myIsSelected = true;
	myWasSelectedThisFrame = true;
	SwitchState();
}

void CButton::DeSelectObject()
{
	myState = eButtonState::Normal;
	myIsSelected = false;
	SwitchState();
}

eButtonState CButton::GetState()
{
	return myState;
}

void CButton::Destroy()
{
	if (mySpriteInstance != nullptr) 
	{
		mySpriteInstance->SetShouldRender(false);
		mySpriteInstance->Remove();
		mySpriteInstance = nullptr;
	}
}

void CButton::SwitchState()
{
	switch (myState)
	{
	case eButtonState::Hovered:
		mySpriteInstance->SetSprite(myHovoredSpriteIndex);
		CWwiseWrapper::GetInstance().PostNon3DEvent("Play_Button_Hovored");
		break;
	case eButtonState::Clicked:
		break;
	case eButtonState::Released:
		break;
	case eButtonState::Normal:
		mySpriteInstance->SetSprite(mySpriteIndex);
		break;
	default:
		break;
	}

}

void CButton::SetState(eButtonState aState)
{
	myState = aState;
}

void CButton::OnConnectedActivation()
{
	OnRelease();
}

void CButton::OnRelease()
{
	CWwiseWrapper::GetInstance().PostNon3DEvent("Play_Button_Click");
	myState = eButtonState::Normal;
	myFunction();
}
