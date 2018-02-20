#pragma once
//class CSphereCollider;
//#include "Vector3.h"

class CLineCollider
{
public:
	CLineCollider();
	~CLineCollider();


	void							Init(const Corgi::Vector3<float>& aFirstPoint, const Corgi::Vector3<float>& aSecondPoint);

	//bool							CheckCollisionWithSphere(const CSphereCollider& aSphere);

private:

	Corgi::Vector3<float> myFirstPoint;
	Corgi::Vector3<float> mySecondPoint;
	Corgi::Vector3<float> myDirection;
};

