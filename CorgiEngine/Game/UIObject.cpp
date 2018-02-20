#include "stdafx.h"
#include "UIObject.h"

void CUIObject::CreateSpriteInstance(const char* aPath)
{
	mySpriteIndex = myGameEngine->GetSpriteLoader().CreateSprite(aPath);
	mySpriteInstance = myGameEngine->GetSpriteLoader().CreateSpriteInstance(mySpriteIndex);
	mySpriteInstance->SetShouldRender(myIsActive);
	mySpriteInstance->SetPosition0To1(myPosition);
}
