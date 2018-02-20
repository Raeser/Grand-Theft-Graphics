#include "stdafx.h"


CConsoleState::CConsoleState(CEngine & aEngine, CGameConsole& aConsole) : 
	myGameEngine(aEngine), myConsole(aConsole)
{
}

CConsoleState::~CConsoleState()
{
}

void CConsoleState::Init()
{
	myName = "Console State";
	myMessage = eStateStackMessage::KeepState;

	long long spriteIndex = myGameEngine.GetSpriteLoader().CreateSprite("../Assets/Sprites/Menus/consoleWindow.dds");
	myBackground = myGameEngine.GetSpriteLoader().CreateSpriteInstance(spriteIndex);
	myBackground->SetShouldRender(true);


	myConsole.Init();
}

void CConsoleState::OnEnter()
{
	InputManager::GetInstance().SetClipCursorToScreen(false);
}

void CConsoleState::OnExit()
{
}

void CConsoleState::Destroy()
{
	myBackground->Remove();
	myConsole.Clear();
}

eStateStackMessage CConsoleState::Update()
{
	UpdateStateTimer(); 
	CCursor::GetInstance().Update();

	if (!myConsole.Update())
	{
		myMessage = eStateStackMessage::PopSubState;
	}
	return myMessage;
}
