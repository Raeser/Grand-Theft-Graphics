#include "stdafx.h"
#include "..\Component\SpriteComponent.h"
#include "../Engine/SpriteInstance.h"


CSpriteComponent::CSpriteComponent()
{
}


CSpriteComponent::~CSpriteComponent()
{
}

void CSpriteComponent::Init(Vector3f aPos, long long aID)
{
	CComponent::Init();
	myInstance->SetPosition0To1(aPos);
	myInstance->SetSprite(aID);
	//myParent->GetData().Init<Vector3f>(eOpaqueKey::Position, Vector3f());
	myComponentTag = eComponentTag::Sprite;
}

void CSpriteComponent::FetchData()
{
	Vector3f aPos = myParent->GetTransform().GetOrientationMatrix().GetPosition();
	aPos.x = (aPos.x * 2.f) - 1.f;
	aPos.y = (aPos.y * 2.f) - 1.f;

	myInstance->SetPosition0To1(aPos);
}

void CSpriteComponent::Update()
{
}

void CSpriteComponent::Release()
{
	CComponent::Release();
}

void CSpriteComponent::SendData()
{
}

void CSpriteComponent::SetIsActive(bool aIsActive)
{
	SetShouldRender(aIsActive);
}

void CSpriteComponent::SetNewSprite(long long aID)
{
	myInstance->SetSprite(aID);
}

void CSpriteComponent::SetShouldRender(bool aShouldRender)
{
	myInstance->SetShouldRender(aShouldRender);
}

void CSpriteComponent::SetPosition(Vector3f & aPos)
{
	myInstance->SetPosition0To1(aPos);
}

void CSpriteComponent::SetColor(Vector4f & aColor)
{
	myInstance->SetColor(aColor);
}

void CSpriteComponent::SetUVOffset(Vector2f & aOffset)
{
	myInstance->SetUVOffset(aOffset);
}

const Vector3f & CSpriteComponent::GetPosition()
{
	return myInstance->GetPosition();
}

const Vector4f & CSpriteComponent::GetColor()
{
	return myInstance->GetColor();
}

const Vector2f & CSpriteComponent::GetUVOffset()
{
	return myInstance->GetUVOffset();
}

void CSpriteComponent::SetSpriteInstance(CSpriteInstance * aInstance)
{
	myInstance = aInstance;
}
