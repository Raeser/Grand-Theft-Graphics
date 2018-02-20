#include "stdafx.h"
#include "OptionsSelector.h"

#include "UIManager.h"



COptionsSelector::COptionsSelector()
{
	myHaveACallbackFunction = false;
}

COptionsSelector::~COptionsSelector()
{
}

void COptionsSelector::Init(const Corgi::Vector2<float>& aPos, CUIManager& aUIManager, CEngine& aEngine, const char* aTextPath)
{
	Init(&aEngine);
	myUIManager = &aUIManager;
	mySelections.Init(8);

	long long spriteIndex = myGameEngine->GetSpriteLoader().CreateSprite(aTextPath);
	myTextSprite = myGameEngine->GetSpriteLoader().CreateSpriteInstance(spriteIndex);
	myTextSprite->SetPosition0To1({ aPos.x, aPos.y - 0.075f, 0.f });
	myTextSprite->SetShouldRender(true);

	spriteIndex = myGameEngine->GetSpriteLoader().CreateSprite("../Assets/Sprites/Menus/selectorBase.dds");
	mySpriteInstance = myGameEngine->GetSpriteLoader().CreateSpriteInstance(spriteIndex);
	mySpriteInstance->SetPosition0To1({aPos.x, aPos.y, 0.f});
	mySpriteInstance->SetShouldRender(true);

	myPosition = aPos;

	InitButtons();

	aUIManager.AddUIObject(*myLeftButton);
	aUIManager.AddUIObject(*myRightButton);

	myCurrentSelectionSprite = myGameEngine->GetSpriteLoader().CreateSpriteInstance(spriteIndex);
	myCurrentSelectionSprite->SetPosition0To1({ aPos.x, aPos.y, 0.f });
	myCurrentSelectionSprite->SetShouldRender(false);

	myArrayIndex = 0;
}

bool COptionsSelector::Update(const Corgi::Vector2<float>& aMousePos)
{
	aMousePos;
	return false;
}

void COptionsSelector::Destroy()
{
	myTextSprite->SetShouldRender(false);
	myTextSprite->Remove();
	myTextSprite = nullptr;

	myCurrentSelectionSprite->SetShouldRender(false);
	myCurrentSelectionSprite->Remove();
	myCurrentSelectionSprite = nullptr;

	mySpriteInstance->SetShouldRender(false);
	mySpriteInstance->Remove();
	mySpriteInstance = nullptr;
}

void COptionsSelector::SetCallbackFunction(std::function<void()> aCallbackFunction)
{
	myCallbackFunction = aCallbackFunction;
	myHaveACallbackFunction = true;
}

void COptionsSelector::GoToNextSelection()
{
	if (myArrayIndex == mySelections.Size() - 1) 
	{
		myArrayIndex = 0;
	}
	else 
	{
		++myArrayIndex;
	}

	UpdateSelection();
}

void COptionsSelector::GoToPrevSelection()
{
	if (myArrayIndex <= 0)
	{
		myArrayIndex = mySelections.Size() - 1;
	}
	else
	{
		--myArrayIndex;
	}

	UpdateSelection();
}

void COptionsSelector::OnChangedSelection()
{
	if (myHaveACallbackFunction) 
	{
		myCallbackFunction();
	}
}

void COptionsSelector::Init(CEngine * aGameEngine)
{
	myGameEngine = aGameEngine;
}

void COptionsSelector::InitButtons()
{
	float distanceFromCenterPos = 0.08f;
	myLeftButton = new CButton();
	myLeftButton->Init(myGameEngine);
	myLeftButton->InitButton([&] { GoToPrevSelection(); }, { myPosition.x - distanceFromCenterPos, myPosition.y, 0.f }, { "../Assets/Sprites/Menus/arrowLeftButton.dds", "../Assets/Sprites/Menus/arrowLeftButtonHighlight.dds" }, 0.05f, 0.05f);

	myRightButton = new CButton();
	myRightButton->Init(myGameEngine);
	myRightButton->InitButton([&] { GoToNextSelection(); }, { myPosition.x + distanceFromCenterPos, myPosition.y, 0.f }, { "../Assets/Sprites/Menus/arrowRightButton.dds", "../Assets/Sprites/Menus/arrowRightButtonHighlight.dds" }, 0.05f, 0.05f);

	myLeftButton->ConnectWithObject(myRightButton, eConnectedDirection::Right);
}

void COptionsSelector::UpdateSelection()
{
	if (mySelections.Size() > 0)
	{
		long long spriteIndex = -1;

		switch (mySelections[myArrayIndex])
		{
		case eSelectionType::Off:
			spriteIndex = myGameEngine->GetSpriteLoader().CreateSprite("../Assets/Sprites/Menus/textOFF.dds");
			myCurrentSelectionSprite->SetSprite(spriteIndex);
			break;
		case eSelectionType::On:
			spriteIndex = myGameEngine->GetSpriteLoader().CreateSprite("../Assets/Sprites/Menus/textON.dds");
			myCurrentSelectionSprite->SetSprite(spriteIndex);
			break;
		case eSelectionType::Res800x600:
			spriteIndex = myGameEngine->GetSpriteLoader().CreateSprite("../Assets/Sprites/Menus/text800x600.dds");
			myCurrentSelectionSprite->SetSprite(spriteIndex);
			break;
		case eSelectionType::Res1280x720:
			spriteIndex = myGameEngine->GetSpriteLoader().CreateSprite("../Assets/Sprites/Menus/text1280x720.dds");
			myCurrentSelectionSprite->SetSprite(spriteIndex);
			break;
		case eSelectionType::Res1920x1080:
			spriteIndex = myGameEngine->GetSpriteLoader().CreateSprite("../Assets/Sprites/Menus/text1920x1080.dds");
			myCurrentSelectionSprite->SetSprite(spriteIndex);
			break;
		default:
			break;
		}

		myCurrentSelectionSprite->SetShouldRender(true);

		OnChangedSelection();
	}

}

void COptionsSelector::AddSelection(eSelectionType aSelectionType)
{
	mySelections.Add(aSelectionType);

	if (mySelections.Size() == 1) 
	{
		UpdateSelection();
	}
}

eSelectionType COptionsSelector::GetSelection()
{
	return mySelections[myArrayIndex];
}

bool COptionsSelector::SetSelection(eSelectionType aSelection)
{
	for (int i = 0; i < mySelections.Size(); ++i)
	{
		if (mySelections[i] == aSelection) 
		{
			myArrayIndex = i;

			UpdateSelection();
			return true;
		}
	}

	return false;
}

void COptionsSelector::ConnectButtonsUp(CUIConnectedObject * aLeftConnection, CUIConnectedObject * aRightConnection)
{
	myRightButton->ConnectWithObject(aRightConnection, eConnectedDirection::Up);
	myLeftButton->ConnectWithObject(aLeftConnection, eConnectedDirection::Up);
}

void COptionsSelector::ConnectButtonsDown(CUIConnectedObject * aLeftConnection, CUIConnectedObject * aRightConnection)
{
	myRightButton->ConnectWithObject(aRightConnection, eConnectedDirection::Down);
	myLeftButton->ConnectWithObject(aLeftConnection, eConnectedDirection::Down);
}
