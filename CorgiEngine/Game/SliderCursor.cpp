#include "stdafx.h"
#include "SliderCursor.h"


void CSliderCursor::Init(const Corgi::Vector2<float>& aPosition, CEngine* aGameEngine, float aHalfWidthFromCentre)
{
	mySliderPosition = aPosition;
	myGameEngine = aGameEngine;

	myMaxPosX = aPosition.x + aHalfWidthFromCentre;
	myMinPosX = aPosition.x - aHalfWidthFromCentre;

	myAABB.Init(0.1f, 0.1f, aPosition);

	myPosition.x = aPosition.x;
	myPosition.y = aPosition.y - 0.01f;

	myNormalSpriteIndex = myGameEngine->GetSpriteLoader().CreateSprite("../Assets/Sprites/Menus/sliderCursor.dds");
	myHighlightSpriteIndex = myGameEngine->GetSpriteLoader().CreateSprite("../Assets/Sprites/Menus/sliderCursorHighlight.dds");

	mySpriteInstance = myGameEngine->GetSpriteLoader().CreateSpriteInstance(myNormalSpriteIndex);
	mySpriteInstance->SetPosition0To1(myPosition);
	mySpriteInstance->SetShouldRender(true);

	myIsHeld = false;
	myCursorWasClickedLastFrame = false;

	float totalDist = (myMaxPosX - myMinPosX);
	myMovementValue = totalDist / 10.0f;
}

void CSliderCursor::SwitchCursorSprite(long long aIndex)
{
	assert(mySpriteInstance != nullptr && "sprite was nullptr");

	if (aIndex == myHighlightSpriteIndex && mySpriteInstance->GetSpriteID() != myHighlightSpriteIndex) 
	{
		CWwiseWrapper::GetInstance().PostNon3DEvent("Play_Button_Hovored");
	}

	mySpriteInstance->SetSprite(aIndex);
}

bool CSliderCursor::Update(const Corgi::Vector2<float>& aMousePos)
{
	bool returnValue = false;

	if (myCursorWasClickedLastFrame && InputManager::GetInstance().CheckKeyDown(KeyCode::MouseLeft))
	{
		myIsHeld = true;
		myCursorWasClickedLastFrame = false;
	}
	else if (myIsHeld == true && InputManager::GetInstance().CheckKeyUp(KeyCode::MouseLeft))
	{
		myIsHeld = false;
	}


	if (myAABB.IsCollidingWithPoint(aMousePos)) 
	{
		returnValue = true;
	}


	if (myIsHeld)
	{
		SetCursorPos(aMousePos);
	}
	else if (returnValue)
	{
		if (myIsHeld == false && InputManager::GetInstance().CheckKeyPressed(KeyCode::MouseLeft)) 
		{
			myCursorWasClickedLastFrame = true;
		}

		SwitchCursorSprite(myHighlightSpriteIndex);
	}
	else 
	{
		SwitchCursorSprite(myNormalSpriteIndex);
	}

	return returnValue;
}

void CSliderCursor::SetPositionThroughPercent(int aPercent)
{
	SetCursorPos({GetXPosByPercent(aPercent) ,myPosition.y});
}

float CSliderCursor::GetXPosByPercent(int aPercent)
{
	float x = myMinPosX + ((myMaxPosX - myMinPosX) * ((float)aPercent / 100.f));

	return x;
}

void CSliderCursor::SetCursorPos(const Vector2f & aPos)
{
	bool shouldMove = (aPos.x != myPosition.x);

	myPosition.x = aPos.x;

	if (myPosition.x > myMaxPosX)
	{
		myPosition.x = myMaxPosX;
	}
	else if (myPosition.x < myMinPosX)
	{
		myPosition.x = myMinPosX;
	}
	else if(shouldMove)
	{
		CWwiseWrapper::GetInstance().PostNon3DEvent("Play_Slider");
	}

	mySpriteInstance->SetPosition0To1(myPosition);
	myAABB.UpdatePosition({ myPosition.x, myPosition.y });
}

void CSliderCursor::MoveCursorLeft()
{
	SetCursorPos({ myPosition.x - myMovementValue, myPosition.y });
}

void CSliderCursor::MoveCursorRight()
{
	SetCursorPos({ myPosition.x + myMovementValue, myPosition.y });
}

void CSliderCursor::Destroy()
{
	if (mySpriteInstance != nullptr)
	{
		mySpriteInstance->Remove();
	}

	mySpriteInstance = nullptr;
}