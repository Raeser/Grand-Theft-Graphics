#pragma once
#include "Component.h"
#include "..\Utilities\PlaneCollider.h"
#include "..\Engine\Subscriber.h"

class CEntity;
class CTransformComponent;

enum class EActorState;

class CPlayerComponent :
	public CComponent, public CSubscriber
{
public:
	CPlayerComponent();
	~CPlayerComponent();

	void Init();

	virtual void Update() override;
	virtual void FetchData() override;
	virtual void SendData() override;

	void Release() override;

	const Corgi::Vector3<float>& GetPosition();
	const CTransformComponent& GetTransform();


	void RecieveMessage(const SMessage& aMessage) override;


private:

	void ComparePlayerStates();

	void OnStartedWalking();
	void OnStartedKicking();
	void OnStartedIdling();

	EActorState GetPlayerState();
	void UpdateAnimations();
	void UpdateWalkingSound();
	void SetAnimation(const char* aAnimation, bool aShouldLoop);

	void ResetAnimation();

	void PlaySound(const char* aEvent);

	EActorState myPlayerStateLastFrame;
	CPlaneCollider myPlaneCollider;

	float myKickAnimationTimer;
	float myWalkingTimer;
	float myWalkSoundFreq;

	bool myIsWalking;
	bool myIsPlayingKickAnimation;
};

