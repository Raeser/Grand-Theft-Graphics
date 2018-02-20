#include "stdafx.h"
#include "Spline.h"
#include "..\Engine\DebugDrawer.h"

CSpline::CSpline()
{
}


CSpline::~CSpline()
{
}

void CSpline::Init()
{
	myPoints.Init(128);
}

void CSpline::AddPoint(const Corgi::Vector3<float>& aPoint)
{
	myPoints.Add(aPoint);
}

Corgi::Vector3<float> CSpline::GetClosestPositionOnSpline(const Corgi::Vector3<float>& aPoint)
{
	float closestDist = 999999999.f;
	Corgi::Vector3<float> closestPoint;

	for (int i = 0; i < myPoints.Size() - 1; i++)
	{
		Corgi::Vector3<float> point = GetClosestPosOnLine(myPoints[i], myPoints[i + 1], aPoint);
		Corgi::Vector3<float> toPos = point - aPoint;
		float dist = toPos.Length();

		if (dist < closestDist)
		{
			closestDist = dist;
			closestPoint = point;
		}
	}

	return closestPoint;
}

void CSpline::ClearPoints()
{
	myPoints.RemoveAll();
}

Corgi::Vector3<float> CSpline::GetClosestPosOnLine(const Corgi::Vector3<float>& aLineStart, const Corgi::Vector3<float>& aLineEnd, const Corgi::Vector3<float>& aPoint)
{

	Corgi::Vector3<float> closestPointOnLine = CGameSettings::GetInstance().GetClosestPointOnLine(aLineStart, aLineEnd, aPoint);
#ifndef _RETAIL
	Corgi::Vector4<float> pos1 = { aLineStart.x, aLineStart.y, aLineStart.z, 1.0f };
	Corgi::Vector4<float> pos2 = { aLineEnd.x, aLineEnd.y, aLineEnd.z, 1.0f };
	CDebugDrawer::GetInstance().DrawArrow(pos1, pos2, { 1.f, 0.f, 1.0f });
#endif

	return closestPointOnLine;
}
