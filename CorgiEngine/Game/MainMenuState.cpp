#include "stdafx.h"



#include "..\Engine\ResourceManager.h"
#include "..\Engine\ModelLoaderWrapper.h"
#include <thread>
#include "..\Utilities\DirectoryCommands.h"

CMainMenuState::CMainMenuState(CEngine& aGameEngine, CGameConsole& aGameConsole) :
	myGameEngine(aGameEngine),
	myGameConsole(aGameConsole)
{
}


CMainMenuState::~CMainMenuState()
{
}

void CMainMenuState::Init()
{
	myReturnLevelIndex = -1;
	myHasPushedLevelSelect = false;
	myName = "MainMenuState";

	CDataManager::GetInstance().Init();
	myReturnMessage = eStateStackMessage::KeepState;

	long long spriteIndex = myGameEngine.GetSpriteLoader().CreateSprite("../Assets/Sprites/Menus/menuBackground.dds");
	mySpriteInstance = myGameEngine.GetSpriteLoader().CreateSpriteInstance(spriteIndex);
	mySpriteInstance->SetShouldRender(true);

	myUIManager.Init(&myGameEngine);

	InitButtons();

	CCursor::GetInstance().Init(&myGameEngine);

	CWwiseWrapper::GetInstance().PostNon3DEvent("Play_Menu_Song");
	CWwiseWrapper::GetInstance().PostNon3DEvent("Play_Menu_Ambience");

	myUnityExporter.Init("../Assets/Data/UnityExport/", CDataManager::GetInstance().GetConfigData().levelsToLoad);
	InitModels();

	myHasPushedSplash = false;
	myHasCreatedGameworld = false;
	CTriggerVolumeManager::GetInstance().Init();

}

void CMainMenuState::InitModels()
{
	myGameEngine.GetResourceManager().GetModelLoaderWrapper()->CreateModel("../Assets/Models/minionCharacter/minionCharacter.fbx", ModelType::Model);
	myGameEngine.GetResourceManager().GetModelLoaderWrapper()->CreateModel("../Assets/Models/playerCharacter/playerCharacter.fbx", ModelType::Model);
	LoadAllModels();
	for (int i = 0; i < myUnityExporter.GetAllUniqueModelsPaths().Size(); ++i)
	{
		myGameEngine.GetResourceManager().GetModelLoaderWrapper()->CreateModel(myUnityExporter.GetAllUniqueModelsPaths()[i].c_str(), ModelType::Model);
	}

}

void CMainMenuState::LoadAllModels()
{
	std::vector<std::string> paths;
	Corgi::GetFileTypePathsInDirectory("../assets/Sprites/Menus", ".dds", paths, true);

	int amountOfModels = static_cast<int>(paths.size());
	for (int i = 0; i < amountOfModels; ++i)
	{
		myGameEngine.GetSpriteLoader().CreateSprite(paths[i].c_str());
	}
}

void CMainMenuState::OnEnter()
{
	InputManager::GetInstance().SetClipCursorToScreen(false);
	myTimeInState = 0.f;
	mySpriteInstance->SetShouldRender(true);
	myUIManager.SetIsActive(true);

	if (!myHasPushedSplash) 
	{
#ifndef _DEBUG
		PushSplashScreen();
#endif
		myHasPushedSplash = true;
	}
}

void CMainMenuState::OnExit()
{
}

void CMainMenuState::Destroy()
{
}

eStateStackMessage CMainMenuState::Update()
{
	UpdateStateTimer();
	CCursor::GetInstance().Update();

	if (InputManager::GetInstance().CheckKeyReleased(KeyCode::Tilde))
	{
		PushConsoleState();
	}

	if (HasBeenInStateLoongEnough()) 
	{
		myUIManager.Update();
	}

	return myReturnMessage;
}

void CMainMenuState::InitButtons()
{
	float x = 0.8f;
	float firstY = 0.125f;
	float yDiff = 0.15f;

	CButton* startButton = new CButton();
	startButton->Init(&myGameEngine);
	startButton->InitButton([&] { PushInGameState(); }, { x, firstY, 0.1f }, { "../Assets/Sprites/Menus/startButton.dds", "../Assets/Sprites/Menus/startButtonHighlight.dds" }, 0.15f, 0.07f);

	CButton* exitButton = new CButton();
	exitButton->Init(&myGameEngine);
	exitButton->InitButton([&] { PopMainState(); }, { x, firstY + (yDiff * 5), 0.1f }, { "../Assets/Sprites/Menus/exitButton.dds", "../Assets/Sprites/Menus/exitButtonHighlight.dds" }, 0.15f, 0.07f);


	exitButton->ConnectWithObject(startButton, eConnectedDirection::Up);
	startButton->ConnectWithObject(exitButton, eConnectedDirection::Down);

	myUIManager.AddUIObject(*startButton);
	myUIManager.AddUIObject(*exitButton);
}

void CMainMenuState::PushInGameState()
{
	if (myHasCreatedGameworld == false)
	{
		myGameworld = new CGameWorld(myGameEngine, myGameConsole, myUnityExporter);
		myGameworld->Init(0);
		myHasCreatedGameworld = true;
	}


 	CInGameState* inGameState = new CInGameState(myGameEngine, myGameConsole, myUnityExporter);
	inGameState->Init(myGameworld,0);
	myStateStack->PushMainState(inGameState);
	mySpriteInstance->SetShouldRender(false);
	myUIManager.SetIsActive(false);

	CWwiseWrapper::GetInstance().PostEventToAllIDs("Stop_Menu_Song");
	CWwiseWrapper::GetInstance().PostNon3DEvent("Stop_Menu_Ambience");
}

void CMainMenuState::PushInGameState(int aLevelIndex)
{
	if (myHasCreatedGameworld == false)
	{
		myGameworld = new CGameWorld(myGameEngine, myGameConsole, myUnityExporter);
		myGameworld->Init(0);
		myHasCreatedGameworld = true;
	}
	CInGameState* inGameState = new CInGameState(myGameEngine, myGameConsole, myUnityExporter);
	inGameState->Init(myGameworld, aLevelIndex);
	myStateStack->PushMainState(inGameState);
	mySpriteInstance->SetShouldRender(false);
	myUIManager.SetIsActive(false);

	CWwiseWrapper::GetInstance().PostEventToAllIDs("Stop_Menu_Song");
	CWwiseWrapper::GetInstance().PostNon3DEvent("Stop_Menu_Ambience");
}



void CMainMenuState::PopMainState()
{
	myReturnMessage = eStateStackMessage::PopMainState;
}

void CMainMenuState::PopSubstate()
{
	myStateStack->PopSubState();
}

void CMainMenuState::PushConsoleState()
{
	CConsoleState* consoleState = new CConsoleState(myGameEngine, myGameConsole);
	consoleState->Init();
	myStateStack->PushSubState(dynamic_cast<CGameState*>(consoleState));
	consoleState = nullptr;
}
