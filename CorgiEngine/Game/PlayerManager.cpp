#include "stdafx.h"
#include "PlayerManager.h"


CPlayerManager & CPlayerManager::GetInstance() 
{
	static CPlayerManager triggerManager;
	return triggerManager;
}

CPlayerManager::CPlayerManager() 
	: myIsInited(false)
{
}


CPlayerManager::~CPlayerManager()
{
}

void CPlayerManager::Init(const SPlayerData& aPlayerData, CEntity* aPlayer)
{
	myPlayer = aPlayer;

	if (myIsInited == false)
	{
		myDefaultPlayerData = aPlayerData;
		myCurrentCheckPointIndex = 0;
		myIsInited = true;
	}
}

void CPlayerManager::Respawn()
{
	 //Todo
}

void CPlayerManager::RespawnAtLocation(Vector3f aPosition)
{
	//Todo
	aPosition;
}

void CPlayerManager::RespawnAtLocation(Matrix44 aOrientation)
{
	//Todo
	aOrientation;
}

const SPlayerData & CPlayerManager::GetResetPlayerData() const
{
	return myDefaultPlayerData;
}

SPlayerData & CPlayerManager::SetResetPlayerData()
{
	return myDefaultPlayerData;
}

void CPlayerManager::Update()
{
}

void CPlayerManager::HandleTriggerEvent(const int& /*aTriggerEventId*/, const int& /*aColliderTypeTriggerID*/, const int& /*aIndex*/, const Vector3f& /*aPosition*/)
{

}
