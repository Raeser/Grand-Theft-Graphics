#pragma once
#include "UIConnectedObject.h"
#include "AABB.h"

class CSpriteInstance;
class CEngine;

class CSliderCursor
{
public:

	void SwitchCursorSprite(long long aIndex);
	void MoveCursorLeft();
	void MoveCursorRight();
	void SetCursorPos(const Vector2f& aPos);
	void Destroy();
	void Init(const Corgi::Vector2<float>& aPosition, CEngine* aGameEngine, float aHalfWidthFromCentre);
	bool Update(const Corgi::Vector2<float>& aMousePos);
	void SetPositionThroughPercent(int aPercent);


private:
	float CSliderCursor::GetXPosByPercent(int aPercent);
	friend class CSlider;
	Corgi::Vector3<float> myPosition;
	Corgi::Vector2<float> mySliderPosition;

	CSpriteInstance* mySpriteInstance;

	long long myNormalSpriteIndex;
	long long myHighlightSpriteIndex;

	float myMaxPosX;
	float myMinPosX;

	float myMovementValue;

	bool myIsHeld;
	bool myCursorWasClickedLastFrame;

	CAABB myAABB;
	CEngine* myGameEngine;
};