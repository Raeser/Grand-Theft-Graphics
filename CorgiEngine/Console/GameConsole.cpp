#include "stdafx.h"


#define CONSOLE_LIMIT 512
#define CONSOLE_ROW_LIMIT 18

CGameConsole::CGameConsole(CEngine & aEngine) :
	myEngine(aEngine)
{
	myResultList.Init(CONSOLE_LIMIT);
	for (int i = 0; i < CONSOLE_LIMIT; ++i)
	{
		myResultList.Add("");
	}

	myPreviousCommandsList.Init(CONSOLE_LIMIT);
	for (int i = 0; i < CONSOLE_LIMIT; ++i)
	{
		myPreviousCommandsList.Add("");
	}
	
	myAutoCompleteSuggestionList.Init(CONSOLE_LIMIT);
	myCurrentAutoComplete = "";
	myCurrentSelectedAutoComplete = -1;

	myAutoCompleteIdexList.Init(CONSOLE_LIMIT);

	myBlockedFunctions.Init(CONSOLE_LIMIT);

	myPreviousInputIndex = 0;
	myCurrentCommandID = 0;
	myCurrentResultID = 0;
	myFirstShownResultID = 0;
	myBlinkingMarkerVisible = false;
	DebugLog::GetInstance().SetConsole(this);
}

CGameConsole::~CGameConsole()
{
	DebugLog::GetInstance().SetConsole(nullptr);
	myResultList.RemoveAll();
	myAutoCompleteSuggestionList.RemoveAll();
}

bool CGameConsole::Init()
{
	myRowSize = 0.025f;
	mySuggestionListBelowCommandText = true;
	myCommandTextPosition.x = -0.5f;
	myCommandTextPosition.y = -0.f;

	myResultTextPosition.x = myCommandTextPosition.x;
	myResultTextPosition.y = CONSOLE_ROW_LIMIT * 0.0306f + myCommandTextPosition.y;

	myCommandTextIndex = myEngine.CreateText(myCurrentText, myCommandTextPosition);
	myResultTextIndex = myEngine.CreateText(myShownResultText, myResultTextPosition, 1);

	Vector4f color = { 0.01f, 1.f, 0.01f, 1.f };
	myEngine.GetTextManager().ChangeTextColor(myResultTextIndex, color);
	myBackspaceTimer = 0.5f;
	myBlinkingMarkerTimer = 0.5f;
	myBlinkingMarkerVisible = false;
	myShowAutoComplete = false;
	return true;
}

bool CGameConsole::Update()
{
	const float& dt = Timer::GetInstance().GetDeltaTime();

	if (InputManager::GetInstance().CheckKeyPressed(KeyCode::Escape) || InputManager::GetInstance().CheckKeyReleased(KeyCode::Tilde))
	{
		myCurrentText.clear();
		myShownResultText.clear();
		myCurrentAutoComplete.clear();
		ClearAutoComplete();
		myAutoCompleteSuggestionList.RemoveAll();
		return false;
	}

	if (InputManager::GetInstance().CheckKeyPressed(KeyCode::Enter) && myCurrentText.size() > 0 && !(myCurrentText.size() == 1 && myBlinkingMarkerVisible))
	{
		if (myBlinkingMarkerVisible)
		{
			myCurrentText.pop_back();
			myBlinkingMarkerVisible = false;
		}
		AddCommandText(myCurrentText);
		AddResultText("-" + myCurrentText);
		SLuaFunction function;
		function.args.Init(8);

		size_t spaceindex = myCurrentText.find(' ');

		if (spaceindex == -1)
		{
			function.functionName = myCurrentText;
		}
		else
		{
			std::string temp = myCurrentText;

			myCurrentText = myCurrentText.substr(0, spaceindex);
			function.functionName = myCurrentText;
			
			std::string remainder = temp.substr(spaceindex + 1);


			while (spaceindex != -1)
			{
				std::string argval;
				spaceindex = remainder.find(' ');
				if (spaceindex == -1)
				{
					argval = remainder;
				}
				else
				{
					argval = remainder.substr(0, spaceindex);
					remainder = remainder.substr(spaceindex + 1);
				}

				Argument arg;

				bool isString = false;

				if (argval[0] >= '0' && argval[0] <= '9')
				{

					if (argval.find('.') != -1)
					{
						arg.myType = eScriptValueTypes::Float;
						double d = std::atof(argval.c_str());
						arg.myFloat = cast_f(d);

						for (char c : argval)
						{
							if ((c < '0' || c > '9' ) && c != '.')
							{
								isString = true;
							}
						}

					}
					else
					{

						for (char c : argval)
						{
							if (c < '0' || c > '9')
							{
								if (c != '.')
								{
									isString = true;
								}
							}
						}

						arg.myType = eScriptValueTypes::Int;
						int i = std::stoi(argval.c_str());
						arg.myInt = i;
					}

				}
				else
				{
					isString = true;
				}


				if (isString)
				{
					arg.myType = eScriptValueTypes::String;
					arg.myString = argval;
				}
				function.args.Add(arg);

			}
		}

		function.state = CScriptManager::GetInstance().GetLuaStatePtr();

		bool valid = true;
		for (std::string& blocked : myBlockedFunctions)
		{
			if (blocked == function.functionName)
			{
				AddResultText("Couldn't call function \"" + function.functionName + "\" in a console environment");
				valid = false;
			}
		}
		

		if (valid)
		{
			CScriptManager::GetInstance().CallLuaFunction(function);
		}
		myCurrentText.clear();

		ClearAutoComplete();
		myAutoCompleteSuggestionList.RemoveAll();
		myShowAutoComplete = false;
	}

	unsigned char keyCharPressed = InputManager::GetInstance().GetPressedKeyToChar();

	if (InputManager::GetInstance().CheckKeyPressed(KeyCode::Backspace) && myCurrentText.size() > 0)
	{
		if (!(myCurrentText.size() == 1 && myBlinkingMarkerVisible))
		{
			myCurrentText.pop_back();
		}

		if (myBlinkingMarkerVisible)
		{
			myBlinkingMarkerTimer = -1.0f;
		}
	}
	else if (InputManager::GetInstance().CheckKeyDown(KeyCode::Backspace))
	{
		myBackspaceTimer -= dt;
		if (myBackspaceTimer < 0 && myCurrentText.size() > 0)
		{
			myCurrentText.pop_back();
			myBackspaceTimer = 0.01f;
			if (myCurrentText.size() <= 0)
			{
				myBlinkingMarkerVisible = false;
			}
		}
	}
	else if (InputManager::GetInstance().CheckKeyReleased(KeyCode::Backspace))
	{
		myBackspaceTimer = 0.5f;
	}


	if (InputManager::GetInstance().CheckKeyPressed(KeyCode::Up))
	{
		if (myAutoCompleteSuggestionList.Size() <= 0)
		{
			ShowNewestCommand();
		}
		else
		{
			if (mySuggestionListBelowCommandText)
			{
				SelectSuggestionDown();
			}
			else
			{
				SelectSuggestionUp();
			}
		}
		//UpdateAutoComplete();
	}
	else if (InputManager::GetInstance().CheckKeyPressed(KeyCode::Down))
	{
		if (myAutoCompleteSuggestionList.Size() <= 0)
		{
			ShowOldestCommand();
		}
		else
		{
			if (mySuggestionListBelowCommandText)
			{
				SelectSuggestionUp();
			}
			else
			{
				SelectSuggestionDown();
			}
		}
		//UpdateAutoComplete();
	}

	if (InputManager::GetInstance().CheckKeyPressed(KeyCode::Tab))
	{
		if (myAutoCompleteSuggestionList.Size() <= 0)
		{
			ShowNewestCommand();
		}
		else
		{
			if (mySuggestionListBelowCommandText)
			{
				SelectSuggestionUp();
			}
			else
			{
				SelectSuggestionDown();
			}
		}
	}

	if (keyCharPressed != '\0')
	{
		if (myBlinkingMarkerVisible && myCurrentText.size() > 0)
		{
			myCurrentText.pop_back();
			myBlinkingMarkerVisible = false;
		}

		if (myShowAutoComplete)
		{
			if (myCurrentSelectedAutoComplete == -1)
			{
				myCurrentSelectedAutoComplete = 0;
			}
			myCurrentText = myAutoCompleteSuggestionList[myCurrentSelectedAutoComplete];
			myShowAutoComplete = false;
		}

		myCurrentText += keyCharPressed;
		

		ClearDoubleSpaces(myCurrentText);


		UpdateAutoComplete();
	}
	else if (!myShowAutoComplete)
	{
		if (myBlinkingMarkerTimer < 0)
		{
			myBlinkingMarkerTimer = 0.5f;
			if (!myBlinkingMarkerVisible)
			{
				myCurrentText += "l";
				myBlinkingMarkerVisible = true;
			}
			else if (myCurrentText.size() > 0)
			{
				myCurrentText.pop_back();
				myBlinkingMarkerVisible = false;
			}
		}
		else
		{
			myBlinkingMarkerTimer -= dt;
		}
	}


	if (InputManager::GetInstance().CheckKeyPressed(KeyCode::Backspace) ||
		InputManager::GetInstance().CheckKeyDown(KeyCode::Backspace))
	{
		if (myShowAutoComplete)
		{
			myShowAutoComplete = false;
			myCurrentText = myCurrentAutoComplete;

		}		
		UpdateAutoComplete();
	}



	if (InputManager::GetInstance().GetMouseWheelRotation() < 0)
	{
		if (myFirstShownResultID != myCurrentResultID - 1)
		{
			myCommandListChanged = true;
			myFirstShownResultID++;
			if (myFirstShownResultID > CONSOLE_LIMIT - 1)
			{
				myFirstShownResultID = 0;
			}
		}
	}
	else if (InputManager::GetInstance().GetMouseWheelRotation() > 0)
	{
		myCommandListChanged = true;
		myFirstShownResultID--;
		if (myFirstShownResultID < 0)
		{
			myFirstShownResultID = CONSOLE_LIMIT - 1;
		}
		if (myFirstShownResultID == myCurrentResultID + 15)
		{
			myFirstShownResultID = myCurrentResultID + 16;
		}
	}

	ShowResultText();

	return true;
}

void CGameConsole::SelectSuggestionUp()
{
	if (myCurrentSelectedAutoComplete == -1 || myCurrentSelectedAutoComplete >= myAutoCompleteSuggestionList.Size() - 1)
	{
		myCurrentSelectedAutoComplete = 0;
	}
	else
	{
		myCurrentSelectedAutoComplete++;
	}

	myCurrentText = myAutoCompleteSuggestionList[myCurrentSelectedAutoComplete];

	if (myBlinkingMarkerVisible)
	{
		myBlinkingMarkerVisible = false;
	}
}

void CGameConsole::SelectSuggestionDown()
{
	if (myCurrentSelectedAutoComplete == -1 || myCurrentSelectedAutoComplete == 0)
	{
		myCurrentSelectedAutoComplete = myAutoCompleteSuggestionList.Size() - 1;
	}
	else
	{
		myCurrentSelectedAutoComplete--;
	}

	myCurrentText = myAutoCompleteSuggestionList[myCurrentSelectedAutoComplete];

	if (myBlinkingMarkerVisible)
	{
		myBlinkingMarkerVisible = false;
	}
}

bool CGameConsole::Clear()
{
	myEngine.GetTextManager().RemoveText(myResultTextIndex);
	myEngine.GetTextManager().RemoveText(myCommandTextIndex);
	ClearAutoComplete();
	return true;
}

void CGameConsole::AddResultText(std::string aResultToAdd)
{
	myResultAddMutex.lock();
	if (myCurrentResultID > CONSOLE_LIMIT - 1)
	{
		myCurrentResultID = 0;
	}
	myResultList[myCurrentResultID] = aResultToAdd;
	myFirstShownResultID = myCurrentResultID;
	++myCurrentResultID;	
	myCommandListChanged = true;
	myResultAddMutex.unlock();
}

void CGameConsole::ShowResultText()
{
	if (myCommandListChanged)
	{
		myCommandListChanged = false;
		myShownResultText = "";
		for (int i = CONSOLE_ROW_LIMIT; i >= 0; --i)
		{
			if (myFirstShownResultID - i < 0)
			{
				myShownResultText += myResultList[CONSOLE_LIMIT - i];
			}
			else
			{
				myShownResultText += myResultList[myFirstShownResultID - i];
			}
			myShownResultText += "\n";
		}
	}
}

void CGameConsole::AddCommandText(std::string aCommandToAdd)
{
	myResultAddMutex.lock();
	int previousCommand = myCurrentCommandID - 1;

	if (myCurrentCommandID > CONSOLE_LIMIT - 1)
	{
		myCurrentCommandID = 0;
	}

	if (myCurrentCommandID == 0)
	{
		previousCommand = CONSOLE_LIMIT - 1;
	}

	if (myPreviousCommandsList[previousCommand].compare(aCommandToAdd) != 0)
	{
		myPreviousCommandsList[myCurrentCommandID] = aCommandToAdd;
		++myCurrentCommandID;
	}

	myPreviousInputIndex = myCurrentCommandID;

	myResultAddMutex.unlock();
}

void CGameConsole::ShowNewestCommand()
{
	myResultAddMutex.lock();

	if (myCurrentCommandID != 0)
	{
		--myPreviousInputIndex;

		if (myPreviousInputIndex == -1)
		{
			myPreviousInputIndex = myCurrentCommandID - 1;
		}

		myCurrentText = myPreviousCommandsList[myPreviousInputIndex];

		if (myBlinkingMarkerVisible)
		{
			myBlinkingMarkerVisible = false;
		}
	}
	myResultAddMutex.unlock();
}

void CGameConsole::ShowOldestCommand()
{
	myResultAddMutex.lock();

	if (myCurrentCommandID != 0)
	{
		++myPreviousInputIndex;

		if (myPreviousInputIndex >= myCurrentCommandID)
		{
			myPreviousInputIndex = 0;
		}

		myCurrentText = myPreviousCommandsList[myPreviousInputIndex];

		if (myBlinkingMarkerVisible)
		{
			myBlinkingMarkerVisible = false;
		}
	}

	myResultAddMutex.unlock();
}

void CGameConsole::UpdateAutoComplete()
{
	GetFuntionNames(myAutoCompleteSuggestionList);
	ClearAutoComplete();

	myCurrentAutoComplete = myCurrentText;
	std::transform(myCurrentAutoComplete.begin(), myCurrentAutoComplete.end(), myCurrentAutoComplete.begin(), ::tolower);

	int currentTextLenght;

	if (myBlinkingMarkerVisible)
	{
		currentTextLenght = static_cast<int>(myCurrentText.length()) - 1;
	}
	else
	{
		currentTextLenght = static_cast<int>(myCurrentText.length());
	}


	int offset = 0;
	for (short funcName = static_cast<short>(myAutoCompleteSuggestionList.Size() - 1); funcName >= 0; --funcName)
	{
		std::string subString = myAutoCompleteSuggestionList[funcName].substr(0, currentTextLenght);
		std::transform(subString.begin(), subString.end(), subString.begin(), ::tolower);

		if ((myCurrentAutoComplete.find(subString) != 0) ||
			((subString.length() == myAutoCompleteSuggestionList[funcName].length()) && (myCurrentAutoComplete.find(subString) == 0)))
		{
			myAutoCompleteSuggestionList.RemoveCyclicAtIndex(funcName);
		}
		else
		{
			++offset;
		}
	}

	for (int i = 0; i < myAutoCompleteSuggestionList.Size(); ++i)
	{
		if (mySuggestionListBelowCommandText)
		{
			myAutoCompleteIdexList.Add(myEngine.CreateText(myAutoCompleteSuggestionList[i], { myCommandTextPosition.x, myCommandTextPosition.y - (myRowSize * (i + 1)) }));
		}
		else
		{
			myAutoCompleteIdexList.Add(myEngine.CreateText(myAutoCompleteSuggestionList[i], { myCommandTextPosition.x, myCommandTextPosition.y + (myRowSize * (i + 1)) }));
		}
		
	}





	if (currentTextLenght == 0 ||
		myAutoCompleteSuggestionList.Size() == 0)
	{
		myCurrentAutoComplete = "";
		myAutoCompleteSuggestionList.RemoveAll();

		ClearAutoComplete();
	}
}

void CGameConsole::GetFuntionNames(Corgi::GrowingArray<std::string>& aArrayToFill)
{
	aArrayToFill.RemoveAll();
	myBlockedFunctions.RemoveAll();

	const Corgi::GrowingArray<SFunctionRegister>& registerArray = CScriptManager::GetInstance().GetExposedFunctions();

	for (SFunctionRegister& funcRegister : registerArray)
	{
		if (funcRegister.ExposedToConsole)
		{
			aArrayToFill.Add(funcRegister.functionName);
			aArrayToFill.GetLast() += " ";
		}
		else
		{
			myBlockedFunctions.Add(funcRegister.functionName);
		}
	}
}

void CGameConsole::ClearDoubleSpaces(std::string & aText)
{

	size_t textsize = aText.size();

	for (size_t i = 1; i < textsize && textsize > 2; ++i)
	{
		if (aText[i] == ' ' && aText[i - 1] == ' ')
		{
			aText.erase(i);
			--i;
			--textsize;
		}
	}


}

void CGameConsole::ClearAutoComplete()
{
	for (long long index : myAutoCompleteIdexList)
	{
		myEngine.GetTextManager().RemoveText(index);
	}
	myAutoCompleteIdexList.RemoveAll();
	myCurrentSelectedAutoComplete = -1;
}
