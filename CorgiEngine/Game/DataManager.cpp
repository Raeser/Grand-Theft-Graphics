#include "stdafx.h"

#include "..\Utilities\FileWatcher\FileWatcher.h"
#include "..\Engine\PostMaster.h"

CDataManager::CDataManager()
{
	myIsInited = false;
	myIsReloadingPlayerData = false;
	myIsReloadingEditorSettings = false;
	myIsReloadingSettingsData = false;
	myIsReloadingConfigData = false;
}

CDataManager::~CDataManager()
{
}

CDataManager & CDataManager::GetInstance()
{
	static CDataManager dataManager;
	return dataManager;
}

void CDataManager::Init()
{
	if (!myIsInited)
	{
		std::string UserConfigFilePath;
		UserConfigFilePath += "../Assets/Data/Config/";
		UserConfigFilePath += CGameSettings::GetInstance().GetCurrentUserString();
		UserConfigFilePath += ".json";


		if (!std::ifstream(UserConfigFilePath))
		{
			UserConfigFilePath = "../Assets/Data/Config/Default.json";
			DL_ERROR(eDebugLogType::ResultMessages, "%s%s%s", "[DataManager] UserConfig : ", UserConfigFilePath.c_str(), " Couldnt be found. Default.json Loaded")
		}
		else
		{
			DL_SUCCESS(eDebugLogType::ResultMessages, "%s%s%s", "[DataManager] UserConfig : ", UserConfigFilePath.c_str(), " Loaded")
		}

		LoadData(UserConfigFilePath.c_str(), eDataWrapper::ConfigData);
	}
	LoadData("../Assets/Data/Data/playerData.json", eDataWrapper::PlayerData);
	LoadData("../Assets/Data/Data/miscData.json", eDataWrapper::MiscData);
	LoadData("../Assets/Data/Data/tikiData.json", eDataWrapper::TikiData);
	LoadData("../Assets/Data/Data/editorSettings.json", eDataWrapper::EditorSettings);
	//LoadData("../Assets/Data/Data/settingsData.json", eDataWrapper::SettingsData);
}

void CDataManager::LoadData(const char * aJsonFileToExport, eDataWrapper aDataWrapperToExport)
{
	myCurrentJsonFile = aJsonFileToExport;
	myJsonDocument.LoadFile(myCurrentJsonFile.c_str());
	JsonValue myJsonValue = myJsonDocument["Data"][0];

	Corgi::CFileWatcher::GetInstance().WatchFileChange(Corgi::ConvertCharPtrToWideString(aJsonFileToExport), std::bind(&CDataManager::RedloadFile, this, std::placeholders::_1,aDataWrapperToExport));
	switch (aDataWrapperToExport)
	{
	case CDataManager::eDataWrapper::PlayerData:
	{
		LoadPlayerData(myJsonValue);
		break;
	}
	case CDataManager::eDataWrapper::TikiData:
	{
		LoadTikiData(myJsonValue);
		break;
	}
	case CDataManager::eDataWrapper::ConfigData:
	{
		LoadConfigData(myJsonValue);
		break;
	}
	case CDataManager::eDataWrapper::SettingsData:
	{
		LoadSettingsData(myJsonValue);
		break;
	}
	case CDataManager::eDataWrapper::EditorSettings:
	{
		LoadEditorSettings(myJsonValue);
		break;
	}
	case CDataManager::eDataWrapper::MiscData:
	{
		LoadMiscData(myJsonValue);
		break;
	}
	default:
		break;
	}
}

void CDataManager::SaveData(const char * aJsonFileToExport, eDataWrapper aDataWrapperToExport)
{
	myCurrentJsonFile = aJsonFileToExport;
	myJsonDocument.LoadFile(myCurrentJsonFile.c_str());
	JsonValue myJsonValue = myJsonDocument["Data"][0];
	Corgi::CFileWatcher::GetInstance().WatchFileChange(Corgi::ConvertCharPtrToWideString(aJsonFileToExport), std::bind(&CDataManager::RedloadFile, this, std::placeholders::_1, aDataWrapperToExport));
	switch (aDataWrapperToExport)
	{

	case CDataManager::eDataWrapper::EditorSettings:
	{
		SaveEditorSettings(myJsonValue);
		myJsonDocument.SaveFile(myCurrentJsonFile.c_str());
		break;
	}
	default:
		break;
	}

}

SPlayerData & CDataManager::GetPlayerData()
{
	while (myIsReloadingPlayerData)
	{
		Sleep(10);
	}
	return myPlayerData;
}

STikiData & CDataManager::GetTikiData()
{
	while (myIsReloadingTikiData)
	{
		Sleep(10);
	}
	return myTikiData;
}


SEditorSettings & CDataManager::GetEditorData()
{
	while (myIsReloadingEditorSettings)
	{
		Sleep(10);
	}
	return myCurrentEditorSettings;
}

void CDataManager::ResetData(eDataWrapper aDataType)
{
	switch (aDataType)
	{
	case CDataManager::eDataWrapper::ConfigData:
		break;
	case CDataManager::eDataWrapper::PlayerData:
		break;
	case CDataManager::eDataWrapper::SettingsData:
		break;
	case CDataManager::eDataWrapper::EditorSettings:
		myCurrentEditorSettings = myDefaultEditorSettings;
		break;
	case CDataManager::eDataWrapper::Count:
		break;
	default:
		break;
	}
}

void CDataManager::SaveCurrentSettingsToData(eDataWrapper aDataType)
{
	switch (aDataType)
	{
	case CDataManager::eDataWrapper::ConfigData:
		break;
	case CDataManager::eDataWrapper::PlayerData:
		break;
	case CDataManager::eDataWrapper::SettingsData:
		break;
	case CDataManager::eDataWrapper::EditorSettings:
		myDefaultEditorSettings = myCurrentEditorSettings;
		SaveData("../Assets/Data/Data/editorSettings.json", eDataWrapper::EditorSettings);
		break;
	case CDataManager::eDataWrapper::Count:
		break;
	default:
		break;
	}
}

void CDataManager::LoadPlayerData(JsonValue& aJsonValue)
{
	if (Exists("playerSpeed", aJsonValue)) myPlayerData.playerSpeed = aJsonValue["playerSpeed"].GetFloat();
 	if (Exists("position", aJsonValue))
	{
		myPlayerData.position.x = aJsonValue["position"]["x"].GetFloat();
		myPlayerData.position.y = aJsonValue["position"]["y"].GetFloat();
		myPlayerData.position.z = aJsonValue["position"]["z"].GetFloat();
	}

	if (Exists("rotation", aJsonValue))
	{
		myPlayerData.rotation.x = aJsonValue["rotation"]["x"].GetFloat();
		myPlayerData.rotation.y = aJsonValue["rotation"]["y"].GetFloat();
		myPlayerData.rotation.z = aJsonValue["rotation"]["z"].GetFloat();
	}

	if (Exists("health", aJsonValue)) myPlayerData.health = aJsonValue["health"].GetInt();
	if (Exists("clickIndicatorSpeed", aJsonValue)) myPlayerData.clickIndicatorSpeed = aJsonValue["clickIndicatorSpeed"].GetFloat();
	if (Exists("clickIndicatorStartHeight", aJsonValue)) myPlayerData.clickIndicatorStartHeight = aJsonValue["clickIndicatorStartHeight"].GetFloat();
	if (Exists("clickIndicatorEndHeight", aJsonValue)) myPlayerData.clickIndicatorEndHeight = aJsonValue["clickIndicatorEndHeight"].GetFloat();
	if (Exists("clickIndicatorLifeTime", aJsonValue)) myPlayerData.clickIndicatorLifeTime = aJsonValue["clickIndicatorLifeTime"].GetFloat();
	//if (Exists("colliderRadius", aJsonValue)) myPlayerData.colliderRadius = aJsonValue["colliderRadius"].GetFloat();

}

void CDataManager::LoadMiscData(JsonValue& aJsonValue)
{
	if (Exists("RandomParticle_EndRotationValue_From", aJsonValue)) myMiscData.randParticleEndDegreeFrom = aJsonValue["RandomParticle_EndRotationValue_From"].GetFloat();
	if (Exists("RandomParticle_EndRotationValue_To", aJsonValue)) myMiscData.randParticleEndDegreeTo = aJsonValue["RandomParticle_EndRotationValue_To"].GetFloat();
}


inline void CDataManager::LoadTikiData(JsonValue & aJsonValue)
{
	if (Exists("tikiSpeed", aJsonValue)) myTikiData.tikiSpeed = aJsonValue["tikiSpeed"].GetFloat();
	if (Exists("throwSpeed", aJsonValue)) myTikiData.throwSpeed = aJsonValue["throwSpeed"].GetFloat();
	if (Exists("maxThrowHeight", aJsonValue)) myTikiData.maxThrowHeight = aJsonValue["maxThrowHeight"].GetFloat();
	if (Exists("maxThrowLength", aJsonValue)) myTikiData.maxThrowLength = aJsonValue["maxThrowLength"].GetFloat();
	if (Exists("jumpSpeed", aJsonValue)) myTikiData.jumpSpeed = aJsonValue["jumpSpeed"].GetFloat();
	if (Exists("minDistianceToPlayer", aJsonValue)) myTikiData.minDistianceToPlayer = aJsonValue["minDistianceToPlayer"].GetFloat();
	if (Exists("restlessMaxTimer", aJsonValue)) myTikiData.restlessMaxTimer = aJsonValue["restlessMaxTimer"].GetFloat();

}
void CDataManager::LoadEditorSettings(JsonValue& aJsonValue)
{
	if (Exists("Camera", aJsonValue))
	{
		if (Exists("cameraDistanceVectorToPlayer", aJsonValue["Camera"]))
		{
			myDefaultEditorSettings.cameraDistianceVectorToPlayer.x = aJsonValue["Camera"]["cameraDistanceVectorToPlayer"]["x"].GetFloat();
			myDefaultEditorSettings.cameraDistianceVectorToPlayer.y = aJsonValue["Camera"]["cameraDistanceVectorToPlayer"]["y"].GetFloat();
			myDefaultEditorSettings.cameraDistianceVectorToPlayer.z = aJsonValue["Camera"]["cameraDistanceVectorToPlayer"]["z"].GetFloat();
		}
		if (Exists("cameraScrollZoomSpeed", aJsonValue["Camera"])) myDefaultEditorSettings.cameraScrollZoomSpeed = aJsonValue["Camera"]["cameraScrollZoomSpeed"].GetFloat();
		if (Exists("cameraMouseZoomSpeed", aJsonValue["Camera"])) myDefaultEditorSettings.cameraMouseZoomSpeed = aJsonValue["Camera"]["cameraMouseZoomSpeed"].GetFloat();
		if (Exists("cameraMouseZoomMultiplier", aJsonValue["Camera"])) myDefaultEditorSettings.cameraMouseZoomMultiplier = aJsonValue["Camera"]["cameraMouseZoomMultiplier"].GetFloat();

		if (Exists("cameraMouseRotationSpeed", aJsonValue["Camera"])) myDefaultEditorSettings.cameraMouseRotationSpeed = aJsonValue["Camera"]["cameraMouseRotationSpeed"].GetFloat();

		if (Exists("cameraSpeed", aJsonValue["Camera"])) myDefaultEditorSettings.cameraSpeed = aJsonValue["Camera"]["cameraSpeed"].GetFloat();
		if (Exists("cameraMouseDragSpeed", aJsonValue["Camera"])) myDefaultEditorSettings.cameraMouseDragSpeed = aJsonValue["Camera"]["cameraMouseDragSpeed"].GetFloat();
		if (Exists("cameraAngleInDegree", aJsonValue["Camera"])) myDefaultEditorSettings.cameraAngleInDegrees = aJsonValue["Camera"]["cameraAngleInDegree"].GetFloat();
		myCurrentEditorSettings = myDefaultEditorSettings;
	}
}
void CDataManager::LoadConfigData(JsonValue& aJsonValue)
{
	if (Exists("LevelsToLoad", aJsonValue))
	{
		int amountOfLevelsToLoad = aJsonValue["LevelsToLoad"].GetSize();
		myConfigData.levelsToLoad.Init(amountOfLevelsToLoad);
		
		for (int i = 0; i < amountOfLevelsToLoad; i++)
		{
			myConfigData.levelsToLoad.Add(aJsonValue["LevelsToLoad"][i].GetString());
		}
	}
}
void CDataManager::LoadSettingsData(JsonValue & aJsonValue)
{
	aJsonValue;
}

void CDataManager::SaveEditorSettings(JsonValue& aJsonValue)
{
	if (Exists("Camera", aJsonValue))
	{
		if (Exists("cameraDistanceVectorToPlayer", aJsonValue["Camera"]))
		{
			aJsonValue["Camera"]["cameraDistanceVectorToPlayer"]["x"].SetFloat(myDefaultEditorSettings.cameraDistianceVectorToPlayer.x);
			aJsonValue["Camera"]["cameraDistanceVectorToPlayer"]["y"].SetFloat(myDefaultEditorSettings.cameraDistianceVectorToPlayer.y);
			aJsonValue["Camera"]["cameraDistanceVectorToPlayer"]["z"].SetFloat(myDefaultEditorSettings.cameraDistianceVectorToPlayer.z);
		}
		if (Exists("cameraScrollZoomSpeed", aJsonValue["Camera"])) aJsonValue["Camera"]["cameraScrollZoomSpeed"].SetFloat(myDefaultEditorSettings.cameraScrollZoomSpeed);
		if (Exists("cameraMouseZoomSpeed", aJsonValue["Camera"])) aJsonValue["Camera"]["cameraMouseZoomSpeed"].SetFloat(myDefaultEditorSettings.cameraMouseZoomSpeed);
		if (Exists("cameraMouseZoomMultiplier", aJsonValue["Camera"])) aJsonValue["Camera"]["cameraMouseZoomMultiplier"].SetFloat(myDefaultEditorSettings.cameraMouseZoomMultiplier);

		if (Exists("cameraMouseRotationSpeed", aJsonValue["Camera"])) aJsonValue["Camera"]["cameraMouseRotationSpeed"].SetFloat(myDefaultEditorSettings.cameraMouseRotationSpeed);

		if (Exists("cameraSpeed", aJsonValue["Camera"]))  aJsonValue["Camera"]["cameraSpeed"].SetFloat(myDefaultEditorSettings.cameraSpeed);
		if (Exists("cameraMouseDragSpeed", aJsonValue["Camera"])) aJsonValue["Camera"]["cameraMouseDragSpeed"].SetFloat(myDefaultEditorSettings.cameraMouseDragSpeed);
		if (Exists("cameraAngleInDegree", aJsonValue["Camera"])) aJsonValue["Camera"]["cameraAngleInDegree"].SetFloat(myDefaultEditorSettings.cameraAngleInDegrees);
	}
}

bool CDataManager::Exists(const char * stringToCheck, const JsonValue& aJsonValue)
{
	if (!aJsonValue.Find(stringToCheck))
	{
		std::string errorString (stringToCheck);
		DL_ERROR(eDebugLogType::ResultMessages, "%s%s%s%s", "[DATAMANAGER] FAILED to load JsonData -- string not found in JsonFile. String: ", errorString.c_str(), ". CurrentJsonFile: ", myCurrentJsonFile.c_str());

#ifndef DEBUG
		errorString = "FAILED to load JsonData -- string not found in JsonFile. ";
		errorString += "String: ";
		errorString += stringToCheck;
		errorString += " CurrentJsonFile: ";
		errorString += myCurrentJsonFile;
#endif
		assert(false && errorString.c_str());
		return false;
	}
	return true;
}

void CDataManager::RedloadFile(std::wstring aPath, eDataWrapper aDataWrapperToExport)
{
	myCurrentJsonFile = Corgi::ConvertWStringToString(aPath);
	myJsonDocument.LoadFile(myCurrentJsonFile.c_str());

	JsonValue myJsonValue = myJsonDocument["Data"][0];
	switch (aDataWrapperToExport)
	{
	case CDataManager::eDataWrapper::PlayerData:
	{
		myIsReloadingPlayerData = true;
		LoadPlayerData(myJsonValue);
		SMessage message;
		message.messageType = eMessageType::PlayerData;
		message.playerData.myPlayerSpeed = myPlayerData.playerSpeed;
		CPostMaster::GetInstance().PostMessageA(message);
		break;
	}
	case CDataManager::eDataWrapper::TikiData:
	{
		myIsReloadingTikiData = true;
		LoadTikiData(myJsonValue);
		SMessage message;
		message.messageType = eMessageType::TikiData;
		message.tikiData.tikiSpeed = myTikiData.tikiSpeed;
		message.tikiData.throwSpeed = myTikiData.throwSpeed;
		message.tikiData.maxThrowHeight = myTikiData.maxThrowHeight;
		message.tikiData.maxThrowLength = myTikiData.maxThrowLength;
		message.tikiData.jumpSpeed = myTikiData.jumpSpeed;
		message.tikiData.minDistianceToPlayer = myTikiData.minDistianceToPlayer;
		message.tikiData.restlessMaxTimer = myTikiData.restlessMaxTimer;
		CPostMaster::GetInstance().PostMessageA(message);
		break;
	}
	case CDataManager::eDataWrapper::ConfigData:
	{
		myIsReloadingConfigData = true;
		LoadConfigData(myJsonValue);
		break;
	}
	case CDataManager::eDataWrapper::SettingsData:
	{
		myIsReloadingSettingsData = true;
		LoadSettingsData(myJsonValue);
		break;
	}
	case CDataManager::eDataWrapper::EditorSettings:
	{
		myIsReloadingEditorSettings= true;
		LoadEditorSettings(myJsonValue);
		break;
	}
	default:
		break;
	}
	myIsReloadingPlayerData = false;
	myIsReloadingConfigData = false;
	myIsReloadingSettingsData = false;
	myIsReloadingEditorSettings = false;
	myIsReloadingTikiData = false;
}