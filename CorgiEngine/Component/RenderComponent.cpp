#include "stdafx.h"
#include "..\Component\RenderComponent.h"
#include "../Engine/ModelInstance.h"
#include "../Engine/AnimationInstance.h"
CRenderComponent::~CRenderComponent()
{

}

void CRenderComponent::Init(const Vector3f& myPositionThisFrame,const Vector3f& aRotation, eCameraState aCameraState)
{
	CComponent::Init();
	myIsInited = true;
	myIsActive = true;
	myShouldRender = true;
	Matrix44 tempOrientation; 
	UpdateRotation(aRotation, tempOrientation);
	tempOrientation.myMatrix[12] = myPositionThisFrame.x;
	tempOrientation.myMatrix[13] = myPositionThisFrame.y;
	tempOrientation.myMatrix[14] = myPositionThisFrame.z;
	myInstance->SetOrientation(tempOrientation);
	myCameraState = aCameraState;

	/*myParent->GetData().Init<bool>(eOpaqueKey::ShouldRenderModel, myShouldRender);*/

	myComponentTag = eComponentTag::Render;
}

void CRenderComponent::UpdateRotation(const Vector3f& aRotation, Matrix44& aOriantation)
{
	Vector4f rot4;
	rot4.x = aRotation.x;
	rot4.y = aRotation.y;
	rot4.z = aRotation.z;

	Matrix44 myRotX;
	Matrix44 myRotY;
	Matrix44 myRotZ;
	myRotX = Matrix44::CreateRotateAroundX(rot4.x);
	myRotY = Matrix44::CreateRotateAroundY(rot4.y);
	myRotZ = Matrix44::CreateRotateAroundZ(rot4.z);

	Matrix44 rotate = myRotY * myRotZ * myRotX;

	aOriantation = rotate * aOriantation;
}

void CRenderComponent::FetchData()
{
	switch (myCameraState)
	{
	case Orientation:
		myInstance->SetOrientation(myParent->GetTransform().GetOrientationMatrix());
		break;

	case Position:
		Vector3f pos = myParent->GetTransform().GetOrientationMatrix().GetPosition();
		Matrix44 matrix;
		matrix.myMatrix[12] = pos.x;
		matrix.myMatrix[13] = pos.y;
		matrix.myMatrix[14] = pos.z;
		myInstance->SetOrientation(matrix);
		break;
	}

	if (myShouldRender == false)
	{
		myInstance->SetShouldRender(false);
	}
}

void CRenderComponent::Update()
{
	//assert(myIsInited && "I SHOULD USE INIT");
	if (myInstance && myInstance->DidAnimationFinishThisFrame())
	{
		SMessage msg;
		msg.messageType = eMessageType::CallbackEvent;
		msg.callbackEvent.callbackEvent = ECallbackEvent::AnimationDone;
		msg.callbackEvent.id = myParent->GetID().c_str();
		CPostMaster::GetInstance().PostMessage(msg);
	}

}

void CRenderComponent::SetShouldRender(bool aShouldRender)
{
	myInstance->SetShouldRender(aShouldRender);
}
void CRenderComponent::SendData()
{
}

void CRenderComponent::SetIsActive(bool aIsActive)
{
	myIsActive = aIsActive;
	myInstance->SetShouldRender(aIsActive);
}

void CRenderComponent::Animate(std::string aAnimation, bool aShouldLoop)
{
	myInstance->Animate(aAnimation, aShouldLoop);
}

void CRenderComponent::ResetCurrentAnimation()
{
	myInstance->myAnimationInstance->Reset();
}
void CRenderComponent::SetIsStatic(bool isStatic)
{
	myInstance->SetIsStatic(isStatic);
}

void CRenderComponent::SetModelInstance(CModelInstance * aInstance)
{
	myInstance = aInstance;
}

void CRenderComponent::SetNewModel(long long aID)
{
	myInstance->SetModel(aID);
}


void CRenderComponent::Release()
{
	myShouldRelease = true;
	SetShouldRender(false);
}