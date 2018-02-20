#include "stdafx.h"
#include "AABB.h"


CAABB::CAABB()
{
}


CAABB::~CAABB()
{
}

void CAABB::Init(float aWidth, float aHeight, const Vector2f& aPos)
{
	myWidth = aWidth;
	myHeight = aHeight;
	myPosition = aPos;

	UpdatePosition(myPosition);
}

bool CAABB::IsCollidingWithPoint(const Corgi::Vector2<float>& aPoint)
{
	if (aPoint.x >= myMinX && aPoint.x <= myMaxX)
	{
		if (aPoint.y >= myMinY && aPoint.y <= myMaxY)
		{
			return true;
		}
	}

	return false;
}

void CAABB::UpdatePosition(const Vector2f & aPos)
{
	myPosition = aPos;

	myMaxX = myPosition.x + (myWidth / 2.f);
	myMinX = myPosition.x - (myWidth / 2.f);

	myMaxY = myPosition.y + (myHeight / 2.f);
	myMinY = myPosition.y - (myHeight / 2.f);
}
