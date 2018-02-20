#pragma once
#include "UIConnectedObject.h"
#include "SliderCursor.h"
#include <functional>

class CSpriteInstance;
class CEngine;


class CSlider : public CUIConnectedObject
{
public:
	CSlider();
	~CSlider();

	void Init(const Vector2f& aPos, int aStartPercent, CEngine* aEngine, const char* aTextPath = "");
	void SetCallBackFunction(std::function<void()> aValueChangedCallBackFunction);
	bool Update(const Corgi::Vector2<float>& aMousePos) override;
	void Destroy() override;

	int GetPercentValue();

	void SelectObject() override;
	void DeSelectObject() override;

	void OnValueChanged();

private:

	void OnPressedLeft() override;
	void OnPressedRight() override;

	void OnConnectedActivation() override;
	void Init(CEngine* aGameEngine) override;

	CEngine* myGameEngine;

	CSliderCursor myCursor;
	CSpriteInstance* myTextSprite;

	std::function<void()> myCallBackFunction;
	bool myHasRecievedCallbackFunction;

	float myPlaySoundTimer;

	int myPreviousValue;

	bool myHasBeenClicked;
};

