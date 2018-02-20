#include "stdafx.h"
#include "EntityBuilder.h"
#include "CollisionComponent.h"
#include "..\Engine\Engine.h"
#include "..\Utilities\PlaneCollider.h"


CEntityBuilder::CEntityBuilder(Corgi::CMemoryPool<CEntity>& aEntityRef, CEngine& aEngine) :
	myEngine(aEngine),
	myEntities(aEntityRef)
{
	mySubscribedEntities.Init(128);
	myMoveControllers.Init(64);
	myInputControllers.Init(4);
}

CEntityBuilder::~CEntityBuilder()
{
	//myComponentBuilder
}

void CEntityBuilder::Init(CComponentBuilder* aComponentBuilder)
{
	myComponentBuilder = aComponentBuilder;
}

long long CEntityBuilder::GetFreeEntityID(EEntityType aEntityType)
{
	long long idToReturn = myEntities.GetFreeItem();
	myEntities[idToReturn].Init(aEntityType);
	myEntities[idToReturn].Activate();
	return idToReturn;
}

long long CEntityBuilder::BuildPlayer(const Vector3f& aPosition, const Vector3f& aRotation, CPlaneCollider& aPlaneCollider)
{

	long long entityID = GetFreeEntityID(EEntityType::Player);

	CRenderComponent* RenderComponent = myComponentBuilder->GetRenderComponent(&myEntities[entityID], "../Assets/Models/playerCharacter/playerCharacter.fbx",0,0);
	RenderComponent->Init(aPosition, aRotation, CRenderComponent::Orientation);
	RenderComponent->Animate("PlayerIdle", true);
	myEntities[entityID].AddComponent(RenderComponent);

	myEntities[entityID].GetTransform().Init(&myEntities[entityID], aPosition, aRotation);
	//CTransformComponent* TransformComponent = myComponentBuilder->GetTransformComponent(&myEntities[entityID]);
	//TransformComponent->Init(&myEntities[entityID], aPosition, aRotation);
	//myEntities[entityID].AddComponent(TransformComponent);

	CSoundComponent* soundComp = myComponentBuilder->GetSoundComponent(&myEntities[entityID]);
	soundComp->Init(&myEntities[entityID]);
	myEntities[entityID].AddComponent(soundComp);

	CScriptComponent* script = myComponentBuilder->GetScriptComponents(&myEntities[entityID]);
	script->Init();
	script->LoadFile("../Assets/Scripts/PlayerTest.lua");
	myEntities[entityID].AddComponent(script);

	CInputController* inputController = GetInputController(&myEntities[entityID]);
	inputController->Init(EControllerType::Input, &aPlaneCollider, aPosition);

	CPlayerComponent* playerComponent = myComponentBuilder->GetPlayerComponent(&myEntities[entityID]);
	playerComponent->Init();
	myEntities[entityID].AddComponent(playerComponent);

	CActorComponent* playerActorComponent = myComponentBuilder->GetActorComponent(&myEntities[entityID]);
	playerActorComponent->Init(inputController, 3.0f, aPosition);
	myEntities[entityID].AddComponent(playerActorComponent);

	

	myEntities[entityID].SetID("Player");

	return entityID;
}

long long CEntityBuilder::BuildTargetPointModel(const Vector3f aPosition, const float& endYHeight,const float& moveDownSpeed, const float& aLifeTime)
{
	long long entityID = GetFreeEntityID(EEntityType::WorldObject);
	myEntities[entityID].GetTransform().Init(&myEntities[entityID], aPosition, Vector3f());

	CMoveController* moveController = GetMoveController(&myEntities[entityID]);
	moveController->Init(EControllerType::Move, moveDownSpeed);
	moveController->MoveToPosition(Vector3f(aPosition.x, endYHeight, aPosition.z), moveDownSpeed);

	CActorComponent* actorComp = myComponentBuilder->GetActorComponent(&myEntities[entityID]);
	actorComp->Init(moveController);
	myEntities[entityID].AddComponent(actorComp);

	CRenderComponent* targetPointModel = myComponentBuilder->GetRenderComponent(&myEntities[entityID], "../Assets/Models/clickArrow/clickArrow.fbx", 0, 0);
	targetPointModel->Init(aPosition, Vector3f(), CRenderComponent::Orientation);
	targetPointModel->SetShouldRender(true);
	targetPointModel->Animate("OnClick", false);
	myEntities[entityID].AddComponent(targetPointModel);

	CLifeTimeComponent* lifeTimeComponent = myComponentBuilder->GetLifeTimeComponent(&myEntities[entityID]);
	lifeTimeComponent->Init(aLifeTime);
	myEntities[entityID].AddComponent(lifeTimeComponent);
	return entityID;
}




long long CEntityBuilder::BuildMovableObject(const Corgi::GrowingArray<std::string>& aPathArray, const Vector3f & aPos, const Vector3f & aRotation,float aSpeed, const char* aID)
{
	long long id = BuildObject(aPathArray, aPos, aRotation, false);
	CEntity& me = myEntities[id];

	CMoveController* moveController = GetMoveController(&me);
	moveController->Init(EControllerType::Move, aSpeed);

	CActorComponent* actorcomponent = myComponentBuilder->GetActorComponent(&me);
	actorcomponent->Init(moveController, 1);
	me.SetID(aID);
	me.AddComponent(actorcomponent);
	me.Activate();
	me.SetIsStatic(false);
	
	CPostMaster::GetInstance().Subscribe(&me, eMessageType::RotateObject);
	CPostMaster::GetInstance().Subscribe(&me, eMessageType::MoveObjectToPosition);
	CPostMaster::GetInstance().Subscribe(&me, eMessageType::MoveObjectToPositionRelative);
	CPostMaster::GetInstance().Subscribe(&me, eMessageType::StartAnimationOnObject);

	mySubscribedEntities.Add(&me);

	return id;
}


long long CEntityBuilder::BuildParticleEmitter(const Vector3f & aPosition,const char* aEmitterSystemFileName, const Vector3f& aRotation, float aLifeTime)
{
	long long entityID = GetFreeEntityID(EEntityType::WorldObject);

	CEntity& currentEntity = myEntities[entityID];

	currentEntity.GetTransform().Init(&currentEntity, aPosition, aRotation);
	currentEntity.Activate();

	CParticleEmitterComponent* particleEmitter = myComponentBuilder->GetParticleEmitterComponent(&myEntities[entityID]);
	particleEmitter->Init(myEngine.GetParticleEmittorLoader().CreateInstance(aEmitterSystemFileName), aLifeTime);
	particleEmitter->SetIsActive(true);
	currentEntity.AddComponent(particleEmitter);


	return entityID;
}


long long CEntityBuilder::BuildObject(const Corgi::GrowingArray<std::string>& aPathArray, const Vector3f&  aPos, const Vector3f& aRotation, const bool& isStatic)
{
	long long entityID = GetFreeEntityID(EEntityType::WorldObject);
	myEntities[entityID].GetTransform().Init(&myEntities[entityID], aPos, aRotation);
	myEntities[entityID].SetIsStatic(isStatic);
	myEntities[entityID].Activate();
	for (int i = 0; i < aPathArray.Size(); i++)
	{
		CRenderComponent* RenderComponent = myComponentBuilder->GetRenderComponent(&myEntities[entityID], aPathArray[i].c_str());
		RenderComponent->Init(aPos, aRotation, CRenderComponent::Orientation);
		myEntities[entityID].AddComponent(RenderComponent);
		RenderComponent->SetIsStatic(isStatic);
		RenderComponent->SetShouldRender(true);
	}

	return entityID;
}


long long CEntityBuilder::BuildPointLight(const Vector3f&  aPos, const Vector3f& aColor, const float& aRange)
{
	long long entityID = GetFreeEntityID(EEntityType::LightSource);
	CPointLightComponent* pointLightComponent = myComponentBuilder->GetPointLightComponent(&myEntities[entityID], aColor, aRange);
	pointLightComponent;
	pointLightComponent->Init(aPos);
	myEntities[entityID].AddComponent(pointLightComponent);

	CTransformComponent* TransformComponent = myComponentBuilder->GetTransformComponent(&myEntities[entityID]);
	TransformComponent->Init(&myEntities[entityID], aPos, { 0.f,0.f,0.f });
	myEntities[entityID].AddComponent(TransformComponent);

	return entityID;
}

void CEntityBuilder::BuildTrigger(const Vector3f& aPosition, const float& aRadius, const char* aLuaFunctionToCall)
{
	aPosition;
	aRadius;
	aLuaFunctionToCall;
}

void CEntityBuilder::RecieveMessage(const SMessage & /*message*/)
{
}



CInputController * CEntityBuilder::GetInputController(CEntity* aParentEntity)
{
	for (int i = 0; i < myInputControllers.Size(); ++i)
	{
		if (myInputControllers[i].ShouldRelease())
		{
			myInputControllers.ReleaseObject(i);
		}
	}
	CInputController* inputController = &myInputControllers[myInputControllers.GetFreeItem()];
	inputController->AssignParent(aParentEntity);
	return inputController;
}


CMoveController * CEntityBuilder::GetMoveController(CEntity * aParentEntity)
{
	for (int i = 0; i < myMoveControllers.Size(); ++i)
	{
		if (myMoveControllers[i].ShouldRelease())
		{
			myMoveControllers.ReleaseObject(i);
		}
	}
	CMoveController* MoveController = &myMoveControllers[myMoveControllers.GetFreeItem()];
	MoveController->AssignParent(aParentEntity);
	return MoveController;
}


void CEntityBuilder::Reset()
{
	for (CEntity* entity : mySubscribedEntities)
	{
		CPostMaster::GetInstance().Unsubscribe(entity, eMessageType::RotateObject);
		CPostMaster::GetInstance().Unsubscribe(entity, eMessageType::MoveObjectToPosition);
		CPostMaster::GetInstance().Unsubscribe(entity, eMessageType::MoveObjectToPositionRelative);
		CPostMaster::GetInstance().Unsubscribe(entity, eMessageType::StartAnimationOnObject);
	}

	myMoveControllers.FreeAll();
	for (int i = 0; i < myInputControllers.Size(); ++i)
	{
		myInputControllers[i].Release();
	}
	myInputControllers.FreeAll();
	mySubscribedEntities.RemoveAll();
}

