#include "stdafx.h"
#include "LevelBuilder.h"
#include <chrono>
#include <iostream>
#include <thread>


LevelBuilder::LevelBuilder(CComponentBuilder& aComonentLoader, Corgi::CMemoryPool<CEntity>& aObjectPool, CInGameState* aInGameState, CEngine& aEngine, UnityExporter& aUnityExporter) :
	myComponentBuilderRef(aComonentLoader),
	myEntityBuilder(aObjectPool, aEngine),
	myEngine(aEngine),
	myUnityExporter(aUnityExporter)
{
	aInGameState;
}


LevelBuilder::~LevelBuilder()
{
}

bool LevelBuilder::Init()
{
	DL_WRITELOG(GREEN, eDebugLogType::Resource, "%s", "[LEVELBUILDER] - Building Levels \n ==========================================================================================");
	myEntityBuilder.Init(&myComponentBuilderRef);
	return true;
}


bool LevelBuilder::BuildLevel()
{
	while (myUnityExporter.GetIsReady() == false)
	{
		using namespace std::chrono_literals;
		std::this_thread::sleep_for(1ms);
	}
	//for each (auto Level in myUnityExporter.GetAllLevels())
	//{
	//	CreateLevelFromUnity(Level.myObjectArray,
	//		Level.myTriggerArray);
	//}
	return true;
}


CEntityBuilder * LevelBuilder::GetEntityBuilder()
{
	return &myEntityBuilder;
}

long long LevelBuilder::BuildPlayer(CCameraInstance* aGlobalCam)
{
	aGlobalCam;
	SPlayerData playerData = CDataManager::GetInstance().GetPlayerData();
	long long aPlayerID = 123;// = myEntityBuilder.BuildPlayer(aGlobalCam);
	return aPlayerID;
}

bool LevelBuilder::CreateLevelFromUnity(
	 const Corgi::GrowingArray<SUnityExporterEnemy>& aEnemyStrucDataArray
	, const Corgi::GrowingArray<SUnityExporterObject>& aObjectStructDataArray
	, const Corgi::GrowingArray<SUnityExporterTrigger>& aTriggerStructDataArray)
{
	aEnemyStrucDataArray;
	/*for each (auto Enemy in aEnemyStrucDataArray)
	{
		myEntityBuilder.BuildEnemy(Enemy.fbxFilePaths, Enemy.position, Enemy.rotation,
			Enemy.colliderHit.localPosition, Enemy.colliderHit.radius,
			Enemy.colliderAimAssist.localPosition, Enemy.colliderAimAssist.radius,
			Enemy.colliderTrigger.localPosition, Enemy.colliderTrigger.radius);
	}*/
	for each (auto Object in aObjectStructDataArray)
	{
		myEntityBuilder.BuildObject(Object.fbxFilePaths, Object.position, Object.rotation, true);
	}
	
	for each (auto Trigger in aTriggerStructDataArray)
	{
		myEntityBuilder.BuildTrigger(Trigger.position, Trigger.sphereCollider.radius, Trigger.luaFunctionToCall.c_str());
	}
	return true;
}
