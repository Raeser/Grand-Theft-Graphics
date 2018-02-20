#pragma once
#include "Component.h"
class CParticleEmitterInstance;
class CParticleEmitterComponent :
	public CComponent
{
	friend class CComponentBuilder;
public:
	CParticleEmitterComponent();
	~CParticleEmitterComponent();
	void Init(CParticleEmitterInstance* aInstance, float aLifeTime = 0.f);
	void Update() override;
	void FetchData() override;
	void SendData() override;
	void SetIsActive(bool aIsActive) override;
	void Release() override;

	void SetNewEmittor(long long aID);
	void SetShouldRender(bool aShouldRender);
	void SetShouldUpdateParticles(bool aShouldUpdate);

private:
	void SetEmitterInstance(CParticleEmitterInstance* aInstance);
private:
	CParticleEmitterInstance* myInstance;
	bool myIsInited;

	long long myParticleID;
	float myParticleTimer;
};

