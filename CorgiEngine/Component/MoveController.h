#pragma once
#include "Controller.h"

enum class EMoveState
{
	Idle,
	Moving,
	Count
};

class CMoveController :
	public CController
{
public:
	CMoveController();
	virtual ~CMoveController();

	void Init(EControllerType aControllType, float aSpeed);
	SDirectionData Update(const Corgi::Vector3<float>& aActorPos) override;
	void MoveToPosition(const Corgi::Vector3<float>& aPosition, float aSpeed);
	void SetState(EMoveState aState);
	EMoveState GetState() const;
private:
	Vector3f myGoToPosition;
	EMoveState myMoveState;
	float mySpeed;



	// Inherited via CController
	virtual void Release() override;
};

