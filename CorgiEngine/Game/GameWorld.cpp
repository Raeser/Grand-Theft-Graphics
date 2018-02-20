#include "stdafx.h"

#include "UnityExporter.h"

#include "..\Engine\ModelLoaderWrapper.h"
#include "..\Engine\ModelInstance.h"
#include "..\Engine\LightLoader.h"
#include "..\Engine\PointLightInstance.h"
#include "..\Engine\PointLight.h"
#include "..\Engine\SpotLightInstance.h"
#include "..\Engine\SpotLight.h"
#include "..\Engine\TikiWallCollider.h"

#include "..\Collision\TriggerSphere.h"
#include "..\Collision\TriggerBox.h"

#include "..\Engine\DebugDrawer.h"
#include "..\Launcher\LuaExposedVariableWrapper.h"

CGameWorld::CGameWorld(CEngine& aEngine, CGameConsole& aConsole, UnityExporter& aUnityExporter)
	: 
	myGameEngine(aEngine), 
	myGameConsole(aConsole),
	myEntityBuilder(myEntityPool, aEngine),
	myComponentBuilder(aEngine),
	myUnityExporter(aUnityExporter),
	myTriggerManager(CTriggerVolumeManager::GetInstance())
{
}

CGameWorld::~CGameWorld()
{
	CPostMaster::GetInstance().Unsubscribe(this, eMessageType::DebugDrawWallColliders);
	CPostMaster::GetInstance().Unsubscribe(this, eMessageType::CreateParticleEmitter);
}

void CGameWorld::Init(int aLevelIndex)
{
	myShouldDrawWallColliders = false;
	myLevelIndesxToChangeTo = -1;
	myComponentBuilder.Init();
	myEntityBuilder.Init(&myComponentBuilder);


	myShouldChangeLevel = false;
	myLevelToChangeTo = "";

	myNavMeshShouldChange = false;
	myNavMeshToChangeTo = "";

	myCurrentLevelName = "";

	myUpdateableEntities.Init(4092);

	myEntityPool.Init(4096);

	myDataManager = &CDataManager::GetInstance();

	InitFirstLevel(aLevelIndex);


	//myPlaneModel = myGameEngine.GetResourceManager().GetModelLoaderWrapper()->CreateInstance("../Assets/Models/groundSurface/groundSurface.fbx", ModelType::Model);
	myPlane.Init({ 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f });

	//myPlateManager.Init([&] {PushVictory(); });
	BuildPlayerEntity();

	SetUpCamera();
	CPostMaster::GetInstance().Subscribe(this, eMessageType::CreateParticleEmitter);
	CPostMaster::GetInstance().Subscribe(this, eMessageType::DebugDrawWallColliders);

	LuaExposedVariableWrapper::GetInstance().InitPointers(&myEntityPool, &myUpdateableEntities);
}

void CGameWorld::InitFirstLevel(int aLevelIndex)
{
	const Corgi::GrowingArray<UnityExporter::Level>& levels = myUnityExporter.GetAllLevels();
	if (aLevelIndex > levels.Size() - 1 || aLevelIndex < 0)
	{
		aLevelIndex = 0;
	}

	myWallColliders.Init(levels[aLevelIndex].myColliderWallArray.Size());
	for (int i = 0; i < levels[aLevelIndex].myColliderWallArray.Size(); ++i)
	{
		// DETTA SKAPAR EN MINNESLÄCKA !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! FUL FIX INNAN INLÄMNING.
		CTikiWallCollider* aWallCollider = new CTikiWallCollider();
		myWallColliders.Add(aWallCollider);
		myWallColliders.GetLast()->Init(
			levels[aLevelIndex].myColliderWallArray[i].position,
			levels[aLevelIndex].myColliderWallArray[i].rotation,
			levels[aLevelIndex].myColliderWallArray[i].size,
			levels[aLevelIndex].myColliderWallArray[i].IDString.c_str());
	}


	CreateLevel(levels[aLevelIndex]);

	myCurrentLevel = aLevelIndex;
	myCurrentLevelName = levels[aLevelIndex].myLevelName;
}


void CGameWorld::Destroy()
{


	for (CEntity& ent : myEntityPool)
	{
		ent.Release();
	}


	myComponentBuilder.Update();
	myComponentBuilder.Reset();
	myEntityBuilder.Reset();
	myTriggerManager.Reset();
	myEntityPool.RemoveAll();
	myUpdateableEntities.RemoveAll();
	//myNavmesh->Reset();
	myGameEngine.CleanupGameScene();

	SMessage resetmessage;
	resetmessage.messageType = eMessageType::Reset;
	CPostMaster::GetInstance().PostMessageA(resetmessage);

	CPostMaster::GetInstance().Unsubscribe(this, eMessageType::CreateParticleEmitter);
	CPostMaster::GetInstance().Unsubscribe(this, eMessageType::DebugDrawWallColliders);

	LuaExposedVariableWrapper::GetInstance().ResetPointers();

	CWwiseWrapper::GetInstance().PostEventToAllIDs("Stop_SFX");

	CTriggerVolumeManager::GetInstance().Reset();
}

void CGameWorld::LoadLevel(const UnityExporter::Level& aLevel)
{
	Destroy();
	myCurrentLevelName = aLevel.myLevelName;

	myWallColliders.ReInit(aLevel.myColliderWallArray.Size());
	for (int i = 0; i < aLevel.myColliderWallArray.Size(); ++i)
	{
		// DETTA SKAPAR EN MINNESLÄCKA !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! FUL FIX INNAN INLÄMNING.
		CTikiWallCollider* aWallCollider = new CTikiWallCollider();
		myWallColliders.Add(aWallCollider);
		myWallColliders.GetLast()->Init(
			aLevel.myColliderWallArray[i].position,
			aLevel.myColliderWallArray[i].rotation,
			aLevel.myColliderWallArray[i].size,
			aLevel.myColliderWallArray[i].IDString.c_str());
	}



	CreateLevel(aLevel);
	if (aLevel.myLevelName == "jonaslevel.json")
	{
		BuildPlayerEntity(CDataManager::GetInstance().GetPlayerData().position);
	}
	else
	{
		BuildPlayerEntity();
	}
	

	LuaExposedVariableWrapper::GetInstance().InitPointers(&myEntityPool, &myUpdateableEntities);
	
	CPostMaster::GetInstance().Subscribe(this, eMessageType::CreateParticleEmitter);
	CPostMaster::GetInstance().Subscribe(this, eMessageType::DebugDrawWallColliders);
}

bool CGameWorld::LoadLevel(const char* aLevelName)
{
	UnityExporter::Level levelToCreate;
	if (myUnityExporter.GetLevel(levelToCreate, aLevelName))
	{
		LoadLevel(levelToCreate);
		return true;
	}
	DL_ERROR(eDebugLogType::Error, "%s%s", "[CONSOLE] - Couldnt find level ", aLevelName);
	return false;
}
bool CGameWorld::LoadLevel(const int& aLevelNumber)
{
	myCurrentLevel = aLevelNumber;
	UnityExporter::Level levelToCreate;
	if (myUnityExporter.GetLevel(levelToCreate, aLevelNumber))
	{
		if (levelToCreate.myLevelName == "")
		{
			assert(false);
		}
		LoadLevel(levelToCreate);
		return true;
	}
	DL_ERROR(eDebugLogType::Error, "%s%i", "[CONSOLE] - Couldnt find specific level number in your config: ", aLevelNumber);
	return false;
}

void CGameWorld::CreateLevel(const UnityExporter::Level& aLevel)
{
	DL_WRITELOG(eConsoleColor::LIGHTBLUE,eDebugLogType::Resource, "%s%s", "[GAMEWORLD] - Creating level : ", aLevel.myLevelName.c_str());
	for each (auto StaticObjectToCreate in aLevel.myObjectArray)
	{
		myEntityBuilder.BuildObject(StaticObjectToCreate.fbxFilePaths, StaticObjectToCreate.position, StaticObjectToCreate.rotation, true);
	}
	for each (auto particleEmittorToCreate in aLevel.myObjectMovableArray)
	{
		long long id = myEntityBuilder.BuildMovableObject(particleEmittorToCreate.fbxFilePaths, particleEmittorToCreate.position, particleEmittorToCreate.rotation, particleEmittorToCreate.speed, particleEmittorToCreate.objectID.c_str());
		myUpdateableEntities.Add(id);
	}
	// Magic Numbers in UnityExporter
	CSpotLightInstance* mySpotLightInstance;
	for each (auto SpotLightToCreate in aLevel.mySpotLightArray)
	{
		SSpotLightData spotLightData;
		spotLightData.myColor = SpotLightToCreate.color;
		spotLightData.myRange = SpotLightToCreate.range;
		spotLightData.myAngle = SpotLightToCreate.spotLightAngle;
		mySpotLightInstance = myGameEngine.GetLightLoader().CreateSpotLightInstance(myGameEngine.GetLightLoader().CreateSpotLight(spotLightData), SpotLightToCreate.position, SpotLightToCreate.rotation);
		mySpotLightInstance->SetPosition(SpotLightToCreate.position);
	}
	for each (auto PointLigthToCreate in aLevel.myPointLightArray)
	{
		myEntityBuilder.BuildPointLight(PointLigthToCreate.position, PointLigthToCreate.color, PointLigthToCreate.range);
	}

	myTriggerManager.AssignSpriteLoader(myGameEngine.GetSpriteLoader());
	myTriggerManager.AssignTextManager(myGameEngine.GetTextManager());

	for each (auto TriggerToCreate in aLevel.myTriggerArray)
	{
		if (TriggerToCreate.isBoxCollider)
		{
			myTriggerManager.AddTriggerBox(TriggerToCreate);
		}
		else
		{
			myTriggerManager.AddTriggerSphere(TriggerToCreate);
		}
	}

	for each (auto particleEmittorToCreate in aLevel.myParticleEmittorArray)
	{
		long long id = myEntityBuilder.BuildParticleEmitter(particleEmittorToCreate.position, particleEmittorToCreate.particleEmittorName.c_str(), particleEmittorToCreate.rotation, -1.0f);
		myUpdateableEntities.Add(id);
	}

	for (int i = 0; i < aLevel.myDataPointArray.Size(); i++)
	{
		if (aLevel.myDataPointArray[i].dataTypeString == "audio")
		{
			//aLevel.myDataPointArray[i].dataString =
		}
	}
		

}

void CGameWorld::SetUpCamera()
{
	myGameEngine.GetGlobalCamera()->SetOrientation(Matrix44::CreateRotateAroundX(myDataManager->GetEditorData().cameraAngleInDegrees * 3.1415927f / 180.f));
	Matrix44 movement;
	movement = myGameEngine.GetGlobalCamera()->GetOrientation();
	movement.myMatrix[12] += myDataManager->GetEditorData().cameraDistianceVectorToPlayer.x;
	movement.myMatrix[13] += myDataManager->GetEditorData().cameraDistianceVectorToPlayer.y;
	movement.myMatrix[14] += myDataManager->GetEditorData().cameraDistianceVectorToPlayer.z;
	myGameEngine.GetGlobalCamera()->SetOrientation(movement);
}

void CGameWorld::ChangeLevelOnNextUpdate(const int aNumber)
{
	myLevelIndesxToChangeTo = aNumber;
	myShouldChangeLevel = true;
}

void CGameWorld::ChangeLevelOnNextUpdate(const char* aLevelName)
{
	myLevelToChangeTo = aLevelName;
	myShouldChangeLevel = true;
}

void CGameWorld::ChangeNavMeshOnNextUpdate(const char* aNavMeshName)
{
	myNavMeshToChangeTo = aNavMeshName;
	myNavMeshShouldChange = true;
}


void CGameWorld::Update()
{
	if (myShouldChangeLevel)
	{
		myShouldChangeLevel = false;
		if (myLevelToChangeTo != "")
		{
			LoadLevel(myLevelToChangeTo.c_str());
			myLevelToChangeTo = "";
		}
		else if (myLevelIndesxToChangeTo != -1)
		{
			LoadLevel(myLevelIndesxToChangeTo);
			myLevelIndesxToChangeTo = -1;
		}
		else
		{
			myCurrentLevel += 1;
			if (myCurrentLevel >= myUnityExporter.GetAllLevels().Size())
			{
				myCurrentLevel = 0;
			}
			LoadLevel(myCurrentLevel);
		}
	}

	//myPlateManager.Update();

	for (long long i = 0; i < myEntityPool.Size(); ++i)
	{
		if (myEntityPool[i].ShouldRelease() && myEntityPool[i].GetActiveStatus())
		{
			myEntityPool[i].Release();
			myEntityPool.ReleaseObject(i);
		}
	}

	myComponentBuilder.Update();

	//TODO:
	//Render all colliders
	//Cull wall colliders, skapa cull funktion i gameWorld
	//Render culled colliders
	//Send to pathfinding

	for (long long id : myUpdateableEntities)
	{
		if (!myEntityPool[id].Update())
		{
			myUpdateableEntities.RemoveCyclic(id);
		}
	}
	
	myGameEngine.GetGlobalCamera()->SetPosition(myEntityPool[myPlayerID].GetTransform().GetPosition() + myDataManager->GetEditorData().cameraDistianceVectorToPlayer);
	
#ifndef _RETAIL
	if (InputManager::GetInstance().CheckKeyPressed(KeyCode::F5))
	{
		CScriptManager::GetInstance().ReloadFiles();
	}

	if (InputManager::GetInstance().CheckKeyPressed(KeyCode::Num1))
	{
		myEntityPool[myPlayerID].CallFunction("FetchSomeSeed", "I am currently looking for seed because player clicked");
	}
#endif

	myTriggerManager.Update(myEntityPool, myUpdateableEntities);
	CScriptManager::GetInstance().UpdateTimers();

	if (myShouldDrawWallColliders)
	{
		for (int i = 0; i < myWallColliders.Size(); ++i)
		{
			myWallColliders[i]->DrawWireFrame();
		}
	}

}

void CGameWorld::PushVictory()
{
	SMessage msg;
	msg.messageType = eMessageType::WonGame;
	CPostMaster::GetInstance().PostMessage(msg);
}



void CGameWorld::BuildPlayerEntity(const Vector3f& aPostion)
{
	myPlayerID = myEntityBuilder.BuildPlayer(aPostion, { 0, 0, 0 }, myPlane);
	myUpdateableEntities.Add(myPlayerID); 
}



CPlaneCollider CGameWorld::GetPlane()
{
	return myPlane;
}

//void CGameWorld::BuildButtonEntity(Corgi::Vector3<float> aPos)
//{
//	CModelInstance* renderComp = myGameEngine.GetResourceManager().GetModelLoaderWrapper()->CreateInstance("../Assets/Models/button/button.fbx", ModelType::Model);
//	renderComp->SetPosition(aPos);
//	renderComp->SetShouldRender(true);
//
//	CEntity* buttonEntity = new CEntity();
//	buttonEntity->GetTransform().SetPosition(aPos);
//
//	myTikiBoyes.push_back(buttonEntity);
//	myPlateManager.AddPressurePlate(buttonEntity);
//
//}

void CGameWorld::RecieveMessage(const SMessage & message)
{
	switch (message.messageType)
	{
	case eMessageType::CreateParticleEmitter:
		myUpdateableEntities.Add(
			myEntityBuilder.BuildParticleEmitter(
			{
				message.createParticleEmitter.x,
				message.createParticleEmitter.y,
				message.createParticleEmitter.z
			},
			message.createParticleEmitter.particlesystem,
			Vector3f(),
			message.createParticleEmitter.lifestime));
		break;
	case eMessageType::DebugDrawWallColliders:
		myShouldDrawWallColliders = !myShouldDrawWallColliders;
		break;
	default:
		break;
	}
}

CEntity* CGameWorld::GetPlayerEntity()
{
	assert(&myEntityPool[myPlayerID] != nullptr && "Player enity is null");
	return &myEntityPool[myPlayerID];
}


