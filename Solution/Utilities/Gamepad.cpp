#include "stdafx.h"
#include "Gamepad.h"

// Link the 'Xinput' library - Important!
#pragma comment(lib, "Xinput.lib")

// Define the 'XButtonIDs' struct as 'XButtons'
XButtonIDs XButtons;

// 'XButtonIDs' - Default constructor
XButtonIDs::XButtonIDs()
{
	// These values are used to index the XINPUT_Buttons array,
	// accessing the matching XINPUT button value

	A = 0;
	B = 1;
	X = 2;
	Y = 3;

	DPad_Up = 4;
	DPad_Down = 5;
	DPad_Left = 6;
	DPad_Right = 7;

	L_Shoulder = 8;
	R_Shoulder = 9;

	L_Thumbstick = 10;
	R_Thumbstick = 11;

	Start = 12;
	Back = 13;
}

CGamepad::CGamepad() {}

CGamepad::CGamepad(int aIndex)
{
	myGamepadIndex = aIndex - 1; // controller IDs go from 1-4 but Xinput wants 0-3. yay/nay?

								 // Iterate through all gamepad buttons
	for (int i = 0; i < myButtonCount; ++i)
	{
		myPrevButtonStates[i] = false;
		myButtonStates[i] = false;
		myGamepadButtonsDown[i] = false;
	}
}

CGamepad::~CGamepad() {}

void CGamepad::Update()
{
	myState = GetState(); // Obtain current gamepad state

						  // Iterate through all gamepad buttons
	for (int i = 0; i < myButtonCount; ++i)
	{
		// Set button state for current frame
		myButtonStates[i] = (myState.Gamepad.wButtons & XINPUT_Buttons[i]) == XINPUT_Buttons[i];

		// Set 'DOWN' state for current frame
		myGamepadButtonsDown[i] = !myPrevButtonStates[i] && myButtonStates[i];
	}
	myRumbleTimer -= Timer::GetInstance().GetDeltaTime();
	if (myRumbleTimer <= 0)
	{
		Rumble();
	}
	RefreshState();
}

// Update button states for next frame
void CGamepad::RefreshState()
{
	memcpy(myPrevButtonStates, myButtonStates, sizeof(myPrevButtonStates));
}

// Deadzone check for Left Thumbstick
bool CGamepad::LStick_InDeadzone()
{
	// Obtain the X & Y axes of the stick
	short sX = myState.Gamepad.sThumbLX;
	short sY = myState.Gamepad.sThumbLY;

	// X axis is outside of deadzone
	if (sX > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE ||
		sX < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		return false;

	// Y axis is outside of deadzone
	if (sY > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE ||
		sY < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		return false;

	// One (or both axes) axis is inside of deadzone
	return true;
}

// Deadzone check for Right Thumbstick
bool CGamepad::RStick_InDeadzone()
{
	// Obtain the X & Y axes of the stick
	short sX = myState.Gamepad.sThumbRX;
	short sY = myState.Gamepad.sThumbRY;

	// X axis is outside of deadzone
	if (sX > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE ||
		sX < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
		return false;

	// Y axis is outside of deadzone
	if (sY > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE ||
		sY < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
		return false;

	// One (or both axes) axis is inside of deadzone
	return true;
}

// Return X axis of left stick
// Return a value between -1 and 0.999969
float CGamepad::LeftStick_X()
{
	// Obtain X axis of left stick
	short sX = myState.Gamepad.sThumbLX;

	// Return axis value, converted to a float
	return (static_cast<float>(sX) / 32768.0f);
}

// Return Y axis of left stick
// Return a value between -1 and 0.999969
float CGamepad::LeftStick_Y()
{
	// Obtain Y axis of left stick
	short sY = myState.Gamepad.sThumbLY;

	// Return axis value, converted to a float
	return (static_cast<float>(sY) / 32768.0f);
}

// Return X axis of right stick
float CGamepad::RightStick_X()
{
	// Obtain X axis of right stick
	short sX = myState.Gamepad.sThumbRX;

	// Return axis value, converted to a float
	return (static_cast<float>(sX) / 32768.0f);
}

// Return Y axis of right stick
float CGamepad::RightStick_Y()
{
	// Obtain the Y axis of the left stick
	short sY = myState.Gamepad.sThumbRY;

	// Return axis value, converted to a float
	return (static_cast<float>(sY) / 32768.0f);
}

// Return value of left trigger
float CGamepad::LeftTrigger()
{
	// Obtain value of left trigger
	BYTE Trigger = myState.Gamepad.bLeftTrigger;

	if (Trigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
		return Trigger / 255.0f;

	return 0.0f; // Trigger was not pressed
}

// Return value of right trigger
float CGamepad::RightTrigger()
{
	// Obtain value of right trigger
	BYTE Trigger = myState.Gamepad.bRightTrigger;

	if (Trigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
		return Trigger / 255.0f;

	return 0.0f; // Trigger was not pressed
}

bool CGamepad::GetButtonPressed(int aButton)
{
	return myGamepadButtonsDown[aButton];
}

bool CGamepad::GetButtonDown(XBOXCODE aButton)
{
	return GetButtonDown(cast_int(aButton));
}

bool CGamepad::GetButtonPressed(XBOXCODE aButton)
{
	return GetButtonPressed(cast_int(aButton));
}

bool CGamepad::GetButtonDown(int aButton)
{
	if (myState.Gamepad.wButtons & XINPUT_Buttons[aButton])
	{
		return true; // The button is pressed
	}

	return false; // The button is not pressed
}

XINPUT_STATE CGamepad::GetState()
{
	// Temporary XINPUT_STATE to return
	XINPUT_STATE GamepadState;

	// Zero memory - Put all 0's in the container
	SecureZeroMemory(&GamepadState, sizeof(XINPUT_STATE));

	// Get the state of the gamepad
	XInputGetState(myGamepadIndex, &GamepadState);

	// Return the gamepad state
	return GamepadState;
}

int CGamepad::GetIndex()
{
	return myGamepadIndex;
}

bool CGamepad::Connected()
{
	// Zero memory - Put all 0's in the container
	SecureZeroMemory(&myState, sizeof(XINPUT_STATE));

	// Get the state of the gamepad
	DWORD Result = XInputGetState(myGamepadIndex, &myState);

	if (Result == ERROR_SUCCESS)
		return true;  // The gamepad is connected
	else
		return false; // The gamepad is not connected
}

void CGamepad::SetShouldRumble(bool aValue)
{
	myShouldRumble = aValue;
}

void CGamepad::Rumble(float aLeftMotor, float aRightMotor, float aRumbleTime)
{
	if (!myShouldRumble)
	{
		return;
	}
	// Vibration state
	XINPUT_VIBRATION VibrationState;

	// Zero memory
	SecureZeroMemory(&VibrationState, sizeof(XINPUT_VIBRATION));

	// Calculate vibration values
	WORD iLeftMotor = WORD(aLeftMotor * 65535.0f);
	WORD iRightMotor = WORD(aRightMotor * 65535.0f);

	// Set vibration values
	VibrationState.wLeftMotorSpeed = iLeftMotor;
	VibrationState.wRightMotorSpeed = iRightMotor;

	// Set the vibration state
	XInputSetState(myGamepadIndex, &VibrationState);

	// Rumble this long
	myRumbleTimer = aRumbleTime;
}

bool CGamepad::GetAnyButtonPressed()
{
	for (bool button : myGamepadButtonsDown)
	{
		if (button)
		{
			return true;
		}
	}
	return false;
}
