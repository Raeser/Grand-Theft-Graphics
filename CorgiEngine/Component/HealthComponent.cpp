#include "stdafx.h"
#include "HealthComponent.h"
#include "..\Engine\PostMaster.h"


CHealthComponent::CHealthComponent()
{
}

CHealthComponent::~CHealthComponent()
{

}

void CHealthComponent::Init(CEntity* aParent, int aMaxHealth, float aDamageDelayTime,const bool& isPlayerBool)
{
	CComponent::Init();
	SetParent(aParent);
	myIsInited = true;
	myIsPlayer = isPlayerBool;

	if (myIsPlayer)
	{
		CPostMaster::GetInstance().Subscribe(this, eMessageType::PickupInfo);
	}

	myMaxHealth = aMaxHealth;
	myCurrentHealth = myMaxHealth;
	myCanTakeDamage = true;
	myTimer = 0;
	myDamageDelayTime = aDamageDelayTime;
	myHealthToAdd = 0;
	myDamageToTake = 0;
	//myParent->GetData().Init(eOpaqueKey::DamageToTake, myDamageToTake);
	//myParent->GetData().ForceInit(eOpaqueKey::Health, myCurrentHealth);
	//myParent->GetData().ForceInit(eOpaqueKey::IsAlive, true);

	myCameraShakeDamage = false;

	myIsAlive = true;
	myMaxValue = static_cast<float>(aMaxHealth);
	myCurrentValue = static_cast<float>(myCurrentHealth);



	myComponentTag = eComponentTag::Health;
}

void CHealthComponent::SendData()
{

	if (myIsAlive)
	{
		//myParent->GetData().SetValue(eOpaqueKey::Health, myCurrentHealth);
		//myParent->GetData().SetValue(eOpaqueKey::IsAlive, myIsAlive);

		SMessage msg;
		msg.messageType = eMessageType::PlayerHealthInfo;
		msg.healthInfo.currentHealth = myCurrentHealth;
		msg.healthInfo.maxHealth = myMaxHealth;
		CPostMaster::GetInstance().PostMessageA(msg);

		myDamageToTake = 0;
	}
	else
	{
		myParent->Release();
	}
}

void CHealthComponent::Release()
{
	CComponent::Release();
	if (myIsPlayer)
	{
		CPostMaster::GetInstance().Unsubscribe(this, eMessageType::PickupInfo);
	}
}

void CHealthComponent::SetCanTakeDamage(bool aCanTakeDamageBool)
{
	myCanTakeDamage = aCanTakeDamageBool;
}

void CHealthComponent::SetDamageToTake(int aDamageAmount)
{
	myDamageToTake = aDamageAmount;
}

void CHealthComponent::TakeDamage(int aDamage)
{
	if (aDamage > 0 && myCanTakeDamage)
	{
		SetCanTakeDamage(false);
		myCurrentHealth -= aDamage;

		if (!myCameraShakeDamage)
		{
			myCameraShakeDamage = true;
		}

		if (myCurrentHealth <= 0)
		{
			myCurrentHealth = 0;
			Die();
		}
	}
	myCurrentValue = static_cast<float>(myCurrentHealth);
}

void CHealthComponent::FetchData()
{
//	myDamageToTake += *myParent->GetData().GetValue<int>(eOpaqueKey::DamageToTake);
}

void CHealthComponent::Update()
{
	assert(myIsInited && "I SHOULD USE INIT");
	TakeDamage(myDamageToTake);
	UpdateDamageTimer();
	AddHealth();
}


void CHealthComponent::RecieveMessage(const SMessage & aMessage)
{
	if (myIsPlayer)
	{
		switch (aMessage.messageType)
		{
		case eMessageType::PickupInfo:
			myHealthToAdd = aMessage.pickupInfo.healthToGive;
			break;
		default:
			break;
		}
	}
}

void CHealthComponent::AddHealth()
{
	if (myCurrentHealth < myMaxHealth && myHealthToAdd > 0)
	{
		myCurrentHealth += (int)myHealthToAdd;


		if (myCurrentHealth > myMaxHealth)
		{
			myCurrentHealth = myMaxHealth;
		}

		myHealthToAdd = 0;
	}
}

void CHealthComponent::UpdateDamageBuffer()
{
	if (myDamageToTake > 0 && myCanTakeDamage)
	{
		SetCanTakeDamage(false);
		myCurrentHealth -= myDamageToTake;

		if (!myCameraShakeDamage)
		{
			myCameraShakeDamage = true;
		}

		if (myCurrentHealth <= 0)
		{
			myCurrentHealth = 0;
			Die();
		}
	}
	
}

void CHealthComponent::UpdateDamageTimer()
{
	if (myCanTakeDamage == false)
	{
		float dt = Timer::GetInstance().GetDeltaTime();
		myTimer += dt;
		if (myTimer > myDamageDelayTime)
		{
			SetCanTakeDamage(true);
			myTimer = 0;		
		}
	}

}

bool CHealthComponent::IsAlive()
{
	return myIsAlive;
}

void CHealthComponent::Die()
{
}

void CHealthComponent::Reset()
{
	myCurrentHealth = myMaxHealth;
}
