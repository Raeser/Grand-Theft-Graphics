#include "stdafx.h"
#include "..\Component\ComponentLoader.h"
#include "..\Engine\Engine.h"

#define RENDERCOMPAMOUNTGLOBAL	100000
#define RENDERCOMPAMOUNTLOCAL	50
#define CAMERACOMPAMOUNT		5
#define POSCOMPAMOUNT			10000
#define SOUNDCOMPAMOUNT			10000
#define WEAPONCOMPONENTS		10000

CComponentLoader::CComponentLoader(CEngine & aEngine) :
	myEngine(aEngine)
{

	myManager.Init();
	//myPosComponents.Init(POSCOMPAMOUNT);
	myGlobalRenderComponents.Init(RENDERCOMPAMOUNTGLOBAL);
	myLocalRenderComponents.Init(RENDERCOMPAMOUNTLOCAL);
	myCameraComponents.Init(CAMERACOMPAMOUNT);
	mySoundComponents.Init(SOUNDCOMPAMOUNT);
	myCollisionComponents.Init(10000);

	for (int i = 0; i < RENDERCOMPAMOUNTGLOBAL; ++i)
	{
		myGlobalRenderComponents[i].SetModelInstance(myEngine.CreateInstance(ModelLoaderSpace::Global));
	}
	for (int i = 0; i < RENDERCOMPAMOUNTLOCAL; ++i)
	{
		myLocalRenderComponents[i].SetModelInstance(myEngine.CreateInstance(ModelLoaderSpace::Local));
	}
	for (int i = 0; i < CAMERACOMPAMOUNT; ++i)
	{
		myCameraComponents[i].SetCameraInstance(myEngine.GetGlobalCamera()); //Move Cameras To someplace better!
	}
}

CComponentLoader::~CComponentLoader()
{
}

CRenderComponent * CComponentLoader::GetRenderComponent(const char * aPath, ModelLoaderSpace aSpace)
{
	CRenderComponent* componentToReturn;
	
	switch (aSpace)
	{
	case Global:
		componentToReturn = &myGlobalRenderComponents[myGlobalRenderComponents.GetFreeItem()];
		break;
	case Local:
		componentToReturn = &myLocalRenderComponents[myLocalRenderComponents.GetFreeItem()];
		break;
	default:
		componentToReturn = &myGlobalRenderComponents[myGlobalRenderComponents.GetFreeItem()];
		break;
	}

	
	long long modelLoadedID = myEngine.CreateModel(aPath);

	componentToReturn->SetNewModel(modelLoadedID);

	myManager.AddComponent(componentToReturn);

	return componentToReturn;
}


CCameraComponent * CComponentLoader::GetCameraComponent()
{
	CCameraComponent* componentToReturn = &myCameraComponents[myCameraComponents.GetFreeItem()];
	myManager.AddComponent(componentToReturn);
	return componentToReturn;
}

CSoundComponent * CComponentLoader::GetSoundComponent()
{
	CSoundComponent* componentToReturn = &mySoundComponents[mySoundComponents.GetFreeItem()];
	myManager.AddComponent(componentToReturn);
	return componentToReturn;
}


CCollisionComponent * CComponentLoader::GetCollisionComponent()
{
	return &myCollisionComponents[myCollisionComponents.GetFreeItem()];
}

void CComponentLoader::Update()
{
	myManager.Update();
}
