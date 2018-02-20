#include "stdafx.h"
#include "TriggerComponent.h"
#include "TriggerManager.h"
#include "ScriptComponent.h"



CTriggerComponent::CTriggerComponent()
{
}


CTriggerComponent::~CTriggerComponent()
{
}

void CTriggerComponent::Init(CEntity* aParent,const Vector3f& aPosition, const float& aRadius, CScriptComponent* aTriggerScriptComponent)
{
	CComponent::Init();
	SetParent(aParent);
	myIsInited = true;
	aTriggerScriptComponent;
	aRadius;

	myIsTriggered = false;
	myPosition = aPosition;


	///TODO: this fetched data from opaque dictionary with Collision events.
	///These should be sent by postmaster!
	//switch (myColliderTriggerType)
	//{
	//case TriggerEvent::EColliderTypeTrigger::OnEnter:
	//	myParent->GetData().Init(eOpaqueKey::OnCollisionEnter, false);
	//	break;
	//case TriggerEvent::EColliderTypeTrigger::OnCollision:
	//	myParent->GetData().Init(eOpaqueKey::OnCollision, false);
	//	break;
	//case TriggerEvent::EColliderTypeTrigger::OnExit:
	//	myParent->GetData().Init(eOpaqueKey::OnCollisionExit, false);
	//	break;
	//}

	myComponentTag = eComponentTag::Trigger;
}

void CTriggerComponent::Update()
{
	if (myIsTriggered)
	{
		PostEvent();
		myIsTriggered = false;
	}
}

void CTriggerComponent::Release()
{
	CComponent::Release();
}

void CTriggerComponent::FetchData()
{
	///TODO: this fetched data from opaque dictionary with Collision events.
	///These should be sent by postmaster!
	//switch (myColliderTriggerType)
	//{
	//case TriggerEvent::EColliderTypeTrigger::OnEnter:
	//	myIsTriggered = *myParent->GetData().GetValue<bool>(eOpaqueKey::OnCollisionEnter);
	//	break;
	//case TriggerEvent::EColliderTypeTrigger::OnCollision:
	//	myIsTriggered = *myParent->GetData().GetValue<bool>(eOpaqueKey::OnCollision);
	//	break;
	//case TriggerEvent::EColliderTypeTrigger::OnExit:
	//	myIsTriggered = *myParent->GetData().GetValue<bool>(eOpaqueKey::OnCollisionExit);
	//	break;
	//}
}

void CTriggerComponent::SendData()
{
}

void CTriggerComponent::PostEvent()
{
	//CTriggerManager::GetInstance().HandleTriggerEvent(myEventType, myColliderTriggerType, myIndex, myPosition);
}
