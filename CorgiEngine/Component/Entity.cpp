#include "stdafx.h"
#include "Entity.h"
#include "..\Engine\PostMaster.h"
#include "HealthComponent.h"
#include "TransformComponent.h"
#include "..\Component\ScriptComponent.h"
#include "..\Component\ActorComponent.h"
#include "..\Component\MoveController.h"
#include "..\Component\RenderComponent.h"

long long CEntity::myStaticIdCounter = 0;

CEntity::CEntity()
{
	myIsActive = false;
	myShouldRelease = false;
	myComponents.Init(64);
	myChildren.Init(64);
	myScriptComponent = nullptr;
	myIsStatic = false;
	NewID();
}


CEntity::~CEntity()
{
}

bool CEntity::Update()
{
	for (CEntity* child : myChildren)
	{
		child->Update();
	}

	if (myShouldRelease && myIsActive)
	{
		InternalRelease();
		return false;
	}
	else if (myIsActive && myIsStatic == false)
	{
		myTransform.Update();

		for (CComponent* c : myComponents)
		{
			c->SetParent(this);
			c->FetchData();
			c->Update();
			c->SendData();
		}

	}
	return true;
}

void CEntity::Activate()
{
	myShouldRelease = false;
	myIsActive = true;
}

void CEntity::DeActivate()
{
	myIsActive = false;
}

void CEntity::SetIsStatic(const bool& aBool)
{
	myIsStatic = aBool;
}

void CEntity::Release()
{
	myShouldRelease = true;
	myIsActive = false;
	for (CEntity* child : myChildren)
	{
		child->Release();
	}
	for (CComponent* comp : myComponents)
	{
		comp->Release();
	}
}

void CEntity::Reset()
{
	//myData.Reset();
	myIsActive = false;
	myShouldRelease = false;
	myScriptComponent = nullptr;
	myIsStatic = false;
	myComponents.RemoveAll();
	myChildren.RemoveAll();
	myTransform.Reset();
	NewID();
}

bool CEntity::GetActiveStatus()
{
	return myIsActive;
}

bool CEntity::ShouldRelease()
{
	return myShouldRelease;
}

//Corgi::OpaqueDictionary<NUMBEROFBYTESOFDATA>& CEntity::GetData()
//{
//	return myData;
//}


void CEntity::RecieveMessage(const SMessage& message)
{
	std::string id;
	switch (message.messageType)
	{
	case eMessageType::RotateObject:
		id = (message.rotateObject.name);
		if (id == myStringId)
		{
			Vector3f rotation = myTransform.GetRotation();
			rotation.x += message.rotateObject.x;
			rotation.y += message.rotateObject.y;
			rotation.z += message.rotateObject.z;

			myTransform.RotateTowardsTarget(rotation, message.rotateObject.speed);
		}

		break;
	case eMessageType::MoveObjectToPosition:
		SetMoveCommandToPositionIfMovable(message);
		break;
	case eMessageType::MoveObjectToPositionRelative:
		SetMoveCommandToPositionIfMovable_Relative(message);
		break;
	case eMessageType::StartAnimationOnObject:
		SetAnimationOnModel(message);
	case eMessageType::Count:
		break;
	}
}

CComponent * CEntity::GetComponent(eComponentTag aComponentTag) const
{
	for (CComponent* c : myComponents)
	{
		if (c->GetComponentTag() == aComponentTag)
		{
			return c;
		}
	}

	DL_WARNING(eDebugLogType::Engine, "%s%i%s", "TRIED TO GET A COMPONENT WITH ID:  ", cast_int(aComponentTag), "BUT COULDN'T FIND IT!");
	return nullptr;
}

CTransformComponent & CEntity::GetTransform()
{
	return myTransform;
}

const CTransformComponent & CEntity::GetTransform() const
{
	return myTransform;
}

void CEntity::AddComponent(CComponent * aComponent)
{
	myComponents.Add(aComponent);
	if (aComponent->GetComponentTag() == eComponentTag::Script)
	{
		myScriptComponent = static_cast<CScriptComponent*>(aComponent);
	}
}

void CEntity::AddChild(CEntity * aEntity)
{
	myChildren.Add(aEntity);
}

//void CEntity::OverrideComponent(eComponentTag aComponentTag, CComponent * aComponent)
//{
//	for (CComponent* c : myComponents)
//	{
//		if (c->GetComponentTag() == aComponentTag)
//		{
//			c = aComponent;
//		}
//	}
//}

void CEntity::UpdateComponentParents()
{
	for (CComponent* c : myComponents)
	{
		c->SetParent(this);
	}
}


void CEntity::InternalRelease()
{
	for (CComponent* c : myComponents)
	{
		c->Release();
	}
	myComponents.RemoveAll();
	myTransform.Reset();
	//myData.Reset();
	myShouldRelease = true;
	myIsActive = false;
}

const EntityID& CEntity::GetID() const
{
	return myStringId;
}

void CEntity::SetID(const EntityID& aID)
{
	myStringId = aID;
}

EEntityType CEntity::GetEntityType() const
{
	return myEntityType;
}

void CEntity::SetAnimationOnModel(const SMessage & message)
{
	std::string tempnameid(message.moveObjectToPosition.nameID);
	if (tempnameid == myStringId)
	{
		CRenderComponent* renderComponent = static_cast<CRenderComponent*>(GetComponent(eComponentTag::Render));
		if (renderComponent)
		{
			renderComponent->Animate(message.startAnimationOnObject.animationName, false);
		}
	}
}

void CEntity::SetMoveCommandToPositionIfMovable(const SMessage & message)
{
	std::string tempnameid(message.moveObjectToPosition.nameID);
	if ( tempnameid == myStringId)
	{
		CActorComponent* actorComponent = static_cast<CActorComponent*>(GetComponent(eComponentTag::Actor));
		if (actorComponent)
		{
			Corgi::GrowingArray<CController*> controllerArray = actorComponent->GetController();
			for (CController* controller : controllerArray)
			{
				if (controller->GetControllerType() == EControllerType::Move)
				{
					CMoveController* moveController = dynamic_cast<CMoveController*>(controller);
					Vector3f movepos;
					movepos.x = message.moveObjectToPosition.x;
					movepos.y = message.moveObjectToPosition.y;
					movepos.z = message.moveObjectToPosition.z;
					moveController->MoveToPosition(movepos, message.moveObjectToPosition.speed);
				}
			}
		}
	}
}

void CEntity::SetMoveCommandToPositionIfMovable_Relative(const SMessage & message)
{
	std::string tempnameid(message.moveObjectToPosition.nameID);
	if (tempnameid == myStringId)
	{
		CActorComponent* actorComponent = static_cast<CActorComponent*>(GetComponent(eComponentTag::Actor));
		if (actorComponent)
		{
			Corgi::GrowingArray<CController*> controllerArray = actorComponent->GetController();
			for (CController* controller : controllerArray)
			{
				if (controller->GetControllerType() == EControllerType::Move)
				{
					CMoveController* moveController = dynamic_cast<CMoveController*>(controller);
					Vector3f movepos;
					movepos.x = myTransform.GetPosition().x + message.moveObjectToPosition.x;
					movepos.y = myTransform.GetPosition().y + message.moveObjectToPosition.y;
					movepos.z = myTransform.GetPosition().z + message.moveObjectToPosition.z;
					moveController->MoveToPosition(movepos, message.moveObjectToPosition.speed);
				}
			}
		}
	}
}

void CEntity::NewID()
{
	++myStaticIdCounter;


	myStringId = "Entity" + std::to_string(myStaticIdCounter);
}

bool CEntity::Init(EEntityType aEntityType)
{
	myEntityType = aEntityType;
	Reset();
	return true;
}