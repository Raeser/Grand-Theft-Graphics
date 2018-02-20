#pragma once
#include "..\Utilities\Vector.h"

class CEntity;
class CCamera;
class CPlaneCollider;

enum class ECallbackEvent;

enum class eMessageType
{
	PlayerHealthInfo,
	PlayerPosition,
	PlayerDirection,
	PickupInfo,
	CursorPosition,
	BuilTargetPointModelAtPosition,
	CursorAimPosition,

	AimLockData,
	TikiData,
	CameraData,
	TriggersData,
	ControllerData,

	FullscreenData,
	SetResolution,

	FullHealthEverything,
	WonGame,
	TikiGotThrown,
	PlayerData,
	BuildTikiAtPosition,
	BuildTikiAtPosition_Idle,
	MoveObjectToPosition,
	MoveObjectToPositionRelative,
	RotateObject,
	CallbackEvent,
	ChangeLevelName,
	ChangeNavmesh,
	StartAnimationOnObject,
	KillTiki,
	DisableCollider,
	CreateParticleEmitter,
	Reset,

	DebugDrawFullNav,
	DebugDrawFullOuterEdges,
	DebugDrawLocalOuterEdges,
	DebugDrawLocalNav,
	DebugDrawNavMeshColliders,
	DebugDrawWallColliders,

	Count
};

namespace msgStruct
{
	struct RotateObject
	{
		const char* name;
		float x, y, z, speed;
	};
	struct ChangeNavmesh
	{
		const char* name;
	};

	struct CreateParticleEmitter
	{
		const char* particlesystem;
		float x, y, z;
		float lifestime;
	};
	struct DisableCollider
	{
		const char* id;
	};

	struct KillTiki
	{
		const char* ID;
	};
	struct StartAnimationOnObject
	{
		const char* nameID;
		const char* animationName;
	};

	struct CallbackEvent
	{
		ECallbackEvent callbackEvent;
		const char* id;
	};
	struct MoveObjectToPosition
	{
		const char* nameID;
		float x, y, z, speed;
	};

	struct ChangeLevelName
	{
		const char* levelName;
	};

	struct BuildTikiAtPosition
	{
		float x, y, z;
	};
	struct BuilTargetPointModelAtPosition
	{
		float x, y, z;
		float endYHeight, downMovementSpeed, lifeTime;
	};
	struct PlayerData
	{
		float myPlayerSpeed;
	};

	struct TikiData
	{
		float tikiSpeed;
		float throwSpeed;
		float maxThrowHeight;
		float maxThrowLength;
		float jumpSpeed;
		float minDistianceToPlayer;
		float restlessMaxTimer;
	};

	struct PlayerHealthInfo
	{
		int currentHealth = 0;
		int maxHealth = 0;
	};

	struct PickupInfo
	{
		int healthToGive = 0;
	};


	struct PlayerPosition
	{
		float matrix[16];
		float x, y, z;
	};

	struct PlayerDirection
	{
		float dirX, dirY, dirZ;
	};

	struct CursorPosition 
	{
		float x, y, z;
	};

	struct AimLockData 
	{
		CEntity* entity;
		bool shouldLock;
		float x, y, z;
	};

	struct TriggersData
	{
		int aEventNumber;
	};

	struct CameraData
	{
		float myToCamera[16];
		float myProjection[16];
	};

	struct ControllerData
	{
		CCamera** aCamera;
		CPlaneCollider** aPlaneCollider;
	};


	struct TikiThrowData
	{
		Corgi::Vector3<float> myThrowPosition;
	};


	struct ResolutionData 
	{
		Corgi::Vector2<int> myResolution;
	};

	struct FullscreenData 
	{
		bool shouldFullscreen;
	};

	struct DebugDrawFullNav
	{
	};

	struct DebugDrawFullOuterEdges
	{
	};

	struct DebugDrawLocalOuterEdges
	{
	};

	struct DebugDrawLocalNav
	{
	};

	struct DebugDrawNavMeshColliders
	{
	};

	struct DebugDrawWallColliders
	{

	};
	
}

struct SMessage
{
	SMessage() {};

	eMessageType messageType;

	union
	{
		msgStruct::PlayerHealthInfo healthInfo;
		msgStruct::PlayerPosition playerPosition;
		msgStruct::TikiData tikiData;
		msgStruct::PlayerDirection playerDirection;
		msgStruct::PlayerData playerData;
		msgStruct::CameraData cameraData;
		msgStruct::AimLockData aimLockData;

		msgStruct::CursorPosition cursorPos;
		msgStruct::TriggersData triggersData;
		msgStruct::PickupInfo pickupInfo;
		msgStruct::ControllerData controllerData;
		msgStruct::BuildTikiAtPosition buildTikiAtPosition;
		msgStruct::BuilTargetPointModelAtPosition builTargetPointModelAtPosition;
		msgStruct::MoveObjectToPosition moveObjectToPosition;
		msgStruct::RotateObject rotateObject;
		msgStruct::TikiThrowData tikiThrowData;
		msgStruct::CallbackEvent callbackEvent;
		msgStruct::ChangeLevelName changeLevelName;
		msgStruct::ChangeNavmesh changeNavmesh;
		msgStruct::StartAnimationOnObject startAnimationOnObject;
		msgStruct::KillTiki killTiki;
		msgStruct::CreateParticleEmitter createParticleEmitter;
		msgStruct::DisableCollider disableCollider;

		msgStruct::ResolutionData myResolutionData;
		msgStruct::FullscreenData fullscreenData;


		msgStruct::DebugDrawFullNav fullNav;
		msgStruct::DebugDrawFullOuterEdges fullOuterEdges;
		msgStruct::DebugDrawLocalOuterEdges localOuterEdges;
		msgStruct::DebugDrawLocalNav localNav;
		msgStruct::DebugDrawNavMeshColliders navColliders;
		msgStruct::DebugDrawWallColliders wallColliders;
	};
};


class CSubscriber
{
public:
	virtual void RecieveMessage(const SMessage& message) = 0;
};