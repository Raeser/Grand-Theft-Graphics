#include "stdafx.h"
#include "CameraInstance.h"
#include <DirectXMath.h>
#include "..\Wwise\Audio\Audio\AudioInterface.h"

#define ROTATION_CLAMP 2.1f

CCameraInstance::CCameraInstance()
{
}


CCameraInstance::~CCameraInstance()
{
}


bool CCameraInstance::Init(SCameraData & aCameraData, CDXFramework* aFramework)
{
	myNearlPlaneDistance = 0.1f;
	myFarPlaneDistance = 1000.f;
	myCamera = new CCamera();
	myOrientation.SetPosition(aCameraData.myPosition);
	myCamera->Init(aFramework, Matrix44(DirectX::XMMatrixPerspectiveFovLH(aCameraData.myFoV, (16.f / 9.f), myNearlPlaneDistance, myFarPlaneDistance)));
	
	//TODO: VALUES FROM JSON!!!  
	
	myDeadzone = 0.02f;
	mySpeed = 8.f;
	return true;
}

void CCameraInstance::SetOrientation(const Matrix44& aOrientation)
{
	myOrientation = aOrientation;
}

void CCameraInstance::SetPosition(const Vector3f & aPosition)
{
	myOrientation.myMatrix[12] = aPosition.x;
	myOrientation.myMatrix[13] = aPosition.y;
	myOrientation.myMatrix[14] = aPosition.z;
}

Matrix44 CCameraInstance::GetOrientation()
{
	return myOrientation;
}

Vector3f CCameraInstance::GetPosition() const
{
	return myOrientation.GetPosition();
}

void CCameraInstance::Transform(const Matrix44 & aTransformation)
{
	myOrientation = aTransformation * myOrientation;
}

Vector3f CCameraInstance::FromWorldSpaceToProjectionSpace(const Vector3f & aPos) const
{
	Vector4f v(aPos, 1);
	v = v * myCamera->GetData().myToCamera;
	v = v * myCamera->GetData().myProjection;

	Vector3f position = v.V3() / v.w;
	position.z = v.w;
	return position;
}

Vector3f CCameraInstance::GetWorldPositionOfMouseOnNearlplane(const Vector3f & aMousePositionOnScreen, const Corgi::Vector2<int>& currentResolution) const
{
	Corgi::Vector3<float> mousePos = aMousePositionOnScreen;

	mousePos = mousePos * 2.f;
	mousePos.x -= 1.f;
	mousePos.y -= 1.f;
	mousePos.y *= -1.f;

	mousePos.x = -mousePos.x;
	mousePos.y = -mousePos.y;

	float tang = tanf(67.5f / 2.f);

	float height = tang * myNearlPlaneDistance;
	float width = height * ((float)currentResolution.x / (float)currentResolution.y);

	Corgi::Vector3<float> cameraUp;
	cameraUp.x = myOrientation.myMatrix[4];
	cameraUp.y = myOrientation.myMatrix[5];
	cameraUp.z = myOrientation.myMatrix[6];

	Corgi::Vector3<float> cameraRight;
	cameraRight.x = myOrientation.myMatrix[0];
	cameraRight.y = myOrientation.myMatrix[1];
	cameraRight.z = myOrientation.myMatrix[2];

	Corgi::Vector3<float> cameraForward;
	cameraForward.x = myOrientation.myMatrix[8];
	cameraForward.y = myOrientation.myMatrix[9];
	cameraForward.z = myOrientation.myMatrix[10];

	Corgi::Vector3<float> pRight = (cameraRight * mousePos.x * width) * ((float)currentResolution.y / (float)currentResolution.x);
	Corgi::Vector3<float> pUp = (cameraUp * mousePos.y * height) * ((float)currentResolution.y / (float)currentResolution.x);
	Corgi::Vector3<float> pForward = cameraForward * myNearlPlaneDistance;

	Corgi::Vector3<float> pointToReturn = pRight + pUp + pForward;

	//pointToReturn.y *=;

	Corgi::Vector3<float> nearPlaneWorldPos;
	nearPlaneWorldPos.x = GetPosition().x + pointToReturn.x;
	nearPlaneWorldPos.y = GetPosition().y + pointToReturn.y;
	nearPlaneWorldPos.z = GetPosition().z + pointToReturn.z;

	return nearPlaneWorldPos;
}

bool CCameraInstance::isPointInViewSpace(const Vector3f & aPosition)
{
	return isPointInViewSpace(aPosition, 1.1f);
}

bool CCameraInstance::isPointInViewSpace(const Vector3f & aPosition, float aCap)
{
	Vector3f position = aPosition;

	Vector4f v(aPosition, 1.f);
	v = v * myOrientation.GetFastInverse();
	v = v * myCamera->GetData().myProjection;

	position = v.V3() / v.w;

	Vector2f screenspace;
	screenspace.x = position.x;
	screenspace.y = position.y;

	float length2 = screenspace.Length2();

	if (length2 > (aCap * aCap))
	{
		return false;
	}

	//if (v.z < 0)
	//{
	//	return false;
	//}

	return true;
}


void CCameraInstance::Update()
{
	//myPosition.Update();

	Corgi::Vector3<float> vY = { myOrientation.myMatrix[4], myOrientation.myMatrix[5], myOrientation.myMatrix[6] };
	Corgi::Vector3<float> vZ = { myOrientation.myMatrix[8], myOrientation.myMatrix[9], myOrientation.myMatrix[10] };

	//Wwise Listener-Position set, please contact your closest Wwise-preofessional before changing or moving
	CAudioInterface::GetInstance()->SetListenerPosition(myOrientation.myMatrix[12], myOrientation.myMatrix[13], myOrientation.myMatrix[14], vZ.x, vZ.y, vZ.z, vY.x, vY.y, vY.z);

	CAudioInterface::GetInstance()->SetPosition(myOrientation.myMatrix[12], myOrientation.myMatrix[13], myOrientation.myMatrix[14], 1);
}

Vector3f CCameraInstance::SetViewToMouse(const float& /*aSensitivity*/)
{
	//Vector3f cursorPos = myCursorPos;

	//cursorPos = { cursorPos.x, cursorPos.y, 0.f };

	//Vector3f middle(0.5f, 0.5f, 0.f);
	//Vector3f dMouse = cursorPos - middle;

	//const float sensitivity = aSensitivity;
	//const float dt = Timer::GetInstance().GetDeltaTime();
	//Vector3f rotation(-(Corgi::RADIANS(static_cast<float>(dMouse.y)* dt* sensitivity)), Corgi::RADIANS(static_cast<float>(dMouse.x)* dt* sensitivity), 0);

	////TODO: Check if this is right to do so:
	//while (rotation.x < -PI) rotation.x += 2 * PI;
	//while (rotation.x > PI) rotation.x += -2 * PI;

	//while (rotation.y < -PI / 2.f) rotation.y += PI;
	//while (rotation.y > PI / 2.f) rotation.y += -PI;

	//while (rotation.z < -PI) rotation.z += 2 * PI;
	//while (rotation.z > PI) rotation.z += -2 * PI;


	//Vector2f dMouse2;
	//dMouse2.x = cursorPos.x - middle.x;
	//dMouse2.y = cursorPos.y - middle.y;

	//if (dMouse2.Length() < myDeadzone)
	//{
	//	rotation.x = 0;
	//	rotation.y = 0;
	//}

	//return rotation;
	////myPosition.Rotate(rotation * -1.f);	
	return Vector3f();
}

void CCameraInstance::UpdateBuffers(CDXFramework * aDXFramework)
{
	myCamera->UpdateBuffers(aDXFramework, this);
}