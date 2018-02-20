#pragma once
#include "Component.h"

class CModelInstance;
class CRenderComponent :
	public CComponent
{
public:

	enum eCameraState
	{
		Orientation,
		Position
	};

	~CRenderComponent();
	void Init(const Vector3f & myPositionThisFrame, const Vector3f & aRotation, eCameraState aCameraState);
	void UpdateRotation(const Vector3f & aRotation, Matrix44 & aOriantation);
	void SetModelInstance(CModelInstance* aInstance);
	void SetNewModel(long long aID);
	void Release() override;
	void SetShouldRender(bool aShouldShow);
	void FetchData() override;
	void Update();
	void SendData() override;
	void SetIsActive(bool aIsActive);
	void Animate(std::string aAnimation, bool aShouldLoop);
	void ResetCurrentAnimation();
	void SetIsStatic(bool isStatic);
private:
	CModelInstance* myInstance;
	eCameraState myCameraState;
	bool myShouldRender;
};

