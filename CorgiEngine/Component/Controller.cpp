#include "stdafx.h"
#include "Controller.h"
#include "ActorComponent.h"

CController::CController()
{
}


CController::~CController()
{
}

void CController::Init(EControllerType aControllerType)
{
	myType = aControllerType;
	myShouldRelease = false;
}

void CController::AssignParent(CEntity* aParent)
{
	myParent = aParent;
}

const Corgi::Vector3<float>& CController::GetPosition()
{
	return myParent->GetTransform().GetPosition();
}

void CController::SetLookAtPosition(const Corgi::Vector3<float>& aPosition)
{
	myLookAtPosition = aPosition;
}

bool CController::ShouldRelease()
{
	return myShouldRelease;
}

void CController::UpdateRotation()
{
	CActorComponent* actorComp = dynamic_cast<CActorComponent*>(myParent->GetComponent(eComponentTag::Actor));

	Corgi::Vector3<float> toLookAt = myLookAtPosition - myParent->GetTransform().GetPosition();

	if (toLookAt.Length() > 0.f)
	{
		if (actorComp)
		{
			actorComp->ApplyRotation(toLookAt.GetNormalized());
		}
	}
}

void CController::Kill()
{
	dynamic_cast<CActorComponent*>(myParent->GetComponent(eComponentTag::Actor))->Kill();
}

void CController::Respawn()
{
	dynamic_cast<CActorComponent*>(myParent->GetComponent(eComponentTag::Actor))->Respawn();
}

EControllerType CController::GetControllerType() const
{
	return myType;
}
