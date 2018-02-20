#pragma once
#include "Component.h"

class CController;
class CPlaneCollider;

enum class EActorState 
{
	Idle,
	Walking
};

class CActorComponent :
	public CComponent
{
public:
	CActorComponent();
	~CActorComponent();

	void Init(CController* aController, float aMoveSpeed = 0.f,const Vector3f& aPosition = Vector3f());

	void Update() override;
	void FetchData() override;
	void SendData() override;

	void Release() override;

	virtual void AssignController(CController* aController);

	void Kill();
	void Respawn();

	EActorState GetState();

	void SetRespawnPosition(const Corgi::Vector3<float> aRespawnPosition);
	void ApplyRotation(const Corgi::Vector3<float>& aDirection);

	//CController* GetController();
	//const CController* GetController() const;
	Corgi::GrowingArray<CController*> GetController();
	const Corgi::GrowingArray<CController*>& GetController() const;
private:

	bool myIsAI;

	void SetState(const EActorState& aState);

	void MoveAtDirection(const Corgi::Vector3<float>& aNormalizedDirection, float aSpeed);

	//CController* myController;
	Corgi::GrowingArray<CController*> myControllers;
	Corgi::Vector3<float> myRespawnPosition;

	EActorState myState;
};

