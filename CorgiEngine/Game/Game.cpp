#include "stdafx.h"
#include "Game.h"
#include "EngineWrapper.h"


CGame::CGame(CEngine& aGameEngine, CGameConsole& aGameConsole) :
	myGameEngine(aGameEngine), myGameConsole(aGameConsole)
{
}

CGame::~CGame()
{

}

void CGame::Init()
{
	myStateStack.Init();
	myWindowID = GetForegroundWindow();
	myMainMenuState = new CMainMenuState(myGameEngine, myGameConsole);
	myMainMenuState->Init();
	myStateStack.PushMainState(dynamic_cast<CGameState*>(myMainMenuState));

	CEngineWrapper::GetInstance().Init(myGameEngine);

	CCursor::GetInstance().SetIsActive(false);

	//InitNetworkClient();
}

bool CGame::IsWindowActive()
{
	if (myWindowID != GetForegroundWindow())
	{
		return false;
	}
	return true;
}




bool CGame::Update()
{
	CGameSettings::GetInstance().UpdateFullscreen();
	InputManager::GetInstance().SetWindowIsActive(IsWindowActive());

	return UpdateStateStack();
}

bool CGame::UpdateStateStack()
{
	CGameState* state = myStateStack.GetCurrentState();

	if (state == nullptr) 	
	{
		return false;
	}


	std::string name;
	eStateStackMessage stateMSG = state->Update();

	CGameState* mainState = nullptr;
	std::string mainName;
	
	switch (stateMSG)
	{
	case eStateStackMessage::KeepState:
		break;
	case eStateStackMessage::PopMainState:
		if (state->HasBeenInStateLoongEnough()) 
		{
			name = state->GetName();
			myStateStack.PopMainState();
			DL_SUCCESS(eDebugLogType::ResultMessages, "%s%s", "Successfully Popped main state: ", name.c_str());
		}
		break;
	case eStateStackMessage::PopSubState:
		if (state->HasBeenInStateLoongEnough())
		{
			name = state->GetName();
			myStateStack.PopSubState();
			DL_SUCCESS(eDebugLogType::ResultMessages, "%s%s", "Successfully popped substate: ", name.c_str());
		}
		break;
	case eStateStackMessage::PopSubAndMainState:
		name = state->GetName();
		myStateStack.PopSubState();
		DL_SUCCESS(eDebugLogType::ResultMessages, "%s%s", "Successfully popped substate: ", name.c_str());
		
		mainState = myStateStack.GetCurrentState();
		mainName = mainState->GetName();
		myStateStack.PopMainState();
		DL_SUCCESS(eDebugLogType::ResultMessages, "%s%s&s&s", "Successfully popped both Substate: '", name.c_str(), "' and main state '", mainName.c_str());
		break;
	case eStateStackMessage::ExitGame:
		name = state->GetName();
		myStateStack.PopSubState();
		DL_SUCCESS(eDebugLogType::ResultMessages, "%s%s", "Successfully popped substate: ", name.c_str());

		mainState = myStateStack.GetCurrentState();
		mainName = mainState->GetName();
		myStateStack.PopMainState();
		myStateStack.PopMainState();
		DL_SUCCESS(eDebugLogType::ResultMessages, "%s%s&s&s", "Successfully popped both Substate: '", name.c_str(), "' and main state '", mainName.c_str());
		break;
	default:
		break;
	}

	return true;

}

