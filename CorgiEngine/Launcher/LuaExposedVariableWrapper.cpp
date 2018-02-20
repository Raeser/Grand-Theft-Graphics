#include "stdafx.h"


LuaExposedVariableWrapper::LuaExposedVariableWrapper()
{
	ResetPointers();
}


LuaExposedVariableWrapper::~LuaExposedVariableWrapper()
{
}

void LuaExposedVariableWrapper::RecieveMessage(const SMessage & message)
{
	std::string tiki_id;
	switch (message.messageType)
	{
	case eMessageType::PlayerPosition:
		myPlayerPosition.x = message.playerPosition.x;
		myPlayerPosition.y = message.playerPosition.y;
		myPlayerPosition.z = message.playerPosition.z;
		break;
	case eMessageType::BuildTikiAtPosition:
		++myTotalTikiCounter;
		break;
	case eMessageType::KillTiki:
		tiki_id = (message.killTiki.ID);
		if (tiki_id.find("Tiki") != -1)
		{
			--myTotalTikiCounter;
		}
		break;
	case eMessageType::Reset:
		myTotalTikiCounter = 0;
	default:
		break;
	}
}

int LuaExposedVariableWrapper::GetTikiCount() const
{
	return myTotalTikiCounter;
}

int LuaExposedVariableWrapper::GetTikiLimit() const
{
	return myTikiMaxLimit;
}

void LuaExposedVariableWrapper::SetTikiLimit(int aLimit)
{
	myTikiMaxLimit = aLimit;
}

const Vector3f & LuaExposedVariableWrapper::GetPlayerPosition() const
{
	return myPlayerPosition;
}

void LuaExposedVariableWrapper::InitPointers(Corgi::CMemoryPool<CEntity>* aEntityPool, Corgi::GrowingArray<long long>* aUpdateArray)
{
	myEntityPoolPtr = aEntityPool;
	myUpdateableIdsPtr = aUpdateArray;
}

void LuaExposedVariableWrapper::ResetPointers()
{
	myEntityPoolPtr = nullptr;
	myUpdateableIdsPtr = nullptr;
}

Corgi::CMemoryPool<CEntity>* LuaExposedVariableWrapper::GetEntityPoolPtr()
{
	return myEntityPoolPtr;
}

Corgi::GrowingArray<long long>* LuaExposedVariableWrapper::GetUpdateableIDsPtr()
{
	return myUpdateableIdsPtr;
}

