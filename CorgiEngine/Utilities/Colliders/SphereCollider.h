#pragma once
#include "..\..\Utilities\Vector.h"
#include "..\..\Utilities\Colliders\CollisionLayer.h"

class CCollisionComponent;

class CSphereCollider
{
public:
	CSphereCollider();
	~CSphereCollider();

	void Init(const Corgi::Vector3<float>& aCenter, float aRadius, int aLayer, ColliderType aColliderType, CCollisionComponent* aParent);

	CCollisionComponent* GetParent() { return myParent; }
	const float& GetRadius() const { return myRadius; }
	const int& GetCollisionLayer() { return myCollisionLayer; }
	const ColliderType& GetColliderType() { return myColliderType; }

	const Corgi::Vector3<float>& GetPosition() const { return myCenter; }

	void SetPosition(Corgi::Vector3<float> aCenter) { myCenter = aCenter; }
	void SetIsActive(bool aActive) { myActive = aActive; }
	const bool IsActive() { return myActive; }

private:
	bool myActive;
	float myRadius;
	int myCollisionLayer;
	ColliderType myColliderType;
	Corgi::Vector3<float> myCenter;
	CCollisionComponent* myParent;
};

