#pragma once
#include "../Utilities/MemoryPool.h"

enum class eComponentTag
{
	Default,
	Shield,
	Boost,
	AI, 
	ObjectBehaviour,
	Camera, 
	Health, 
	MouseControl,
	PointLight,
	SpotLight,
	Weapon,
	Transform,
	Collision,
	PickUp,
	Trigger,
	Move,
	Player,
	Render,	
	Sprite,
	Script,
	ParticleEmitter,
	Follower,

	//not sorted
	Input,
	Sound,
	Timer,
	Actor,
	Count
};


class CEntity;
class CComponentManager;

class CComponent
{
public:
	friend CComponentManager;
	CComponent();
	virtual ~CComponent();

	virtual void Init();
	virtual void Update() = 0;
	virtual void FetchData() = 0;
	virtual void SendData() = 0;
	bool IsActive();

	virtual void Release() = 0;

	void SetParent(CEntity* aParent);
	void DeInit();
	bool GetIsActive();
	bool GetIsParentActive();
	bool ShouldRelease();
	virtual void SetIsActive(bool isActive);

	eComponentTag GetComponentTag() const;

	virtual const float& GetScreenMaxData() { return myMaxValue; }
	virtual const float& GetScreenCurrentData() { return myCurrentValue; }

protected:

	eComponentTag myComponentTag;
	CEntity* myParent;
	float myMaxValue;		
	float myCurrentValue;	
	bool myIsActive;
	bool myIsInited;
	bool myShouldRelease;
};

