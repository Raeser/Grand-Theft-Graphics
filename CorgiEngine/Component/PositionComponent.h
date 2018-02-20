#pragma once
#include "Component.h"
#include "..\Utilities\Vector3.h"
#include "..\Utilities\Matrix44.h"

class CPositionComponent :
	public CComponent
{
public:
	CPositionComponent();
	~CPositionComponent();
	void SetOrientation(const Matrix44& aOrientation) { myOrientation = aOrientation; }


	void Move(Vector3f aMovement);
	void Rotate(const Vector3f& aRotation);

	//RUN THIS ONLY ONCE PER FRAME
	//IT MERGES POS AND ROTATION 
	//INTO A GIMBAL LOCK SECURE MATRIX44
	void Update();


	Matrix44 GetOrientationMatrix() const;
	Vector3f GetPosition() const;
	Vector3f GetRotation() const;

private:

	Vector3f myPosition;
	Vector3f myRotation;


	Vector3f myPrevView;
	Vector3f myPrevUp;

	Vector3f myTargetView;
	Vector3f myTargetUp;

	Vector3f myView;
	Vector3f myUpVector;

	float rotationDeltaSpeed;


	Matrix44 myOrientation;

};

