#pragma once
#include "..\Utilities\Colliders\CollisionLayer.h"
#include "..\Engine\Subscriber.h"



class CSphereCollider;
class CLineCollider;

class CCollisionManager
{
public:

	CCollisionManager();
	~CCollisionManager();

	void Init();
	void Update();
	void FetchData();
	void SendData();

	void CheckSphereColliders();
	void HandleSphereCollision(CSphereCollider* aFirstSphere, CSphereCollider* aSecondSphere);
	void HandleLineCollision(CSphereCollider* aSphere);
	
private:

};

