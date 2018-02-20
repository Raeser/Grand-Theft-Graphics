#pragma once

#include <Xinput.h>

// XInput Button values
static const WORD XINPUT_Buttons[] = {
	XINPUT_GAMEPAD_A,
	XINPUT_GAMEPAD_B,
	XINPUT_GAMEPAD_X,
	XINPUT_GAMEPAD_Y,
	XINPUT_GAMEPAD_DPAD_UP,
	XINPUT_GAMEPAD_DPAD_DOWN,
	XINPUT_GAMEPAD_DPAD_LEFT,
	XINPUT_GAMEPAD_DPAD_RIGHT,
	XINPUT_GAMEPAD_LEFT_SHOULDER,
	XINPUT_GAMEPAD_RIGHT_SHOULDER,
	XINPUT_GAMEPAD_LEFT_THUMB,
	XINPUT_GAMEPAD_RIGHT_THUMB,
	XINPUT_GAMEPAD_START,
	XINPUT_GAMEPAD_BACK
};

enum class XBOXCODE
{
	A,
	B,
	X,
	Y,
	DPAD_UP,
	DPAD_DOWN,
	DPAD_LEFT,
	DPAD_RIGHT,
	LEFT_SHOULDER,
	RIGHT_SHOULDER,
	LEFT_THUMB,
	RIGHT_THUMB,
	START,
	BACK
};

// XInput Button IDs
struct XButtonIDs
{
	XButtonIDs();

	// Member variables
	//---------------------//

	// 'Action' buttons
	int A;
	int B;
	int X;
	int Y;

	// Directional Pad (D-Pad)
	int DPad_Up;
	int DPad_Down;
	int DPad_Left;
	int DPad_Right;

	// Shoulder ('Bumper') buttons
	int L_Shoulder;
	int R_Shoulder;

	// Thumbstick buttons
	int L_Thumbstick;
	int R_Thumbstick;

	int Start;
	int Back;
};

class InputManager;

class CGamepad
{
	friend InputManager;

public:


	void Update();


	bool LStick_InDeadzone();
	bool RStick_InDeadzone();

	float LeftStick_X();  
	float LeftStick_Y();  
	float RightStick_X(); 
	float RightStick_Y(); 

	float LeftTrigger();  // Return value of left trigger
	float RightTrigger(); // Return value of right trigger

	bool GetButtonDown(int aButton);
	bool GetButtonPressed(int aButton);

	bool GetButtonDown(XBOXCODE aButton);
	bool GetButtonPressed(XBOXCODE aButton);

	// Utility functions
	XINPUT_STATE GetState(); // Return gamepad state
	int GetIndex();          // Return gamepad index
	bool Connected();        // Return true if gamepad is connected

	void SetShouldRumble(bool aValue);
	// Vibrate the gamepad (0.0f cancel, 1.0f max speed)
	void Rumble(float aLeftMotor = 0.0f, float aRightMotor = 0.0f, float aRumbleTime = 0.0f);
	bool GetAnyButtonPressed();


private:

	CGamepad();
	CGamepad(int aIndex);
	~CGamepad();
	void RefreshState();
	XINPUT_STATE myState; // Current gamepad state
	int myGamepadIndex;  // Gamepad index (eg. 1,2,3,4)

	static const int myButtonCount = 14;    // Total gamepad buttons

	bool myPrevButtonStates[myButtonCount]; // Previous frame button states
	bool myButtonStates[myButtonCount];     // Current frame button states

											// Buttons pressed on current frame
	bool myGamepadButtonsDown[myButtonCount];
	float myRumbleTimer;
	bool myShouldRumble;
};

// Externally define the 'XButtonIDs' struct as 'XButtons'
extern XButtonIDs XButtons;

/*

// The left stick is outside of its deadzone
if (!LStick_InDeadzone())
{
// Use left stick input here...
}

if (LeftTrigger() > 0.0f)
{
// The trigger has been pressed (even slightest tap)...
}

// Rumble left
Rumble(1.0f, 0.0f);

// Rumble right
Rumble(0.0f, 0.6f);

// Rumble both
Rumble(0.3f, 0.3f);

// Cancel rumble
Rumble();

*/