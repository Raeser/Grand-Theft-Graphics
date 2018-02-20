#pragma once
//#include "stdafx.h"  <---LOL#include "../Utilities/JsonDocument.h"

#include <atomic> 
#include <unordered_set>


enum class EExportType;


struct SUnityExporterAsteroid;
struct SUnityExporterEnemy;
struct SUnityExporterStructure;
struct SUnityExporterObject;
struct SUNityExporterMovableObject;
struct SUnityExportCheckPoint;
struct SUnityExporterPickup;
struct SUnityExporterTrigger;
struct SUnityExporterPointLight;
struct SUnityExporterSpotLight;
struct SUnityExporterWave;
struct SUnityExporterParticleEmittor;
struct SUnityExporterAudioSplines;
struct SUnityExporterData;
struct SUnityExporterColliderNavMesh;
struct SUnityExporterColliderWall;

struct SUnityExportCounts
{
	int myAsteroidCount = 0;
	int myEnemyCount = 0;
	int myStructureCount = 0;
	int myObjectCount = 0;
	int myMovableObjectCount = 0;
	int myCheckPointCount = 0;
	int myPickupsCount = 0;
	int myTriggerCounts = 0;
	int myParticleEmittorCount = 0;
	int myPointLightCounts = 0;
	int myWavesCounts = 0;
	int mySpotLightCounts = 0;
	int myAudioSplinesCount = 0;
	int myDataCount = 0;
	int myColliderCount = 0;

};


class UnityExporter
{
public:
	struct Level
	{
		//Corgi::GrowingArray<SUnityExporterAsteroid> myAsteroidArray;
		//Corgi::GrowingArray<SUnityExporterEnemy> myEnemyArray;
		//Corgi::GrowingArray<SUnityExporterStructure> myStructureArray;
		Corgi::GrowingArray<SUnityExporterObject> myObjectArray;
		Corgi::GrowingArray<SUNityExporterMovableObject> myObjectMovableArray;
		//Corgi::GrowingArray<SUnityExportCheckPoint> myCheckPointArray;
		//Corgi::GrowingArray<SUnityExporterPickup> myPickupArray;
		Corgi::GrowingArray<SUnityExporterTrigger> myTriggerArray;
		Corgi::GrowingArray<SUnityExporterParticleEmittor> myParticleEmittorArray;
		Corgi::GrowingArray<SUnityExporterPointLight> myPointLightArray;
		Corgi::GrowingArray<SUnityExporterSpotLight> mySpotLightArray;
		Corgi::GrowingArray<SUnityExporterAudioSplines> myAudioSplinesArray;
		Corgi::GrowingArray<SUnityExporterData> myDataPointArray;
		Corgi::GrowingArray<SUnityExporterColliderNavMesh> myColliderNavArray;
		Corgi::GrowingArray<SUnityExporterColliderWall> myColliderWallArray;
		//Corgi::GrowingArray<SUnityExporterWave> myWaveArray;
		std::string myLevelName;
		int myTotalInstances = 0;
	};
public:
	UnityExporter();
	~UnityExporter();

	// FilePath to Unity export folder from solution workbed
	// Example: "../Assets/Data/UnityExport/"
	void Init(const char * aFilePathExportFolder, const Corgi::GrowingArray<std::string>& aStringsOfJsonLevels);
	void CopyUniqueModelsPathsSetToVector();
	const bool GetLevel(Level& aLevel, const int& number);
	const bool GetLevel(Level& aLevel, const char * aLevelName) const;
	const Corgi::GrowingArray<UnityExporter::Level>& GetAllLevels() const;
	Corgi::GrowingArray<SUnityExporterColliderWall>& GetCollidersWall();
	SUnityExportCounts & GetCount();
	bool GetIsReady();
	Corgi::GrowingArray<std::string>& GetAllUniqueModelsPaths();
private:
	void ImportAll();
	void LoadModels();
	bool LoadFile(const char* aJsonFileToExport, const EExportType& aExportType);
	void ExportObjects(JsonValue & myJsonValue, const int& aIndex);
	void ExportMovableObject(JsonValue & aJsonValue, const int & aIndex);
	void ExportTriggers(JsonValue & aJsonValue, const int & aIndex);
	void ExportPointLight(JsonValue & aJsonValue, const int & aIndex);
	void ExportParticleEmittor(JsonValue & aJsonValue, const int & aIndex);
	void ExportSpotLight(JsonValue & aJsonValue, const int & aIndex);
	void ExportAudioSplines(JsonValue & aJsonValue, const int & aIndex);
	void ExportDataPoints(JsonValue & aJsonValue, const int & aIndex);

	void ExportColliders(JsonValue & aJsonValue, const int & aIndex, const char* colliderType);
	
	bool IsStringValid(JsonValue & currentJsonValue, int aIndex, const char * aStringToFind, const char * aStringCurrentlyExporting);
	
	Corgi::GrowingArray<SUnityExporterAsteroid>& GetAsteroids();
	Corgi::GrowingArray<SUnityExporterEnemy>& GetEnemys();
	Corgi::GrowingArray<SUnityExporterStructure>& GetStructures();
	Corgi::GrowingArray<SUnityExporterObject>& GetObjects();
	Corgi::GrowingArray<SUNityExporterMovableObject>& GetMovableObjects();
	Corgi::GrowingArray<SUnityExportCheckPoint>& GetCheckPoints();
	Corgi::GrowingArray<SUnityExporterPickup>& GetPickups();
	Corgi::GrowingArray<SUnityExporterTrigger>& GetTriggers();
	Corgi::GrowingArray<SUnityExporterParticleEmittor>& GetParticleEmittors();
	Corgi::GrowingArray<SUnityExporterPointLight>& GetPointLights();
	Corgi::GrowingArray<SUnityExporterSpotLight>& GetSpotLights();
	Corgi::GrowingArray<SUnityExporterWave>& GetWaves();
	Corgi::GrowingArray<SUnityExporterAudioSplines>& GetAudioSplines();
	Corgi::GrowingArray<SUnityExporterData>& GetDataPoints();
	Corgi::GrowingArray<SUnityExporterColliderNavMesh>& GetCollidersNav();


	void ConvertTriggerEventStringToIDs();
	JsonDocument myJsonDocument;
	std::string myCurrentJsonFile;
	const char* myFilePathToExportFolder;
	bool myIsInitiated;

	SUnityExportCounts myDataCount;

	Corgi::GrowingArray<SUnityExporterAsteroid> myCurrentAsteroidArray;
	Corgi::GrowingArray<SUnityExporterEnemy> myCurrentEnemyArray;
	Corgi::GrowingArray<SUnityExporterStructure> myCurrentStructureArray;
	Corgi::GrowingArray<SUnityExporterObject> myCurrentObjectArray;
	Corgi::GrowingArray<SUNityExporterMovableObject> myCurrentMovableObjectArray;
	Corgi::GrowingArray<SUnityExportCheckPoint> myCurrentCheckPointArray;
	Corgi::GrowingArray<SUnityExporterPickup> myCurrentPickupArray;
	Corgi::GrowingArray<SUnityExporterTrigger> myCurrentTriggerArray;
	Corgi::GrowingArray<SUnityExporterParticleEmittor> myCurrentParticleEmittorArray;
	Corgi::GrowingArray<SUnityExporterPointLight> myCurrentPointLightArray;
	Corgi::GrowingArray<SUnityExporterSpotLight> myCurrentSpotLightArray;
	Corgi::GrowingArray<SUnityExporterWave> myCurrentWaveArray;
	Corgi::GrowingArray<SUnityExporterAudioSplines> myCurrentAudioSplinesArray;
	Corgi::GrowingArray<SUnityExporterData> myCurrentDataPointsArray;
	Corgi::GrowingArray<SUnityExporterColliderNavMesh> myCurrentCollidersNavArray;
	Corgi::GrowingArray<SUnityExporterColliderWall> myCurrentCollidersWallArray;

	//TODO: Should Be Removed or changed after first projekt
	Corgi::GrowingArray < std::string> myStringsOfJsonLevels;

	Corgi::GrowingArray<std::string> myUniqueModelsPaths;
	std::unordered_set<std::string> myUniqueModelsPathsSet;

	Corgi::GrowingArray<UnityExporter::Level> myLevels;
	std::atomic<bool> myIsReadyToUse;

	Corgi::GrowingArray<bool> myInitedBools;
};

