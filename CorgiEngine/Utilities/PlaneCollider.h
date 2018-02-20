#pragma once
#include "Vector.h"

struct SLinedata
{
	Corgi::Vector3<float> firstpoint;
	Corgi::Vector3<float> secondPoint;
};

class CPlaneCollider
{
public:

	CPlaneCollider();
	~CPlaneCollider();

	void Init(Corgi::Vector3<float> aP1, Corgi::Vector3<float> aP2, Corgi::Vector3<float> aP3);
	Corgi::Vector3<float> GetLineIntersection(Corgi::Vector3<float> aPoint1, Corgi::Vector3<float> aPoint2);

private:

	Corgi::Vector3<float> myNormal;
	Corgi::Vector3<float> myPosition;
	float myDistanceFromOrigin;

	bool myHasBeenInited;

};

