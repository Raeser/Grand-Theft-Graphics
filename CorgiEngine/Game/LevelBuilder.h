#pragma once

#include "UnityExporter.h"
#include "..\Component\ComponentBuilder.h"
#include "..\Component\EntityBuilder.h"

class CInGameState;
class CEngine;

class LevelBuilder
{
public:
	LevelBuilder(CComponentBuilder& aComonentLoader, Corgi::CMemoryPool<CEntity>& aObjectPool, CInGameState* aInGameState, CEngine& aEngine, UnityExporter& aUnityExporter);
	~LevelBuilder();
	bool Init();
	bool BuildLevel();
	long long BuildPlayer(CCameraInstance* aCameraInstance);

	CEntityBuilder* GetEntityBuilder();

	bool CreateLevelFromUnity(
		  const Corgi::GrowingArray<SUnityExporterEnemy>& aEnemyStrucDataArray
		, const Corgi::GrowingArray<SUnityExporterObject>& aObjectStructDataArray
		, const Corgi::GrowingArray<SUnityExporterTrigger>& aTriggerStructDataArray);

private:
	CEngine& myEngine;
	UnityExporter& myUnityExporter;
	CComponentBuilder& myComponentBuilderRef;
	CEntityBuilder myEntityBuilder;
};

