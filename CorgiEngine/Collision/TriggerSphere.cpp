#include "stdafx.h"
#include "TriggerSphere.h"
#include "..\Game\unitystruct.h"
#include "..\Engine\DebugDrawer.h"

CTriggerSphere::CTriggerSphere()
{
}

CTriggerSphere::CTriggerSphere(const SUnityExporterTrigger & aTrigger)
{
	myTriggerShape = ETriggerShape::Sphere;
	Init(aTrigger);
}


CTriggerSphere::~CTriggerSphere()
{
}

void CTriggerSphere::Init(const SUnityExporterTrigger & aTrigger)
{
	Init(aTrigger.position, aTrigger.sphereCollider.radius, aTrigger.luaFunctionToCall);
}

void CTriggerSphere::Init(const Vector3f & aPos, float aRadius, const std::string & aLuaFilepath)
{
	ITriggerVolume::Init(aLuaFilepath);
	myPosition = aPos;
	myRadius = aRadius;

#ifndef _RETAIL

#define NUMBER_OF_POINTS 36

	debugpointsXY.Init(NUMBER_OF_POINTS + 1);
	debugpointsXZ.Init(NUMBER_OF_POINTS + 1);
	debugpointsZY.Init(NUMBER_OF_POINTS + 1);

	for (int i = 0; i < NUMBER_OF_POINTS + 1; ++i)
	{
		const float rot = Corgi::RADIANS(i * (360.f/ NUMBER_OF_POINTS));

		Vector4f xy = { myPosition.x, myPosition.y, myPosition.z, 1 };
		Vector4f xz = { myPosition.x, myPosition.y, myPosition.z, 1 };
		Vector4f zy = { myPosition.x, myPosition.y, myPosition.z, 1 };

		xy.x += cos(rot) * myRadius;
		xy.y += sin(rot) * myRadius;
		xz.x += cos(rot) * myRadius;
		xz.z += sin(rot) * myRadius;
		zy.z += cos(rot) * myRadius;
		zy.y += sin(rot) * myRadius;

		debugpointsXY.Add(xy);
		debugpointsXZ.Add(xz);
		debugpointsZY.Add(zy);
	}

#endif
	//myLuaScript.RegisterFunction("GetTikiCount", TIKICOUNT, "No args", "Within a trigger script, gets the amount of tikis withing that particar trigger");
}

float CTriggerSphere::GetRadius() const
{
	return myRadius;
}
#ifndef _RETAIL
void CTriggerSphere::DrawWireframe() const
{
	CDebugDrawer::GetInstance().DrawLine(debugpointsXY.begin(), NUMBER_OF_POINTS + 1);
	CDebugDrawer::GetInstance().DrawLine(debugpointsXZ.begin(), NUMBER_OF_POINTS + 1);
	CDebugDrawer::GetInstance().DrawLine(debugpointsZY.begin(), NUMBER_OF_POINTS + 1);
}
#endif
bool CTriggerSphere::CheckCollision(const Vector3f & aPosition)
{
	float distanceToEntity = (GetPosition() - aPosition).Length();
	if (distanceToEntity < GetRadius())
	{
		return true;
	}

	return false;
}

