#pragma once
#include "Component.h"
class CLifeTimeComponent :
	public CComponent
{
public:
	CLifeTimeComponent();
	virtual ~CLifeTimeComponent();

	void Init(float aLifeTime);
	
	virtual void Update() override;
	virtual void FetchData()override;
	virtual void SendData() override;

	void Release() override;

	float GetTime() const;

private:

	float myTime;
};

