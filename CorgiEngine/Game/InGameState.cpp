#include "stdafx.h"

#include "..\Script\ScriptManager.h"

#include "WinState.h"
#include "EditState.h"

CInGameState::CInGameState(CEngine& aGameEngine, CGameConsole& aGameConsole, UnityExporter& aUnityExporter) :
	myGameEngine(aGameEngine),
	myGameConsole(aGameConsole),
	myUnityExporter(aUnityExporter)
{
}

CInGameState::~CInGameState()
{
}

void CInGameState::Init(CGameWorld* aGameWorld, int aLevelIndex)
{
	CScriptManager::GetInstance().RegisterFunction("WinGame", CInGameState::WinGame, "No args", "Wins the Game", false);

	myGameWorld = aGameWorld;
	myGameWorld->ChangeLevelOnNextUpdate(aLevelIndex);
	//myGameWorld->LoadLevel(aLevelIndex);

	myWindowID = GetForegroundWindow();

	CPostMaster::GetInstance().Subscribe(this, eMessageType::WonGame);

	myHasPushedIntro = false;
	myHasStartedPlaying = false;
}

void CInGameState::OnEnter()
{
	InputManager::GetInstance().SetClipCursorToScreen(true);

	if (!myHasStartedPlaying) 
	{
		CWwiseWrapper::GetInstance().PostNon3DEvent("Play_Game_Loop");
		CWwiseWrapper::GetInstance().PostNon3DEvent("Play_Forest_Ambience");
		myHasStartedPlaying = true;
	}

	CTriggerVolumeManager::GetInstance().SetShouldRender(true);
}

void CInGameState::OnExit()
{
	CTriggerVolumeManager::GetInstance().SetShouldRender(false);
}

void CInGameState::Destroy()
{
	CPostMaster::GetInstance().Unsubscribe(this, eMessageType::WonGame);

	CWwiseWrapper::GetInstance().PostNon3DEvent("Play_Menu_Song");
	CWwiseWrapper::GetInstance().PostNon3DEvent("Play_Menu_Ambience");

	CWwiseWrapper::GetInstance().PostNon3DEvent("Stop_Game_Loop");
	CWwiseWrapper::GetInstance().PostNon3DEvent("Stop_Forest_Ambience");
	myGameWorld->Destroy();
}

void CInGameState::CheckCurrentWindow()
{
	if (myWindowID != GetForegroundWindow())
	{
	}
}

eStateStackMessage CInGameState::Update()
{
	UpdateStateTimer();
	CCursor::GetInstance().Update();

#ifdef _RETAIL
	CheckCurrentWindow();
#endif // _RETAIL

	myGameWorld->Update();

	if (InputManager::GetInstance().CheckKeyReleased(KeyCode::Tilde))
	{
		PushConsoleState();
	}



#ifndef _RETAIL
	if (InputManager::GetInstance().CheckKeyReleased(KeyCode::F5))
	{
		PushWinState();
	}

	if (InputManager::GetInstance().CheckKeyReleased(KeyCode::F1))
	{
		PushEditState();
	}
#endif

	return eStateStackMessage::KeepState;
}

void CInGameState::RecieveMessage(const SMessage & message)
{
	if (message.messageType == eMessageType::WonGame) 
	{
		PushWinState();
	}
}


void CInGameState::PushConsoleState()
{
	CConsoleState* consoleState = new CConsoleState(myGameEngine, myGameConsole);
	consoleState->Init();
	myStateStack->PushSubState(dynamic_cast<CGameState*>(consoleState));
	consoleState = nullptr;
}

void CInGameState::PushEditState()
{
	CEditState* editState = new CEditState();
	editState->Init(myGameWorld, myGameEngine.GetGlobalCamera(), &myGameEngine.GetTextManager());
	myStateStack->PushSubState(dynamic_cast<CEditState*>(editState));
	editState = nullptr;
}



void CInGameState::PushWinState() 
{
	CWinState* winState = new CWinState(myGameEngine, myGameConsole); 
	winState->Init();
	myStateStack->PushSubState(dynamic_cast<CGameState*>(winState));
	winState = nullptr;
}

int CInGameState::WinGame(lua_State *)
{
	SMessage wonGameMessage;
	wonGameMessage.messageType = eMessageType::WonGame;
	CPostMaster::GetInstance().PostMessage(wonGameMessage);

	return 0;
}
