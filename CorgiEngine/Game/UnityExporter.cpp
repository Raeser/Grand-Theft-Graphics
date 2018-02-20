#include "stdafx.h"
#include "UnityExporter.h"
#include "..\Component\TriggerManager.h"
#include <iostream>


#define LEVELLOADER_POINTLIGHTS_INWORLD_MAXCOUNT 64
#define LEVELLOADER_SPOTLIGHTS_INWORLD_MAXCOUNT 64

UnityExporter::UnityExporter()
{
}
UnityExporter::~UnityExporter()
{
}
void UnityExporter::Init(const char* aFilePathExportFolder,const Corgi::GrowingArray<std::string>& aStringsOfJsonLevels)
{
	myIsInitiated = true;
	myIsReadyToUse = false;
	myFilePathToExportFolder = aFilePathExportFolder;

	myInitedBools.Init(static_cast<int>(EExportTag::count));
	for (int i = 0; i < static_cast<int>(EExportTag::count); ++i)
	{
		myInitedBools.Add(false);
	}

	DL_WRITELOG(GREEN, eDebugLogType::UnityExporter, "%s", "[UNITY EXPORTER] - Begin \n ==========================================================================================");
	DL_WRITELOG(GREEN, eDebugLogType::UnityExporter, "%s", "[UNITY EXPORTER] - Loading Levels");

	myStringsOfJsonLevels.Init(aStringsOfJsonLevels.Size());
	//
	//va_list arguments;
	//for (va_start(arguments, aLevels); aLevels != NULL; aLevels = va_arg(arguments, const char *)) {
	//	myStringsOfJsonLevels.Add(aLevels);
	//}
	//va_end(arguments);
	myStringsOfJsonLevels = aStringsOfJsonLevels;
	ImportAll();
	DL_SUCCESS(eDebugLogType::UnityExporter, "%s", "[UnityExporter] Summary: ");
	DL_SUCCESS(eDebugLogType::UnityExporter, "%s%i", "[UnityExporter] Total Object count: ", myDataCount.myObjectCount);
	DL_SUCCESS(eDebugLogType::UnityExporter, "%s%i", "[UnityExporter] Total MovableObject count: ", myDataCount.myMovableObjectCount);
	DL_SUCCESS(eDebugLogType::UnityExporter, "%s%i", "[UnityExporter] Total Trigger count: ", myDataCount.myTriggerCounts);
	DL_SUCCESS(eDebugLogType::UnityExporter, "%s%i", "[UnityExporter] Total ParticleEmittor count: ", myDataCount.myParticleEmittorCount);
	DL_SUCCESS(eDebugLogType::UnityExporter, "%s%i", "[UnityExporter] Total SpotLight count: ", myDataCount.mySpotLightCounts);
	DL_SUCCESS(eDebugLogType::UnityExporter, "%s%i", "[UnityExporter] Total PointLight count: ", myDataCount.myPointLightCounts);
	DL_SUCCESS(eDebugLogType::UnityExporter, "%s%i", "[UnityExporter] Total AudiSplines count: ", myDataCount.myAudioSplinesCount);
	DL_SUCCESS(eDebugLogType::UnityExporter, "%s%i", "[UnityExporter] Total DataPoint count: ", myDataCount.myDataCount);
	DL_SUCCESS(eDebugLogType::UnityExporter, "%s%i", "[UnityExporter] Total Colliders count: ", myDataCount.myColliderCount);

	myIsReadyToUse = true;
	DL_WRITELOG(GREEN, eDebugLogType::UnityExporter, "%s", "[UNITY EXPORTER] - END \n ==========================================================================================");
	CopyUniqueModelsPathsSetToVector();
}

void UnityExporter::CopyUniqueModelsPathsSetToVector()
{
	myUniqueModelsPaths.Init(static_cast<int>(myUniqueModelsPathsSet.size()));
	for (const auto& aModelsPath : myUniqueModelsPathsSet) {
		myUniqueModelsPaths.Add(aModelsPath);
	}
}


void UnityExporter::ImportAll()
{
	myLevels.Init(myStringsOfJsonLevels.Size());
	for (int i = 0; i < myStringsOfJsonLevels.Size(); i++)
	{
		UnityExporter::Level LevelToCreate;

		LevelToCreate.myLevelName = myStringsOfJsonLevels[i];
		Corgi::ConvertStringToLowerCase(LevelToCreate.myLevelName);
		LevelToCreate.myObjectArray.Init(LEVELLOADER_OBJECTS_INWORLD_MAXCOUNT);
		LevelToCreate.myObjectMovableArray.Init(LEVELLOADER_OBJECTS_INWORLD_MAXCOUNT);
		LevelToCreate.myTriggerArray.Init(LEVELLOADER_TRIGGER_INWORLD_MAXCOUNT);
		LevelToCreate.myParticleEmittorArray.Init(LEVELLOADER_PARTICLEEMITTORS_INWORLD_MAXCOUNT);
		LevelToCreate.myPointLightArray.Init(LEVELLOADER_POINTLIGHTS_INWORLD_MAXCOUNT);
		LevelToCreate.mySpotLightArray.Init(LEVELLOADER_POINTLIGHTS_INWORLD_MAXCOUNT);
		LevelToCreate.myAudioSplinesArray.Init(LEVELLOADER_AUDIOSPLINES_INWORLD_MAXCOUNT);
		LevelToCreate.myDataPointArray.Init(LEVELLOADER_DATAPOINTS_INWORLD_MAXCOUNT);
		LevelToCreate.myColliderNavArray.Init(LEVELLOADER_COLLIDERS_INWORLD_MAXCOUNT);
		LevelToCreate.myColliderWallArray.Init(LEVELLOADER_COLLIDERS_INWORLD_MAXCOUNT);
		if (LoadFile(myStringsOfJsonLevels[i].c_str(), EExportType::EModel))
		{
			LevelToCreate.myObjectArray << GetObjects();
			LevelToCreate.myObjectMovableArray << GetMovableObjects();
			LevelToCreate.myTriggerArray << GetTriggers();
			LevelToCreate.myParticleEmittorArray << GetParticleEmittors();
			LevelToCreate.myPointLightArray << GetPointLights();
			LevelToCreate.mySpotLightArray << GetSpotLights();
			LevelToCreate.myAudioSplinesArray << GetAudioSplines();
			LevelToCreate.myDataPointArray << GetDataPoints();
			LevelToCreate.myColliderNavArray << GetCollidersNav();
			LevelToCreate.myColliderWallArray << GetCollidersWall();

			//LevelToCreate.myObjectArray << Get<SUnityExporterObject>(static_cast<int>(EExportTag::Object));
			//LevelToCreate.myObjectMovableArray << Get<SUNityExporterMovableObject>(static_cast<int>(EExportTag::MovableObject));
			//LevelToCreate.myTriggerArray << Get<SUnityExporterTrigger>(static_cast<int>(EExportTag::Trigger));
			//LevelToCreate.myParticleEmittorArray << Get<SUnityExporterParticleEmittor>(static_cast<int>(EExportTag::ParticleEmittor));
			//LevelToCreate.myPointLightArray << Get<SUnityExporterPointLight>(static_cast<int>(EExportTag::PointLight));
			//LevelToCreate.mySpotLightArray << Get<SUnityExporterSpotLight>(static_cast<int>(EExportTag::SpotLight));
		}
		LevelToCreate.myTotalInstances += 
			LevelToCreate.myObjectArray.Size() + 
			LevelToCreate.myObjectMovableArray.Size() +
			LevelToCreate.myParticleEmittorArray.Size() +
			LevelToCreate.myTriggerArray.Size() + 
			LevelToCreate.myPointLightArray.Size() + 
			LevelToCreate.mySpotLightArray.Size() +
			LevelToCreate.myAudioSplinesArray.Size() + 
			LevelToCreate.myDataPointArray.Size() +
			LevelToCreate.myColliderNavArray.Size();
		DL_SUCCESS(eDebugLogType::UnityExporter, "%s%i", "[UnityExporter] Object count: ", LevelToCreate.myObjectArray.Size());
		DL_SUCCESS(eDebugLogType::UnityExporter, "%s%i", "[UnityExporter] MovableObject count: ", LevelToCreate.myObjectMovableArray.Size());
		DL_SUCCESS(eDebugLogType::UnityExporter, "%s%i", "[UnityExporter] Trigger count: ", LevelToCreate.myTriggerArray.Size());
		DL_SUCCESS(eDebugLogType::UnityExporter, "%s%i", "[UnityExporter] ParticleEmittor count: ", LevelToCreate.myParticleEmittorArray.Size());
		DL_SUCCESS(eDebugLogType::UnityExporter, "%s%i", "[UnityExporter] SpotLight count: ", LevelToCreate.mySpotLightArray.Size());
		DL_SUCCESS(eDebugLogType::UnityExporter, "%s%i", "[UnityExporter] PointLight count: ", LevelToCreate.myPointLightArray.Size());
		DL_SUCCESS(eDebugLogType::UnityExporter, "%s%i", "[UnityExporter] AudiSplines count: ", LevelToCreate.myAudioSplinesArray.Size());
		DL_SUCCESS(eDebugLogType::UnityExporter, "%s%i", "[UnityExporter] DataPoint count: ", LevelToCreate.myDataPointArray.Size());
		DL_SUCCESS(eDebugLogType::UnityExporter, "%s%i", "[UnityExporter] Colliders count: ", LevelToCreate.myColliderNavArray.Size());
		DL_SUCCESS(eDebugLogType::UnityExporter, "%s%i", "[UnityExporter] Total Istances loaded from JsonFiles: ", LevelToCreate.myTotalInstances, "\n");
		myLevels.Add(LevelToCreate);
	}
	
}
const bool UnityExporter::GetLevel(Level& aLevel, const int& number)
{
	if (number >= myLevels.Size() || number < 0)
	{
		assert(false && "levelNameDoesntExcist");
		DL_ERROR(eDebugLogType::UnityExporter, "%s%i", "[UnityExporter] Unable to getlevel number: ", number, "\n");
		return false;
	}
	aLevel = myLevels[number];
	return true;
}

const Corgi::GrowingArray<UnityExporter::Level>& UnityExporter::GetAllLevels() const
{
	return myLevels;
}

const bool UnityExporter::GetLevel(Level& aLevel, const char* aLevelName) const
{
	std::string levelName(aLevelName);
	Corgi::ConvertStringToLowerCase(levelName);
	for (auto i = 0; i < myLevels.Size(); ++i)
	{
		if (myLevels[i].myLevelName == levelName)
		{
			aLevel = myLevels[i];
			return true;
		}
	}
	DL_ERROR(eDebugLogType::UnityExporter, "%s%s%s", "[UnityExporter] Unable to getlevel: ", levelName, "this level doesnt exists in unityexporter\n");
	return false;
}

bool UnityExporter::LoadFile(const char * aJsonFileToExport, const EExportType& aExportType)
{
	myCurrentJsonFile = "";
	myCurrentJsonFile += myFilePathToExportFolder;
	myCurrentJsonFile += aJsonFileToExport;

	if (Corgi::FileExists(myCurrentJsonFile.c_str()))
	{
		myJsonDocument.LoadFile(myCurrentJsonFile.c_str());

		switch (aExportType)
		{
		case EExportType::EModel:
		{
			LoadModels();
			break;
		}
		}
		return true;
	}
	else 
	{
		DL_ERROR(eDebugLogType::UnityExporter, "%s%s", "[UNITYEXPORTER] WARNING! Couldnt open file: ", myCurrentJsonFile.c_str());
		return false;
	}
}

void UnityExporter::LoadModels()
{
	if (!myIsInitiated)
	{
		DL_ERROR(eDebugLogType::UnityExporter, "%s", "[UNITYEXPORTER] WARNING! UnityExporter uninitialized");
		assert(false && "UnityExporter uninitialized");
		myCurrentSpotLightArray.Init(0);
		myCurrentPointLightArray.Init(0);
		myCurrentParticleEmittorArray.Init(0);
		myCurrentTriggerArray.Init(0);
		myCurrentObjectArray.Init(0);
		myCurrentAudioSplinesArray.Init(0);
		myCurrentDataPointsArray.Init(0);
		myCurrentCollidersNavArray.Init(0);
		myCurrentCollidersWallArray.Init(0);
		return;
	}

	DL_SUCCESS(eDebugLogType::UnityExporter, "%s", "[UNITYEXPORTER] >>> ");
	DL_SUCCESS(eDebugLogType::UnityExporter, "%s%s%s" ,"[UNITYEXPORTER] ", myCurrentJsonFile.c_str(), " Loading data from jsonfile ");
	if (!myJsonDocument.Find("JsonDocument"))
	{
		DL_ERROR(eDebugLogType::UnityExporter, "%s%s", "[UNITYEXPORTER] FAILED to load JsonFileData -- JsonDocument array not found in JsonFile: ", myCurrentJsonFile.c_str());
		assert(false && "FAILED to load JsonFileData -- JsonDocument array not found in JsonFile. Are LDers using the right MotherScript?");
		return;
	}

	JsonValue myJsonValue = myJsonDocument["JsonDocument"];
	int jsonDokumentSize = myJsonValue.GetSize();
	for (int indexJsonObjects = 0; indexJsonObjects < jsonDokumentSize; indexJsonObjects++)
	{
		ExportObjects(myJsonValue, indexJsonObjects);
		ExportMovableObject(myJsonValue, indexJsonObjects);
		ExportTriggers(myJsonValue, indexJsonObjects);
		ExportParticleEmittor(myJsonValue, indexJsonObjects);
		ExportPointLight(myJsonValue, indexJsonObjects);
		ExportSpotLight(myJsonValue, indexJsonObjects);
		ExportAudioSplines(myJsonValue, indexJsonObjects);
		ExportDataPoints(myJsonValue, indexJsonObjects);
		ExportColliders(myJsonValue, indexJsonObjects, "Wall");
		ExportColliders(myJsonValue, indexJsonObjects, "NavMesh");
	}
	//ConvertTriggerEventStringToIDs();
	int totalAmountModels = myCurrentObjectArray.Size();
	DL_SUCCESS(eDebugLogType::UnityExporter, "%s%s%s%i", "[UNITYEXPORTER] ", myCurrentJsonFile.c_str(), " Ended !  InstancesData loaded from JsonFile: ", totalAmountModels);
	DL_SUCCESS(eDebugLogType::UnityExporter, "%s", "[UNITYEXPORTER] <<< ");
}


void UnityExporter::ExportObjects(JsonValue& aJsonValue, const int& aIndex)
{
	if (!aJsonValue[aIndex].Find("ObjectData"))
	{
		myInitedBools[static_cast<int>(EExportTag::Object)] = true;
		myCurrentObjectArray.Init(0);
		return;
	}
	JsonValue currentJsonValue = aJsonValue[aIndex]["ObjectData"];
	int arraysize = currentJsonValue.GetSize();
	myInitedBools[static_cast<int>(EExportTag::Object)] = true;
	myCurrentObjectArray.Init(arraysize);
	std::string myPath;
	std::string mystring;
	SUnityExporterObject objectToImport;
	for (int i = 0; i < currentJsonValue.GetSize(); i++)
	{
		if (IsStringValid(currentJsonValue, i, "position", "Object"))
		{
			objectToImport.position =
			{
				currentJsonValue[i]["position"]["x"].GetFloat() ,
				currentJsonValue[i]["position"]["y"].GetFloat() ,
				currentJsonValue[i]["position"]["z"].GetFloat()
			};
		}

		if (IsStringValid(currentJsonValue, i, "rotation", "Object"))
		{
			objectToImport.rotation =
			{
				Corgi::RADIANS(currentJsonValue[i]["rotation"]["x"].GetFloat()) ,
				Corgi::RADIANS(currentJsonValue[i]["rotation"]["y"].GetFloat()) ,
				Corgi::RADIANS(currentJsonValue[i]["rotation"]["z"].GetFloat())
			};
		}

		if (IsStringValid(currentJsonValue, i, "objectID", "Object"))
		{
			objectToImport.objectID = currentJsonValue[i]["objectID"].GetString();
		}

		if (!currentJsonValue[i].Find("path") && !currentJsonValue[i].Find("pathArray"))
		{
			DL_ERROR(eDebugLogType::UnityExporter, "%s%d%s%s", "[UNITYEXPORTER][Object] FAILED to load Object at index: ", i, " -- path string or pathArray not found in JsonFile: ", myCurrentJsonFile.c_str());
		}
		else
		{
			if (currentJsonValue[i].Find("path"))
			{
				if (std::string(currentJsonValue[i]["path"].GetString()) == "")
				{
					DL_ERROR(eDebugLogType::UnityExporter, "%s%d%s%s", "[UNITYEXPORTER][Object] FAILED to load Object at index: ", i, " -- path string was empty in JsonFile: ", myCurrentJsonFile.c_str());
					myPath = "";
					objectToImport.fbxFilePaths.Add(myPath);
				}
				else
				{
					objectToImport.fbxFilePaths.Init(1);
					myPath += "../Assets/Models/";
					myPath += currentJsonValue[i]["path"].GetString();
					objectToImport.fbxFilePaths.Add(myPath);
					myPath = "";
				}

			}
			else
			{
				int stringArraySize = currentJsonValue[i]["pathArray"].GetSize();
				if (stringArraySize <= 0)
				{
					objectToImport.fbxFilePaths.Init(1);
					DL_WARNING(eDebugLogType::UnityExporter, "%s%d%s%s", "[UNITYEXPORTER][Object] WARNING to load Object at index: ", i, " -- path string was null in JsonFile: ", myCurrentJsonFile.c_str());
					myPath = "";
					objectToImport.fbxFilePaths.Add(myPath);
				}
				else
				{
					if (stringArraySize > 1)
					{
						DL_WARNING(eDebugLogType::UnityExporter, "%s%d%s%s", "[UNITYEXPORTER][Object] WARNING Object at index: ", i, " -- path string was more then 1 in JsonFile: ", myCurrentJsonFile.c_str());
					}
					objectToImport.fbxFilePaths.Init(1);
					if (std::string(currentJsonValue[i]["pathArray"][0].GetString()) == "")
					{
						DL_ERROR(eDebugLogType::UnityExporter, "%s%d%s%s", "[UNITYEXPORTER][Object] FAILED to load Object at index: ", i, " -- path string was empty in JsonFile: ", myCurrentJsonFile.c_str());
						myPath = "";
						objectToImport.fbxFilePaths.Add(myPath);
						continue;
					}
					else
					{
						myPath += "../Assets/Models/";
						myPath += currentJsonValue[i]["pathArray"][0].GetString();
						objectToImport.fbxFilePaths.Add(myPath);

						myUniqueModelsPathsSet.insert(myPath);
						myPath = "";
					}

					//objectToImport.fbxFilePaths.Init(stringArraySize);
					//for (int stringIndex = 0; stringIndex < stringArraySize; stringIndex++)
					//{
					//	if (std::string(currentJsonValue[i]["pathArray"][stringIndex].GetString()) == "")
					//	{
					//		DL_ERROR(eDebugLogType::UnityExporter, "%s%d%s%s", "[UNITYEXPORTER][Object] FAILED to load Object at index: ", i, " -- path string was empty in JsonFile: ", myCurrentJsonFile.c_str());
					//		myPath = "";
					//		objectToImport.fbxFilePaths.Add(myPath);
					//	}
					//	else
					//	{
					//		myPath += "../Assets/Models/";
					//		myPath += currentJsonValue[i]["pathArray"][stringIndex].GetString();
					//		objectToImport.fbxFilePaths.Add(myPath);
					//
					//		myUniqueModelsPathsSet.insert(myPath);
					//		myPath = "";
					//	}
					//}
				}
			}
		}

		myCurrentObjectArray.Add(objectToImport);
		myDataCount.myObjectCount++;
	}
}
void UnityExporter::ExportMovableObject(JsonValue& aJsonValue, const int& aIndex)
{
	if (!aJsonValue[aIndex].Find("MovableObjectData"))
	{
		myInitedBools[static_cast<int>(EExportTag::MovableObject)] = true;
		myCurrentMovableObjectArray.Init(0);
		return;
	}
	JsonValue currentJsonValue = aJsonValue[aIndex]["MovableObjectData"];
	int arraysize = currentJsonValue.GetSize();
	myInitedBools[static_cast<int>(EExportTag::MovableObject)] = true;
	myCurrentMovableObjectArray.Init(arraysize);
	std::string myPath;
	std::string mystring;
		SUNityExporterMovableObject movableObjectToImport;
	for (int i = 0; i < currentJsonValue.GetSize(); i++)
	{
		if (IsStringValid(currentJsonValue, i, "position", "MovableObject"))
		{
			movableObjectToImport.position =
			{
				currentJsonValue[i]["position"]["x"].GetFloat() ,
				currentJsonValue[i]["position"]["y"].GetFloat() ,
				currentJsonValue[i]["position"]["z"].GetFloat()
			};
		}

		if (IsStringValid(currentJsonValue, i, "rotation", "MovableObject"))
		{
			movableObjectToImport.rotation =
			{
				Corgi::RADIANS(currentJsonValue[i]["rotation"]["x"].GetFloat()) ,
				Corgi::RADIANS(currentJsonValue[i]["rotation"]["y"].GetFloat()) ,
				Corgi::RADIANS(currentJsonValue[i]["rotation"]["z"].GetFloat())
			};
		}

		if (IsStringValid(currentJsonValue, i, "objectID", "MovableObject"))
		{
			movableObjectToImport.objectID = currentJsonValue[i]["objectID"].GetString();
		}

		if (IsStringValid(currentJsonValue, i, "moveSpeed", "MovableObject"))
		{
			movableObjectToImport.speed = currentJsonValue[i]["moveSpeed"].GetFloat();
		}

		if (!currentJsonValue[i].Find("path") && !currentJsonValue[i].Find("pathArray"))
		{
			DL_ERROR(eDebugLogType::UnityExporter, "%s%d%s%s", "[UNITYEXPORTER][MovableObject] FAILED to load MovableObject at index: ", i, " -- path string or pathArray not found in JsonFile: ", myCurrentJsonFile.c_str());
		}
		{
			if (currentJsonValue[i].Find("path"))
			{
				if (std::string(currentJsonValue[i]["path"].GetString()) == "")
				{
					DL_ERROR(eDebugLogType::UnityExporter, "%s%d%s%s", "[UNITYEXPORTER][MovableObject] FAILED to load MovableObject at index: ", i, " -- path string was empty in JsonFile: ", myCurrentJsonFile.c_str());
					myPath = "";
					movableObjectToImport.fbxFilePaths.Add(myPath);
				}
				{
					movableObjectToImport.fbxFilePaths.Init(1);
					myPath += "../Assets/Models/";
					myPath += currentJsonValue[i]["path"].GetString();
					movableObjectToImport.fbxFilePaths.Add(myPath);
					myPath = "";
				}

			}
			else
			{
				int stringArraySize = currentJsonValue[i]["pathArray"].GetSize();
				if (stringArraySize <= 0)
				{
					movableObjectToImport.fbxFilePaths.Init(1);
					DL_ERROR(eDebugLogType::UnityExporter, "%s%d%s%s", "[UNITYEXPORTER][MovableObject] FAILED to load MovableObject at index: ", i, " -- path string was null in JsonFile: ", myCurrentJsonFile.c_str());
					myPath = "";
					movableObjectToImport.fbxFilePaths.Add(myPath);
				}
				else
				{
					if (stringArraySize > 1)
					{
						DL_ERROR(eDebugLogType::UnityExporter, "%s%d%s%s", "[UNITYEXPORTER][MovableObject] WARNING Object at index: ", i, " -- path string was more then 1 in JsonFile: ", myCurrentJsonFile.c_str());
					}
					movableObjectToImport.fbxFilePaths.Init(1);
					if (std::string(currentJsonValue[i]["pathArray"][0].GetString()) == "")
					{
						DL_ERROR(eDebugLogType::UnityExporter, "%s%d%s%s", "[UNITYEXPORTER][MovableObject] FAILED to load Object at index: ", i, " -- path string was empty in JsonFile: ", myCurrentJsonFile.c_str());
						myPath = "";
						movableObjectToImport.fbxFilePaths.Add(myPath);
						continue;
					}
					else
					{
						myPath += "../Assets/Models/";
						myPath += currentJsonValue[i]["pathArray"][0].GetString();
						movableObjectToImport.fbxFilePaths.Add(myPath);

						myUniqueModelsPathsSet.insert(myPath);
						myPath = "";
					}

					//movableObjectToImport.fbxFilePaths.Init(stringArraySize);
					//for (int stringIndex = 0; stringIndex < stringArraySize; stringIndex++)
					//{
					//	if (std::string(currentJsonValue[i]["pathArray"][stringIndex].GetString()) == "")
					//	{
					//		DL_ERROR(eDebugLogType::UnityExporter, "%s%d%s%s", "[UNITYEXPORTER][MovableObject] FAILED to load MovableObject at index: ", i, " -- path string was empty in JsonFile: ", myCurrentJsonFile.c_str());
					//		myPath = "";
					//		movableObjectToImport.fbxFilePaths.Add(myPath);
					//	}
					//	else
					//	{
					//		myPath += "../Assets/Models/";
					//		myPath += currentJsonValue[i]["pathArray"][stringIndex].GetString();
					//		movableObjectToImport.fbxFilePaths.Add(myPath);
					//
					//		myUniqueModelsPathsSet.insert(myPath);
					//
					//		myPath = "";
					//	}
					//}
				}				
			}
		}

		myCurrentMovableObjectArray.Add(movableObjectToImport);
		myDataCount.myMovableObjectCount++;
	}
}
void UnityExporter::ExportTriggers(JsonValue & aJsonValue, const int & aIndex)
{
	std::string aStringType = "TriggerData";
	SUnityExporterTrigger triggerToExport;

	if (!aJsonValue[aIndex].Find(aStringType.c_str()))
	{
		myInitedBools[static_cast<int>(EExportTag::Trigger)] = true;
		myCurrentTriggerArray.Init(0);
		return;
	}
	JsonValue currentJsonValue = aJsonValue[aIndex][aStringType.c_str()];
	int arraysize = currentJsonValue.GetSize();
	myInitedBools[static_cast<int>(EExportTag::Trigger)] = true;
	myCurrentTriggerArray.Init(arraysize);
	std::string myPath;
	for (int i = 0; i < currentJsonValue.GetSize(); i++)
	{
		if (IsStringValid(currentJsonValue, i, "position","Trigger"))
		{
			triggerToExport.position =
			{
				currentJsonValue[i]["position"]["x"].GetFloat() ,
				currentJsonValue[i]["position"]["y"].GetFloat() ,
				currentJsonValue[i]["position"]["z"].GetFloat()
			};
		}

		if (IsStringValid(currentJsonValue, i, "rotation", "Trigger"))
		{
			triggerToExport.rotation =
			{
				Corgi::RADIANS(currentJsonValue[i]["rotation"]["x"].GetFloat()) ,
				Corgi::RADIANS(currentJsonValue[i]["rotation"]["y"].GetFloat()) ,
				Corgi::RADIANS(currentJsonValue[i]["rotation"]["z"].GetFloat())
			};
		}

		if (IsStringValid(currentJsonValue, i, "size", "Trigger"))
		{
			triggerToExport.boxCollider.size =
			{
				currentJsonValue[i]["size"]["x"].GetFloat() ,
				currentJsonValue[i]["size"]["y"].GetFloat() ,
				currentJsonValue[i]["size"]["z"].GetFloat()
			};
		}

		if (IsStringValid(currentJsonValue, i, "radius", "Trigger"))
		{
			triggerToExport.sphereCollider.radius = currentJsonValue[i]["radius"].GetFloat();
		}

		if (IsStringValid(currentJsonValue, i, "myLuaFile", "Trigger"))
		{
			triggerToExport.luaFunctionToCall = currentJsonValue[i]["myLuaFile"].GetString();
		}

		if (IsStringValid(currentJsonValue, i, "hasBoxCollider", "Trigger"))
		{
			triggerToExport.isBoxCollider = currentJsonValue[i]["hasBoxCollider"].GetBool();
		}

		if (!IsStringValid(currentJsonValue, i, "linkedObjectID", "Trigger"))
		{
			triggerToExport.linkedObjectID = "";
		}
		else
		{
			triggerToExport.linkedObjectID = currentJsonValue[i]["linkedObjectID"].GetString();
		}

		if (std::string(currentJsonValue[i]["myLuaFile"].GetString()) == "")
		{
			DL_ERROR(eDebugLogType::UnityExporter, "%s%d%s%s", "[UNITYEXPORTER][Trigger] FAILED to load Trigger at index: ", i, " -- path string was empty in JsonFile: ", myCurrentJsonFile.c_str());
			continue;
		}
		myCurrentTriggerArray.Add(triggerToExport);
		myDataCount.myTriggerCounts++;
	}
}
void UnityExporter::ExportPointLight(JsonValue & aJsonValue, const int & aIndex)
{
	std::string aStringType = "PointLightData";
	SUnityExporterPointLight pointLightToExport;

	if (!aJsonValue[aIndex].Find(aStringType.c_str()))
	{
		myInitedBools[static_cast<int>(EExportTag::PointLight)] = true;
		myCurrentPointLightArray.Init(0);
		return;
	}
	JsonValue currentJsonValue = aJsonValue[aIndex][aStringType.c_str()];
	int arraysize = currentJsonValue.GetSize();
	myInitedBools[static_cast<int>(EExportTag::PointLight)] = true;
	myCurrentPointLightArray.Init(arraysize);
	std::string myPath;
	for (int i = 0; i < currentJsonValue.GetSize(); i++)
	{
		if (IsStringValid(currentJsonValue,i,"position", "PointLight"))
		{
			pointLightToExport.position =
			{
				currentJsonValue[i]["position"]["x"].GetFloat() ,
				currentJsonValue[i]["position"]["y"].GetFloat() ,
				currentJsonValue[i]["position"]["z"].GetFloat()
			};
		}
		if (IsStringValid(currentJsonValue, i, "color", "PointLight"))
		{
			pointLightToExport.color =
			{
				currentJsonValue[i]["color"]["x"].GetFloat() ,
				currentJsonValue[i]["color"]["y"].GetFloat() ,
				currentJsonValue[i]["color"]["z"].GetFloat()
			};
		}
		if (IsStringValid(currentJsonValue, i, "range", "PointLight"))
		{
			pointLightToExport.range = currentJsonValue[i]["range"].GetFloat();
		}
		if (IsStringValid(currentJsonValue, i, "intensity", "PointLight"))
		{
			pointLightToExport.intensity = currentJsonValue[i]["intensity"].GetFloat();
		}
		
		myCurrentPointLightArray.Add(pointLightToExport);
		myDataCount.myPointLightCounts++;
	}
}
void UnityExporter::ExportParticleEmittor(JsonValue & aJsonValue, const int & aIndex)
{
	std::string aStringType = "ParticleEmittorData";
	SUnityExporterParticleEmittor particleEmittorToExport;

	if (!aJsonValue[aIndex].Find(aStringType.c_str()))
	{
		myInitedBools[static_cast<int>(EExportTag::ParticleEmittor)] = true;
		myCurrentParticleEmittorArray.Init(0);
		return;
	}
	JsonValue currentJsonValue = aJsonValue[aIndex][aStringType.c_str()];
	int arraysize = currentJsonValue.GetSize();
	myInitedBools[static_cast<int>(EExportTag::ParticleEmittor)] = true;
	myCurrentParticleEmittorArray.Init(arraysize);
	std::string myPath;
	for (int i = 0; i < currentJsonValue.GetSize(); i++)
	{
		if (IsStringValid(currentJsonValue, i, "position", "ParticleEmittor"))
		{
			particleEmittorToExport.position =
			{
				currentJsonValue[i]["position"]["x"].GetFloat() ,
				currentJsonValue[i]["position"]["y"].GetFloat() ,
				currentJsonValue[i]["position"]["z"].GetFloat()
			};
		}
		if (IsStringValid(currentJsonValue, i, "rotation", "ParticleEmittor"))
		{
			particleEmittorToExport.rotation =
			{
				Corgi::RADIANS(currentJsonValue[i]["rotation"]["x"].GetFloat()) ,
				Corgi::RADIANS(currentJsonValue[i]["rotation"]["y"].GetFloat()) ,
				Corgi::RADIANS(currentJsonValue[i]["rotation"]["z"].GetFloat()) 
			};
		}

		if (IsStringValid(currentJsonValue, i, "particleEmittor", "ParticleEmittor"))
		{
			particleEmittorToExport.particleEmittorName = currentJsonValue[i]["particleEmittor"].GetString();
		}

		myCurrentParticleEmittorArray.Add(particleEmittorToExport);
		myDataCount.myParticleEmittorCount++;
	}
}
void UnityExporter::ExportSpotLight(JsonValue & aJsonValue, const int & aIndex)
{
	std::string aStringType = "SpotLightData";

	if (!aJsonValue[aIndex].Find(aStringType.c_str()))
	{
		myInitedBools[static_cast<int>(EExportTag::SpotLight)] = true;
		myCurrentSpotLightArray.Init(0);
		return;
	}
	SUnityExporterSpotLight SpotLightToExport;
	JsonValue currentJsonValue = aJsonValue[aIndex][aStringType.c_str()];
	int arraysize = currentJsonValue.GetSize();
	myInitedBools[static_cast<int>(EExportTag::SpotLight)] = true;
	myCurrentSpotLightArray.Init(arraysize);

	std::string myPath;
	for (int i = 0; i < currentJsonValue.GetSize(); i++)
	{
		if (IsStringValid(currentJsonValue, i, "position", "SpotLight"))
		{
			SpotLightToExport.position =
			{
				currentJsonValue[i]["position"]["x"].GetFloat() ,
				currentJsonValue[i]["position"]["y"].GetFloat() ,
				currentJsonValue[i]["position"]["z"].GetFloat()
			};
		}
		if (IsStringValid(currentJsonValue, i, "rotation", "SpotLight"))
		{
			SpotLightToExport.rotation =
			{
				sin(Corgi::RADIANS(currentJsonValue[i]["rotation"]["y"].GetFloat())),
				-(sin(Corgi::RADIANS(currentJsonValue[i]["rotation"]["x"].GetFloat()))*cos(Corgi::RADIANS(currentJsonValue[i]["rotation"]["y"].GetFloat()))),
				(cos(Corgi::RADIANS(currentJsonValue[i]["rotation"]["x"].GetFloat()))*cos(Corgi::RADIANS(currentJsonValue[i]["rotation"]["y"].GetFloat())))
			};
		}
		if (IsStringValid(currentJsonValue, i, "color", "SpotLight"))
		{
			SpotLightToExport.color =
			{
				currentJsonValue[i]["color"]["x"].GetFloat() ,
				currentJsonValue[i]["color"]["y"].GetFloat() ,
				currentJsonValue[i]["color"]["z"].GetFloat()
			};
		}
		if (IsStringValid(currentJsonValue, i, "range", "SpotLight"))
		{
			SpotLightToExport.range = currentJsonValue[i]["range"].GetFloat();
		}
		if (IsStringValid(currentJsonValue, i, "spotLightAngle", "SpotLight"))
		{
			SpotLightToExport.spotLightAngle = currentJsonValue[i]["spotLightAngle"].GetFloat() * 0.45f;
		}
		if (IsStringValid(currentJsonValue, i, "intensity", "SpotLight"))
		{
			SpotLightToExport.intensity = currentJsonValue[i]["intensity"].GetFloat();
		}
		myCurrentSpotLightArray.Add(SpotLightToExport);
		myDataCount.mySpotLightCounts++;
	}
}

void UnityExporter::ExportAudioSplines(JsonValue & aJsonValue, const int & aIndex)
{
	if (!aJsonValue[aIndex].Find("AudioSplineData"))
	{
		myInitedBools[static_cast<int>(EExportTag::AudioSpline)] = true;
		myCurrentAudioSplinesArray.Init(0);
		return;
	}
	JsonValue currentJsonValue = aJsonValue[aIndex]["AudioSplineData"];
	int arraysize = currentJsonValue.GetSize();
	myInitedBools[static_cast<int>(EExportTag::AudioSpline)] = true;
	myCurrentAudioSplinesArray.Init(arraysize);
	std::string myPath;
	std::string mystring;
	SUnityExporterAudioSplines audioSplineToImport;
	for (int i = 0; i < currentJsonValue.GetSize(); i++)
	{
		if (IsStringValid(currentJsonValue, i, "myListOfSplines", "AudioSplines"))
		{
			audioSplineToImport.nodePositions.Init(currentJsonValue[i]["myListOfSplines"].GetSize());
			for (int splineIndex = 0; splineIndex < currentJsonValue[i]["myListOfSplines"].GetSize(); ++splineIndex)
			{			
					audioSplineToImport.nodePositions.Add(
				{
					currentJsonValue[i]["myListOfSplines"][splineIndex]["x"].GetFloat(),
					currentJsonValue[i]["myListOfSplines"][splineIndex]["y"].GetFloat(),
					currentJsonValue[i]["myListOfSplines"][splineIndex]["z"].GetFloat()
				});
			}
		}

		if (IsStringValid(currentJsonValue, i, "myAudioString", "AudioSplines"))
		{
			if (std::string(currentJsonValue[i]["myAudioString"].GetString()) == "")
			{
				DL_ERROR(eDebugLogType::UnityExporter, "%s%d%s%s", "[UNITYEXPORTER][AudioSplines] FAILED to load AudioSpline at index: ", i, " -- path string was empty: ", myCurrentJsonFile.c_str());
				continue;
			}
			audioSplineToImport.audioString = currentJsonValue[i]["myAudioString"].GetString();	
		}
		myCurrentAudioSplinesArray.Add(audioSplineToImport);
		myDataCount.myAudioSplinesCount++;
	}
}
void UnityExporter::ExportDataPoints(JsonValue & aJsonValue, const int & aIndex)
{
	if (!aJsonValue[aIndex].Find("DataData"))
	{
		myInitedBools[static_cast<int>(EExportTag::Data)] = true;
		myCurrentDataPointsArray.Init(0);
		return;
	}
	JsonValue currentJsonValue = aJsonValue[aIndex]["DataData"];
	int arraysize = currentJsonValue.GetSize();
	myInitedBools[static_cast<int>(EExportTag::Data)] = true;
	myCurrentDataPointsArray.Init(arraysize);
	std::string myPath;
	std::string mystring;
	SUnityExporterData dataToImport;
	for (int i = 0; i < currentJsonValue.GetSize(); i++)
	{
		if (IsStringValid(currentJsonValue, i, "myDataString", "Data"))
		{
			if (std::string(currentJsonValue[i]["myDataString"].GetString()) == "")
			{
				DL_ERROR(eDebugLogType::UnityExporter, "%s%d%s%s", "[UNITYEXPORTER][Data] FAILED to load Data at index: ", i, " -- myDataString string was empty: ", myCurrentJsonFile.c_str());
				continue;
			}
			dataToImport.dataString = currentJsonValue[i]["myDataString"].GetString();
		}

		if (IsStringValid(currentJsonValue, i, "myDataTypeString", "Data"))
		{
			if (std::string(currentJsonValue[i]["myDataTypeString"].GetString()) == "")
			{
				DL_ERROR(eDebugLogType::UnityExporter, "%s%d%s%s", "[UNITYEXPORTER][Data] FAILED to load Data at index: ", i, " -- myDataTypeString string was empty: ", myCurrentJsonFile.c_str());
				continue;
			}
			dataToImport.dataTypeString = currentJsonValue[i]["myDataTypeString"].GetString();
		}

		if (IsStringValid(currentJsonValue, i, "myPosition", "Data"))
		{
			dataToImport.position =
			{
				currentJsonValue[i]["myPosition"]["x"].GetFloat(),
				currentJsonValue[i]["myPosition"]["y"].GetFloat(),
				currentJsonValue[i]["myPosition"]["z"].GetFloat()
			};
		}
		myCurrentDataPointsArray.Add(dataToImport);
		myDataCount.myDataCount++;
	}
}
void UnityExporter::ExportColliders(JsonValue & aJsonValue, const int & aIndex, const char* colliderType)
{
	std::string colliderTypeToUse = colliderType;
	if (!aJsonValue[aIndex].Find("ColliderData"))
	{
		myInitedBools[static_cast<int>(EExportTag::Collider)] = true;
		if (colliderTypeToUse == "Wall")
		{
			myCurrentCollidersWallArray.Init(0);
		}
		else if (colliderTypeToUse == "NavMesh")
		{
			myCurrentCollidersNavArray.Init(0);
		}
		return;
	}
	JsonValue currentJsonValue = aJsonValue[aIndex]["ColliderData"];
	int arraysize = currentJsonValue.GetSize();

	if (arraysize > 0)
	{
		if (!IsStringValid(currentJsonValue, 0, "myColliderTypeString", "Collider"))
		{
			if (colliderTypeToUse == "Wall")
			{
				myCurrentCollidersWallArray.Init(0);
				return;
			}
		}
	}

	myInitedBools[static_cast<int>(EExportTag::Collider)] = true;

	if (colliderTypeToUse == "Wall")
	{
		myCurrentCollidersWallArray.Init(arraysize);
	}
	else if (colliderTypeToUse == "NavMesh")
	{
		myCurrentCollidersNavArray.Init(arraysize);
	}

	SUnityExporterColliderNavMesh colliderToImportNav;
	SUnityExporterColliderWall colliderWallToImport;

	std::string idString;
	Vector3f position;
	Vector3f rotation;
	Vector3f size;

	for (int i = 0; i < currentJsonValue.GetSize(); i++)
	{
		if (IsStringValid(currentJsonValue, i, "myColliderTypeString", "Collider"))
		{
			if (std::string(currentJsonValue[i]["myColliderTypeString"].GetString()) != colliderTypeToUse)
			{
				continue;
			}
		}
		else
		{
			colliderTypeToUse = "NavMesh";
		}

		if (IsStringValid(currentJsonValue, i, "myIDString", "Collider"))
		{
			if (std::string(currentJsonValue[i]["myIDString"].GetString()) == "")
			{
				DL_ERROR(eDebugLogType::UnityExporter, "%s%d%s%s", "[UNITYEXPORTER][Collider] FAILED to load Collider at index: ", i, " -- myIDString string was empty: ", myCurrentJsonFile.c_str());
				continue;
			}
			idString = currentJsonValue[i]["myIDString"].GetString();
		}

		if (IsStringValid(currentJsonValue, i, "myPosition", "Collider"))
		{
			position =
			{
				currentJsonValue[i]["myPosition"]["x"].GetFloat(),
				currentJsonValue[i]["myPosition"]["y"].GetFloat(),
				currentJsonValue[i]["myPosition"]["z"].GetFloat()
			};
		}

		if (IsStringValid(currentJsonValue, i, "myRotation", "Collider"))
		{
			rotation =
			{
				
				Corgi::RADIANS(currentJsonValue[i]["myRotation"]["x"].GetFloat()),
				Corgi::RADIANS(currentJsonValue[i]["myRotation"]["y"].GetFloat()),
				Corgi::RADIANS(currentJsonValue[i]["myRotation"]["z"].GetFloat())
			};
		}

		if (IsStringValid(currentJsonValue, i, "size", "Collider"))
		{
			size =
			{
				currentJsonValue[i]["size"]["x"].GetFloat(),
				currentJsonValue[i]["size"]["y"].GetFloat(),
				currentJsonValue[i]["size"]["z"].GetFloat()
			};
		}


		if (colliderTypeToUse == "Wall")
		{
			colliderWallToImport.IDString = idString;
			colliderWallToImport.position = position;
			colliderWallToImport.rotation = rotation;
			colliderWallToImport.size = size;
			myCurrentCollidersWallArray.Add(colliderWallToImport);
			myDataCount.myColliderCount++;
		}
		else if (colliderTypeToUse == "NavMesh")
		{
			colliderToImportNav.IDString = idString;
			colliderToImportNav.position = position;
			colliderToImportNav.rotation = rotation;
			colliderToImportNav.size = size;
			myCurrentCollidersNavArray.Add(colliderToImportNav);
			myDataCount.myColliderCount++;
		}
	}
}

Corgi::GrowingArray<SUnityExporterAsteroid>& UnityExporter::GetAsteroids()
{
	if (myInitedBools[static_cast<int>(EExportTag::Asteroid)])
	{
		return myCurrentAsteroidArray;
	}
	assert(false && "there are no Asteroids in this array. Check error log");
	return myCurrentAsteroidArray;
}
Corgi::GrowingArray<SUnityExporterEnemy>& UnityExporter::GetEnemys()
{
	if (myInitedBools[static_cast<int>(EExportTag::Enemy)])
	{
		return myCurrentEnemyArray;
	}
	assert(false && "there are no enemys in this array. Check error log");
	return myCurrentEnemyArray;

}
Corgi::GrowingArray<SUnityExporterStructure>& UnityExporter::GetStructures()
{
	if (myInitedBools[static_cast<int>(EExportTag::Structure)])
	{
		return myCurrentStructureArray;
	}
	assert(false && "there are no structures in this array. Check error log");
	return myCurrentStructureArray;
}
Corgi::GrowingArray<SUnityExporterObject>& UnityExporter::GetObjects()
{
	if (myInitedBools[static_cast<int>(EExportTag::Object)])
	{
		return myCurrentObjectArray;
	}
	assert(false && "there are no objects in this array. Check error log");
	return myCurrentObjectArray;
}
Corgi::GrowingArray<SUNityExporterMovableObject>& UnityExporter::GetMovableObjects()
{
	if (myInitedBools[static_cast<int>(EExportTag::MovableObject)])
	{
		return myCurrentMovableObjectArray;
	}
	assert(false && "there are no movableObjects in this array. Check error log");
	return myCurrentMovableObjectArray;
}
Corgi::GrowingArray<SUnityExportCheckPoint>& UnityExporter::GetCheckPoints()
{
	if (myInitedBools[static_cast<int>(EExportTag::CheckPoint)])
	{
		return myCurrentCheckPointArray;
	}
	assert(false && "there are no checkpoints in this array. Check error log");
	return myCurrentCheckPointArray;
}
Corgi::GrowingArray<SUnityExporterPickup>& UnityExporter::GetPickups()
{
	if (myInitedBools[static_cast<int>(EExportTag::Pickup)])
	{
		return myCurrentPickupArray;
	}
	assert(false && "there are no pickups in this array. Check error log");
	return myCurrentPickupArray;
}
Corgi::GrowingArray<SUnityExporterTrigger>& UnityExporter::GetTriggers()
{
	if (myInitedBools[static_cast<int>(EExportTag::Trigger)])
	{
		return myCurrentTriggerArray;
	}
	assert(false && "there are no Triggers in this array. Check error log");
	return myCurrentTriggerArray;
}

Corgi::GrowingArray<SUnityExporterParticleEmittor>& UnityExporter::GetParticleEmittors()
{
	if (myInitedBools[static_cast<int>(EExportTag::ParticleEmittor)])
	{
		return myCurrentParticleEmittorArray;
	}
	assert(false && "there are no pointLights in this array. Check error log");
	return myCurrentParticleEmittorArray;
}
Corgi::GrowingArray<SUnityExporterPointLight>& UnityExporter::GetPointLights()
{
	if (myInitedBools[static_cast<int>(EExportTag::PointLight)])
	{
		return myCurrentPointLightArray;
	}
	assert(false && "there are no pointLights in this array. Check error log");
	return myCurrentPointLightArray;
}
Corgi::GrowingArray<SUnityExporterSpotLight>& UnityExporter::GetSpotLights()
{
	if (myInitedBools[static_cast<int>(EExportTag::SpotLight)])
	{
		return myCurrentSpotLightArray;
	}
	assert(false && "there are no pointLights in this array. Check error log");
	return myCurrentSpotLightArray;
}
Corgi::GrowingArray<SUnityExporterWave>& UnityExporter::GetWaves()
{
	if (myInitedBools[static_cast<int>(EExportTag::Wave)])
	{
		return myCurrentWaveArray;
	}
	assert(false && "there are no Waves in this array. Check error log");
	return myCurrentWaveArray;
}
Corgi::GrowingArray<SUnityExporterAudioSplines>& UnityExporter::GetAudioSplines()
{
	if (myInitedBools[static_cast<int>(EExportTag::AudioSpline)])
	{
		return myCurrentAudioSplinesArray;
	}
	assert(false && "there are no AudiSplines in this array. Check error log");
	return myCurrentAudioSplinesArray;
}
Corgi::GrowingArray<SUnityExporterData>& UnityExporter::GetDataPoints()
{
	if (myInitedBools[static_cast<int>(EExportTag::Data)])
	{
		return myCurrentDataPointsArray;
	}
	assert(false && "there are no AudiSplines in this array. Check error log");
	return myCurrentDataPointsArray;
}
Corgi::GrowingArray<SUnityExporterColliderNavMesh>& UnityExporter::GetCollidersNav()
{
	if (myInitedBools[static_cast<int>(EExportTag::Collider)])
	{
		return myCurrentCollidersNavArray;
	}
	assert(false && "there are no AudiSplines in this array. Check error log");
	return myCurrentCollidersNavArray;
}

Corgi::GrowingArray<SUnityExporterColliderWall>& UnityExporter::GetCollidersWall()
{
	if (myInitedBools[static_cast<int>(EExportTag::Collider)])
	{
		return myCurrentCollidersWallArray;
	}
	assert(false && "there are no AudiSplines in this array. Check error log");
	return myCurrentCollidersWallArray;
}
SUnityExportCounts& UnityExporter::GetCount()
{
	return myDataCount;
}

bool UnityExporter::GetIsReady()
{
	return myIsReadyToUse;
}
Corgi::GrowingArray<std::string>& UnityExporter::GetAllUniqueModelsPaths()
{
	return myUniqueModelsPaths;
}
bool UnityExporter::IsStringValid(JsonValue& currentJsonValue,int aIndex, const char* aStringToFind, const char* aStringOfExportingType)
{
	if (!currentJsonValue[aIndex].Find(aStringToFind))
	{
		DL_ERROR(eDebugLogType::UnityExporter, "%s%s%s%d%s%s%s%s", "[UNITYEXPORTER][", aStringOfExportingType,"] FAILED to load at index: ", aIndex, " -- ", aStringToFind, " not found in JsonFile: ", myCurrentJsonFile.c_str());
		return false; 
	}
	return true;
}
