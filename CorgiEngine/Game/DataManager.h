#pragma once
#include "../Utilities/JsonDocument.h"
#include "DataStructs.h"
#include <atomic>

class JsonDocument;
class JsonValue;

class CDataManager
{
public:
	enum class eDataWrapper
	{
		ConfigData,
		PlayerData,
		TikiData,
		SettingsData,
		EditorSettings,
		MiscData,
		Count
	};
public:
	~CDataManager();

	static CDataManager& GetInstance();
	void Init();

	SPlayerData& GetPlayerData();
	STikiData & GetTikiData();
	SEditorSettings & GetEditorData();
	void ResetData(eDataWrapper aDataType);
	void SaveCurrentSettingsToData(eDataWrapper aDataType);
	const SSettingsData& GetSettingsData() const { return mySettingsData; };
	const SConfigData& GetConfigData() const { return myConfigData; };
	const SMiscData& GetMiscData() const { return myMiscData; };

	SPlayerData myPlayerData;
	SMiscData myMiscData;
	STikiData myTikiData;
	SEditorSettings myDefaultEditorSettings;
	SEditorSettings myCurrentEditorSettings;
	SSettingsData mySettingsData;
	SConfigData myConfigData;

private:
	CDataManager();

	void LoadData(const char* aJsonFileToExport, eDataWrapper aDataWrapperToExport);
	void SaveData(const char * aJsonFileToExport, eDataWrapper aDataWrapperToExport);
	bool Exists(const char * aJsonFileToExport, const JsonValue& aJsonValue);

	inline void LoadPlayerData(JsonValue& aJsonValue);
	void LoadMiscData(JsonValue & aJsonValue);
	inline void LoadTikiData(JsonValue& aJsonValue);

	void LoadEditorSettings(JsonValue & aJsonValue);

	void LoadConfigData(JsonValue & aJsonValue);

	void LoadSettingsData(JsonValue & aJsonValue);

	void SaveEditorSettings(JsonValue & aJsonValue);

	void RedloadFile(std::wstring aPath, eDataWrapper aDataWrapperToExport);

	bool myIsInited;
	JsonDocument myJsonDocument;
	std::string myCurrentJsonFile;

	std::atomic<bool> myIsReloadingPlayerData;
	std::atomic<bool> myIsReloadingTikiData;
	std::atomic<bool> myIsReloadingEditorSettings;
	std::atomic<bool> myIsReloadingConfigData;
	std::atomic<bool> myIsReloadingSettingsData;

};


