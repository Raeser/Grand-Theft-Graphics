#include "stdafx.h"
#include "PlaneCollider.h"


CPlaneCollider::CPlaneCollider()
{
	myHasBeenInited = false;
}

CPlaneCollider::~CPlaneCollider()
{
}

void CPlaneCollider::Init(Corgi::Vector3<float> aP1, Corgi::Vector3<float> aP2, Corgi::Vector3<float> aP3)
{
	myPosition = aP1;

	Corgi::Vector3<float> vector1 = aP2 - aP1;
	Corgi::Vector3<float> vector2 = aP3 - aP1;

	myNormal = vector2.Cross(vector1);

	myDistanceFromOrigin = myNormal.Dot(aP1);

	myHasBeenInited = true;
}

Corgi::Vector3<float> CPlaneCollider::GetLineIntersection(Corgi::Vector3<float> aPoint1, Corgi::Vector3<float> aPoint2)
{
	assert(myHasBeenInited && "Plane has not been inited");

	Corgi::Vector3<float> line = aPoint2 - aPoint1;

	float normalDotPoint1 = myNormal.Dot(aPoint1);
	float normalDotLine = myNormal.Dot(line);

	return aPoint1 + (((myDistanceFromOrigin - normalDotPoint1) / normalDotLine) * line);
}
