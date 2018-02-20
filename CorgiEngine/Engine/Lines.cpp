#include "stdafx.h"
#include "Lines.h"


CLines::CLines()
{
}


CLines::~CLines()
{
}

void CLines::Init(int myMaxAmountOfPoints)
{
	myPoints.Init(myMaxAmountOfPoints);
}

void CLines::Reinit(int myMaxAmountOfPoints)
{
	myPoints.ReInit(myMaxAmountOfPoints);
}

void CLines::AddPoint(Vector4f aPosition, Vector3f aColor, float aWidth)
{
	SLinePoint pointToAdd;
	pointToAdd.myPosition = aPosition;
	pointToAdd.myColor = aColor;
	pointToAdd.myWidth = aWidth;

	myPoints.Add(pointToAdd);
}

__int64 CLines::GetAmountOfPoints() const
{
	return myPoints.Size();
}

Corgi::GrowingArray<SLinePoint, long long>& CLines::GetPoints() 
{
	return myPoints;
}




