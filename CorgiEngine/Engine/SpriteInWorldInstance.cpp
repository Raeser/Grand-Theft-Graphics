#include "stdafx.h"
#include "SpriteInWorldInstance.h"
#include "..\Game\GameSettings.h"


CSpriteInWorldInstance::CSpriteInWorldInstance()
{
	mySprite = nullptr;
}

CSpriteInWorldInstance::~CSpriteInWorldInstance()
{
}

void CSpriteInWorldInstance::SetSprite(CSpriteInstance * aSpriteInstance)
{
	mySprite = aSpriteInstance;
}

CSpriteInstance * CSpriteInWorldInstance::GetSprite() const
{
	return mySprite;
}

void CSpriteInWorldInstance::SetWorldPositionFromOwnSprite()
{
	myWorldPosition = mySprite->GetPosition();
}
void CSpriteInWorldInstance::Render(CSpriteRenderer & aRenderer)
{

	const float scale = 10.f;
	Vector2int targetSize_int = CGameSettings::GetInstance().GetWindowResolution();
	targetSize_int.x = cast_int((targetSize_int.x * myWorldPosition.z) / scale);
	targetSize_int.y = cast_int((targetSize_int.y * myWorldPosition.z) / scale);
	aRenderer.Render(mySprite, targetSize_int);
}