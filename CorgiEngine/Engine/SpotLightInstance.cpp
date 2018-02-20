#include "stdafx.h"
#include "SpotLightInstance.h"


CSpotLightInstance::CSpotLightInstance()
{
	myPosition = { 0,0,0,0 };
	myDirection = { 0,0,1,0 };
	myIsInitiated = false;
	myIsOn = false;
}

CSpotLightInstance::~CSpotLightInstance()
{
}

bool CSpotLightInstance::Init(const Vector4f & aPosition, const Vector4f & aDirection)
{
	myIsOn = true;
	myPosition = aPosition;
	myDirection = aDirection;
	myIsInitiated = true;
	return myIsInitiated;
}

void CSpotLightInstance::SetPosition(const Vector3f & aPos)
{
	myPosition.x = aPos.x;
	myPosition.y = aPos.y;
	myPosition.z = aPos.z;
}

void CSpotLightInstance::SetPosition(const Vector4f & aPos)
{
	myPosition = aPos;
}

void CSpotLightInstance::SetDirection(const Vector4f & aDir)
{
	myDirection = aDir;
}

void CSpotLightInstance::SetSpotLight(const long long & aID)
{
	myID = aID;
}

void CSpotLightInstance::SetIsOn(bool aIsOn)
{
	myIsOn = aIsOn;
}

bool CSpotLightInstance::GetShouldShine()
{
	return myIsOn;
}

const Vector4f & CSpotLightInstance::GetPosition()
{
	return myPosition;
}

const Vector4f & CSpotLightInstance::GetDirection()
{
	return myDirection;
}

void CSpotLightInstance::SetOrientation(const Matrix44 & aOrientation)
{
	
	SetPosition({ aOrientation.myMatrix[12], aOrientation.myMatrix[13], aOrientation.myMatrix[14], aOrientation.myMatrix[15] });
}
