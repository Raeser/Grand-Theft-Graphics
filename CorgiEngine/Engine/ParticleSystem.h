#pragma once
#include"ParticleEmitter.h"

class CParticleSystem
{
public:
	CParticleSystem();
	~CParticleSystem();
	bool Init();
private:
	CParticleEmitter myEmitter;
};

