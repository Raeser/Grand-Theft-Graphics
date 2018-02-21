#include "stdafx.h"

#include "..\Audio\Audio\AudioInterface.h"

#include "..\Utilities\JsonDocument.h"
#include "..\Utilities\JsonDocument.h"
#include <Shlobj.h>
#include <utility>
#include "InputWrapper.h"

#pragma warning(push)
#pragma warning( disable : 4996)

CGameSettings::CGameSettings()
{
	myCurrentUserEnum = eUser::defualt;

}


CGameSettings::~CGameSettings()
{
}

CGameSettings & CGameSettings::GetInstance()
{
	static CGameSettings gameSettings;
	return gameSettings;
}

Corgi::Vector3<float> CGameSettings::GetClosestPointOnLine(Corgi::Vector3<float> aLineStart, Corgi::Vector3<float> aLineEnd, Corgi::Vector3<float> aPoint)
{
	Corgi::Vector3<float> fromStartToEnd = aLineEnd - aLineStart;
	Corgi::Vector3<float> n = fromStartToEnd.GetNormalized();
	Corgi::Vector3<float> fromStartToPoint = aPoint - aLineStart;

	float length = n.Dot(fromStartToPoint);

	Corgi::Vector3<float> point = aLineStart + (n * length);

	if (length < 0)
	{
		return aLineStart;
	}
	else if (length > fromStartToEnd.Length())
	{
		return aLineEnd;
	}

	return point;
}

void CGameSettings::PreInit()
{
	DL_SUCCESS(eDebugLogType::ResultMessages, "%s%s", "[GameSettings] Current user : ", myUserStruct.users[static_cast<int>(myCurrentUserEnum)]);
	myGameName = "Tikmin";
	myIsFullscreen = false;
	mySettingsFileName = "../Assets/Data/Data/OriginalSettings.json";

	UpdateDesktopResolution();
	JsonDocument file;
	if (DoesFileExist(std::string(GetSettingsPath() + "settings.json")))
	{
		file.LoadFile(std::string(GetSettingsPath() + "settings.json").c_str());
	}
	else
	{
		file.LoadFile(mySettingsFileName.c_str());
	}

	JsonValue value = file["Settings"];
	mySensitivityClamps.x = value["minSensitivity"].GetFloat();
	mySensitivityClamps.y = value["maxSensitivity"].GetFloat();

	mySFXVolume = value["volumeSFX"].GetInt();
	myMusicVolume = value["volumeMusic"].GetInt();

	InitVolumes();
//
//#ifndef _DEBUG
//	SetIsFullscreen(value["fullscreen"].GetBool());
//#else
//	SetIsFullscreen(false);
//#endif // !_DEBUG
	SetIsFullscreen(false);

	SetSensitivity(value["aimSensitivity"].GetFloat());

	if (DoesFileExist(std::string(GetSettingsPath() + "settings.json")))
	{
		SaveSettingsToFile();
	}
	else
	{
		SaveSettingsToFileFirstTime();
	}

	LoadButtonData();

	myOptionsResolution = EOptionsResolution::e1920x1080;
}

void CGameSettings::Init()
{
	UpdateResolutions();
	gameWindowHWND = GetActiveWindow();
	//SetResolution(myResolutions.desktop);
}


void CGameSettings::InitVolumes()
{
	SetMusicVolumeInPercent(myMusicVolume);
	SetSFXVolumeInPercent(mySFXVolume);

	DL_SUCCESS(eDebugLogType::ResultMessages, "%s%i%s%i%s", "Music Volume was set as ", myMusicVolume, "% and SFX Volume was set to ", mySFXVolume, "%");
}

const Vector2int& CGameSettings::GetWindowResolution()
{
	return myResolutions.clientWindow;
}

const Vector2int & CGameSettings::GetWindowResolutionWithBorder()
{
	return myResolutions.windowWithBorder;
}

void CGameSettings::UpdateClientWindowResolution()
{
	RECT clientWindowRect;
	GetClientRect(GetActiveWindow(), &clientWindowRect);
	myResolutions.clientWindow = { clientWindowRect.right - clientWindowRect.left, clientWindowRect.bottom - clientWindowRect.top };
}

void CGameSettings::UpdateWindowResolution()
{
	RECT windowResolution;
	GetWindowRect(GetActiveWindow(), &windowResolution);
	myResolutions.windowWithBorder = { windowResolution.right - windowResolution.left, windowResolution.bottom - windowResolution.top };
}

void CGameSettings::UpdateDesktopResolution()
{
	RECT desktop;
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &desktop);

	myResolutions.desktop = { desktop.right - desktop.left, desktop.bottom - desktop.top };

}

void CGameSettings::UpdateFullscreen()
{
	//if (InputManager::GetInstance().CheckKeyDown(KeyCode::LeftAlt) &&  InputManager::GetInstance().CheckKeyDown(KeyCode::Enter))
	//{
	//	std::cout << "Set Fullscreen wowa xDD" << std::endl;
	//	//SetIsFullscreen(!myIsFullscreen);
	//	myIsFullscreen = !myIsFullscreen;
	//}
}

void CGameSettings::SetIsFullscreen(const bool & aIsFullscreen)
{
	myIsFullscreen = aIsFullscreen;
	SMessage msg;
	msg.messageType = eMessageType::FullscreenData;
	msg.fullscreenData.shouldFullscreen = myIsFullscreen;
	CPostMaster::GetInstance().PostMessageA(msg);
}

bool CGameSettings::GetIsFullscreen()
{
	return myIsFullscreen;
}

void CGameSettings::SetResolution(const Corgi::Vector2<int>& aResolution)
{
	SMessage msg;
	msg.messageType = eMessageType::SetResolution;
	msg.myResolutionData.myResolution = aResolution;

	CPostMaster::GetInstance().PostMessageA(msg);
}

const SResolutions & CGameSettings::GetResolutions() const
{
	return myResolutions;
}

void CGameSettings::UpdateResolutions()
{
	UpdateDesktopResolution();
	UpdateWindowResolution();
	UpdateClientWindowResolution();
}

void CGameSettings::SetMouseConstrainToScreenIngame(bool aBool)
{
	InputManager::GetInstance().SetForceClipping(aBool);
}


bool CGameSettings::GetIsInEditorState()
{
	return myIsInEditorState;
}

void CGameSettings::SetIsInEditorState(const bool & aIsInEditorState)
{
	myIsInEditorState = aIsInEditorState;
}

void CGameSettings::SetUser(const char * aUser)
{
	for (int i = 0; i < static_cast<int>(eUser::defualt); i++)
	{
		if (myUserStruct.users[i] == aUser)
		{
			myCurrentUserEnum = static_cast<eUser>(i);
		}
	}
}

eUser CGameSettings::GetCurrentUser()
{
	return myCurrentUserEnum;
}


const char* CGameSettings::GetCurrentUserString() const
{
	return myUserStruct.users[static_cast<int>(myCurrentUserEnum)];
}

const std::vector<char*>& CGameSettings::GetUserVector() const
{
	return myUserStruct.users;
}


void CGameSettings::SetSensitivity(const float & aSensitivity)
{
	if (aSensitivity < mySensitivityClamps.x)
	{
		mySensitivity = mySensitivityClamps.x;
	}
	else if (aSensitivity > mySensitivityClamps.y)
	{
		mySensitivity = mySensitivityClamps.y;
	}
	else
	{
		mySensitivity = aSensitivity;
	}
}

float CGameSettings::GetSensitivity()
{
	return mySensitivity;
}

const Vector2f & CGameSettings::GetSensitivityClamps()
{
	return mySensitivityClamps;
}


float CGameSettings::VolumeToDb(float aVolume)
{
	return 20.0f * log10f(aVolume);
}

int CGameSettings::GetSFXVolume()
{
	return mySFXVolume;
}

int CGameSettings::GetMusicVolume()
{
	return myMusicVolume;
}

void CGameSettings::SetSFXVolumeInPercent(int aPercentage)
{
	CAudioInterface::GetInstance()->SetRTPCForAllObjects("SFX_Volume", aPercentage);
	mySFXVolume = aPercentage;
}

void CGameSettings::SetMusicVolumeInPercent(int aPercentage)
{
	CAudioInterface::GetInstance()->SetRTPCForAllObjects("Music_Volume", aPercentage);
	myMusicVolume = aPercentage;
}


void CGameSettings::SaveSettingsToFile()
{
	JsonDocument file;
	std::stringstream filePath;
	filePath << GetSettingsPath() << "settings.json";


	file.LoadFile(filePath.str().c_str());
	JsonValue value = file["Settings"];

	value["volumeSFX"].SetInt(mySFXVolume);
	value["volumeMusic"].SetInt(myMusicVolume);
	value["fullscreen"].SetBool(myIsFullscreen);
	value["aimSensitivity"].SetFloat(mySensitivity);


	CreateDocumentDirectories();

	std::stringstream filepath;
	filepath << GetSettingsPath() << "\\settings.json";

	file.SaveFile(filepath.str().c_str());
}

void CGameSettings::SaveSettingsToFileFirstTime()
{
	JsonDocument file;
	file.LoadFile(mySettingsFileName.c_str());
	JsonValue value = file["Settings"];

	value["volumeSFX"].SetInt(mySFXVolume);
	value["volumeMusic"].SetInt(myMusicVolume);
	value["fullscreen"].SetBool(myIsFullscreen);
	value["aimSensitivity"].SetFloat(mySensitivity);


	CreateDocumentDirectories();

	std::stringstream filepath;
	filepath << GetSettingsPath() << "\\settings.json";

	file.SaveFile(filepath.str().c_str());
}

bool CGameSettings::DoesFileExist(const std::string& name)
{
	std::ifstream f(name.c_str());
	return f.good();
}


void CGameSettings::CreateDocumentDirectories()
{
	std::string documents = GetDocumentsPath();

	std::string documentsFolderPath(documents + "\\" + "my games");
	std::string gameFolderPath(documentsFolderPath + "\\" + myGameName);

	CreateDirectory(documentsFolderPath.c_str(), NULL);
	CreateDirectory(gameFolderPath.c_str(), NULL);
}

std::string CGameSettings::GetDocumentsPath()
{
	wchar_t Folder[1024];
	HRESULT hr = SHGetFolderPathW(0, CSIDL_MYDOCUMENTS, 0, 0, Folder);
	if (SUCCEEDED(hr))
	{
		char str[1024];
		wcstombs(str, Folder, 1023);
		return str;
	}
	else return "";
}
#pragma warning(pop)

std::string CGameSettings::GetSettingsPath()
{
	std::string documents = GetDocumentsPath();

	std::string documentsFolderPath(documents + "\\" + "my games");
	std::string gameFolderPath(documentsFolderPath + "\\" + myGameName + "\\");

	return gameFolderPath;
}


void CGameSettings::LoadButtonData()
{
	/*JsonDocument buttonFile;
	buttonFile.LoadFile("../Assets/Data/Menus/menus.json");
	JsonValue buttonArray = buttonFile["mainMenu"];
	int size = buttonArray.GetSize();


	for (int i = 0; i < size; i++)
	{
	SButtonData buttonData;
	buttonData.x = buttonArray[i]["X"].GetFloat();
	buttonData.y = buttonArray[i]["Y"].GetFloat();
	std::string id = buttonArray[i]["id"].GetString();

	myButtonData.insert(std::pair<std::string, SButtonData>(id, buttonData));
	}*/

}

std::map<std::string, SButtonData>& CGameSettings::GetButtonData()
{
	return myButtonData;
}





