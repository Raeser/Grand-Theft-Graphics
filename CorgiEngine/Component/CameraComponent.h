#pragma once
#include "Component.h"

class CCameraInstance;

class CCameraComponent :
	public CComponent
{
public:
	CCameraComponent();
	~CCameraComponent();
	void Init(CEntity* aParent, CCameraInstance * aCameraInstance, const float& aRotationSpeed);
	void SetCameraInstance(CCameraInstance* aCameraInstance);
	void FetchData() override;
	void Update();
	void SendData() override;
	void Release() override;

	CCameraInstance* GetCameraInstance() const;
private:
	CCameraInstance* myCameraInstance;
	Vector3f myRotation;
	Matrix44 myOrientation;
	float myRotationSpeed;


	//Shake
	bool myCameraShakeDamage;
	bool myCameraShakeBoost;
	float myDuration;
	float myCurrentTime;
	Vector3f myShakeOffset;
	Vector3f myCurrentOffset;

	float myMaxDuration;
	float myLocalDamageShakeAmount;
	float myLocalBoostShakeAmount;
	float myGlobalShakeAmount;
};

