#include "stdafx.h"
#include "TriggerManager.h"

CTriggerManager & CTriggerManager::GetInstance()
{
	static CTriggerManager triggerManager;
	return triggerManager;
}

CTriggerManager::CTriggerManager()
{
}

void CTriggerManager::Init()
{
	mySpacestationEventActivated = false;
	myBriefingOneEventActivated = false;
}


void CTriggerManager::HandleTriggerEvent(const TriggerEvent::EEventType& aEventToHandle, const TriggerEvent::EColliderTypeTrigger& aColliderTypeTrigger,const int& aIndex, const Vector3f& aPosition)
{
	aPosition;
	aIndex;
	aColliderTypeTrigger;

	switch (aEventToHandle)
	{
	case TriggerEvent::EEventType::PlayerWin:
		break;
	case TriggerEvent::EEventType::PlayerDie :
		break;
	case TriggerEvent::EEventType::Unknown:
		DL_ERROR(eDebugLogType::Resource, "%s", "[TRIGGER MANAGER] Unknown Event sent");
		break;
	}
}


CTriggerManager::~CTriggerManager()
{
}


int CTriggerManager::GetEventIDFromString(const std::string & aEventStringFromUnity)
{
	for (int i = 0; i < static_cast<int>(TriggerEvent::EEventType::count); i++)
	{
		if (aEventStringFromUnity== TriggerEvent::EventStrings[i])
		{
			return i;
		}
	}

	DL_ERROR(eDebugLogType::Resource, "%s%s", "[TRIGGER ID] FAILED -- Couldnt find matching event from string: ", aEventStringFromUnity);
	return static_cast<int>(TriggerEvent::EEventType::Unknown);
}