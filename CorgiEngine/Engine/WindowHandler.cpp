#include "stdafx.h"

#include "../Game/GameSettings.h"

LRESULT CALLBACK CWindowHandler::WinProc(HWND aWindowHandle, UINT aWindowsMessage, WPARAM aWParam, LPARAM aLParam)
{

	MSG mess;
	mess.hwnd = aWindowHandle;
	mess.message = aWindowsMessage;
	mess.wParam = aWParam;
	mess.lParam = aLParam;
	InputManager::GetInstance().UpdateCursorToScreen(mess);

	switch (aWindowsMessage)
	{
	case WM_SYSKEYDOWN:
		InputManager::GetInstance().SetKeyIsDown(KeyCode::LeftAlt);
		aWindowsMessage = 0;
		break;
	case WM_SYSKEYUP:
		InputManager::GetInstance().SetKeyIsUp(KeyCode::LeftAlt);
		aWindowsMessage = 0;
		break;
	case WM_KEYDOWN:
		if (aWParam < 256)
		{
			InputManager::GetInstance().SetKeyIsDown(static_cast<KeyCode>(aWParam));
		}
		break;
	case WM_KEYUP:
		if (aWParam < 256)
		{
			InputManager::GetInstance().SetKeyIsUp(static_cast<KeyCode>(aWParam));
		}
		break;
	case WM_LBUTTONDOWN:
		InputManager::GetInstance().SetKeyIsDown(static_cast<KeyCode>(VK_LBUTTON));
		break;
	case WM_LBUTTONUP:
		InputManager::GetInstance().SetKeyIsUp(static_cast<KeyCode>(VK_LBUTTON));
		break;
	case WM_RBUTTONDOWN:
		InputManager::GetInstance().SetKeyIsDown(static_cast<KeyCode>(VK_RBUTTON));
		break;
	case WM_RBUTTONUP:
		InputManager::GetInstance().SetKeyIsUp(static_cast<KeyCode>(VK_RBUTTON));
		break;
	case WM_MOUSEWHEEL:
		InputManager::GetInstance().SetMouseWheelDelta(static_cast<int>(aWParam));
		break;
		// this message is read when the window is closed
	case WM_DESTROY:
	{
		// close the application entirely
		PostQuitMessage(0);
		return 0;
	}
	
	}
	return DefWindowProc(aWindowHandle, aWindowsMessage, aWParam, aLParam);
}

CWindowHandler::CWindowHandler()
{
	myWindowHandle = nullptr;
}


CWindowHandler::~CWindowHandler()
{
}

bool CWindowHandler::Init(WindowData aWindowData, bool aBorderless)
{
	myWindowData = aWindowData;
	WNDCLASS windowclass = {};
	windowclass.style = CS_HREDRAW | CS_VREDRAW;
	windowclass.lpfnWndProc = CWindowHandler::WinProc;
	windowclass.hCursor = LoadCursor(GetModuleHandle(NULL), MAKEINTRESOURCE(512));
	windowclass.lpszClassName = "hej";
	RegisterClass(&windowclass);
	RECT wr = { 0, 0, static_cast<long>(aWindowData.width), static_cast<long>(aWindowData.height) };
	
	DWORD windowStyle = 0;
	if (aBorderless)
	{
		windowStyle = WS_POPUP | WS_SYSMENU | WS_VISIBLE | WS_OVERLAPPEDWINDOW | WS_MINIMIZEBOX;
	}
	else
	{
		windowStyle = WS_CAPTION | WS_SYSMENU | WS_MAXIMIZEBOX | WS_SYSMENU | WS_VISIBLE | WS_OVERLAPPED | WS_MINIMIZEBOX;		
	}

	myWindowHandle = CreateWindowEx(
		WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
		"hej",
		"Tikmin", 
		windowStyle,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		wr.right - wr.left,    // width of the window
		wr.bottom - wr.top,    // height of the window
		nullptr, 
		nullptr, 
		nullptr, 
		nullptr
	);
	ShowWindow(myWindowHandle, false);
	//UpdateWindow(myWindowHandle);

	// Fix to set the window to the actual resolution as the borders will mess with the resolution wanted
	if (!aBorderless)
	{
		Vector2int windowSize = { aWindowData.width, aWindowData.height };
		Vector2int resolution = windowSize;
		Vector2int myResolutionWithBorderDifference = resolution;

		RECT r;
		GetClientRect(myWindowHandle, &r); //get window rect of control relative to screen
		int horizontal = r.right - r.left;
		int vertical = r.bottom - r.top;

		int diffX = windowSize.x - horizontal;
		int diffY = windowSize.y - vertical;

		resolution = windowSize + Vector2int(diffX, diffY);
		myResolutionWithBorderDifference = windowSize + Vector2int(diffX, diffY);
		//::SetWindowPos(GetActiveWindow(), 0, -diffX / 2, -diffY / 2, myResolutionWithBorderDifference.x, myResolutionWithBorderDifference.y, SWP_NOMOVE | SWP_HIDEWINDOW | SWP_NOOWNERZORDER | SWP_NOZORDER);
		//::MoveWindow(GetActiveWindow(), -diffX/2, -diffY/2, myResolutionWithBorderDifference.x, myResolutionWithBorderDifference.y, true);
		::MoveWindow(myWindowHandle, -diffX/2, 0, myResolutionWithBorderDifference.x, myResolutionWithBorderDifference.y, true);
		
	}	
	ShowWindow(GetActiveWindow(), true);
	ShowWindow(myWindowHandle, true);
	return true;
}

HWND CWindowHandler::GetWindowHandle()
{
	return myWindowHandle;
}

CWindowHandler::WindowData CWindowHandler::GetWindowData()
{
	return myWindowData;
}
