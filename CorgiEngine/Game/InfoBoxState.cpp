#include "stdafx.h"
#include "InfoBoxState.h"


CInfoBoxState::CInfoBoxState(CEngine& aEngine) 
	: myGameEngine(aEngine)
{
}


CInfoBoxState::~CInfoBoxState()
{
}

void CInfoBoxState::Init(const char* aInfoBoxSprite, eInfoBoxType aType, EInfoBoxAnswer& aAnswer)
{
	myAnswer = &aAnswer;

	myName = "Info Box State";
	myMessage = eStateStackMessage::KeepState;

	long long myDimSpriteIndex = myGameEngine.GetSpriteLoader().CreateSprite("../Assets/Sprites/Menus/dim.dds");
	myBackgroundSpriteInstance = myGameEngine.GetSpriteLoader().CreateSpriteInstance(myDimSpriteIndex);
	myBackgroundSpriteInstance->SetShouldRender(true);

	long long myInfoBoxSpriteIndex = myGameEngine.GetSpriteLoader().CreateSprite(aInfoBoxSprite);
	myInfoBoxSpriteInstance = myGameEngine.GetSpriteLoader().CreateSpriteInstance(myInfoBoxSpriteIndex);
	myInfoBoxSpriteInstance->SetShouldRender(true);

	myUIManager.Init(&myGameEngine);


	switch (aType)
	{
	case eInfoBoxType::OK:
		InitOKButtons();
		break;
	case eInfoBoxType::YES_NO:
		InitYESNOButtons();
		break;
	default:
		break;
	}

}

void CInfoBoxState::OnEnter()
{
	InputManager::GetInstance().SetClipCursorToScreen(false);
}

void CInfoBoxState::OnExit()
{
}

void CInfoBoxState::Destroy()
{
	myBackgroundSpriteInstance->Remove();
	myInfoBoxSpriteInstance->Remove();
	myUIManager.Destroy();
}

eStateStackMessage CInfoBoxState::Update()
{
	UpdateStateTimer();
	CCursor::GetInstance().Update();
	myUIManager.Update();
	return myMessage;
}

void CInfoBoxState::InitOKButtons()
{
	CButton* okButton = new CButton();
	okButton->Init(&myGameEngine);
	okButton->InitButton([&] {PopSubstate(); }, { 0.5f, 0.6f, 0.0f }, { "../Assets/Sprites/Menus/okButton.dds", "../Assets/Sprites/Menus/okButtonHighlight.dds" }, 0.125f, 0.075f);

	myUIManager.AddUIObject(*okButton);
}

void CInfoBoxState::InitYESNOButtons()
{
	CButton* yesButton = new CButton();
	yesButton->Init(&myGameEngine);
	yesButton->InitButton([&] {PopSubstateAndConfirm(); }, { 0.5f, 0.6f, 0.0f }, { "../Assets/Sprites/Menus/yesButton.dds", "../Assets/Sprites/Menus/yesButtonHighlight.dds" }, 0.125f, 0.075f);

	myUIManager.AddUIObject(*yesButton);

	CButton* noButton = new CButton();
	noButton->Init(&myGameEngine);
	noButton->InitButton([&] {PopSubstateAndDeny(); }, { 0.5f, 0.75f, 0.0f }, { "../Assets/Sprites/Menus/noButton.dds", "../Assets/Sprites/Menus/noButtonHighlight.dds" }, 0.125f, 0.075f);

	noButton->ConnectWithObject(yesButton, eConnectedDirection::Up);

	myUIManager.AddUIObject(*yesButton);
	myUIManager.AddUIObject(*noButton);
}

void CInfoBoxState::Init()
{
}

void CInfoBoxState::PopSubstate()
{
	myMessage = eStateStackMessage::PopSubState;
}

void CInfoBoxState::PopSubstateAndConfirm()
{
	*myAnswer = EInfoBoxAnswer::Yes;
	myMessage = eStateStackMessage::PopSubState;
}

void CInfoBoxState::PopSubstateAndDeny()
{
	*myAnswer = EInfoBoxAnswer::No;
	myMessage = eStateStackMessage::PopSubState;
}

