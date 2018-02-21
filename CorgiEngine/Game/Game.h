#include "stdafx.h"

class CGameConsole;
class CEngine;
class CInGameState;
class CMainMenuState;

class CGame
{
public:
	CGame(CEngine& aGameEngine, CGameConsole& aGameConsole);
	~CGame();

	void Init();
	bool IsWindowActive();
	bool Update();

	bool UpdateStateStack();	


private:

	CEngine& myGameEngine;
	CGameConsole& myGameConsole;
	HWND myWindowID;
	CStateStack myStateStack;
	CMainMenuState* myMainMenuState;

};

