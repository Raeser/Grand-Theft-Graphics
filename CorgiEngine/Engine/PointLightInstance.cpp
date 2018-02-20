#include "stdafx.h"
#include "PointLightInstance.h"


CPointLightInstance::CPointLightInstance()
{
	myIsOn = false;
}


CPointLightInstance::~CPointLightInstance()
{
}

bool CPointLightInstance::Init(const Vector4f & aPosition)
{
	myPosition = aPosition;
	myIsInitiated = true;
	myIsOn = true;
	return myIsInitiated;
}

bool CPointLightInstance::Init(const Vector3f & aPosition)
{
	myPosition.x = aPosition.x;
	myPosition.y = aPosition.y;
	myPosition.z = aPosition.z;
	myIsInitiated = true;
	myIsOn = true;
	return myIsInitiated;
}

void CPointLightInstance::SetPosition(const Vector4f & aPos)
{
	myPosition = aPos;
}

void CPointLightInstance::SetPosition(const Vector3f & aPos)
{
	myPosition.x = aPos.x;
	myPosition.y = aPos.y;
	myPosition.z = aPos.z;
}


void CPointLightInstance::SetPointLight(const long long & aID)
{
	myID = aID;
}

void CPointLightInstance::SetIsOn(bool aIsOn)
{
	myIsOn = aIsOn;
}

bool CPointLightInstance::GetShouldShine()
{
	return myIsOn;
}

const Vector4f & CPointLightInstance::GetPosition()
{
	return myPosition;
}

void CPointLightInstance::SetOrientation(const Matrix44 & aOrientation)
{
	SetPosition({ aOrientation.myMatrix[12], aOrientation.myMatrix[13], aOrientation.myMatrix[14], aOrientation.myMatrix[15] });
}
