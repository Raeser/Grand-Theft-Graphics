#pragma once
#include "..\Utilities\Matrix44.h"

class CCamera;
class CDXFramework;
//struct SLinedata;

class CCameraInstance
{
	friend class CForwardRenderer;
	friend class CDeferredRenderer;

	friend class CParticleRenderer;
	friend CCamera;
	friend class CCameraLoader;

public:
	struct SCameraData 
	{
		float myFoV = 90.f;
		Vector3f myPosition = Vector3f(0.0f, 0.0f, 0.0f);
	};


	CCameraInstance();
	~CCameraInstance();

	bool Init(SCameraData& aCameraData, CDXFramework* aFramework);

	void Update();

	Vector3f SetViewToMouse(const float& aSensitivity);

	void UpdateBuffers(CDXFramework* aDXFramework);
	void SetOrientation(const Matrix44& aOrientation);
	void SetPosition(const Vector3f& aPosition);

	void SetFarPlaneDistance(float aDistance) { myFarPlaneDistance = aDistance; };

	Matrix44 GetOrientation();
	Vector3f GetPosition() const;
	void Transform(const Matrix44 & aTransformation);
	Vector3f FromWorldSpaceToProjectionSpace(const Vector3f & aPos) const;
	Vector3f GetWorldPositionOfMouseOnNearlplane(const Vector3f& aMousePositionOnScreen, const Corgi::Vector2<int>&  currentResolution) const;
	bool isPointInViewSpace(const Vector3f& aPosition);
	bool isPointInViewSpace(const Vector3f & aPosition, float aCap);

	bool isPointInViewSpace(const Vector4f& aPosition) { return isPointInViewSpace(Vector3f(aPosition.x, aPosition.y, aPosition.z)); };
	bool isPointInViewSpace(float aX, float aY, float aZ) { return isPointInViewSpace(Vector3f(aX, aY, aZ)); };
	bool isPointInViewSpace(float aX,float aY,float aZ, float aW) { return isPointInViewSpace(Vector4f(aX, aY, aZ, aW)); };
	CCamera* GetCamera() { return myCamera; }
	const CCamera* GetCamera() const { return myCamera; }
private:
	friend class CShowRoomState;

	CCamera* myCamera;
	float myDeadzone;
	float mySpeed;
	//CTransformComponent myPosition;
	Matrix44 myOrientation;

	float myNearlPlaneDistance;
	float myFarPlaneDistance;
};

