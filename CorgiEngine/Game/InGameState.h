#pragma once
#include "GameState.h"
#include "..\Engine\Subscriber.h"

class CPlayerTemp;
class CModelInstance;
class CPointLightInstance;
class CGameConsole;
struct lua_State;
class CGameWorld;
class CEngine;
class UnityExporter;

class CInGameState :
	public CGameState, 
	public CSubscriber
{
public:
	CInGameState(CEngine& aGameEngine, CGameConsole& aGameConsole, UnityExporter& aUnityExporter);
	~CInGameState();

	void Init(CGameWorld* aGameWorld, int aLevelIndex);
	void OnEnter() override;
	void OnExit() override;
	void Destroy() override;
	 
	void CheckCurrentWindow();

	eStateStackMessage Update() override;


private:

	void Init() override {};
	void PushWinState();
	void RecieveMessage(const SMessage& message) override;
	void PushPauseState();
	void PushConsoleState();
	void PushEditState();
	void PushIntroCutsceneState();

	static int WinGame(lua_State*);

	CGameWorld* myGameWorld;
	CGameConsole& myGameConsole;
	CEngine& myGameEngine;

	HWND myWindowID;
	UnityExporter& myUnityExporter;
	int myWwiseIndex;

	bool myHasPushedIntro;
	bool myHasStartedPlaying;
};

