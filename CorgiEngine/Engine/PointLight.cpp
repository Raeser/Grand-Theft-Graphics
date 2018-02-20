#include "stdafx.h"
#include "PointLight.h"


CPointLight::CPointLight()
{
}


CPointLight::~CPointLight()
{
}

bool CPointLight::Init(const SPointLightData & aData)
{
	myData = aData;
	return true;
}

const SPointLightData & CPointLight::GetPointLightData() const
{
	return myData;
}

const float CPointLight::GetRange()
{
	return myData.myRange;
}

void CPointLight::SetRange(float aRange)
{
	myData.myRange = aRange;
}

const Vector3f& CPointLight::GetColor()
{
	return myData.myColor;
}

void CPointLight::SetColor(Vector3f & aColor)
{
	myData.myColor = aColor;
}
