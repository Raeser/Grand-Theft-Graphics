#pragma once
#include "..\Utilities\Vector3.h"

class CEntity;

struct SDirectionData 
{
	Corgi::Vector3<float> direction;
	float speedModifier;
};

enum class EControllerType
{
	AI,
	Input,
	Move,
	Count
};


class CController
{
public:
	CController();
	~CController();

	virtual void Init(EControllerType aControllerType);
	virtual SDirectionData Update(const Corgi::Vector3<float>& aActorPos) = 0;
	const Corgi::Vector3<float>& GetPosition();

	void AssignParent(CEntity* aParent);
	void SetLookAtPosition(const Corgi::Vector3<float>& aPosition);
	bool ShouldRelease();
	virtual void Release() = 0;

	void UpdateRotation();

	virtual void Kill();
	void Respawn();

	EControllerType GetControllerType() const;

protected:
	EControllerType myType;
	Corgi::Vector3<float> myLookAtPosition;
	CEntity* myParent;
	bool myShouldRelease;
};

