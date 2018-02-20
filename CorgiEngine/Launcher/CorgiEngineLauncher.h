#pragma once

#include "..\Console\GameConsole.h"
#include "..\Engine\Engine.h"

#include "ScriptLoader.h"

#ifdef _DEBUG
#pragma comment(lib, "..\\Wwise\\libs\\Audio_Debug.lib")
#else
#pragma comment(lib, "..\\Wwise\\libs\\Audio_Release.lib")
#endif

struct lua_State;

class CorgiEngineLauncher
{
public:
	CorgiEngineLauncher(CEngine& graphicsEngine, CGameConsole& gameConsole);
	~CorgiEngineLauncher();


	static int StartNewEngine(int argc, char* argv[]);
private:

	int StartNewGame();
	void CleanUp();
	bool InitializerPass();

	static void UpdateWWise();

	void InitWwise();
	void LaunchConsole();
	void HandleCommandlines(int argc, char* argv[]);
	void UpdateDebug();
	int RunGame();
	void LoadScript();

	CScriptLoader myScriptLoader;

	std::thread myWWiseUpdateThread;

	static bool myShouldTerminateThread;
	CEngine& myEngine;
	CGameConsole& myGameConsole;

	void Frame(CGame& aGame, bool& aShouldRun);

};

