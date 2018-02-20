#pragma once
#include "Subscriber.h"

struct SLineWallCollider
{
	Vector3f point;
	Vector3f normal;
};

class CTikiWallCollider : CSubscriber
{
public:
	CTikiWallCollider();
	~CTikiWallCollider();

	void Init(const Vector3f & aPosition, const Vector3f & aRotation, const Vector3f & aSize, const char * aID);
	bool IsActive() { return myIsActive; };
	void SetIsActive() { myIsActive = true; };
	void DrawWireFrame();
	bool CheckCollision(const Vector3f & aPosition);
	void RecieveMessage(const SMessage& message) override;

private:
	friend class CPathfinding;
	Vector3f myPosition;
	Vector3f myRotation;
	Vector3f mySize;
	bool myIsActive;
	std::string myID;
	std::string myIDToCheck;
	Corgi::GrowingArray<SLineWallCollider> myEdges;
#ifndef _RETAIL
	Corgi::GrowingArray<Vector4f> myWireframeLinesBottom;
	Corgi::GrowingArray<Vector4f> myWireframeLinesTop;
#endif
};

