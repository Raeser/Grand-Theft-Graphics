#pragma once
#include "Component.h"

class CHealthComponent :
	public CComponent, public CSubscriber
{
public:
	CHealthComponent();
	~CHealthComponent();

	void Init(CEntity* aParent, int aMaxHealth, float aDamageDelayTime, const bool& isPlayerBool);
	void Update() override;
	void FetchData() override;
	void SendData() override;
	void Release() override;

	void SetCanTakeDamage(bool aCanTakeDamageBool);

	void SetDamageToTake(int aDamageAmount);

	void TakeDamage(int aDamage);
	bool IsAlive();
private:

	void RecieveMessage(const SMessage& aMessage) override;

	void AddHealth();
	void UpdateDamageBuffer();
	void UpdateDamageTimer();

	void Die();
	void Reset();

	int myCurrentHealth;
	int myMaxHealth;	
	int myDamageToTake;
	int myHealthToAdd;
	bool myCanTakeDamage;
	bool myIsAlive;

	float myTimer;
	float myDamageDelayTime;

	bool myIsPlayer;
	//Shake
	bool myCameraShakeDamage;
};

