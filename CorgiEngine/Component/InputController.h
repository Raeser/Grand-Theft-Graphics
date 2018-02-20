#pragma once
#include "Controller.h"
#include "..\Engine\Subscriber.h"

class CCameraInstance;
class CPlaneCollider;

class CInputController :
	public CController, public CSubscriber
{
public:
	CInputController();
	~CInputController();

	void Init(EControllerType aControllType, CPlaneCollider* aPlaneCollider, const Vector3f& aPosition = Vector3f());

	void UpdateArrival(float aLengthToTarget);

	SDirectionData Update(const Corgi::Vector3<float>& aActorPos) override;

	void DrawTarget();

	void RecieveMessage(const SMessage& aMessage) override;

	void Release() override;

private:
	struct cursorIndicatorData
	{
		float movementSpeed, startYHeight, endYHeight, lifetime;
	};
	void SendMessage(const Corgi::Vector3<float>& aActorPos);

	//DEBUG
	void DrawPath(const Corgi::GrowingArray<Vector3f>& aPath, const Corgi::Vector3<float>& aActorPos);
	bool myDebugDraw;

	Corgi::Vector3<float> myPosToGoTowards;
	Corgi::Vector3<float> myMousePosInWorld;

	CCameraInstance& myCamera;
	CPlaneCollider* myPlaneCollider;

	float mySpeed;
	float myMaxSpeed;
	float mySlowedRadius;
	bool myHasSpawnedAPointModelPointOnClick;

	Corgi::GrowingArray<Vector3f> myPathNodes;
	std::thread myPathfindingThread;
	bool myPathFindingShouldEnd;

	cursorIndicatorData myCursorIndicatorData;
};

