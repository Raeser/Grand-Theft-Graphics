#include "stdafx.h"
#include "CollisionManager.h"
#include "..\Utilities\Colliders\SphereCollider.h"
#include "..\Utilities\Colliders\LineCollider.h"
#include "..\Component\CollisionComponent.h"

#include "..\Component\HealthComponent.h"

#include "..\Audio\Audio\AudioInterface.h"

CCollisionManager::CCollisionManager()
{
}

CCollisionManager::~CCollisionManager()
{
}

void CCollisionManager::Init()
{
}

void CCollisionManager::Update()
{
}

void CCollisionManager::FetchData()
{
}

void CCollisionManager::SendData()
{
}

void CCollisionManager::CheckSphereColliders()
{
}

void CCollisionManager::HandleSphereCollision(CSphereCollider* aFirstSphere, CSphereCollider* aSecondSphere)
{
	aFirstSphere; aSecondSphere;
}

void CCollisionManager::HandleLineCollision(CSphereCollider * aSphere)
{
	aSphere;
}
