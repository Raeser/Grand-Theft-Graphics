#pragma once
#include "DataStructs.h"

class CEntity;
class CPlayerManager
{
	friend class CDataManager;
	friend class CTriggerManager;
	friend class PlayerComponent;

public:
	~CPlayerManager();
	static CPlayerManager& GetInstance();
	void Init(const SPlayerData& aPlayerData, CEntity* aPlayer);
	void Respawn();
	void RespawnAtLocation(Vector3f aPosition);
	void RespawnAtLocation(Matrix44 aOrientation);
	bool GetHasFinishedGame();

	void SetHasFinishedGame(const bool aBool);



	const SPlayerData& GetResetPlayerData() const;
	SPlayerData& SetResetPlayerData();
private:
	void Update();
	void DamagePlayer(const int & aDamageAmount);
	void HandleTriggerEvent(const int& aTriggerEventId, const int& aColliderTypeTriggerID, const int& aIndex, const Vector3f& aPosition);
	CPlayerManager();

	SPlayerData myDefaultPlayerData;

	bool myIsInited;
	int myCurrentCheckPointIndex;
	bool tempPlayerIsTakingDamage;

	CEntity* myPlayer;
};

