#pragma once

#include "WindowHandler.h"
#include "DXFramework.h"
#include "CameraLoader.h"
#include "GraphicsEngine.h"
#include "FullscreenTexture.h"
#include "SpriteLoader.h"
#include "ShaderManager.h"
#include "TextManager.h"
#include "AnimationLoader.h"
#include "ResourceManager.h"
#include "AnimationManager.h"
#include <atomic>

#include "..\Utilities\Vector.h"

class CScene;
class CCameraInstance;
class CSpriteInstance;
enum ModelLoaderSpace;
class CParticleManager;

namespace Corgi
{
	class CFileWatcher;
}

class CEngine
{
public:
	CEngine();
	~CEngine();
	bool Init(const CWindowHandler::WindowData& aWindowData);

	CModelLoader& GetModelLoader() { return myModelLoader; }
	CLightLoader& GetLightLoader() { return myLightLoader; }
	CSpriteLoader& GetSpriteLoader() { return mySpriteLoader; }
	CParticleEmitterLoader& GetParticleEmittorLoader() { return myParticleEmittorLoader; }
	CTextManager& GetTextManager() { return myTextManager; }
	CResourceManager& GetResourceManager() { return myResourceManager; }

	long long CreateModel(const char* aModelPath);
	long long CreateCube();
	long long CreateTriangle();
	CModelInstance* CreateInstance();
	CModelInstance* CreateInstance(const char* aPath);
	CModelInstance* CreateInstance(long long aID);

	long long CreateText(std::string& aTextRefOut, Vector2f aPosition, FontID aFont = 0, Corgi::Vector4<float> aColor = { 1,1,1,1 }, float aScale = 1.f, eTextType aType = eText_Normal, int aScrollingStartIndex = 0);

	void Update();
	void BeginFrame();
	void EndFrame();
	
	void RenderFrame();
	//CSpriteRenderer* GetSpriteRenderer() { return &mySpriteRenderer; }
	CDXFramework* GetDXFrameWork() { return &myFramework; }
	CCameraInstance* GetGlobalCamera();
	void ToggleVSync();
	bool GetIsVsyncEnabled();

	void CleanupGameScene();

	void Screendump(const std::string& aPath);

	const Vector2int& GetWindowSize() const;
	void UpdateCameraMovement();

private:
	struct SDebugFeatures
	{
		std::atomic<bool>fileWatcherOn;
	};

private:
	void LoadCameras();
	void CreateSampleStates();
	void LoadLights();


	long long mySkyboxID;
	SDebugFeatures myDebugFeatures;
	Vector2int myWindowSize; //FIX This will be wrong if player changes resolution 
	CResourceManager myResourceManager;
	CShaderManager myShaderManager;

	CWindowHandler myWindowHandler;
	CDXFramework myFramework;


	CCameraLoader myCameraLoader;
	CModelLoader myModelLoader;
	CSpriteLoader mySpriteLoader;
	CLightLoader myLightLoader;
	CParticleEmitterLoader myParticleEmittorLoader;
	CAnimationLoader myAnimationLoader;
	CAnimationManager myAnimationManager;

	CParticleManager* myParticleManager; //pointer because of std::vector include inside
	CTextManager myTextManager;
	CScene* myScene;

	long long myLeftScreenID;
	long long myMiddleScreenID;
	long long myRightScreenID;

	CCameraInstance* myGlobalCamera;
	CGraphicsEngine myGraphicsEngine;

	//Test Variables
	FontID myDefaultFontID;
	std::string myTestText;
	long long myCorgi;
	Corgi::GrowingArray<CSpriteInstance*> spInstances;
	Corgi::GrowingArray<CParticleEmitterInstance*> myParticleEmittorInstancesToCull;

};

