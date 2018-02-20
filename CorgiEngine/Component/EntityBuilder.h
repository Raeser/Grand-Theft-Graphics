#pragma once
#include "Entity.h"
#include "ComponentBuilder.h"
#include "..\Utilities\MemoryPool.h"
#include "..\Engine\Subscriber.h"

class CInGameState;
class CEngine;
class CPlaneCollider;

class CEntityBuilder : public CSubscriber
{
public:
	CEntityBuilder(Corgi::CMemoryPool<CEntity>& aEntityRef, CEngine& aEngine);
	~CEntityBuilder();

	void Init(CComponentBuilder* aComponentBuilder);

	long long BuildPlayer(const Vector3f& aPosition, const Vector3f& aRotation, CPlaneCollider& aPlaneCollider);
	long long BuildTargetPointModel(const Vector3f aPosition, const float& yHeight, const float& moveDownSpeed, const float& aLifeTime);

	long long BuildMovableObject(const Corgi::GrowingArray<std::string>& aPathArray, const Vector3f&  aPos, const Vector3f& aRotation, float aSpeed, const char* aID);
	long long BuildParticleEmitter(const Vector3f & aPosition, const char* aEmitterSystemFileName, const Vector3f& aRotation, float aLifeTime);
	long long BuildObject(const Corgi::GrowingArray<std::string>& aPathArray, const Vector3f & aPos, const Vector3f & aRotation, const bool & isStatic);
	long long BuildPointLight(const Vector3f & aPos, const Vector3f & aColor, const float & aRange);
	void BuildTrigger(const Vector3f & aPosition, const float & aRadius, const char * aLuaFunctionToCall);
	Corgi::CMemoryPool<CEntity>& GetEntities() { return myEntities; }

	void RecieveMessage(const SMessage& message) override;

	CInputController * GetInputController(CEntity * aParentEntity);


	CMoveController * GetMoveController(CEntity * aParentEntity);

	void Reset();

private:
	long long GetFreeEntityID(EEntityType aEntityType);
	CComponentBuilder* myComponentBuilder;
	Corgi::CMemoryPool<CEntity>& myEntities;

	Corgi::CMemoryPool<CMoveController> myMoveControllers;
	Corgi::CMemoryPool<CInputController> myInputControllers;

	CEngine& myEngine;

	Corgi::GrowingArray<CEntity*> mySubscribedEntities;
	long long myPlayerEmitterID;
};

