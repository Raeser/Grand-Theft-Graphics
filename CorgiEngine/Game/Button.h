#pragma once
#include <functional>
#include "AABB.h"
#include "UIObject.h"
#include "UIConnectedObject.h"

class CSpriteInstance;
class CEngine;

enum class eButtonState 
{
	Hovered,
	Clicked,
	Released,
	Normal
};

struct SButtonSprites
{
	std::string myNormalSprite;
	std::string myHovoredSprite;
};

class CButton : public CUIConnectedObject
{
public:
	CButton();
	~CButton();

	//Inits the UI part of the button
	void Init(CEngine* aEngine) override;
	void InitButton(std::function<void()> aFunction, const Corgi::Vector3<float>& aPos, const SButtonSprites& aButtonSprites, float aWidth, float aHeight);
	
	bool Update(const Corgi::Vector2<float>& aMousePos) override;
	bool UpdateButtonWithMouse(const Vector2f& aMousePos);


	void SetIsActive(const bool& aIsActive) override;

	void SelectObject() override;
	void DeSelectObject() override;

	void Destroy() override;
	void SwitchState();

	void SetState(eButtonState aState);
	eButtonState GetState();


private:

	void OnConnectedActivation() override;
	void OnRelease();

	long long myHovoredSpriteIndex;

	std::function<void()> myFunction;

	CAABB myAABB;
	eButtonState myState;
	eButtonState myPreviousState;
	SButtonSprites myButtonSprites;

	CSpriteInstance* myDotSprite;
};

