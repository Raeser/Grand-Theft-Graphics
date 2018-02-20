#pragma once

class CInputWrapper
{
public:
	~CInputWrapper();

	static CInputWrapper& GetInstance();

	bool HasMouseMoved();
	const Vector2int & GetMousePositionDesktop_Pixel();
	const Vector2int& GetMousePositionInGame_Pixel();
	const Vector2int GetMousePositionInWindow_Pixel();
	const Vector2f& GetMousePositionInGame_Normal();

private:
	CInputWrapper();
	Corgi::Vector2<float> myMousePos;
};

