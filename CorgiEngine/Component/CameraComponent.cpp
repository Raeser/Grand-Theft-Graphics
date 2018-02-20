#include "stdafx.h"
#include "CameraComponent.h"
#include "../Engine/CameraInstance.h"
#include "..\Engine\Camera.h"
#include"..\Engine\PostMaster.h"

CCameraComponent::CCameraComponent()
{
}


CCameraComponent::~CCameraComponent()
{
}

void CCameraComponent::Init(CEntity* aParent, CCameraInstance * aCameraInstance, const float& aRotationSpeed)
{
	CComponent::Init();
	SetParent(aParent);
	myIsInited = true;
	myRotationSpeed = aRotationSpeed;
	myCameraInstance = aCameraInstance;
	myRotation = myCameraInstance->SetViewToMouse(myRotationSpeed);
	myComponentTag = eComponentTag::Camera;

	myCurrentTime = 0.0f;
	myDuration = 0.0f;
	myShakeOffset = { 0.0f, 0.0f, 0.0f, };
	myCurrentOffset = { 0.0f, 0.0f, 0.0f };
}

void CCameraComponent::SetCameraInstance(CCameraInstance * aCameraInstance)
{
	myCameraInstance = aCameraInstance;
}

CCameraInstance * CCameraComponent::GetCameraInstance() const
{
	return myCameraInstance;
}

void CCameraComponent::FetchData()
{
	myIsActive = myParent->GetActiveStatus();
	myOrientation = myParent->GetTransform().GetOrientationMatrix();
	myRotation = myParent->GetTransform().GetRotation();
/*
	myCameraShakeDamage = *myParent->GetData().GetValue<bool>(eOpaqueKey::CameraShakeDamage);
	myCameraShakeBoost = *myParent->GetData().GetValue<bool>(eOpaqueKey::CameraShakeBoost);*/
}

void CCameraComponent::Update()
{
	assert(myIsInited && "I SHOULD USE INIT");

	/*if (myCameraShakeDamage)
	{
		myShakeOffset = { myLocalDamageShakeAmount, myLocalDamageShakeAmount, myLocalDamageShakeAmount, };
		myCurrentOffset = { myLocalDamageShakeAmount, myLocalDamageShakeAmount, myLocalDamageShakeAmount, };
		myDuration = myMaxDuration;
		myCurrentTime = myMaxDuration;
		myCameraShakeDamage = false;
	}*/

	/*if (myCameraShakeBoost)
	{
		if (myShakeOffset.x < myLocalBoostShakeAmount)
			myShakeOffset.x = myLocalBoostShakeAmount;

		if (myShakeOffset.y < myLocalBoostShakeAmount)
			myShakeOffset.y = myLocalBoostShakeAmount;

		if (myShakeOffset.z < myLocalBoostShakeAmount)
			myShakeOffset.z = myLocalBoostShakeAmount;

		if (myCurrentOffset.x < myLocalBoostShakeAmount)
			myCurrentOffset.x = myLocalBoostShakeAmount;

		if (myCurrentOffset.y < myLocalBoostShakeAmount)
			myCurrentOffset.y = myLocalBoostShakeAmount;

		if (myCurrentOffset.z < myLocalBoostShakeAmount)
			myCurrentOffset.z = myLocalBoostShakeAmount;

		myDuration = myMaxDuration;
		myCurrentTime = myMaxDuration;
		myCameraShakeBoost = false;
	}
*/

	float dt = Timer::GetInstance().GetDeltaTime();

	myCurrentTime -= dt;

	if (myCurrentTime <= 0.0f)
	{
		myCurrentTime = 0.0f;
		myDuration = 0.0f;
		myShakeOffset = { 0.0f, 0.0f, 0.0f, };
		myCurrentOffset = { 0.0f, 0.0f, 0.0f };
	}
	else
	{
		float randX = -myShakeOffset.x + Corgi::RandomFloat(0.0f, 1.0f) * (myShakeOffset.x - (-myShakeOffset.x));
		float randY = -myShakeOffset.y + Corgi::RandomFloat(0.0f, 1.0f) * (myShakeOffset.y - (-myShakeOffset.y));
		float randZ = -myShakeOffset.z + Corgi::RandomFloat(0.0f, 1.0f) * (myShakeOffset.z - (-myShakeOffset.z));

		float alpha = myCurrentTime / myDuration;

		alpha = alpha * alpha * (3.0f - 2.0f * alpha);

		myCurrentOffset.x = randX * alpha;
		myCurrentOffset.y = randY * alpha;
		myCurrentOffset.z = randZ * alpha;
	}
	
	myOrientation.myMatrix[12] += myCurrentOffset.x * myGlobalShakeAmount;
	myOrientation.myMatrix[13] += myCurrentOffset.y * myGlobalShakeAmount;
	myOrientation.myMatrix[14] += myCurrentOffset.z * myGlobalShakeAmount;

	myCameraInstance->SetOrientation(myOrientation);
	myRotation += myCameraInstance->SetViewToMouse(myRotationSpeed);

	SMessage msg;
	msg.messageType = eMessageType::CameraData;
	memcpy(&msg.cameraData.myToCamera, &myCameraInstance->GetCamera()->GetData().myToCamera, sizeof(float)*16);
	memcpy(&msg.cameraData.myProjection, &myCameraInstance->GetCamera()->GetData().myProjection, sizeof(float) * 16);

	CPostMaster::GetInstance().PostMessage(msg);
}

void CCameraComponent::SendData()
{
	///TODO: ROTATE TRANSFORM
	//myParent->GetData().SetValue(eOpaqueKey::Rotation, myRotation);
	/*
		myParent->GetData().SetValue(eOpaqueKey::CameraShakeDamage, myCameraShakeDamage);
		myParent->GetData().SetValue(eOpaqueKey::CameraShakeBoost, myCameraShakeBoost);*/
}

void CCameraComponent::Release()
{
	CComponent::Release();
}
