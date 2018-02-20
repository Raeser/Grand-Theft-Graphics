#pragma once
#include "RenderComponent.h"
#include "CameraComponent.h"
#include "SoundComponent.h"
#include "TransformComponent.h"
#include "PointLightComponent.h"
#include "SpotLightComponent.h"
#include "HealthComponent.h"
#include "CollisionComponent.h"
#include "ComponentManager.h"
#include "ParticleEmitterComponent.h"
#include "SpriteComponent.h"
#include "LifeTimeComponent.h"
#include "TriggerComponent.h"
#include "PlayerComponent.h"
#include "ActorComponent.h"
#include "ScriptComponent.h"
#include "Controller.h"
#include "InputController.h"
#include "MoveController.h"

#include "../Utilities/MemoryPool.h"

struct SPointLightData;
struct SSpotLightData;
class CEngine;
enum eScene;

class CComponentBuilder
{
public:
	CComponentBuilder(CEngine& aEngine);
	~CComponentBuilder();
	bool Init();
	void InitSpriteComponents(eScene aSpriteScene);
	CRenderComponent * GetRenderComponent(CEntity * aParentEntity, const char * aPath,const char * aVSShader, const char * aPSShader, const int& aPixelShaderID = 0, const int& aVertextShaderID = 0);
	CRenderComponent * GetRenderComponent(CEntity * aParentEntity, const char * aPath, const int& aPixelShaderID = 0, const int& aVertextShaderID = 0);
	CTransformComponent* GetTransformComponent(CEntity* aParentEntity);
	CCameraComponent* GetCameraComponent(CEntity* aParentEntity);
	CSoundComponent* GetSoundComponent(CEntity* aParentEntity);
	
	CPointLightComponent * GetPointLightComponent(CEntity* aParentEntity, const Vector3f& aColor, const float& aRange);
	CSpotLightComponent * GetSpotLightComponent(CEntity * aParentEntity, SSpotLightData& aSpotLightData);
	CHealthComponent*  GetHealthComponent(CEntity* aParentEntity);
	CCollisionComponent* GetCollisionComponent(CEntity* aParentEntity);
	CLifeTimeComponent* GetLifeTimeComponent(CEntity* aParentEntity);
	CParticleEmitterComponent* GetParticleEmitterComponent(CEntity* aParentEntity);
	CSpriteComponent* GetSpriteComponent(CEntity* aParentEntity);
	CTriggerComponent* GetTriggerComponent(CEntity* aParentEntity);
	CPlayerComponent* GetPlayerComponent(CEntity* aParentEntity);
	CActorComponent* GetActorComponent(CEntity* aParentEntity);
	CScriptComponent* GetScriptComponents(CEntity* aParentEntity);
	void Update();
	void Reset();
private:
	Corgi::CMemoryPool<CRenderComponent> myRenderComponents;
	Corgi::CMemoryPool<CCameraComponent> myCameraComponents;
	Corgi::CMemoryPool<CTransformComponent> myTransformComponents;
	Corgi::CMemoryPool<CSoundComponent> mySoundComponents;

	Corgi::CMemoryPool<CPointLightComponent> myPointLightComponents;
	Corgi::CMemoryPool<CSpotLightComponent> mySpotLightComponents;

	Corgi::CMemoryPool<CCollisionComponent> myCollisionComponents;
	Corgi::CMemoryPool<CHealthComponent> myHealthComponents;
	Corgi::CMemoryPool<CParticleEmitterComponent> myParticleEmitterComponents;
	Corgi::CMemoryPool<CSpriteComponent> mySpriteComponents;
	Corgi::CMemoryPool<CLifeTimeComponent> myLifeTimeComponents;
	Corgi::CMemoryPool<CTriggerComponent> myTriggerComponents;
	Corgi::CMemoryPool<CPlayerComponent> myPlayerComponenets;
	Corgi::CMemoryPool<CActorComponent> myActorComponent;
	Corgi::CMemoryPool<CScriptComponent> myScriptComponents;
	//Sprites is only available in localScene atm, we can add them to the others when needed;


	CInputController myInputComponent;

	CEngine& myEngine;
};

