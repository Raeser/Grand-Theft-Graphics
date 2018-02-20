#include "stdafx.h"
#include "ActorComponent.h"

#include "Controller.h"
#include "InputController.h"
#include "MoveController.h"

#include "..\Utilities\Timer.h"
#include "..\Game\EngineWrapper.h"
#include "..\Engine\CameraInstance.h"
#include "..\Engine\TextRenderer.h"
#include "..\Utilities\PlaneCollider.h"

#include "SoundComponent.h"
#include "RenderComponent.h"

CActorComponent::CActorComponent()
{
	//myController = nullptr;
	myControllers.Init(4);
}


CActorComponent::~CActorComponent()
{
}

void CActorComponent::Init(CController* aController, float aMoveSpeed,const Vector3f& aPosition)
{
	aMoveSpeed;
	myControllers.RemoveAll();
	myComponentTag = eComponentTag::Actor;
	AssignController(aController);
	myRespawnPosition = aPosition;

	myState = EActorState::Idle;


}

void CActorComponent::Update()
{
	bool canMove = true;




	for (CController* controller : myControllers)
	{
		if (canMove == false && controller->GetControllerType() == EControllerType::Move)
		{
			continue;
		}

		SDirectionData data = controller->Update(myParent->GetTransform().GetPosition());

		Corgi::Vector3<float> direction = data.direction;

		if ((direction.x + direction.y + direction.z) != 0.f)
		{
			SetState(EActorState::Walking);
			MoveAtDirection(direction.GetNormalized(), data.speedModifier);
		}
		else 
		{
			SetState(EActorState::Idle);
		}
	}
}

void CActorComponent::FetchData()
{
}

void CActorComponent::SendData()
{
}

void CActorComponent::Release()
{
	CComponent::Release();
	//myController->Release();
	for (CController* c : myControllers)
	{
		c->Release();
	}
}

void CActorComponent::AssignController(CController* aController)
{
	//myController = aController;
	myControllers.Add(aController);
}

void CActorComponent::Kill()
{
	myParent->Release();
	
	CRenderComponent* renderComp = dynamic_cast<CRenderComponent*>(myParent->GetComponent(eComponentTag::Render));
	if (renderComp) 
	{
		renderComp->SetShouldRender(false);
	}
}

void CActorComponent::Respawn()
{
	myParent->GetTransform().SetPosition(myRespawnPosition);
	myParent->Activate();

	CRenderComponent* renderComp = dynamic_cast<CRenderComponent*>(myParent->GetComponent(eComponentTag::Render));
	if (renderComp)
	{
		renderComp->SetShouldRender(true);
	}
}

EActorState CActorComponent::GetState()
{
	return myState;
}

void CActorComponent::SetRespawnPosition(const Corgi::Vector3<float> aRespawnPosition)
{
	myRespawnPosition = aRespawnPosition;
}

void CActorComponent::ApplyRotation(const Corgi::Vector3<float>& aDirection)
{
	Corgi::Vector3<float> forwardVec;
	Corgi::Vector3<float> rotation;

	forwardVec.z = 1.f;

	if (aDirection.x > 0)
	{
		rotation.y = -acos(aDirection.GetNormalized().Dot(forwardVec));
	}
	else
	{
		rotation.y = acos(aDirection.GetNormalized().Dot(forwardVec));
	}


	myParent->GetTransform().RotateTowardsTarget(rotation, 5.f);
}

Corgi::GrowingArray<CController*> CActorComponent::GetController()
{
	return myControllers;
}

const Corgi::GrowingArray<CController*>& CActorComponent::GetController() const
{
	return myControllers;
}

void CActorComponent::SetState(const EActorState & aState)
{
	myState = aState;
}

void CActorComponent::MoveAtDirection(const Corgi::Vector3<float>& aNormalizedDirection, float aSpeed)
{
	float dt = Timer::GetInstance().GetDeltaTime();
	Corgi::Vector3<float> movement = aNormalizedDirection * (aSpeed) * dt;

	myParent->GetTransform().Move(movement);
}
