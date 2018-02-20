#include "stdafx.h"
#include "Particle.h"
#include "ParticleDataWrapper.h"


CParticle::CParticle()
{

}

//CParticle::CParticle(const SParticleEmitterData & aData, const Matrix44& myOrientation)
//{
//	aData;
//	myOrientation;
//	myPosition = { myOrientation.myMatrix[12], myOrientation.myMatrix[13], myOrientation.myMatrix[14] };
//	float angle = Corgi::RandomFloat(aData.myMinAngle, aData.myMaxAngle);
//	angle = Corgi::RADIANS(angle);
//
//	myVelocity = { 0, sin(angle), cos(angle) };
//	myVelocity *= Corgi::RandomFloat(aData.myMinStartSpeed, aData.myMaxStartSpeed);
//	myTime = 0.01f;
//	myLifeTime = Corgi::RandomFloat(aData.myMinLifeTime, aData.myMaxLifeTime);
//	mySize = aData.myStartScale;
//	if (mySize <= 0)
//	{
//		mySize = 0.000001f;
//	}
//	myColor = aData.myStartColor;
//}
//


CParticle::~CParticle()
{
}

CParticle & CParticle::operator=(const CParticle aParticle)
{
	myPosition = aParticle.myPosition;
	myLifeTime = aParticle.myLifeTime;
	myVelocity = aParticle.myVelocity;
	myTime = aParticle.myTime;
	return *this;
}

bool CParticle::operator<(const CParticle& aParticle)
{
	return myDistanceToEmitter < aParticle.myDistanceToEmitter;
}

//void CParticle::Update(const SParticleEmitterData& aData, const float dt)
//{
//	Accelerate(dt, aData.myAcceleration);
//	myTime += dt;
//	myPosition += myVelocity * dt;
//	mySize = Corgi::Lerp(aData.myStartScale, aData.myEndScale, GetTime());
//	myColor = Corgi::Lerp(aData.myStartColor, aData.myEndColor, GetTime());
//}

void CParticle::Accelerate(float dt, Corgi::Vector3<float> aAcceleration)
{
	myVelocity += aAcceleration * dt;
}

Corgi::Vector3<float> CParticle::GetPosition()
{
	Vector3f returnThis;
	returnThis.x = myPosition.x;
	returnThis.y = myPosition.y;
	return returnThis;
}

bool CParticle::IsDead()
{
	return myTime > myLifeTime;
}

void CParticle::SetDistance(float aDistance)
{
	myDistanceToEmitter = aDistance;
}

float CParticle::GetDistance()
{
	return myDistanceToEmitter;
}

float CParticle::GetTime()
{
	return myTime/myLifeTime;
}
