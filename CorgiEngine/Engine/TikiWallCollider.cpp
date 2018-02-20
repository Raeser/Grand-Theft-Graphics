#include "stdafx.h"
#include "TikiWallCollider.h"
#include "..\Engine\DebugDrawer.h"

CTikiWallCollider::CTikiWallCollider()
{
	myIsActive = true;
	myID = "";

}


CTikiWallCollider::~CTikiWallCollider()
{
	CPostMaster::GetInstance().Unsubscribe(this, eMessageType::DisableCollider);
}

void CTikiWallCollider::Init(const Vector3f& aPosition, const Vector3f& aRotation, const Vector3f& aSize, const char* aID)
{
	myPosition = aPosition;
	myRotation = aRotation;
	myID = aID;
	Corgi::ConvertStringToLowerCase(myID);
	mySize = aSize;
	CPostMaster::GetInstance().Subscribe(this, eMessageType::DisableCollider);

	const float& rot = aRotation.y + PIHalf;
	myEdges.Init(4);

	Vector3f N;
	N.x = aPosition.x + cos(rot) * (aSize.z / 2.f);
	N.z = aPosition.z + sin(rot) * (aSize.z / 2.f);

	Vector3f E;
	E.x = aPosition.x + cos(rot + PIHalf) * (aSize.x / 2.f);
	E.z = aPosition.z + sin(rot + PIHalf) * (aSize.x / 2.f);

	Vector3f S;
	S.x = aPosition.x + cos(rot + PI) * (aSize.z / 2.f);
	S.z = aPosition.z + sin(rot + PI) * (aSize.z / 2.f);

	Vector3f W;
	W.x = aPosition.x + cos(rot + PI + PIHalf) * (aSize.x / 2.f);
	W.z = aPosition.z + sin(rot + PI + PIHalf) *(aSize.x / 2.f);





	SLineWallCollider left;
	left.point = W;
	left.normal = (aPosition - left.point).GetNormalized();


	SLineWallCollider right;
	right.point = E;
	right.normal = (aPosition - right.point).GetNormalized();

	SLineWallCollider up;
	up.point = N;
	up.normal = (aPosition - up.point).GetNormalized();

	SLineWallCollider down;
	down.point = S;
	down.normal = (aPosition - down.point).GetNormalized();

	myEdges.Add(left);
	myEdges.Add(right);
	myEdges.Add(up);
	myEdges.Add(down);



#ifndef _RETAIL


	Vector3f NW = N + (right.normal.GetNormalized() * aSize.x / 2.f);
	Vector3f NE = N + (left.normal.GetNormalized() * aSize.x / 2.f);
	Vector3f SW = S + (right.normal.GetNormalized() * aSize.x / 2.f);
	Vector3f SE = S + (left.normal.GetNormalized() * aSize.x / 2.f);

	Vector4f NW4(NW, 1);
	Vector4f NE4(NE, 1);
	Vector4f SW4(SW, 1);
	Vector4f SE4(SE, 1);


	myWireframeLinesBottom.Init(5);
	myWireframeLinesTop.Init(5);

	myWireframeLinesBottom.Add(NW4);
	myWireframeLinesBottom.Add(NE4);
	myWireframeLinesBottom.Add(SE4);
	myWireframeLinesBottom.Add(SW4);
	myWireframeLinesBottom.Add(NW4);

	NW4 += Vector4f(0, aSize.y, 0, 0);
	NE4 += Vector4f(0, aSize.y, 0, 0);
	SE4 += Vector4f(0, aSize.y, 0, 0);
	SW4 += Vector4f(0, aSize.y, 0, 0);

	myWireframeLinesTop.Add(NW4);
	myWireframeLinesTop.Add(NE4);
	myWireframeLinesTop.Add(SE4);
	myWireframeLinesTop.Add(SW4);
	myWireframeLinesTop.Add(NW4);


#endif
}


void CTikiWallCollider::DrawWireFrame()
{
#ifndef _RETAIL
	if (myIsActive)
	{
		CDebugDrawer::GetInstance().DrawLine(myWireframeLinesBottom.begin(), myWireframeLinesBottom.Size());
		CDebugDrawer::GetInstance().DrawLine(myWireframeLinesTop.begin(), myWireframeLinesTop.Size());

		for (int i = 0; i < 4; ++i)
		{
			CDebugDrawer::GetInstance().DrawLine(myWireframeLinesBottom[i], myWireframeLinesTop[i]);
		}
	}
#endif
}


bool CTikiWallCollider::CheckCollision(const Vector3f & aPosition)
{
	if (myIsActive)
	{
		for (SLineWallCollider& edge : myEdges)
		{
			Vector3f posnormal = aPosition - edge.point;
			if (posnormal.Dot(edge.normal) < 0)
			{
				return false;
			}
		}
		return true;
	}
	else
	{
		return false;
	}

}


void CTikiWallCollider::RecieveMessage(const SMessage & message)
{
	myIDToCheck = message.disableCollider.id;
	Corgi::ConvertStringToLowerCase(myIDToCheck);
	if (myIDToCheck == myID)
	{
		if (myIsActive)
		{
			DL_WRITELOG(eConsoleColor::PURPLE, eDebugLogType::ResultMessages, "%s%s", " [WallCollider] - WallCollider disabled : ", myIDToCheck.c_str());
		}

		myIsActive = false;
	}
}
