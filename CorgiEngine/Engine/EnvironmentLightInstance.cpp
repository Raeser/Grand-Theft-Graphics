#include "stdafx.h"
#include "EnvironmentLightInstance.h"


CEnvironmentLightInstance::CEnvironmentLightInstance()
{
	myIsInitiated = false;
	myLightInstanceData.myColor = { 0.7f, 0.5f, 1.f, 1.f };
	myLightInstanceData.myDirection = { 0.7f, 0.7f, 0.7f, 1.f };
}


CEnvironmentLightInstance::~CEnvironmentLightInstance()
{
}

bool CEnvironmentLightInstance::Init(const Vector4f& aDirection, const Vector4f& aColor)
{
	myLightInstanceData.myColor = aColor;
	myLightInstanceData.myDirection = aDirection;
	myIsInitiated = true;

	return true;
}

const SLightInstanceData & CEnvironmentLightInstance::GetEnvironmentLightData()
{
	return myLightInstanceData;
}

void CEnvironmentLightInstance::SetEnvironmentLight(const long long aLightID)
{
	if (!myIsInitiated)
	{
		DL_ERROR(eDebugLogType::Error, "%s", "EnvironmentLight not initiated");
		assert(false && "EnvironmentLight not initiated");
	}
	myLightID = aLightID;
}

void CEnvironmentLightInstance::SetDirection(Vector4f aRot)
{
	myLightInstanceData.myDirection = aRot;
}

