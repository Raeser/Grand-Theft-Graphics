#include "stdafx.h"
#include "InputManager.h"
#include <assert.h>


InputManager::~InputManager()
{
}

// Updates cursor from WinProc in CWindowHandler;
void InputManager::UpdateCursorToScreen(const MSG &aMsg)
{
	GetCursorPos(&myMouseCursorPointClient);
	ScreenToClient(aMsg.hwnd, &myMouseCursorPointClient);
	myCurrentMousePositionClientPixel.x = myMouseCursorPointClient.x;
	myCurrentMousePositionClientPixel.y = myMouseCursorPointClient.y;

}
void InputManager::Update()
{
	GetCursorPos(&myMouseCursorPointDesktop);
	GetClientRect(GetActiveWindow(), &myClientRect);
	GetWindowRect(GetActiveWindow(), &myWindowRect);

	myCurrentMousePositionDesktopPixel.x = myMouseCursorPointDesktop.x;
	myCurrentMousePositionDesktopPixel.y = myMouseCursorPointDesktop.y;

	myPreviousFrameKeyStates = myCurrentFrameKeyStates;
	myMouseWheelRotation = 0;
	myDeltaMousePos = myCurrentMousePositionDesktopPixel - myPreviousMousePos;
	myPreviousMousePos = myCurrentMousePositionDesktopPixel;

	UpdateCusorPositionInGame();

	if (myWindowIsActive && myCursorShouldClipToSCreen && myShouldForceClipping)
	{
		POINT ul;
		ul.x = myClientRect.left;
		ul.y = myClientRect.top;

		POINT lr;
		lr.x = myClientRect.right;
		lr.y = myClientRect.bottom;

		MapWindowPoints(GetActiveWindow(), nullptr, &ul, 1);
		MapWindowPoints(GetActiveWindow(), nullptr, &lr, 1);

		myClientRect.left = ul.x;
		myClientRect.top = ul.y;

		myClientRect.right = lr.x;
		myClientRect.bottom = lr.y;

		ClipCursor(&myClientRect);
	}
	else
	{
		ClipCursor(nullptr);
	}
	GetGamepad().Update();
}

void InputManager::SetForceClipping(const bool& aBool)
{
	myShouldForceClipping = aBool;
}

void InputManager::SetClipCursorToScreen(const bool& aBool)
{
	myCursorShouldClipToSCreen = aBool;
}

const Vector2int& InputManager::GetDeltaMousePos()
{
	return myDeltaMousePos;
}
const Vector2int& InputManager::GetMousePositionInDesktop_Pixel()
{
	return myCurrentMousePositionDesktopPixel;
}
const Vector2int  InputManager::GetMousePositionInWindow_Pixel()
{
	myCursorPositionWindowPixel = myCurrentMousePositionDesktopPixel;
	const Vector2int windowSize(myWindowRect.right - myWindowRect.left, myWindowRect.bottom - myWindowRect.top);
	const Vector2int windowPos_BottomRight(myWindowRect.right, myWindowRect.bottom);
	const Vector2int windowPos_TopLeft(myWindowRect.left, myWindowRect.top);

	if (myCursorPositionWindowPixel.x <= windowPos_TopLeft.x)
	{
		myCursorPositionWindowPixel.x = windowPos_TopLeft.x;
	}
	else if (myCursorPositionWindowPixel.x >= windowPos_BottomRight.x)
	{
		myCursorPositionWindowPixel.x = windowPos_BottomRight.x;
	}

	if (myCursorPositionWindowPixel.y <= windowPos_TopLeft.y)
	{
		myCursorPositionWindowPixel.y = windowPos_TopLeft.y;
	}
	else if (myCursorPositionWindowPixel.y >= windowPos_BottomRight.y)
	{
		myCursorPositionWindowPixel.y = windowPos_BottomRight.y;
	}
	myCursorPositionWindowPixel.x -= windowPos_TopLeft.x;
	myCursorPositionWindowPixel.y -= windowPos_TopLeft.y;
	return myCursorPositionWindowPixel;

}
const Vector2int& InputManager::GetMousePositionIngame_Pixel()
{
	return myCursorPositionClientPixel;
}
const Vector2f& InputManager::GetMousePositionIngame_Normal()
{
	return myCursorPositionClientNormal;
}

void InputManager::SetWindowIsActive(bool abool)
{
	myWindowIsActive = abool;
}

void InputManager::UpdateCusorPositionInGame()
{	
	myCursorPositionClientPixel.x = myCurrentMousePositionClientPixel.x;
	myCursorPositionClientPixel.y = myCurrentMousePositionClientPixel.y;
	const Vector2int clientWindowSize(myClientRect.right - myClientRect.left, myClientRect.bottom - myClientRect.top);
	const Vector2int clientWindowPos_BottomRight(myClientRect.right, myClientRect.bottom);
	const Vector2int clientWindowPos_TopLeft(myClientRect.left, myClientRect.top);

	if (myCursorPositionClientPixel.x <= clientWindowPos_TopLeft.x)
	{
		myCursorPositionClientPixel.x = clientWindowPos_TopLeft.x;
	}
	else if (myCursorPositionClientPixel.x >= clientWindowPos_BottomRight.x)
	{
		myCursorPositionClientPixel.x = clientWindowPos_BottomRight.x;
	}

	if (myCursorPositionClientPixel.y <= clientWindowPos_TopLeft.y)
	{
		myCursorPositionClientPixel.y = clientWindowPos_TopLeft.y;
	}
	else if (myCursorPositionClientPixel.y >= clientWindowPos_BottomRight.y)
	{
		myCursorPositionClientPixel.y = clientWindowPos_BottomRight.y;
	}

	myCursorPositionClientNormal.x = static_cast<float>(myCursorPositionClientPixel.x) / static_cast<float>(clientWindowSize.x);
	myCursorPositionClientNormal.y = static_cast<float>(myCursorPositionClientPixel.y) / static_cast<float>(clientWindowSize.y);
}

void InputManager::InitKeysToCheck()
{
	myCharsToCheck.Init(256);



	myCharsToCheck.Add(KeyCode::Spacebar);

	myCharsToCheck.Add(KeyCode::Num0);
	myCharsToCheck.Add(KeyCode::Num1);
	myCharsToCheck.Add(KeyCode::Num2);
	myCharsToCheck.Add(KeyCode::Num3);
	myCharsToCheck.Add(KeyCode::Num4);
	myCharsToCheck.Add(KeyCode::Num5);
	myCharsToCheck.Add(KeyCode::Num6);
	myCharsToCheck.Add(KeyCode::Num7);
	myCharsToCheck.Add(KeyCode::Num8);
	myCharsToCheck.Add(KeyCode::Num9);


	myCharsToCheck.Add(KeyCode::Num9);

	myCharsToCheck.Add(KeyCode::Comma);
	myCharsToCheck.Add(KeyCode::Dot);
	myCharsToCheck.Add(KeyCode::Minus);
	myCharsToCheck.Add(KeyCode::Plus);

	myCharsToCheck.Add(KeyCode::A);
	myCharsToCheck.Add(KeyCode::B);
	myCharsToCheck.Add(KeyCode::C);
	myCharsToCheck.Add(KeyCode::D);
	myCharsToCheck.Add(KeyCode::E);
	myCharsToCheck.Add(KeyCode::F);
	myCharsToCheck.Add(KeyCode::G);
	myCharsToCheck.Add(KeyCode::H);
	myCharsToCheck.Add(KeyCode::I);
	myCharsToCheck.Add(KeyCode::J);
	myCharsToCheck.Add(KeyCode::K);
	myCharsToCheck.Add(KeyCode::L);
	myCharsToCheck.Add(KeyCode::M);
	myCharsToCheck.Add(KeyCode::N);
	myCharsToCheck.Add(KeyCode::O);
	myCharsToCheck.Add(KeyCode::P);
	myCharsToCheck.Add(KeyCode::Q);
	myCharsToCheck.Add(KeyCode::R);
	myCharsToCheck.Add(KeyCode::S);
	myCharsToCheck.Add(KeyCode::T);
	myCharsToCheck.Add(KeyCode::U);
	myCharsToCheck.Add(KeyCode::V);
	myCharsToCheck.Add(KeyCode::W);
	myCharsToCheck.Add(KeyCode::X);
	myCharsToCheck.Add(KeyCode::Y);
	myCharsToCheck.Add(KeyCode::Z);
}



void InputManager::SetMousePos(MSG &aMsg, int aXPosition, int aYPosition)
{
	POINT position;
	position.x = aXPosition;
	position.y = aYPosition;

	myCurrentMousePositionDesktopPixel.x = aXPosition;
	myCurrentMousePositionDesktopPixel.y = aYPosition;

	ClientToScreen(aMsg.hwnd, &position);
	SetCursorPos(position.x, position.y);
}

void InputManager::SetMousePos(MSG & aMsg, Vector2int aV)
{
	SetMousePos(aMsg, aV.x, aV.y);
}



bool InputManager::CheckKeyPressed(const KeyCode aKeyIndex)
{
	if (myCurrentFrameKeyStates[static_cast<int>(aKeyIndex)] == true && myPreviousFrameKeyStates[static_cast<int>(aKeyIndex)] == false)
	{
		return true;
	}
	return false;
}
bool InputManager::CheckKeyDown(const KeyCode aKeyIndex)
{

	if (myCurrentFrameKeyStates[static_cast<int>(aKeyIndex)] == true)
	{
		return true;
	}
	return false;


}
bool InputManager::CheckKeyReleased(const KeyCode aKeyIndex)
{
	if (myCurrentFrameKeyStates[static_cast<int>(aKeyIndex)] == false && myPreviousFrameKeyStates[static_cast<int>(aKeyIndex)] == true)
	{
		return true;
	}
	return false;
}
bool InputManager::CheckKeyUp(const KeyCode aKeyIndex)
{
	if (myCurrentFrameKeyStates[static_cast<int>(aKeyIndex)] == false && myPreviousFrameKeyStates[static_cast<int>(aKeyIndex)] == false)
	{
		return true;
	}
	return false;
}

void InputManager::SetKeyIsDown(const KeyCode aKey)
{
	myCurrentFrameKeyStates[static_cast<int>(aKey)] = true;
}
void InputManager::SetKeyIsUp(const KeyCode aKey)
{
	myCurrentFrameKeyStates[static_cast<int>(aKey)] = false;
}

int InputManager::GetMouseWheelRotation()
{
	return myMouseWheelRotation;
}

void InputManager::SetMouseWheelDelta(const int aDelta)
{
	myMouseWheelRotation = aDelta;
}

unsigned char CheckSpecialCases(unsigned char aCharToCheck, bool aIsShiftPressed) 
{
	if (aCharToCheck == static_cast<int>(KeyCode::Num7) && aIsShiftPressed)
	{
		return '/';
	}
	else if (aCharToCheck == static_cast<int>(KeyCode::Comma))
	{
		if (aIsShiftPressed)
		{
			return ';';
		}

		return ',';
	}
	else if (aCharToCheck == static_cast<int>(KeyCode::Dot))
	{
		if (aIsShiftPressed)
		{
			return ':';
		}

		return '.';
	}
	else if (aCharToCheck == static_cast<int>(KeyCode::Minus))
	{
		if (aIsShiftPressed)
		{
			return '_';
		}

		return '-';
	}
	else if (aCharToCheck == static_cast<int>(KeyCode::Plus))
	{
		if (aIsShiftPressed)
		{
			return '?';
		}

		return '+';
	}

	return 0;
}


char InputManager::GetPressedKeyToChar()
{
	bool shiftIsDown = false;
	bool isCapsLockOn = false;

	if ((GetKeyState(VK_CAPITAL) & 0x0001) != 0)
	{
		isCapsLockOn = true;
	}
	else
	{
		isCapsLockOn = false;
	}

	if (CheckKeyDown(KeyCode::Shift) || CheckKeyDown(KeyCode::RShift))
	{
		shiftIsDown = true;
	}

	//Check Alphabet
	for (int i = 0; i < myCharsToCheck.Size(); ++i)
	{
		if (CheckKeyPressed(static_cast<KeyCode>(myCharsToCheck[i])))
		{
			unsigned char charToReturn = static_cast<char>(myCharsToCheck[i]);

			//Special Cases
			char specialCaseChar = CheckSpecialCases(charToReturn, shiftIsDown);

			if (specialCaseChar != '\0') 
			{
				return specialCaseChar;
			}

			if (isalpha(charToReturn)) 
			{
				if (shiftIsDown && isCapsLockOn)
				{
					return static_cast<char>(tolower(charToReturn));
				}
				else if (shiftIsDown || isCapsLockOn)
				{
					return static_cast<char>(toupper(charToReturn));
				}
				

				return static_cast<char>(tolower(charToReturn));
				
			}

			return charToReturn;
		}
	}

	return 0;
}