#include "stdafx.h"
#include "PositionComponent.h"
#include "Entity.h"

CPositionComponent::CPositionComponent()
{
	myPosition = { 0, 0, 0 };
	myView = { 0, 0, 1 };
	myUpVector = { 0, 1, 0 };

	rotationDeltaSpeed = 8000.f;
}


CPositionComponent::~CPositionComponent()
{
}

void CPositionComponent::Move(Vector3f aMovement)
{
	Vector4f v(aMovement.x, aMovement.y, aMovement.z, 1);
	myPosition.x = (v * myOrientation).x;
	myPosition.y = (v * myOrientation).y;
	myPosition.z = (v * myOrientation).z;

	//Quick fix by Mr. Zeb, u can kill me later
	myOrientation.SetPosition(myPosition);
}

void CPositionComponent::Rotate(const Vector3f & aRotation)
{
	const float dt = Timer::GetInstance().GetDeltaTime();

	Vector3f v = aRotation;

	myRotation += v;


	//Matrix44 rotX = Matrix44::CreateRotateAroundX(myRotation.x);
	//Matrix44 rotY = Matrix44::CreateRotateAroundY(myRotation.y);
	//Matrix44 rotZ = Matrix44::CreateRotateAroundZ(myRotation.z);
	//
	//Matrix44 totalRotation = rotZ * rotY * rotX;
	//
	//
	//
	//Vector4f v(aRotation.x, aRotation.y, aRotation.z, 0);
	//myRotation.x += (v * totalRotation).x;
	//myRotation.y += (v * totalRotation).y;
	//myRotation.z += (v * totalRotation).z;
// 
 	const float max = Corgi::RADIANS(360);

// 	float l = myRotation.Length();
// 	if (l > max)
// 	{
// 		l = max;
// 	}
// 	myRotation.Normalize();
/*	myRotation *= l;*/
	if (myRotation.x > max)
	{
		myRotation.x -= max;
	}
	if (myRotation.x < -max)
	{
		myRotation.x += max;
	}
	if (myRotation.y > max)
	{
		myRotation.y -= max;
	}
	if (myRotation.y < -max)
	{
		myRotation.y += max;
	}
	if (myRotation.z > max)
	{
		myRotation.z -= max;
	}
	if (myRotation.z < -max)
	{
		myRotation.z += max;
	}

}

void CPositionComponent::Update()
{

	//myPrevView = myView;
	//myPrevUp = myUpVector;
	//
	//
	//myTargetView.x = (Corgi::Vector4<float>(0, 0, 1, 0) *myOrientation).x;
	//myTargetView.y = (Corgi::Vector4<float>(0, 0, 1, 0) *myOrientation).y;
	//myTargetView.z = (Corgi::Vector4<float>(0, 0, 1, 0) *myOrientation).z;
	//
	//myTargetUp.x = (Corgi::Vector4<float>(0, 1, 0, 0) * myOrientation).x;
	//myTargetUp.y = (Corgi::Vector4<float>(0, 1, 0, 0) * myOrientation).y;
	//myTargetUp.z = (Corgi::Vector4<float>(0, 1, 0, 0) * myOrientation).z;

	//myView.x = (Corgi::Vector4<float>(0, 0, 1, 0) *myOrientation).x;
	//myView.y = (Corgi::Vector4<float>(0, 0, 1, 0) *myOrientation).y;
	//myView.z = (Corgi::Vector4<float>(0, 0, 1, 0) *myOrientation).z;
	//
	//myUpVector.x = (Corgi::Vector4<float>(0, 1, 0, 0) * myOrientation).x;
	//myUpVector.y = (Corgi::Vector4<float>(0, 1, 0, 0) * myOrientation).y;
	//myUpVector.z = (Corgi::Vector4<float>(0, 1, 0, 0) * myOrientation).z;


	Matrix44 rotX = Matrix44::CreateRotateAroundX(myRotation.x);
	Matrix44 rotY = Matrix44::CreateRotateAroundY(myRotation.y);
	Matrix44 rotZ = Matrix44::CreateRotateAroundZ(myRotation.z);

	//Matrix44 totalRotation = rotZ * rotY * rotX;
	//Matrix44 totalRotation = rotZ * rotX * rotY;

	Matrix44 totalRotation = rotY * rotZ * rotX; // <- DENNA VERKAR FUNKA
	//Matrix44 totalRotation = rotY *  rotX * rotZ;

	//Matrix44 totalRotation = rotX * rotZ * rotY;
	//Matrix44 totalRotation = rotX *  rotY * rotZ;

	myView.x = (Corgi::Vector4<float>(0, 0, 1, 0) * totalRotation).x;
	myView.y = (Corgi::Vector4<float>(0, 0, 1, 0) *totalRotation).y;
	myView.z = (Corgi::Vector4<float>(0, 0, 1, 0) *totalRotation).z;

	myUpVector.x = (Corgi::Vector4<float>(0, 1, 0, 0) * totalRotation).x;
	myUpVector.y = (Corgi::Vector4<float>(0, 1, 0, 0) * totalRotation).y;
	myUpVector.z = (Corgi::Vector4<float>(0, 1, 0, 0) * totalRotation).z;


	Vector3f rightVector;
	rightVector.x = (Corgi::Vector4<float>(1, 0, 0, 0) * totalRotation).x;
	rightVector.y = (Corgi::Vector4<float>(1, 0, 0, 0) * totalRotation).y;
	rightVector.z = (Corgi::Vector4<float>(1, 0, 0, 0) * totalRotation).z;

	//Vector3f dView = myTargetView - myPrevView;
	//Vector3f dUp = myTargetUp - myPrevUp;
	//
	//float dViewStep = dView.Length() / rotationDeltaSpeed;
	//float dUpStep = dUp.Length() / rotationDeltaSpeed;
	//
	//myView = myPrevView + (dView * dViewStep);
	//myUpVector = myPrevUp + (dUp * dUpStep);
	//
	//myView.Normalize();
	//myUpVector.Normalize();

	Corgi::Vector3<float> zaxis = myView;
	Corgi::Vector3<float> xaxis = rightVector;
	Corgi::Vector3<float> yaxis = myUpVector;


	const float dt = Timer::GetInstance().GetDeltaTime();
	Quat qZ(zaxis.x, zaxis.y, zaxis.z, myRotation.z);
	Quat qX(xaxis.x, xaxis.y, xaxis.z, myRotation.x);
	Quat qY(yaxis.x, yaxis.y, yaxis.z, myRotation.y);

	Quat orientation = qZ * qX * qY;
	orientation.Normalize();

	Matrix44 rotate = Matrix44::TransposeOpenGL(Matrix44::CreateRotationFromQuat(orientation));

	Matrix44 translate;
	translate.myMatrix[12] = myPosition.x;
	translate.myMatrix[13] = myPosition.y;
	translate.myMatrix[14] = myPosition.z;


	myOrientation = rotate * translate;

	myPosition.x = myOrientation.myMatrix[12];
	myPosition.y = myOrientation.myMatrix[13];
	myPosition.z = myOrientation.myMatrix[14];

}

Matrix44 CPositionComponent::GetOrientationMatrix() const
{
	return myOrientation;
}

Vector3f CPositionComponent::GetPosition() const
{
	return myPosition;
}

Vector3f CPositionComponent::GetRotation() const
{
	return myRotation;
}
