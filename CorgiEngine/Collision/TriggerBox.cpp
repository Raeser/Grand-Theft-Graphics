#include "stdafx.h"
#include "TriggerBox.h"
#include "..\Game\unitystruct.h"
#include "..\Engine\DebugDrawer.h"

CTriggerBox::CTriggerBox()
{
}


CTriggerBox::~CTriggerBox()
{
}

void CTriggerBox::Init(const SUnityExporterTrigger & aTrigger)
{
	Init(aTrigger.position, aTrigger.boxCollider.size, aTrigger.rotation, aTrigger.luaFunctionToCall);
}

void CTriggerBox::Init(const Vector3f & aPos, const Vector3f& aSize, const Vector3f & aRotation, const std::string & aLuaFilepath)
{
	const float& rot = aRotation.y + PIHalf;

	ITriggerVolume::Init(aLuaFilepath);
	myPosition = aPos;

	myEdges.Init(4);


	Vector3f N;
	N.x = aPos.x + cos(rot) * (aSize.z / 2.f);
	N.z = aPos.z + sin(rot) * (aSize.z / 2.f);

	Vector3f E;
	E.x = aPos.x + cos(rot + PIHalf) * (aSize.x / 2.f);
	E.z = aPos.z + sin(rot + PIHalf) * (aSize.x / 2.f);

	Vector3f S;
	S.x = aPos.x + cos(rot + PI) * (aSize.z / 2.f);
	S.z = aPos.z + sin(rot + PI) * (aSize.z / 2.f);

	Vector3f W;
	W.x = aPos.x + cos(rot + PI + PIHalf) * (aSize.x / 2.f);
	W.z = aPos.z + sin(rot + PI + PIHalf) *(aSize.x / 2.f);





	SLineCollider left;
	left.point = W;
	left.normal = (aPos - left.point).GetNormalized();


	SLineCollider right;
	right.point = E;
	right.normal = (aPos - right.point).GetNormalized();

	SLineCollider up;
	up.point = N;
	up.normal = (aPos - up.point).GetNormalized();

	SLineCollider down;
	down.point = S;
	down.normal = (aPos - down.point).GetNormalized();

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

	NW4	+= Vector4f(0, aSize.y, 0, 0);
	NE4	+= Vector4f(0, aSize.y, 0, 0);
	SE4	+= Vector4f(0, aSize.y, 0, 0);
	SW4	+= Vector4f(0, aSize.y, 0, 0);

	myWireframeLinesTop.Add(NW4);
	myWireframeLinesTop.Add(NE4);
	myWireframeLinesTop.Add(SE4);
	myWireframeLinesTop.Add(SW4);
	myWireframeLinesTop.Add(NW4);


#endif
}

bool CTriggerBox::CheckCollision(const Vector3f & aPosition)
{
	for (SLineCollider& edge : myEdges)
	{
		Vector3f posnormal = aPosition - edge.point;
		if (posnormal.Dot(edge.normal) < 0)
		{
			return false;
		}
	}
	return true;
}
#ifndef _RETAIL
void CTriggerBox::DrawWireframe() const
{
	CDebugDrawer::GetInstance().DrawLine(myWireframeLinesBottom.begin(), myWireframeLinesBottom.Size());
	CDebugDrawer::GetInstance().DrawLine(myWireframeLinesTop.begin(), myWireframeLinesTop.Size());

	for (int i = 0; i < 4; ++i)
	{
		CDebugDrawer::GetInstance().DrawLine(myWireframeLinesBottom[i], myWireframeLinesTop[i]);
	}
}
#endif