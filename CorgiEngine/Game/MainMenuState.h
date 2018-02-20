#pragma once
#include "GameState.h"
#include "UnityExporter.h"
#include <thread>
#include "UIManager.h"

class CEngine;
class CGameConsole;
class CInGameState;

class CMainMenuState :
	public CGameState
{
public:
	CMainMenuState(CEngine& aGameEngine, CGameConsole& aGameConsole);
	~CMainMenuState();

	void Init() override;
	void InitModels();
	void LoadAllModels();
	void OnEnter() override;
	void OnExit() override;
	void Destroy() override;

	eStateStackMessage Update() override;

private:


	void InitButtons();

	void PushInGameState();
	void PushInGameState(int aLevelIndex);
	void PushOptionstate();
	void PushShowroomState();
	void PushCreditsState();
	void PopMainState();
	void PopSubstate();
	void PushSplashScreen();
	void PushLevelSelect();

	void PushConsoleState();

	CEngine& myGameEngine;
	CGameConsole& myGameConsole;
	UnityExporter myUnityExporter;

	eStateStackMessage myReturnMessage;

	CSpriteInstance* mySpriteInstance;

	CUIManager myUIManager;


	CGameWorld* myGameworld;

	int myReturnLevelIndex;
	bool myHasPushedLevelSelect;
	bool myHasPushedSplash;

	
	bool myHasCreatedGameworld;
};

