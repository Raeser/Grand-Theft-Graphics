#include "stdafx.h"


#include "SpriteRenderer.h"

#include <wincodec.h>
#include "ParticleManager.h"
#include "ModelLoaderWrapper.h"

#include "..\Collision\TriggerVolumeManager.h"

CEngine::CEngine() :
	myGraphicsEngine(myModelLoader, myLightLoader, mySpriteLoader, myResourceManager, myTextManager, myShaderManager),
	myTextManager(*this),
	myModelLoader(myShaderManager, myAnimationLoader),
	myResourceManager(myShaderManager, myAnimationLoader),
	myAnimationManager(myAnimationLoader)
{
	myParticleManager = new CParticleManager(myParticleEmittorLoader);
	myGraphicsEngine.SetParticleManager(myParticleManager);
	myScene = new CScene(myAnimationManager,myResourceManager);
}


CEngine::~CEngine()
{
	delete myScene;
	myScene = nullptr;
	delete myParticleManager;
	myParticleManager = nullptr;

	myShouldUpdateNetwork = false;
	if (myNetworkThread.joinable())
	{
		myNetworkThread.join();
	}
}

bool CEngine::Init(const CWindowHandler::WindowData & aWindowData)
{
#ifdef _RETAIL
	myDebugFeatures.fileWatcherOn = false;
#else
	myDebugFeatures.fileWatcherOn = true;
#endif
	CFullscreenTexture::AssignFramework(&myFramework);
	CGBuffer::AssignFramework(&myFramework);

	myWindowSize = { aWindowData.width, aWindowData.height };

	DL_INITCHECK(Corgi::CFileWatcher::GetInstance().Init(myDebugFeatures.fileWatcherOn), "FileWatcher", "Engine.cpp");

	DL_INITCHECK(myWindowHandler.Init(aWindowData, false), "WindowHandler", "Engine.cpp");
	DL_INITCHECK(myFramework.Init(myWindowHandler), "FrameWork", "Engine.cpp");
	CreateSampleStates(); //NeedsFixing

	DL_INITCHECK(myScene->Init(), "Scene", "Engine.cpp");
	DL_INITCHECK(myLightLoader.Init(&myFramework, myScene), "LightLoader", "Engine.cpp");
	myLightLoader.LoadLights(); //NeverSpace


	DL_INITCHECK(myShaderManager.Init(myFramework.GetDevice(), myFramework.GetContext()), "ShaderManager", "Engine.cpp");
	DL_INITCHECK(myGraphicsEngine.Init(&myFramework ,myWindowSize), "GraphicsEngine", "Engine.cpp");
	myGraphicsEngine.SetWorldLight(myLightLoader.CreateEnvironmentLightInstance("../Assets/CubeMaps/spaceCubemap.dds", { 23.f, 60.f, -20.f, 1.f }, { 1.f, 1.f, 1.f, 1.f }));

	mySpriteLoader.Init({ static_cast<float>(aWindowData.width), static_cast<float>(aWindowData.height) }, myFramework.GetDevice(), eScene_mainMenu);

	DL_INITCHECK(myParticleEmittorLoader.Init(&myFramework, myScene), "ParticleEmittorLoader", "Engine.cpp");
	
	DL_INITCHECK(myParticleManager->Init(), "ParticleManager", "Engine.cpp");
	DL_INITCHECK(myTextManager.Init(), "TextManager", "Engine.cpp");
	myDefaultFontID = myTextManager.LoadFont("../Assets/Fonts/comic.ttf", EFontSize_18, 3);
	myTextManager.LoadFont("../Assets/Fonts/arial.ttf", EFontSize_18, 1);

	DL_INITCHECK(myResourceManager.Init(myFramework.GetDevice(), myScene), "ResourceManager", "Engine.cpp");
	mySkyboxID =  myResourceManager.GetModelLoaderWrapper()->CreateSkybox("../Assets/CubeMaps/skybox.dds"); 

	myParticleEmittorInstancesToCull.Init(256);

	myAnimationLoader.Init();
	myAnimationManager.Init(100000);

	myModelLoader.Init(myFramework.GetDevice());
	myModelLoader.AssignScene(myScene);
	LoadCameras();
	

	return true;
}


void CEngine::InitNetworkClient()
{
	myNetworkClient = new CClient();
	myShouldUpdateNetwork = myNetworkClient->Init();


	myNetworkThread = std::thread([&]()
	{
		UpdateNetworkClient();
	}
	);
}

void CEngine::DisconnectNetwork()
{

	if (myNetworkClient)
	{
		myNetworkClient->Shutdown();
		delete myNetworkClient;
		myNetworkClient = nullptr;
		myShouldUpdateNetwork = false;

		if (myNetworkThread.joinable())
		{
			myNetworkThread.join();
		}
	}

}

void CEngine::UpdateNetworkClient()
{
	while (myShouldUpdateNetwork)
	{
		myNetworkClient->Update();
	}
}

CCameraInstance* CEngine::GetGlobalCamera()
{
	return myGlobalCamera;
}



void CEngine::ToggleVSync()
{
	myFramework.SetIsVsyncEnabled(!myFramework.GetIsVsyncEnabled());
}

bool CEngine::GetIsVsyncEnabled()
{
	return myFramework.GetIsVsyncEnabled();
}

void CEngine::CleanupGameScene()
{
	myScene->Clean();
}

long long CEngine::CreateModel(const char * aModelPath)
{
	return myModelLoader.CreateModel(aModelPath, "../Assets/Shaders/VS_PBLModel.hlsl", "../Assets/Shaders/PS_PBLModel.hlsl");
}


long long CEngine::CreateCube()
{
	return myModelLoader.CreateCube();
}

long long CEngine::CreateTriangle()
{
	DL_ERROR(eDebugLogType::Resource, "%s", "Triangle function is only saved for future use but not up to date, creating a triangle will crash your graphicscard!");
	return myModelLoader.CreateCube();
}

CModelInstance * CEngine::CreateInstance()
{
	return myModelLoader.CreateInstance();
}

CModelInstance * CEngine::CreateInstance(const char * aPath)
{
	return myModelLoader.CreateInstance(aPath);
}

CModelInstance * CEngine::CreateInstance(long long aID)
{
	return myModelLoader.CreateInstance(aID);
}

long long CEngine::CreateText(std::string & aTextRefOut, Vector2f aPosition, FontID aFont, Vector4f aColor, float aScale, eTextType aType, int aScrollingStartIndex)
{
	return myTextManager.AddText(aTextRefOut, aPosition, aFont, aColor, aScale, aType, aScrollingStartIndex);
}

void CEngine::Update()
{
	myScene->Update();
	myAnimationManager.Update();
	myGlobalCamera->Update();
	myTextManager.Update();
	mySpriteLoader.UpdateActiveSpriteInstances();
	Corgi::CFileWatcher::GetInstance().FlushChanges();
}

void CEngine::BeginFrame()
{
	myGraphicsEngine.BeginFrame();
}

void CEngine::EndFrame()
{
	myFramework.EndFrame();
	myGraphicsEngine.EndFrame();
}

void CEngine::RenderFrame()
{
	myScene->CullParticleEmitterInstances(myParticleEmittorInstancesToCull);
	myParticleManager->AssignParticleEmittorInstances(myParticleEmittorInstancesToCull);
	myParticleManager->Update(myGlobalCamera->GetPosition());
	myGlobalCamera->UpdateBuffers(&myFramework);

	myGraphicsEngine.RenderSkybox(myGlobalCamera, mySkyboxID);
	myGraphicsEngine.RenderDeferred(myGlobalCamera, myScene);
	myGraphicsEngine.RenderLights(myGlobalCamera, myScene);
	myGraphicsEngine.RenderParticles(myGlobalCamera);
	myGraphicsEngine.BloomEffect();
	myGraphicsEngine.ColorGrading();
	myGraphicsEngine.RenderSprites();

	myGraphicsEngine.GetBlendStateManager().SetBlendState(EBlendState_SpriteBlend);
	CTriggerVolumeManager::GetInstance().Render(myGlobalCamera, myGraphicsEngine.GetSpriteRenderer());
	myGraphicsEngine.GetBlendStateManager().SetDefaultState();
#ifndef _RETAIL
	if (InputManager::GetInstance().CheckKeyDown(KeyCode::LeftAlt) && InputManager::GetInstance().CheckKeyPressed(KeyCode::Tab))
	{
		InputManager::GetInstance().SetForceClipping(false);
	}
	myGraphicsEngine.RenderDebug(myGlobalCamera);
#endif // DEBUG


	myGraphicsEngine.RenderText();
	myParticleEmittorInstancesToCull.RemoveAll();
	CreateSampleStates();
}

void CEngine::Screendump(const std::string& aPath)
{
	std::stringstream ss;
	static int counter = 0;
	ss << aPath.c_str() << "\\Screenshot_" << counter << ".png";
	std::string str = ss.str();
	std::wstring ws(str.begin(), str.end());
	DirectX::SaveWICTextureToFile(myFramework.GetContext(), myGraphicsEngine.GetLastFrameTexture(), GUID_ContainerFormatPng, ws.c_str());


}

const Vector2int & CEngine::GetWindowSize() const
{
	return myWindowSize;
}


void CEngine::LoadCameras()
{
	CCameraInstance::SCameraData camData;
	//float ratio = 16.f / 9.f;
	camData.myFoV = (Corgi::RADIANS(60.f));
	camData.myPosition = { 0.f, 0.f, 0.f };

	myGlobalCamera = myCameraLoader.CreateCameraInstance(camData, &myFramework);
}

void CEngine::CreateSampleStates()
{


}



void CEngine::UpdateCameraMovement()
{
#pragma region CameraMovement
	Matrix44 movement;
	float speed = 10.f;

	movement = myGlobalCamera->GetOrientation();

	if (InputManager::GetInstance().CheckKeyDown(KeyCode::W))
	{
		movement.myMatrix[14] += speed * Timer::GetInstance().GetDeltaTime();
		myGlobalCamera->SetOrientation(movement);
	}
	if (InputManager::GetInstance().CheckKeyDown(KeyCode::A))
	{
		movement.myMatrix[12] -= speed * Timer::GetInstance().GetDeltaTime();
		myGlobalCamera->SetOrientation(movement);
	}
	if (InputManager::GetInstance().CheckKeyDown(KeyCode::S))
	{
		movement.myMatrix[14] -= speed * Timer::GetInstance().GetDeltaTime();
		myGlobalCamera->SetOrientation(movement);
	}
	if (InputManager::GetInstance().CheckKeyDown(KeyCode::D))
	{
		movement.myMatrix[12] += speed * Timer::GetInstance().GetDeltaTime();
		myGlobalCamera->SetOrientation(movement);
	}

	if (InputManager::GetInstance().CheckKeyDown(KeyCode::Spacebar))
	{
		movement.myMatrix[13] += speed * Timer::GetInstance().GetDeltaTime();
		myGlobalCamera->SetOrientation(movement);
	}
	if (InputManager::GetInstance().CheckKeyDown(KeyCode::LeftAlt))
	{
		movement.myMatrix[13] -= speed * Timer::GetInstance().GetDeltaTime();
		myGlobalCamera->SetOrientation(movement);
	}

	//Rotations
	if (InputManager::GetInstance().CheckKeyDown(KeyCode::Q))
	{
		movement *= movement.CreateRotateAroundY(-0.2f * Timer::GetInstance().GetDeltaTime());
		myGlobalCamera->SetOrientation(movement);
	}
	if (InputManager::GetInstance().CheckKeyDown(KeyCode::E))
	{
		movement *= movement.CreateRotateAroundY(0.2f * Timer::GetInstance().GetDeltaTime());
		myGlobalCamera->SetOrientation(movement);
	}

	//if (InputManager::GetInstance().CheckKeyDown(KeyCode::Ctrl))
	//{
	//	movement *= movement.CreateRotateAroundX(-0.2f * Timer::GetInstance().GetDeltaTime());
	//	myGlobalCamera->SetOrientation(movement);
	//}
	//if (InputManager::GetInstance().CheckKeyDown(KeyCode::Shift))
	//{
	//	movement *= movement.CreateRotateAroundX(0.2f * Timer::GetInstance().GetDeltaTime());
	//	myGlobalCamera->SetOrientation(movement);
	//}

#pragma endregion CameraMovement
#pragma region WorldLightMovement

	//Vector4f position = myWorldLight->GetPosition();

	//if (InputManager::GetInstance().CheckKeyDown(KeyCode::X))
	//{
	//	Matrix44 orientation = myWorldLight->GetOrientation();
	//	orientation *= Matrix44::CreateRotateAroundY(1.f * Timer::GetInstance().GetDeltaTime());
	//	myWorldLight->SetOrientation(orientation);
	//}
	//if (InputManager::GetInstance().CheckKeyDown(KeyCode::Z))
	//{
	//	Matrix44 orientation = myWorldLight->GetOrientation();
	//	orientation *= Matrix44::CreateRotateAroundY(-1.f * Timer::GetInstance().GetDeltaTime());
	//	myWorldLight->SetOrientation(orientation);
	//}
	//
	//if (InputManager::GetInstance().CheckKeyDown(KeyCode::V))
	//{
	//	Matrix44 orientation = myWorldLight->GetOrientation();
	//	orientation *= Matrix44::CreateRotateAroundX(1.f * Timer::GetInstance().GetDeltaTime());
	//	myWorldLight->SetOrientation(orientation);
	//}
	//if (InputManager::GetInstance().CheckKeyDown(KeyCode::C))
	//{
	//	Matrix44 orientation = myWorldLight->GetOrientation();
	//	orientation *= Matrix44::CreateRotateAroundX(-1.f * Timer::GetInstance().GetDeltaTime());
	//	myWorldLight->SetOrientation(orientation);
	//}

	//if (InputManager::GetInstance().CheckKeyDown(KeyCode::Up))
	//{
	//	position.z += 10 * Timer::GetInstance().GetDeltaTime();
	//}
	//if (InputManager::GetInstance().CheckKeyDown(KeyCode::Down))
	//{
	//	position.z -= 10 * Timer::GetInstance().GetDeltaTime();
	//}
	//if (InputManager::GetInstance().CheckKeyDown(KeyCode::Right))
	//{
	//	position.x += 10 * Timer::GetInstance().GetDeltaTime();
	//}
	//if (InputManager::GetInstance().CheckKeyDown(KeyCode::Left))
	//{
	//	position.x -= 10 * Timer::GetInstance().GetDeltaTime();
	//}
	//if (InputManager::GetInstance().CheckKeyDown(KeyCode::MouseLeft) && InputManager::GetInstance().GetDeltaMousePos().y > 0)
	//{
	//	position.y += 10 * Timer::GetInstance().GetDeltaTime();
	//}
	//if (InputManager::GetInstance().CheckKeyDown(KeyCode::MouseLeft) && InputManager::GetInstance().GetDeltaMousePos().y < 0)
	//{
	//	position.y -= 10 * Timer::GetInstance().GetDeltaTime();
	//}
	//if (InputManager::GetInstance().CheckKeyDown(KeyCode::MouseRight) && InputManager::GetInstance().GetDeltaMousePos().y > 0)
	//{
	//	position.y += 100 * Timer::GetInstance().GetDeltaTime();
	//}
	//if (InputManager::GetInstance().CheckKeyDown(KeyCode::MouseRight) && InputManager::GetInstance().GetDeltaMousePos().y < 0)
	//{
	//	position.y -= 100 * Timer::GetInstance().GetDeltaTime();
	//}
	//
	//
	//myWorldLight->SetPosition(position);
	//position.y += 0.5f;
	//myLightPositionIndicator->SetOrientation(myWorldLight->GetOrientation());
	
#pragma endregion WorldLightMovement

}


