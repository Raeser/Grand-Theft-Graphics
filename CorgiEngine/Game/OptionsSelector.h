#pragma once
#include "stdafx.h"
#include "UIConnectedObject.h"
#include <functional>

class CEngine;
class CSpriteInstance;
class CButton;
class CUIManager;

enum class eSelectionType
{
	On,
	Off,
	Res1920x1080,
	Res1280x720,
	Res800x600
};


class COptionsSelector 
	: public CUIObject
{
public:
	COptionsSelector();
	~COptionsSelector();

	void Init(const Corgi::Vector2<float>& aPos, CUIManager& aUIManager, CEngine& aEngine, const char* aTextPath);
	bool Update(const Corgi::Vector2<float>& aMousePos) override;
	void Destroy() override;

	void SetCallbackFunction(std::function<void()> aCallbackFunction);

	void AddSelection(eSelectionType aSelectionType);

	bool SetSelection(eSelectionType aSelection);
	void ConnectButtonsUp(CUIConnectedObject* aLeftConnection, CUIConnectedObject* aRightConnection);
	void ConnectButtonsDown(CUIConnectedObject* aLeftConnection, CUIConnectedObject* aRightConnection);

	CButton* GetLeftButton() { return myLeftButton; };
	CButton* GetRightButton() { return myRightButton; };

	eSelectionType GetSelection();
private:

	void OnChangedSelection();

	void Init(CEngine* aGameEngine) override;

	void InitButtons();

	void UpdateSelection();
	void GoToNextSelection();
	void GoToPrevSelection();

	Vector2f myPosition;
	Corgi::GrowingArray<eSelectionType> mySelections;
	CSpriteInstance* myCurrentSelectionSprite;
	CSpriteInstance* myTextSprite;
	int myArrayIndex;

	std::function<void()> myCallbackFunction;
	bool myHaveACallbackFunction;

	CButton* myLeftButton;
	CButton* myRightButton;
	CUIManager* myUIManager;
};

