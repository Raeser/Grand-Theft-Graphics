#pragma once
#include "Component.h"
#include "../Utilities/Vector4.h"

class CSpotLightInstance;

class CSpotLightComponent :
	public CComponent
{
public:
	CSpotLightComponent();
	~CSpotLightComponent();
	void SetLightData(Vector4f aPosition, Vector4f aDirection, long long aID);
	void CSpotLightComponent::Init(CEntity* aParent, long long aSpotLightTypeID, Vector4f& aDirection, bool aShouldShine);
	void Update() override;
	void Release() override;
	void FetchData() override;
	void SendData() override;

	void SetSpotLightInstance(CSpotLightInstance* aInstance);
	void SetNewSpotLight(long long aID);
	void ToggleLight(bool aShouldShine);
	void SetDirection(Vector4f& aDirection);
	void SetIsActive(bool isActive);
private:
	CSpotLightInstance* myLightInstance;
	Matrix44 myOrientation;

};

