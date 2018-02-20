#include "stdafx.h"
#include "MoveController.h"
#include "..\Utilities\JsonDocument.h"
#include "..\Script\lua_argument_struct.h"


CMoveController::CMoveController()
{
	myMoveState = EMoveState::Idle;
}


CMoveController::~CMoveController()
{
}

void CMoveController::Init(EControllerType aControllType, float aSpeed)
{
	CController::Init(aControllType);
	myMoveState = EMoveState::Idle;
	mySpeed = aSpeed;
	myShouldRelease = false;
}

SDirectionData CMoveController::Update(const Corgi::Vector3<float>& aActorPos)
{
	Vector3f dir = myGoToPosition - aActorPos;
	const float mindistance = 0.1f;

	SDirectionData out = SDirectionData();
	switch (myMoveState)
	{
	case EMoveState::Idle:
		out.direction = Vector3f();
		out.speedModifier = 0;
		break;
	case EMoveState::Moving:
		//''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''|
		//myMoveState = static_cast<EMoveState>(cast_int((dir.Length() < mindistance)));|
		//This is untested optimised code!''''''''''''''''''''''''''''''''''''''''''''''|

		if (dir.Length() < mindistance)
		{
			SetState(EMoveState::Idle);

			SMessage mess;
			mess.messageType = eMessageType::CallbackEvent;
			mess.callbackEvent.callbackEvent = ECallbackEvent::MoveDone;
			mess.callbackEvent.id = myParent->GetID().c_str();

			CPostMaster::GetInstance().PostMessageA(mess);

		}

		out.direction = dir;
		out.speedModifier = mySpeed;
		break;
	default:
		assert(false && "Invalid EMoveState");
		break;
	}
	return out;
}

void CMoveController::MoveToPosition(const Corgi::Vector3<float>& aPosition, float aSpeed)
{
	myGoToPosition = aPosition;
 	mySpeed = aSpeed;
	SetState(EMoveState::Moving);
}

void CMoveController::SetState(EMoveState aState)
{
	myMoveState = aState;
}

EMoveState CMoveController::GetState() const
{
	return myMoveState;
}


void CMoveController::Release()
{
	myGoToPosition = Vector3f();
	myShouldRelease = true;
}

