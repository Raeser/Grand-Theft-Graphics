#include "stdafx.h"
#include "SpotLight.h"


CSpotLight::CSpotLight()
{
}


CSpotLight::~CSpotLight()
{
}

bool CSpotLight::Init(const SSpotLightData & aData)
{
	myData = aData;
	return true;
}

const SSpotLightData & CSpotLight::GetSpotLightData() const
{
	return myData;
}

const float CSpotLight::GetRange()
{
	return myData.myRange;
}

void CSpotLight::SetRange(float aRange)
{
	myData.myRange = aRange;
}

const Vector3f & CSpotLight::GetColor()
{
	return myData.myColor;
}

void CSpotLight::SetColor(Vector3f & aColor)
{
	myData.myColor = aColor;
}

const float CSpotLight::GetAngle()
{
	return myData.myAngle;
}

void CSpotLight::SetAngle(float aAngle)
{
	myData.myAngle = aAngle;
}

