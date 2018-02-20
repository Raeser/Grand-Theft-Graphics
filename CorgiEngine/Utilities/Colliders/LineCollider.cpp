#include "stdafx.h"
#include "LineCollider.h"


CLineCollider::CLineCollider()
{
}


CLineCollider::~CLineCollider()
{
}

void CLineCollider::Init(const Corgi::Vector3<float>& aFirstPoint, const Corgi::Vector3<float>& aSecondPoint)
{
	myFirstPoint = aFirstPoint;
	mySecondPoint = aSecondPoint;

	myDirection = (mySecondPoint - aFirstPoint).GetNormalized();
}

//static bool closestpointonline(Vector3f direction, Vector3f start, Vector3f end, Vector3f Spherecenter, float fRadius)
//{
//	struct Segment
//	{
//		Vector3f start;
//		Vector3f end;
//	};
//	struct Sphere
//	{
//		Vector3f center;
//		float fRadius;
//	};
//
//	Vector3f point = Spherecenter;
//	end;
//	// Get the direction of the line segment from start to end
//	Vector3f line = direction;
//	float length =1000.f; // The length of the line
//	//line.Normalize();
//
//					// The Vector from the center of the sphere to the start of the line seg
//	Vector3f toStart = point - start;
//
//	// How far up the line the closest point is
//	float dist = line.Dot(toStart);
//	//dist *= -1.f;
//
//	// ensure that the closest point is on the line segment
//	if (dist < 0)
//		dist = 0;
//	else if (dist > length)
//		dist = length;
//
//	// Calculate the closest point
//	Vector3f ClosestPoint = start + line * dist;
//
//	Vector3f toPoint = Spherecenter - ClosestPoint;
//
//	// For optimization you can use the square distances
//	float dist2 = toPoint.Length(); // Equivelant to LengthSqu()
//
//	if (dist2 <= fRadius*fRadius)
//		return true;
//
//	return false;
//}

