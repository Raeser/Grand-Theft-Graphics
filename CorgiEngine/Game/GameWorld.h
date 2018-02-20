#pragma once
#include <vector>
#include "..\Utilities\PlaneCollider.h"
#include "..\Engine\Subscriber.h"
#include "..\Component\EntityBuilder.h"

#include "..\Collision\TriggerVolumeManager.h"


#include "Spline.h"

class UnityExporter;
class CEngine;
class CGameConsole;
class CModelInstance;
class CEntity;
class CDataManager;
class CTikiWallCollider;
class CGameWorld 
	: public CSubscriber
{
public:
	CGameWorld(CEngine& aEngine, CGameConsole& aConsole, UnityExporter& aUnityExporter);
	virtual ~CGameWorld();

	void Init(int aLevelIndex);
	void InitFirstLevel(int aLevelIndex);
	bool LoadLevel(const char * aLevelName);
	bool LoadLevel(const int & aLevelNumber);
	void LoadLevel(const UnityExporter::Level & aLevel);

	void CreateLevel(const UnityExporter::Level& aLevel);
	void SetUpCamera();
	void ChangeLevelOnNextUpdate(const int aNumber);
	void ChangeLevelOnNextUpdate(const char * aLevelName);

	void ChangeNavMeshOnNextUpdate(const char * aNavMeshName);

	void Update();

	void Destroy();

	void BuildTikiEntity(Corgi::Vector3<float> aPos);

	CPlaneCollider GetPlane();
	CEntity * GetPlayerEntity();

	Corgi::Vector3<float> GetWaterSoundPoint(Corgi::Vector3<float> aLineStart, Corgi::Vector3<float> aLineEnd);

private:

	Corgi::Vector3<float> myFirstLinePoint;
	Corgi::Vector3<float> mySecondLinePoint;


	void PushVictory();

	void TikiKiller(const char* id);
	void BuildPlayerEntity(const Vector3f& aPostion = Vector3f());

	void RecieveMessage(const SMessage& message) override;

	Corgi::CMemoryPool<CEntity> myEntityPool;

	CPlaneCollider myPlane;
	CEntityBuilder myEntityBuilder;
	
	long long myPlayerID;
	int myWwiseIndex;
	int myCurrentLevel;

	int myRiverSoundIndex;

	CEngine& myGameEngine;
	UnityExporter& myUnityExporter;
	CGameConsole& myGameConsole;
	CTriggerVolumeManager& myTriggerManager;

	CModelInstance* myPlaneModel;
	CDataManager* myDataManager;


	Corgi::GrowingArray<long long> myUpdateableEntities;

	CComponentBuilder myComponentBuilder; // Has to be here since componentbuilder is dependent on another declared class

	bool myShouldChangeLevel;
	std::string myLevelToChangeTo;

	bool myNavMeshShouldChange;
	std::string myNavMeshToChangeTo;

	int myLevelIndesxToChangeTo;

	std::string myCurrentLevelName;

	bool myShouldDrawWallColliders;
	Corgi::GrowingArray<CTikiWallCollider*> myWallColliders;

};

