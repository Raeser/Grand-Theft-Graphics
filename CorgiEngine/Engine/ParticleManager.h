#pragma once
#include "D3D11DataWrappers.h"
#include "..\Utilities\GrowingArray.h"
#include "ParticleEmitterLoader.h"

class CParticle;
class CParticleRenderer;

class CParticleManager
{
	friend CParticleRenderer;
private:
	struct SEmitterRenderData
	{
		Corgi::GrowingArray<CParticle, long long> myParticles;
		SParticleVertexDataWrapper vertexData;
		Matrix44 orientation;
		long long emittorId;
	};
public:
	CParticleManager(CParticleEmitterLoader& aEmittorLoader);
	~CParticleManager();
	bool Init();
	void AssignParticleEmittorInstances(const Corgi::GrowingArray<CParticleEmitterInstance*>& myEmittorInstances);
	CParticleEmitterLoader& GetParticleLoader();
	void Update(const Vector3f& aPositionToSortAfter);
	Corgi::GrowingArray<CParticleEmitterInstance*>& GetEmittorData();

	void SortInstances(const Vector3f& aPositionToSortAfter);

private:
	CParticleEmitterLoader& myLoader;
	Corgi::GrowingArray<CParticleEmitterInstance*> myParticleEmittorInstances;
	Corgi::GrowingArray<CParticleEmitterInstance*> myRenderData;
};

