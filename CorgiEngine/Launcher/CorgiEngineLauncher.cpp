#include "stdafx.h"

#include "CorgiEngineLauncher.h"

#include "..\Utilities\ExceptionHandler.h"

#include "..\Wwise\Audio\Audio\AudioInterface.h"

#include "..\Game\WwiseWrapper.h"
#include "..\Game\GameSettings.h"

#include "..\Engine\PostMaster.h"
#include "..\Engine\DebugDrawer.h"
#include "..\Engine\TextToSpeechManager.h"


bool CorgiEngineLauncher::myShouldTerminateThread;
CorgiEngineLauncher::CorgiEngineLauncher(CEngine& engine, CGameConsole& console)	:
	myEngine(engine),
	myGameConsole(console)
{
	myShouldTerminateThread = false;
}


CorgiEngineLauncher::~CorgiEngineLauncher()
{
}

int CorgiEngineLauncher::StartNewEngine(int argc, char * argv[])
{
	CEngine engine;
	CGameConsole console(engine);
	CorgiEngineLauncher launcher(engine, console);

	int returnCode = 0;

#ifndef _RETAIL
	launcher.HandleCommandlines(argc, argv);
	launcher.LaunchConsole();
	ExceptionHandler::GetInstance().InitCPUException();
#define USE_FILTERLOG
#else
	argc, argv;
#endif

	returnCode = launcher.StartNewGame();


	return returnCode;
}




void CorgiEngineLauncher::InitWwise()
{
	CWwiseWrapper::GetInstance().Init();
}

void CorgiEngineLauncher::LaunchConsole()
{
	AllocConsole();
	FILE* cinFile;
	freopen_s(&cinFile, "CONIN$", "r", stdin);
	FILE* coutFile;
	freopen_s(&coutFile, "CONOUT$", "w", stdout);
	FILE* cerrFile;
	freopen_s(&cerrFile, "CONOUT$", "w", stderr);
}

void CorgiEngineLauncher::HandleCommandlines(int argc, char * argv[])
{

	argc; argv;
#ifndef _RETAIL

	for (int i = 0; i < argc; ++i)
	{
		CommandLineManager::GetInstance().Populate(argv[i]);
	}

	if (CommandLineManager::GetInstance().HasParameter("-user"))
	{
		for (const auto UserString : CGameSettings::GetInstance().GetUserVector())
		{
			if (CommandLineManager::GetInstance().HasArgument("-user", UserString))
			{
				CGameSettings::GetInstance().SetUser(UserString);
			}
		}
	}
	if (CommandLineManager::GetInstance().HasParameter("-activelog"))
	{
		if (CommandLineManager::GetInstance().HasArgument("-activelog", "Resource"))
		{
			DebugLog::GetInstance().ActivateFilterlog(eDebugLogType::Resource);
		}
		if (CommandLineManager::GetInstance().HasArgument("-activelog", "UnityExporter"))
		{
			DebugLog::GetInstance().ActivateFilterlog(eDebugLogType::UnityExporter);
		}
		if (CommandLineManager::GetInstance().HasArgument("-activelog", "Engine"))
		{
			DebugLog::GetInstance().ActivateFilterlog(eDebugLogType::Engine);
		}
		if (CommandLineManager::GetInstance().HasArgument("-activelog", "Render"))
		{
			DebugLog::GetInstance().ActivateFilterlog(eDebugLogType::Render);
		}
		if (CommandLineManager::GetInstance().HasArgument("-activelog", "Timer"))
		{
			DebugLog::GetInstance().ActivateFilterlog(eDebugLogType::Timer);
		}
	}
	//DebugLog::GetInstance().ActivateFilterlog(eDebugLogType::Timer);
	//DebugLog::GetInstance().ActivateFilterlog(eDebugLogType::Shader);
	//DebugLog::GetInstance().ActivateFilterlog(eDebugLogType::Resource);
	//DebugLog::GetInstance().ActivateFilterlog(eDebugLogType::UnityExporter);
	//DebugLog::GetInstance().ActivateFilterlog(eDebugLogType::ResultMessages);
	//DebugLog::GetInstance().ActivateFilterlog(eDebugLogType::Error);
	//DebugLog::GetInstance().ActivateFilterlog(eDebugLogType::Script);
	//DebugLog::GetInstance().ActivateFilterlog(eDebugLogType::Resource);

#endif
}

void CorgiEngineLauncher::UpdateDebug()
{
	if (InputManager::GetInstance().CheckKeyPressed(KeyCode::M))
	{
		CAudioInterface::GetInstance()->PostEvent("Set_Volume_Muted", 1);
	}

	if (InputManager::GetInstance().CheckKeyPressed(KeyCode::U))
	{
		CAudioInterface::GetInstance()->PostEvent("Set_Volume_Default", 1);
	}

#ifdef MUTE_AUDIO
	CAudioInterface::GetInstance()->PostEvent("Set_Volume_Muted", 1);
#endif

}

int CorgiEngineLauncher::StartNewGame()
{
	int out = 0;
	__try
	{
		out = RunGame();
	}
	__except (out = ExceptionHandler::GetInstance().ExceptionFilterFunction(GetExceptionInformation()))
	{
		CWwiseWrapper::GetInstance().Destroy();
		return out;
	}

	return out;
}

void CorgiEngineLauncher::CleanUp()
{
	myShouldTerminateThread = true;

	myWWiseUpdateThread.join();
	CWwiseWrapper::GetInstance().Destroy();
	CTextToSpeechManager::GetInstance().Release();
}

bool CorgiEngineLauncher::InitializerPass()
{

	InitWwise();
	myWWiseUpdateThread = std::thread(UpdateWWise);
	InputManager::GetInstance().InitKeysToCheck();

	CPostMaster::GetInstance().Init();
	CGameSettings::GetInstance().PreInit();
	Corgi::Vector2<int> resolution = CGameSettings::GetInstance().GetResolutions().desktop; // Testres { 1280, 720 };
	CWindowHandler::WindowData windowData;
	windowData.width = resolution.x;
	windowData.height = resolution.y;

	bool engineValidation = myEngine.Init(windowData);
	bool consoleValidation = myGameConsole.Init();

	CGameSettings::GetInstance().Init();
	CGameSettings::GetInstance().UpdateResolutions();

#ifndef _RETAIL
	CDebugDrawer::GetInstance().Init(&myEngine);
	ExceptionHandler::GetInstance().AssignEngine(&myEngine);
#endif

	CTextToSpeechManager::GetInstance().Init();

	LoadScript();

	return engineValidation && consoleValidation;

}

void CorgiEngineLauncher::UpdateWWise()
{
	while (!myShouldTerminateThread)
	{
		CAudioInterface::GetInstance()->Update();
	}
}

int CorgiEngineLauncher::RunGame()
{
	int out = 0;
	bool shouldRun = InitializerPass();

	CGame game(myEngine, myGameConsole);
	game.Init();

	MSG windowsmessage = { 0 };
	while (shouldRun)
	{
		while (PeekMessage(&windowsmessage, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&windowsmessage);
			DispatchMessage(&windowsmessage);

			if (windowsmessage.message == WM_QUIT)
			{
				shouldRun = false;
			}
		}
		Frame(game, shouldRun);
	}

	CleanUp();

	return out;
}

void CorgiEngineLauncher::LoadScript()
{

	CScriptManager::GetInstance().Init();

	myScriptLoader.SubscribeOnScripts();
	myScriptLoader.LoadScripts();
	myScriptLoader.ReadFiles();
	
}

void CorgiEngineLauncher::Frame(CGame& aGame, bool& aShouldRun)
{

	myEngine.BeginFrame();

	Timer::GetInstance().Update();
	myEngine.Update();
#ifndef _RETAIL
	CDebugDrawer::GetInstance().Update();
#endif
	if (aShouldRun)
	{
		aShouldRun = aGame.Update();
	}

#ifdef _DEBUG
	UpdateDebug();
#endif

	InputManager::GetInstance().Update();
	//Rendering
	myEngine.RenderFrame();
	CPostMaster::GetInstance().MailOut();
	myEngine.EndFrame();

#ifndef _RETAIL
	CDebugDrawer::GetInstance().EndFrame();
#endif
}
