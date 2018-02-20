#include "..\stdafx.h"
#include "SphereCollider.h"


CSphereCollider::CSphereCollider()
{
}


CSphereCollider::~CSphereCollider()
{
}

void CSphereCollider::Init(const Corgi::Vector3<float>& aCenter, float aRadius, int aLayer, ColliderType aColliderType, CCollisionComponent* aParent)
{
	myCenter = aCenter;
	myRadius = aRadius;
	myActive = true;
	myColliderType = aColliderType;
	aLayer;
	aParent;
}
