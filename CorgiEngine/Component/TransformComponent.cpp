#include "stdafx.h"
#include "TransformComponent.h"
#include "Entity.h"

#define ROTATION_FORGIVNESS 0.01

CTransformComponent::CTransformComponent()
{
	myView = { 0, 0, 1 };
	myUpVector = { 0, 1, 0 };

	myRotationTarget = Vector3f();
	myRotationSpeed = 0.f;

	myShouldFaceObject = false;
}


CTransformComponent::~CTransformComponent()
{
}

void CTransformComponent::InternalInit(CEntity * aParent)
{
	CComponent::Init();
	SetParent(aParent);
	myIsInited = true;
	//myParent->GetData().Init(eOpaqueKey::IsAlive, true);
	myComponentTag = eComponentTag::Transform;
	myShouldRotateTowardsTarget = false;
}

void CTransformComponent::Init(CEntity * aParent, const Corgi::Matrix44<float>& aOriantation)
{
	DL_WARNING(eDebugLogType::Engine, "%s", "You inited a Transform with a Orientation! This is untested and might not work. If possible init with a position and rotation instead");
	myOrientation = aOriantation;
	myPosition = myOrientation.GetPosition();
	myRotation = myOrientation.GetRotation();
	InternalInit(aParent);
}

void CTransformComponent::Init(CEntity* aParent, const Corgi::Vector3<float>& aPosition, const Corgi::Vector3<float>& aRotation)
{
	myPosition = aPosition;
	myRotation = aRotation;
	BuildOrientationFromRotationAndPosition();
	InternalInit(aParent);
}

void CTransformComponent::Release()
{
	Reset();
	CComponent::Release();
}

void CTransformComponent::Reset()
{
	myPosition = Vector3f();
	myRotation = Vector3f();
	myRotationLastFrame = Vector3f();
	myPrevView = Vector3f();
	myPrevUp = Vector3f();
	myTargetView = Vector3f();
	myTargetUp = Vector3f();
	myView = Vector3f();
	myUpVector = Vector3f();
	myOrientation = Matrix44();
	myRotX = Matrix44();
	myRotY = Matrix44();
	myRotZ = Matrix44();
	myShouldFaceObject = false;
}

void CTransformComponent::Move(const Vector3f & v)
{
	myPosition += v;
}

void CTransformComponent::Move(const Vector4f & v)
{
	Move(v.V3());
}

void CTransformComponent::SetPosition(const Vector3f & v)
{
	myPosition = v;
}

void CTransformComponent::Rotate(const Vector3f & v)
{
	myRotation += v;
}

void CTransformComponent::Rotate(const Vector4f & v)
{
	Rotate(v.V3());
}


void CTransformComponent::SetShouldFace()
{
	myShouldFaceObject = true;
}

void CTransformComponent::FetchData()
{
}

void CTransformComponent::Update()
{
	TargetRotation();
	BuildOrientationFromRotationAndPosition();
}

void CTransformComponent::UpdateEulerAngles()
{
	Vector3f rotationDelta = myRotation - myRotationLastFrame;
	BuildOrientationFromEulerRotationDelta(rotationDelta);
}

void CTransformComponent::SendData()
{
}



void CTransformComponent::BuildOrientationFromEulerRotationDelta(const Vector3f& aRotation)
{
	myRotationLastFrame = myRotation;

	Vector4f rot4(aRotation, 0);

	myRotX = Matrix44::CreateRotateAroundX(rot4.x);
	myRotY = Matrix44::CreateRotateAroundY(rot4.y);
	myRotZ = Matrix44::CreateRotateAroundZ(rot4.z);

	Matrix44 rotate = myRotY * myRotZ * myRotX;
	myOrientation = rotate * myOrientation;

	myOrientation.myMatrix[12] = myPosition.x;
	myOrientation.myMatrix[13] = myPosition.y;
	myOrientation.myMatrix[14] = myPosition.z;
}

const Matrix44& CTransformComponent::GetOrientationMatrix() const
{
	return myOrientation;
}

const Vector3f& CTransformComponent::GetPosition() const
{
	return myPosition;
}

const Vector3f& CTransformComponent::GetRotation() const
{
	return myRotation;
}

void CTransformComponent::RotateTowardsTarget(const Vector3f & aTargetRotation, float aSpeed)
{
	myShouldRotateTowardsTarget = true;
	myRotationSpeed = aSpeed;

	myRotationTarget.x = SetTargetRotationToClosestPoint(aTargetRotation.x, myRotation.x);
	myRotationTarget.y = SetTargetRotationToClosestPoint(aTargetRotation.y, myRotation.y);
	myRotationTarget.z = SetTargetRotationToClosestPoint(aTargetRotation.z, myRotation.z);

	ClampRotationAndRotationTarget(myRotation.x, myRotationTarget.x);
	ClampRotationAndRotationTarget(myRotation.y, myRotationTarget.y);
	ClampRotationAndRotationTarget(myRotation.z, myRotationTarget.z);


}

void CTransformComponent::RotateTowardsTarget(const Vector4f& aTargetRotation, float aSpeed)
{
	RotateTowardsTarget(aTargetRotation.V3(), aSpeed);
}

void CTransformComponent::BuildOrientationFromRotationAndPosition()
{
	myRotX = Matrix44::CreateRotateAroundX(myRotation.x);
	myRotY = Matrix44::CreateRotateAroundY(myRotation.y);
	myRotZ = Matrix44::CreateRotateAroundZ(myRotation.z);

	Matrix44 matrixRotation = myRotY * myRotZ * myRotX;

	myView.x = (Corgi::Vector4<float>(0, 0, 1, 0) * matrixRotation).x;
	myView.y = (Corgi::Vector4<float>(0, 0, 1, 0) * matrixRotation).y;
	myView.z = (Corgi::Vector4<float>(0, 0, 1, 0) * matrixRotation).z;

	myUpVector.x = (Corgi::Vector4<float>(0, 1, 0, 0) * matrixRotation).x;
	myUpVector.y = (Corgi::Vector4<float>(0, 1, 0, 0) * matrixRotation).y;
	myUpVector.z = (Corgi::Vector4<float>(0, 1, 0, 0) * matrixRotation).z;

	Vector3f rightVector;
	rightVector.x = (Corgi::Vector4<float>(1, 0, 0, 0) * matrixRotation).x;
	rightVector.y = (Corgi::Vector4<float>(1, 0, 0, 0) * matrixRotation).y;
	rightVector.z = (Corgi::Vector4<float>(1, 0, 0, 0) * matrixRotation).z;

	Corgi::Vector3<float>& zaxis = myView;
	Corgi::Vector3<float>& xaxis = rightVector;
	Corgi::Vector3<float>& yaxis = myUpVector;

	Quat qZ(zaxis.x, zaxis.y, zaxis.z, myRotation.z);
	Quat qX(xaxis.x, xaxis.y, xaxis.z, myRotation.x);
	Quat qY(yaxis.x, yaxis.y, yaxis.z, myRotation.y);

	Quat quaternionOrientation = qZ * qX * qY;
	quaternionOrientation.Normalize();

	Matrix44 rotate = Matrix44::TransposeOpenGL(Matrix44::CreateRotationFromQuat(quaternionOrientation));
	Matrix44 translate;
	translate.myMatrix[12] = myPosition.x;
	translate.myMatrix[13] = myPosition.y;
	translate.myMatrix[14] = myPosition.z;

	myOrientation = rotate * translate;


	///If something is wrong with the position, try un-commenting back this code again
	///It should not contribute anything, but don't want to assume anything
	//myPosition.x = myOrientation.myMatrix[12];
	//myPosition.y = myOrientation.myMatrix[13];
	//myPosition.z = myOrientation.myMatrix[14];

}

float CTransformComponent::SetTargetRotationToClosestPoint(float aTargetRotation, float aCurrentRotation)
{
	float a = aTargetRotation;
	float b = aCurrentRotation;

	float closest;
	if (abs(a - PI2 - b) < abs(a - b))
	{
		closest = a - PI2;
	}
	else if (abs(a + PI2 - b) < abs(a - b))
	{
		closest = a + PI2;
	}
	else
	{
		closest = a;
	}

	return closest;
}

void CTransformComponent::ClampRotationAndRotationTarget(float& aRotation, float& aTargetRotation)
{
	if (aRotation > PI2)
	{
		aRotation -= PI2;
		aTargetRotation -= PI2;
	}
	if (aRotation < -PI2)
	{
		aRotation += PI2;
		aTargetRotation += PI2;
	}
}

void CTransformComponent::TargetRotation()
{
	if (myShouldRotateTowardsTarget)
	{
		const float& dt = Timer::GetInstance().GetDeltaTime();
		Vector3f step = (myRotationTarget - myRotation);
		if (step.Length() < ROTATION_FORGIVNESS)
		{
			myShouldRotateTowardsTarget = false;
		}
		else
		{
			myRotation += step.GetNormalized() * myRotationSpeed * dt;
		}


	}
}

