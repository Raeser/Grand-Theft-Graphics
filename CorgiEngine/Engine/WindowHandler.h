#pragma once
#include <windows.h>

class CWindowHandler
{

public:
	struct WindowData
	{
		int width;
		int height;
	};
	static LRESULT CALLBACK WinProc(HWND aWindowHandle, UINT aWindowsMessage, WPARAM aWParam, LPARAM aLParam);

	CWindowHandler();
	~CWindowHandler();


	bool Init(CWindowHandler::WindowData aWindowData, bool aFullScreen);

	HWND GetWindowHandle();
	WindowData GetWindowData();
private:
	WindowData myWindowData;
	HWND myWindowHandle;
};

