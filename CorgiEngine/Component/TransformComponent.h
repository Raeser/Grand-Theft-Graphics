#pragma once
#include "Component.h"
#include "..\Utilities\Vector3.h"
#include "..\Utilities\Matrix44.h"

class CTransformComponent :
	public CComponent
{
public:
	CTransformComponent();
	~CTransformComponent();
	void Init(CEntity* aParent, const Corgi::Matrix44<float>& aOriantation);
	void Init(CEntity* aParent, const Corgi::Vector3<float>& aPosition, const Corgi::Vector3<float>& aRotation);

	void Release() override;
	void Reset();

	void Move(const Vector3f& v);
	void Move(const Vector4f& v);

	void SetPosition(const Vector3f& v);

	void Rotate(const Vector3f& v);
	void Rotate(const Vector4f& v);

	void SetShouldFace();

	void FetchData() override;
	void SendData() override;

	void Update() override;
	void UpdateEulerAngles();

	const Matrix44& GetOrientationMatrix() const;
	const Vector3f& GetPosition() const;
	const Vector3f& GetRotation() const;

	void SetOrientation(const Matrix44& aOrientation) { myOrientation = aOrientation; }

	void RotateTowardsTarget(const Vector3f& aTargetRotation, float aSpeed);
	void RotateTowardsTarget(const Vector4f& aTargetRotation, float aSpeed);

private:
	void InternalInit(CEntity * aParent);
	void BuildOrientationFromEulerRotationDelta(const Vector3f& aRotation);
	void BuildOrientationFromRotationAndPosition();

	float SetTargetRotationToClosestPoint(float aTargetRotation, float aCurrentRotation);
	void ClampRotationAndRotationTarget(float& aRotation, float& aTargetRotation);
	void TargetRotation();

	Matrix44 myOrientation;
	Matrix44 myRotX;
	Matrix44 myRotY;
	Matrix44 myRotZ;

	Vector3f myPosition;
	Vector3f myLookAtPosition;

	Vector3f myRotation;
	Vector3f myRotationLastFrame;

	Vector3f myPrevView;
	Vector3f myPrevUp;

	Vector3f myTargetView;
	Vector3f myTargetUp;

	Vector3f myView;
	Vector3f myUpVector;

	Vector3f myRotationTarget;
	float myRotationSpeed;
	bool myShouldRotateTowardsTarget;



	//TODO: LUKAS? AI legacy?
	Vector3f myCameraShakeOffset;
	bool myShouldFaceObject;
	bool myIsAI;

};

