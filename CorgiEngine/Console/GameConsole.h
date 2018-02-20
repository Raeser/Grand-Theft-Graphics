#pragma once

#include <string>
#include "..\Utilities\GrowingArray.h"
#include "..\Utilities\Vector2.h"
#include <mutex>
class CSpriteInstance;
class CEngine;

class CGameConsole
{
public:
	CGameConsole(CEngine& aEngine);
	~CGameConsole();
	
	bool Init();
	bool Update();
	void SelectSuggestionUp();
	void SelectSuggestionDown();
	bool Clear();

	void AddResultText(std::string aResultToAdd);

private:

	void ShowResultText();

	void AddCommandText(std::string aCommandToAdd);
	void ShowNewestCommand();
	void ShowOldestCommand();
	void UpdateAutoComplete();
	void GetFuntionNames(Corgi::GrowingArray<std::string>& aArrayToFill);

	void ClearDoubleSpaces(std::string& aText);

	void ClearAutoComplete();

	struct STextWrapper
	{
		//std::string myText;
		//Vector4f myColor;
		//long long my;
	};
	Corgi::GrowingArray<std::string> myResultList; //pls robin kolla om du kan göra denna mer trådsäker //POSSIBLE HEAP CORRUPTION
	Corgi::GrowingArray<std::string> myBlockedFunctions;
	Corgi::GrowingArray<std::string> myPreviousCommands;


	Corgi::GrowingArray<std::string> myPreviousCommandsList;
	Corgi::GrowingArray<std::string> myAutoCompleteSuggestionList;
	Corgi::GrowingArray<long long> myAutoCompleteIdexList;
	std::string myShownResultText;
	std::string myCurrentText;
	std::string myCurrentTextSuggestionShown;
	std::string myCurrentAutoComplete;
	std::mutex myResultAddMutex;

	long long myLatestCommandIndex;
	long long myResultTextIndex;
	long long myResultFontIndex;
	long long myCommandTextIndex;
	long long myCommandFontIndex;

	int myCurrentResultID;
	int myFirstShownResultID;
	int myPreviousInputIndex;
	int myCurrentCommandID;
	int myCurrentSelectedAutoComplete;

	Vector2f myCommandTextPosition;
	Vector2f myResultTextPosition;

	CEngine& myEngine;
	float myBackspaceTimer;
	float myBlinkingMarkerTimer;
	float myRowSize;

	bool mySuggestionListBelowCommandText;
	bool myCommandListChanged;
	bool myBlinkingMarkerVisible;
	bool myShowAutoComplete;
};

