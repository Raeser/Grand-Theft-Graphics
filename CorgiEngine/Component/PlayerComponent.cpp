#include "stdafx.h"
#include "PlayerComponent.h"
#include "..\Utilities\InputManager.h"
#include "..\Game\GameSettings.h"
#include "..\Game\EngineWrapper.h"
#include "..\Engine\CameraInstance.h"
#include "..\Game\Cursor.h"
#include "SoundComponent.h"
#include "RenderComponent.h"
#include "ActorComponent.h"

CPlayerComponent::CPlayerComponent()
{
}


CPlayerComponent::~CPlayerComponent()
{
}

void CPlayerComponent::Init()
{
	myPlaneCollider.Init({ 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f });
	CPostMaster::GetInstance().Subscribe(this, eMessageType::TikiGotThrown);

	myKickAnimationTimer = 0.f;
	myWalkingTimer = 0.f;
	myWalkSoundFreq = 0.35f; //change with caution plz

	myIsPlayingKickAnimation = false;
	myIsWalking = false;

	myPlayerStateLastFrame = EActorState::Idle;
}

void CPlayerComponent::Update()
{
	ComparePlayerStates();
	UpdateAnimations();

	if (myIsWalking) 
	{
		UpdateWalkingSound();
	}
}

void CPlayerComponent::ComparePlayerStates()
{
	EActorState currentState = GetPlayerState();

	if (myPlayerStateLastFrame != currentState) 
	{
		switch (currentState)
		{
		case EActorState::Idle:
			OnStartedIdling();
			break;
		case EActorState::Walking:
			OnStartedWalking();
			break;
		default:
			break;
		}
	}

	myPlayerStateLastFrame = currentState;
}

void CPlayerComponent::UpdateAnimations() 
{
	if (myIsPlayingKickAnimation)
	{
		myKickAnimationTimer += Timer::GetInstance().GetDeltaTime();

		if (myKickAnimationTimer > 0.4f)
		{
			myIsPlayingKickAnimation = false;
			myKickAnimationTimer = 0.f;

			if (GetPlayerState() == EActorState::Walking)
			{
				OnStartedWalking();
			}
			else
			{
				OnStartedIdling();
			}

		}
	}
	else
	{
		ComparePlayerStates();
	}
}

void CPlayerComponent::UpdateWalkingSound()
{
	myWalkingTimer += Timer::GetInstance().GetDeltaTime();

	if (myWalkingTimer >= myWalkSoundFreq) 
	{
		PlaySound("Play_Player_Walk");
		myWalkingTimer = 0.f;
	}
}

EActorState CPlayerComponent::GetPlayerState() 
{
	CActorComponent* actorComp = dynamic_cast<CActorComponent*>(myParent->GetComponent(eComponentTag::Actor));

	
	return actorComp->GetState();
	
}

void CPlayerComponent::FetchData()
{
}

void CPlayerComponent::SendData()
{
}

const Corgi::Vector3<float>& CPlayerComponent::GetPosition()
{
	return myParent->GetTransform().GetPosition();
}

const CTransformComponent& CPlayerComponent::GetTransform()
{
	return myParent->GetTransform();
}


void CPlayerComponent::RecieveMessage(const SMessage & aMessage)
{
	if (aMessage.messageType == eMessageType::TikiGotThrown)
	{
		OnStartedKicking();
	}
}

void CPlayerComponent::OnStartedWalking() 
{
	SetAnimation("PlayerRun", true);
	ResetAnimation();
	myIsWalking = true;
}

void CPlayerComponent::OnStartedKicking()
{
	PlaySound("Play_Throw");
	PlaySound("Play_Tiki_Kicked");

	SetAnimation("PlayerKick", false);
	ResetAnimation();

	myIsPlayingKickAnimation = true;
	myKickAnimationTimer = 0.f;
}

void CPlayerComponent::OnStartedIdling()
{
	SetAnimation("PlayerIdle", true);
	ResetAnimation();
	myIsWalking = false;
	myWalkingTimer = 0.f;
}

void CPlayerComponent::SetAnimation(const char* aAnimation, bool aShouldLoop)
{
	CRenderComponent* renderComp = dynamic_cast<CRenderComponent*>(myParent->GetComponent(eComponentTag::Render));

	if (renderComp)
	{
		renderComp->Animate(aAnimation, aShouldLoop);
	}
}

void CPlayerComponent::ResetAnimation()
{
	CRenderComponent* renderComp = dynamic_cast<CRenderComponent*>(myParent->GetComponent(eComponentTag::Render));

	if (renderComp)
	{
		renderComp->ResetCurrentAnimation();
	}
}

void CPlayerComponent::PlaySound(const char * aEvent)
{
	CSoundComponent* soundComp = dynamic_cast<CSoundComponent*>(myParent->GetComponent(eComponentTag::Sound));

	if (soundComp)
	{
		soundComp->PostEvent(aEvent);
	}
}

void CPlayerComponent::Release() 
{
	CPostMaster::GetInstance().Unsubscribe(this, eMessageType::TikiGotThrown);
	CComponent::Release();
}