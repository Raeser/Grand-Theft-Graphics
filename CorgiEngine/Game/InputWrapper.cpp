#include "stdafx.h"
#include "InputWrapper.h"
#include "..\Utilities\InputManager.h"


CInputWrapper::CInputWrapper()
{
}


CInputWrapper::~CInputWrapper()
{
}

CInputWrapper & CInputWrapper::GetInstance()
{
	static CInputWrapper inputWrapper;
	return inputWrapper;
}

bool CInputWrapper::HasMouseMoved()
{
	//std::cout << "GetMousePositionDesktop_Pixel:  " << "X: [" << GetMousePositionDesktop_Pixel().x << "] Y: [" << GetMousePositionDesktop_Pixel().y << "]  --- ";
	//std::cout << "GetMousePositionInGame_Pixel:   " << "X: [" << GetMousePositionInGame_Pixel().x << "] Y: [" << GetMousePositionInGame_Pixel().y << "]  --- ";
	//std::cout << "GetMousePositionInWindow_Pixel: " << "X: [" << GetMousePositionInWindow_Pixel().x << "] Y: [" << GetMousePositionInWindow_Pixel().y << "]  --- ";
	//std::cout << "GetMousePositionInGame_Normal:  " << "X: [" << GetMousePositionInGame_Normal().x << "] Y: [" << GetMousePositionInGame_Normal().y << "]\n";
	Corgi::Vector2<float> mousePos = GetMousePositionInGame_Normal();

	if (myMousePos.x != mousePos.x || myMousePos.y != mousePos.y)
	{
		myMousePos = mousePos;
		return true;
	}

	myMousePos = mousePos;

	return false;
}

const Vector2int & CInputWrapper::GetMousePositionDesktop_Pixel()
{
	return InputManager::GetInstance().GetMousePositionInDesktop_Pixel();
}

const Vector2int & CInputWrapper::GetMousePositionInGame_Pixel()
{
	return InputManager::GetInstance().GetMousePositionIngame_Pixel();
}

const Vector2int  CInputWrapper::GetMousePositionInWindow_Pixel()
{
	return InputManager::GetInstance().GetMousePositionInWindow_Pixel();
}

const Vector2f & CInputWrapper::GetMousePositionInGame_Normal()
{
	return InputManager::GetInstance().GetMousePositionIngame_Normal();
}
