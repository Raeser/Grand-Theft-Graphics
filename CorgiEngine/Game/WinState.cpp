#include "stdafx.h"
#include "WinState.h"

CWinState::CWinState(CEngine& aEngine, CGameConsole& aConsole)
	: myGameEngine(aEngine), myGameConsole(aConsole)
{
	myHasPushed = false;
}

CWinState::~CWinState()
{
}

void CWinState::Init()
{
	myMessage = eStateStackMessage::KeepState;
	myName = "Win State";
}

void CWinState::OnEnter()
{
	InputManager::GetInstance().SetClipCursorToScreen(true);
}

void CWinState::OnExit()
{
	CWwiseWrapper::GetInstance().PostEventToAllIDs("Stop_Victory_Theme");
}

void CWinState::Destroy()
{
}

eStateStackMessage CWinState::Update()
{
	UpdateStateTimer();
	CCursor::GetInstance().Update();

	if(InputManager::GetInstance().CheckKeyPressed(KeyCode::Escape) || myHasPushed)
	{
		myMessage = eStateStackMessage::PopSubAndMainState;
	}

	return myMessage;
}


