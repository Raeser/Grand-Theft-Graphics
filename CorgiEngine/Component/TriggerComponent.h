#pragma once
#include "Component.h"
#include "TriggerEvents.h"

class CScriptComponent;
class CTriggerComponent :
	public CComponent
{
public:
	CTriggerComponent();
	~CTriggerComponent();

	void Init(CEntity* aParent, const Vector3f& aPosition, const float& aRadius, CScriptComponent* aTriggerScriptComponent);

	void Update()override;
	void Release() override;
	void FetchData()override;
	void SendData()override;
	void PostEvent();

private:
	TriggerEvent::EEventType myEventType;
	TriggerEvent::EColliderTypeTrigger myColliderTriggerType;
	bool myIsTriggered;
	Vector3f myPosition;
	int myIndex = 0;
};

