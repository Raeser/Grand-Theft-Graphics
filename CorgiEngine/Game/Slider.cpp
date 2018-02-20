#include "stdafx.h"
#include "Slider.h"
#include "WwiseWrapper.h"


CSlider::CSlider()
{
	myGameEngine = nullptr;
	myHasRecievedCallbackFunction = false;
}

CSlider::~CSlider()
{
}

void CSlider::OnPressedLeft()
{
	myCursor.MoveCursorLeft();
	OnValueChanged();
}

void CSlider::OnPressedRight()
{
	myCursor.MoveCursorRight();
	OnValueChanged();
}

void CSlider::OnConnectedActivation()
{
}

void CSlider::Init(CEngine * aGameEngine)
{
	myGameEngine = aGameEngine;
}

void CSlider::Init(const Vector2f & aPos, int aStartPercent, CEngine* aEngine, const char* aTextPath)
{
	Init(aEngine);
	InitConnectedObject();

	assert(myGameEngine != nullptr && "GameEngine pointer was null!");

	myPosition.x = aPos.x;
	myPosition.y = aPos.y;

	long long myTextIndex = myGameEngine->GetSpriteLoader().CreateSprite(aTextPath);
	myTextSprite = myGameEngine->GetSpriteLoader().CreateSpriteInstance(myTextIndex);
	myTextSprite->SetPosition0To1({ myPosition.x, myPosition.y - 0.07f, 0.f });
	myTextSprite->SetShouldRender(true);

	long long mySliderIndex = myGameEngine->GetSpriteLoader().CreateSprite("../Assets/Sprites/Menus/slider.dds");
	mySpriteInstance = myGameEngine->GetSpriteLoader().CreateSpriteInstance(mySliderIndex);
	mySpriteInstance->SetPosition0To1(myPosition);
	mySpriteInstance->SetShouldRender(true);


	myCursor.Init(aPos, myGameEngine, 0.12f);
	myCursor.SetPositionThroughPercent(aStartPercent);

	myPreviousValue = GetPercentValue();


	myPlaySoundTimer = 0.f;
}

void CSlider::SetCallBackFunction(std::function<void()> aValueChangedCallBackFunction)
{
	myCallBackFunction = aValueChangedCallBackFunction;
	myHasRecievedCallbackFunction = true;
}

bool CSlider::Update(const Corgi::Vector2<float>& aMousePos)
{
	bool washovored = myCursor.Update(aMousePos);
	
	if (myPreviousValue != GetPercentValue())
	{
		OnValueChanged();
	}

	myPreviousValue = GetPercentValue();

	return washovored;
}

void CSlider::Destroy()
{
	
	mySpriteInstance->Remove();
	
	mySpriteInstance = nullptr;

	myTextSprite->Remove();
	myTextSprite = nullptr;

	myCursor.Destroy();
}

int CSlider::GetPercentValue()
{
	float totalDist = (myCursor.myMaxPosX - myCursor.myMinPosX);
	int percent = (int)(((myCursor.myPosition.x - myCursor.myMinPosX) / totalDist) * 100.f);

	return percent;
}

void CSlider::SelectObject()
{
	myIsSelected = true;
	myWasSelectedThisFrame = true;

	myCursor.SwitchCursorSprite(myCursor.myHighlightSpriteIndex);
}

void CSlider::DeSelectObject()
{
	myIsSelected = false;
	myCursor.SwitchCursorSprite(myCursor.myNormalSpriteIndex);
}

void CSlider::OnValueChanged()
{
	if (myHasRecievedCallbackFunction) 
	{
		myCallBackFunction();
	}
}
