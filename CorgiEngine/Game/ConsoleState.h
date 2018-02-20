#pragma once
#include "GameState.h"

class CEngine;
class CGameConsole;
class CSpriteInstance;

class CConsoleState :
	public CGameState
{
public:
	CConsoleState(CEngine& aEngine, CGameConsole& aConsole);
	~CConsoleState();


	virtual void Init() override;

	virtual void OnEnter() override;
	virtual void OnExit() override;
	virtual void Destroy() override;

	virtual eStateStackMessage Update() override;

private:
	eStateStackMessage myMessage;
	CEngine& myGameEngine;
	CGameConsole& myConsole;


	CSpriteInstance* myBackground;



};

