#pragma once
#include "CorgiUsers.h"

enum class EOptionsResolution
{
	e1920x1080,
	e1280x720,
	e800x600
};

struct SButtonData 
{
	float x;
	float y;
};

struct SResolutions
{
	Vector2int desktop;
	Vector2int windowWithBorder;
	Vector2int clientWindow;
};

struct SMousePositions
{
	Vector2int inDesktopPixel;
	Vector2int inWindowPixel;
	Vector2int inGamePixel;
	Vector2int inGameNormal;
};

class CGameSettings
{
public:
	static CGameSettings& GetInstance();
	Corgi::Vector3<float> GetClosestPointOnLine(Corgi::Vector3<float> aLineStart, Corgi::Vector3<float> aLineEnd, Corgi::Vector3<float> aPoint);
	~CGameSettings();

	void PreInit();
	void Init();
	void InitVolumes();

	const Vector2int& GetWindowResolution();
	const Vector2int& GetWindowResolutionWithBorder();

	void UpdateFullscreen();
	void SetIsFullscreen(const bool& aIsFullscreen);
	bool GetIsFullscreen();
	void SetResolution(const Corgi::Vector2<int>& aResolution);
	const SResolutions& CGameSettings::GetResolutions() const;
	void UpdateResolutions();

	void SetMouseConstrainToScreenIngame(bool aBool);

	bool GetIsInEditorState();
	void SetIsInEditorState(const bool& aIsInEditorState);

	void SetUser(const char* aUser);
	eUser GetCurrentUser();
	const char* GetCurrentUserString() const;
	const std::vector<char*>& GetUserVector() const;

	void SetSensitivity(const float& aSensitivity);
	float GetSensitivity();
	const Vector2f& GetSensitivityClamps();

	float VolumeToDb(float aVolume);
	int GetSFXVolume();
	int GetMusicVolume();
	void SetSFXVolumeInPercent(int aPercentage);
	void SetMusicVolumeInPercent(int aPercentage);

	void SaveSettingsToFile();
	void SaveSettingsToFileFirstTime();
	
	bool CGameSettings::DoesFileExist(const std::string& name);
	void CreateDocumentDirectories();
	std::string GetDocumentsPath();
	std::string GetSettingsPath();


	void LoadButtonData();
	std::map<std::string, SButtonData>& GetButtonData();

	EOptionsResolution GetOptionsResolution() { return myOptionsResolution; };
	void SetOptionsResolution(EOptionsResolution aResolution) { myOptionsResolution = aResolution; };

private:
	void UpdateDesktopResolution();
	void UpdateClientWindowResolution();
	void UpdateWindowResolution();

private:
	
	CGameSettings();
	SResolutions myResolutions;

	HWND gameWindowHWND;

	Vector2f mySensitivityClamps;
	bool myIsFullscreen;
	bool myIsInEditorState;

	int mySFXVolume;
	int myMusicVolume;

	float mySensitivity;
	std::string mySettingsFileName;
	std::string myGameName;
	eUser myCurrentUserEnum;
	const SUserString myUserStruct;

	std::map<std::string, SButtonData> myButtonData;
	EOptionsResolution myOptionsResolution;

};

