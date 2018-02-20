#include "stdafx.h"
#include "../Utilities/InputManager.h"

CCursor::CCursor()
{
}


CCursor & CCursor::GetInstance()
{
	static CCursor cursor;
	return cursor;
}

void CCursor::Init(CEngine * aEngine)
{
	assert(aEngine != nullptr && "Engine-pointer was null!");

	myGameEngine = aEngine;

	myIsActive = true;
	/*
		mySpriteInstance = &myGameEngine->GetSpriteLoader().GetCursorSprite();
		mySpriteInstance->SetShouldRender(true);*/

	mySpriteWidth = (float)(32.f / CGameSettings::GetInstance().GetWindowResolution().x);
	mySpriteHeight = (float)(32.f / CGameSettings::GetInstance().GetWindowResolution().y);
}

void CCursor::ReInit()
{
}

Corgi::Vector3<float> CCursor::GetMousePos()
{
	//Corgi::Vector3<float> returnVec;
	//
	//RECT rect;
	//HWND hWnd = GetActiveWindow();
	//
	//GetWindowRect(hWnd, &rect);
	//
	//POINT windowPos;
	//windowPos.x = (rect.left);
	//windowPos.y = (rect.top);
	//
	//POINT p;
	//if (GetCursorPos(&p))
	//{
	//	returnVec.x = (float)p.x - (float)windowPos.x;
	//	returnVec.y = (float)p.y - (float)windowPos.y;
	//}
	//
	//
	//Corgi::Vector2<int> resolution = CGameSettings::GetInstance().GetWindowResolution();
	//
	//bool isFullscreen = CGameSettings::GetInstance().GetIsFullscreen();
	//
	//if (!isFullscreen)
	//{
	//	resolution.y -= (resolution.y / 27);
	//	resolution.x -= (resolution.x / 128);
	//}
	//
	//
	//returnVec.x /= resolution.x;
	//returnVec.y /= resolution.y;
	//
	Vector2f myVector(InputManager::GetInstance().GetMousePositionIngame_Normal());
	return Vector3f(myVector.x, myVector.y,0.f);
	//return returnVec;
}

Corgi::Vector2<int> CCursor::GetResolution()
{
	Corgi::Vector2<int> returnVec;

	RECT rect;
	HWND hWnd = GetActiveWindow();

	GetWindowRect(hWnd, &rect);

	POINT windowRes;
	windowRes.x = (rect.right);
	windowRes.y = (rect.bottom);

	returnVec.x = windowRes.x;
	returnVec.y = windowRes.y;

	return returnVec;
}

bool CCursor::Update(const Corgi::Vector2<float>& aMousePos)
{
	aMousePos;

	myPosition = Vector3f(InputManager::GetInstance().GetMousePositionIngame_Normal().x, InputManager::GetInstance().GetMousePositionIngame_Normal().y, 0.f);

	if (!CGameSettings::GetInstance().GetIsFullscreen())
	{
		myPosition.y -= 0.02575f;
		myPosition.x -= 0.002f;
	}
	else
	{
		myPosition.y += 0.0075f;
	}


	//if (mySpriteInstance) 
	//{
	//	float width = 32.f / (float)CGameSettings::GetInstance().GetWindowResolution().x;
	//	float height = 32.f / (float)CGameSettings::GetInstance().GetWindowResolution().y;

	//	mySpriteInstance->SetPosition({ myPosition.x + width / 2.5f, myPosition.y + height / 2.5f, 0.f });
	//}

	return true;
}

void CCursor::SetIsActive(const bool & aIsActive)
{
	myIsActive = aIsActive;
//	mySpriteInstance->SetShouldRender(myIsActive);
}

void CCursor::Destroy()
{
	/*if (mySpriteInstance != nullptr)
	{
		mySpriteInstance->Remove();
		mySpriteInstance = nullptr;
	}*/
}


CCursor::~CCursor()
{
}
