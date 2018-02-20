#pragma once
#include "GameState.h"

class CEngine;
class CGameConsole;
class CSpriteInstance;

class CWinState :
	public CGameState
{
public:
	CWinState(CEngine& aEngine, CGameConsole& aConsole);
	~CWinState();


	virtual void Init() override;

	virtual void OnEnter() override;
	virtual void OnExit() override;
	virtual void Destroy() override;

	virtual eStateStackMessage Update() override;


private:

	void PushCutscene();

	CEngine& myGameEngine;
	CGameConsole& myGameConsole;

	eStateStackMessage myMessage;

	bool myHasPushed;
};

