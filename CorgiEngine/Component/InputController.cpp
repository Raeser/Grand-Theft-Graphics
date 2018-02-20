#include "stdafx.h"
#include "InputController.h"
#include "..\Utilities\PlaneCollider.h"



#include "..\Engine\CameraInstance.h"
#include "..\Game\DataManager.h"

#include "ActorComponent.h"
#include "..\Engine\DebugDrawer.h" 

CInputController::CInputController() : 
	myCamera(*CEngineWrapper::GetInstance().GetGlobalCamera())
{
}

CInputController::~CInputController()
{	

}

void CInputController::Init(EControllerType aControllType, CPlaneCollider* aPlaneCollider, const Vector3f& aPosition)
{
	mySlowedRadius = 3.0f;
	myPosToGoTowards = aPosition;
	CController::Init(aControllType);
	myHasSpawnedAPointModelPointOnClick = false;
	myPlaneCollider = aPlaneCollider;
	CPostMaster::GetInstance().Subscribe(this, eMessageType::PlayerData);
	myMaxSpeed = CDataManager::GetInstance().GetPlayerData().playerSpeed;
	mySpeed = myMaxSpeed;

	myPathNodes.Init(10);
	myPathFindingShouldEnd = false;
	myShouldRelease = false;

	myDebugDraw = false;
}


SDirectionData CInputController::Update(const Corgi::Vector3<float>& aActorPos)
{
	myCamera.SetPosition(aActorPos + CDataManager::GetInstance().GetEditorData().cameraDistianceVectorToPlayer);
	SendMessage(aActorPos);

	SDirectionData data;

	int moveX = 0;
	int moveZ = 0;

	Vector4f dir(0, 0, 0, 0);
	if (InputManager::GetInstance().CheckKeyDown(KeyCode::W) && !CGameSettings::GetInstance().GetIsInEditorState())
	{
		dir.z += 1;
		moveZ += 1;
	}
	if (InputManager::GetInstance().CheckKeyDown(KeyCode::A) && !CGameSettings::GetInstance().GetIsInEditorState())
	{
		dir.x += -1;
		//moveX -= 1;
	}
	if (InputManager::GetInstance().CheckKeyDown(KeyCode::S) && !CGameSettings::GetInstance().GetIsInEditorState())
	{
		dir.z += -1;
		moveZ -= 1;
	}
	if (InputManager::GetInstance().CheckKeyDown(KeyCode::D) && !CGameSettings::GetInstance().GetIsInEditorState())
	{
		dir.x += 1;
		//moveX += 1;
	}

	if (moveX != 0 || moveZ != 0)
	{
		dir = dir * myParent->GetTransform().GetOrientationMatrix();
		data.speedModifier = mySpeed;
		data.direction = dir.V3();
		data.direction.y = 0.f;
	}

	SetLookAtPosition(dir.V3() + aActorPos);

	UpdateRotation();

	return data;
}

void CInputController::DrawTarget()
{
}

void CInputController::RecieveMessage(const SMessage & aMessage)
{
	switch (aMessage.messageType)
	{
	case eMessageType::PlayerData:
		myMaxSpeed = aMessage.playerData.myPlayerSpeed;
		break;
	default:
		break;
	}
}

void CInputController::Release()
{
	CPostMaster::GetInstance().Unsubscribe(this, eMessageType::PlayerData);
	myShouldRelease = true;
}

void CInputController::SendMessage(const Corgi::Vector3<float>& aActorPos)
{
	SMessage msg;
	msg.messageType = eMessageType::PlayerPosition;
	msg.playerPosition.x = aActorPos.x;
	msg.playerPosition.y = aActorPos.y;
	msg.playerPosition.z = aActorPos.z;

	CPostMaster::GetInstance().PostMessage(msg);
}

void CInputController::DrawPath(const Corgi::GrowingArray<Vector3f>& aPath, const Corgi::Vector3<float>& aActorPos)
{

	Corgi::Vector4<float> currentFromPos = { aActorPos.x, aActorPos.y,aActorPos.z, 1 };
	Corgi::Vector4<float> currentToPos = { aPath[aPath.Size() - 1].x, aPath[aPath.Size() - 1].y, aPath[aPath.Size() - 1].z, 1 };

#ifndef _RETAIL
	CDebugDrawer::GetInstance().DrawArrow(currentFromPos, currentToPos, { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f });
#endif

	if (aPath.Size() != 1)
	{
		for (unsigned short Index = 0; Index < aPath.Size() - 1; ++Index)
		{
			Corgi::Vector4<float> fromPos = { aPath[Index + 1].x, aPath[Index + 1].y, aPath[Index + 1].z, 1 };
			Corgi::Vector4<float> toPos = { aPath[Index].x, aPath[Index].y, aPath[Index].z, 1 };
#ifndef _RETAIL
			CDebugDrawer::GetInstance().DrawArrow(fromPos, toPos, { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f });
#endif
		}
	}
}

