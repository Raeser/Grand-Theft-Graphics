#pragma once
#include "Component.h"

class CPointLightInstance;

class CPointLightComponent :
	public CComponent
{
public:
	CPointLightComponent();
	~CPointLightComponent();
	void Init(const Vector3f & aPosition);
	void SetPointLightInstance(CPointLightInstance* aInstance);
	void SetNewPointLight(long long aID);
	void Update() override;
	void Release() override;
	void FetchData() override;
	void SendData() override;
	void ToggleLight(bool aShouldShine);
	void SetOffset(Vector3f aOffset);
	void SetIsActive(bool isActive);
private:
	CPointLightInstance* myLightInstance;
	Matrix44 myOrientation;
	Vector3f myOffset;
};

