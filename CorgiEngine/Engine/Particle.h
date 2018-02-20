#pragma once
#include "..\Utilities\Vector3.h"

class CParticle
{
public:
	CParticle();
	//CParticle(const SParticleEmitterData& aData, const Matrix44& myOrientation);
	~CParticle();

	CParticle& operator=(const CParticle aParticle);
	bool operator<(const CParticle& aParticle);


	//void Update(const SParticleEmitterData& aData, const float dt);
	void Accelerate(float dt, Corgi::Vector3<float> aAcceleration);
	Corgi::Vector3<float> GetPosition();
	bool IsDead();
	void SetDistance(float aDistance);
	float GetDistance();
	float GetTime();
private:

	Corgi::Vector3<float> myPosition;
	float myLifeTime;
	Corgi::Vector3<float> myVelocity;
	float myTime;
	Corgi::Vector3<float> myColor;
	float mySize;
	Corgi::Vector3<float> TRASH;
	float myDistanceToEmitter;
};
