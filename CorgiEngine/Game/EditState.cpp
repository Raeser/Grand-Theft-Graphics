#include "stdafx.h"
#include "EditState.h"
#include "..\Utilities\PlaneCollider.h"
#include "EngineWrapper.h"


#include "..\Engine\TextManager.h"
#include "..\Engine\DebugDrawer.h"


CEditState::CEditState()
{
}

CEditState::~CEditState()
{
}
void CEditState::Init()
{
}
void CEditState::Init(CGameWorld* aGameWorld, CCameraInstance* globalCamera, CTextManager* aTextManager)
{
	myName = "Editing state";
	myMessage = eStateStackMessage::KeepState;
	myGameWorld = aGameWorld;
	myGlobalCamera = globalCamera;

	myDataManager = &CDataManager::GetInstance();
	myTextManager = aTextManager;

	myHelpText.myTextMode = "Editor Mode";
	myTextMode = "Editor Mode";
	myHelpTextShortCuts;
	myHelpTextShortCuts +=/*			*/"F : Reset camera\n";
	myHelpTextShortCuts +=/*			*/"Alt + Mouse <-> : Zoom in/out\n";
	myHelpTextShortCuts +=/*			*/"MouseScroll : Zoom in/out\n";
	myHelpTextShortCuts +=/*			*/"CTRL + S : Save settings to file\n";
	myHelpTextShortCuts +=/*			*/"ALT + LMouse + Mouse <-> : Rotate camera\n";
	myHelpTextShortCuts +=/*			*/"WASD : Move XZ-Plane\n";
	myHelpTextShortCuts +=/*			*/"LMouse + Mouse <-> : Move XZ-Plane\n";

	myTextModeID = static_cast<long long>(myTextManager->AddText(myTextMode, { -0.05f, 0.54f }));
	myHelpTextShortCutsID = static_cast<long long>(myTextManager->AddText(myHelpTextShortCuts, { -1.0f, -0.35f }));
}

void CEditState::OnEnter()
{
	InputManager::GetInstance().SetClipCursorToScreen(true);
	MouseZoomActive = false;
	MouseRotationActive = false;
	MouseCameraMovementXZ = false;
	CGameSettings::GetInstance().SetIsInEditorState(true);
}

void CEditState::OnExit()
{
	myTextManager->RemoveText(myTextModeID);
	myTextManager->RemoveText(myHelpTextShortCutsID);
	CGameSettings::GetInstance().SetIsInEditorState(false);

}

void CEditState::Destroy()
{
}

eStateStackMessage CEditState::Update()
{
	UpdateStateTimer();
	//CDebugDrawer::GetInstance().DrawDebugText("EDITOR MODE");
	CCursor::GetInstance().Update();


	if (InputManager::GetInstance().CheckKeyReleased(KeyCode::Escape) || InputManager::GetInstance().CheckKeyReleased(KeyCode::F1))
	{
		myMessage = eStateStackMessage::PopSubState;
	}


	myGameWorld->Update();
	CameraEditorUpdate();

	return myMessage;
}

void CEditState::CameraEditorUpdate()
{
	// FetchEditorData
	myCameraSettings.cameraDistianceVectorToPlayer = myDataManager->GetEditorData().cameraDistianceVectorToPlayer;
	myCameraSettings.cameraAngleInDegrees = myDataManager->GetEditorData().cameraAngleInDegrees;
	myCameraSettings.cameraMovementSpeed = myDataManager->GetEditorData().cameraSpeed;

	myCameraSettings.cameraScrollZoomSpeed = myDataManager->GetEditorData().cameraScrollZoomSpeed;

	myCameraSettings.cameraMouseZoomSpeed = myDataManager->GetEditorData().cameraMouseZoomSpeed;
	myCameraSettings.cameraMouseZoomMultiplier = myDataManager->GetEditorData().cameraMouseZoomMultiplier;

	myCameraSettings.cameraMouseRotationSpeed = myDataManager->GetEditorData().cameraMouseRotationSpeed;

	myCameraSettings.cameraMouseDragSpeed = myDataManager->GetEditorData().cameraMouseDragSpeed;

	if (InputManager::GetInstance().CheckKeyDown(KeyCode::Ctrl))
	{
		// SaveSettings
		if (InputManager::GetInstance().CheckKeyDown(KeyCode::S))
		{
			myDataManager->SaveCurrentSettingsToData(CDataManager::eDataWrapper::EditorSettings);
			return;
		}
	}
	else
	{

		// ResetSettings
		if (InputManager::GetInstance().CheckKeyReleased(KeyCode::F))
		{
			myDataManager->ResetData(CDataManager::eDataWrapper::EditorSettings);
			return;
		}

		//WASD Movement XZ-Plane
		if (InputManager::GetInstance().CheckKeyDown(KeyCode::W))
		{
			myCameraSettings.cameraDistianceVectorToPlayer.z += myCameraSettings.cameraMovementSpeed * Timer::GetInstance().GetDeltaTime();
		}
		if (InputManager::GetInstance().CheckKeyDown(KeyCode::A))
		{
			myCameraSettings.cameraDistianceVectorToPlayer.x -= myCameraSettings.cameraMovementSpeed * Timer::GetInstance().GetDeltaTime();
		}
		if (InputManager::GetInstance().CheckKeyDown(KeyCode::S))
		{
			myCameraSettings.cameraDistianceVectorToPlayer.z -= myCameraSettings.cameraMovementSpeed * Timer::GetInstance().GetDeltaTime();
		}
		if (InputManager::GetInstance().CheckKeyDown(KeyCode::D))
		{
			myCameraSettings.cameraDistianceVectorToPlayer.x += myCameraSettings.cameraMovementSpeed * Timer::GetInstance().GetDeltaTime();
		}

		//Mouse Movement XZ-Plane
		if (InputManager::GetInstance().CheckKeyDown(KeyCode::MouseLeft) && !InputManager::GetInstance().CheckKeyDown(KeyCode::LeftAlt))
		{
			if (MouseCameraMovementXZ)
			{
				const float mouseDistanceFromStartY = CurrentSavedMousePosition.y - CCursor::GetInstance().GetPosition().y;
				const float mouseDistanceFromStartX = CurrentSavedMousePosition.x - CCursor::GetInstance().GetPosition().x;
				if (mouseDistanceFromStartY < -0.01f || mouseDistanceFromStartY > 0.01f)
				{
					myCameraSettings.cameraDistianceVectorToPlayer.z -= mouseDistanceFromStartY * myCameraSettings.cameraMouseDragSpeed * Timer::GetInstance().GetDeltaTime();
				}

				if (mouseDistanceFromStartX < -0.01f || mouseDistanceFromStartX > 0.01f)
				{
					myCameraSettings.cameraDistianceVectorToPlayer.x += mouseDistanceFromStartX * myCameraSettings.cameraMouseDragSpeed * Timer::GetInstance().GetDeltaTime();
				}
			}
			else
			{
				CurrentSavedMousePosition = CCursor::GetInstance().GetPosition();
				MouseCameraMovementXZ = true;
			}
		}
		else
		{
			MouseCameraMovementXZ = false;
		}

		// Zoom in/out && Rotation

		// MouseScroll
		if (InputManager::GetInstance().GetMouseWheelRotation() < 0)
		{
			myCameraSettings.cameraDistianceVectorToPlayer.y += myCameraSettings.cameraScrollZoomSpeed;
			myCameraSettings.cameraDistianceVectorToPlayer.z += myCameraSettings.cameraScrollZoomSpeed * sin(myCameraSettings.cameraAngleInDegrees);
		}
		else if (InputManager::GetInstance().GetMouseWheelRotation() > 0)
		{
			myCameraSettings.cameraDistianceVectorToPlayer.y -= myCameraSettings.cameraScrollZoomSpeed;
			myCameraSettings.cameraDistianceVectorToPlayer.z -= myCameraSettings.cameraScrollZoomSpeed * sin(myCameraSettings.cameraAngleInDegrees);
		}
		else if (InputManager::GetInstance().CheckKeyDown(KeyCode::LeftAlt))
		{

			// MouseMovement && Rotation
			if (InputManager::GetInstance().CheckKeyDown(KeyCode::MouseLeft))
			{
				MouseZoomActive = false;
				if (MouseRotationActive)
				{
					const float mouseDistanceFromStart = CurrentSavedMousePosition.y - CCursor::GetInstance().GetPosition().y;
					const float cameraRotationSpeed = mouseDistanceFromStart;
					if (mouseDistanceFromStart < -0.01f)
					{
						myCameraSettings.cameraAngleInDegrees += cameraRotationSpeed * myCameraSettings.cameraMouseRotationSpeed;
					}
					else if (mouseDistanceFromStart > 0.01f)
					{
						myCameraSettings.cameraAngleInDegrees += cameraRotationSpeed * myCameraSettings.cameraMouseRotationSpeed;
					}
				}
				else
				{
					CurrentSavedMousePosition = CCursor::GetInstance().GetPosition();
					MouseRotationActive = true;
				}
			}
			else
			{
				MouseRotationActive = false;
				if (MouseZoomActive)
				{
					const float mouseDistanceFromStart = CurrentSavedMousePosition.x - CCursor::GetInstance().GetPosition().x;
					const float cameraMovementSpeed = pow(mouseDistanceFromStart * myCameraSettings.cameraMouseZoomMultiplier, 2);
					if (mouseDistanceFromStart < -0.05f && myCameraSettings.cameraDistianceVectorToPlayer.y > 0.5f)
					{
						myCameraSettings.cameraDistianceVectorToPlayer.y -= cameraMovementSpeed * myCameraSettings.cameraMouseZoomSpeed;
						myCameraSettings.cameraDistianceVectorToPlayer.z -= sin(myCameraSettings.cameraAngleInDegrees) * (cameraMovementSpeed * myCameraSettings.cameraMouseZoomSpeed);
					}
					else if (mouseDistanceFromStart > 0.05f)
					{
						myCameraSettings.cameraDistianceVectorToPlayer.y += cameraMovementSpeed * myCameraSettings.cameraMouseZoomSpeed;
						myCameraSettings.cameraDistianceVectorToPlayer.z += sin(myCameraSettings.cameraAngleInDegrees) * (cameraMovementSpeed * myCameraSettings.cameraMouseZoomSpeed);
					}
				}
				else
				{
					CurrentSavedMousePosition = CCursor::GetInstance().GetPosition();
					MouseZoomActive = true;
				}
			}

		}
		else
		{
			MouseZoomActive = false;
		}

		myGlobalCamera->SetOrientation(Matrix44::CreateRotateAroundX(myCameraSettings.cameraAngleInDegrees * 3.1415927f / 180.f));
		//Matrix44 cameraOrientation;
		//cameraOrientation = myGlobalCamera->GetOrientation();
		//cameraOrientation.myMatrix[12] +=  myCameraSettings.cameraDistianceVectorToPlayer.x;
		//cameraOrientation.myMatrix[13] +=  myCameraSettings.cameraDistianceVectorToPlayer.y;
		//cameraOrientation.myMatrix[14] +=  myCameraSettings.cameraDistianceVectorToPlayer.z;
		//myGlobalCamera->SetOrientation(cameraOrientation);

		// SetEditorData
		myDataManager->GetEditorData().cameraDistianceVectorToPlayer = myCameraSettings.cameraDistianceVectorToPlayer;
		myDataManager->GetEditorData().cameraAngleInDegrees = myCameraSettings.cameraAngleInDegrees;
		myDataManager->GetEditorData().cameraScrollZoomSpeed = myCameraSettings.cameraScrollZoomSpeed;
		myDataManager->GetEditorData().cameraSpeed = myCameraSettings.cameraMovementSpeed;

		myGlobalCamera->SetPosition(myGameWorld->GetPlayerEntity()->GetTransform().GetPosition() + myCameraSettings.cameraDistianceVectorToPlayer);


	}
}

