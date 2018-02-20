#pragma once
#include "TriggerEvents.h"
//#include "..\Engine\Subscriber.h"

class CTriggerManager// : public CSubscriber
{
public:
	~CTriggerManager();

	static CTriggerManager& GetInstance();

	void Init();
	int GetEventIDFromString(const std::string & aEventStringFromUnity);
	void HandleTriggerEvent(const TriggerEvent::EEventType& aEventToHandle, const TriggerEvent::EColliderTypeTrigger& aColliderTypeTrigger, const int& aIndex, const Vector3f& aPosition);
private:
	CTriggerManager();
	bool mySpacestationEventActivated;
	bool myBriefingOneEventActivated;
};

