#pragma once
#include "GameState.h"

class CPlaneCollider;
class CGameWorld;
class CCameraInstance;
class CDataManager;
class CTextManager;

class CEditState :
	public CGameState
{
public:
	CEditState();
	~CEditState();

	virtual void Init(CGameWorld* aGameWorld, CCameraInstance* globalCamera, CTextManager* aTextManager);

	virtual void OnEnter() override;
	virtual void OnExit() override;
	virtual void Destroy() override;

	virtual eStateStackMessage Update() override;
private:
	struct SEditorHelpTextToScreenIDList
	{
		long long myTextMode;
		long long myScrollZoom;
		long long myMouseMovementZoom;
		long long myRotation;
		long long myXYPlaneMovement;
		long long myXZPlaneMovement;
		long long myResetCamera;
		long long mySaveSettings;
	};
	struct SEditorHelpTextToScreenList
	{
		std::string myTextMode;
		std::string myScrollZoom;
		std::string myMouseMovementZoom;
		std::string myRotation;
		std::string myXYPlaneMovement;
		std::string myXZPlaneMovement;
		std::string myResetCamera;
		std::string mySaveSettings;
	};
	struct SCameraSettings
	{
		float cameraScrollZoomSpeed;
		float cameraMovementSpeed;
		float cameraAngleInDegrees;
		float cameraMouseZoomSpeed;
		float cameraMouseZoomMultiplier;
		float cameraMouseRotationSpeed;
		float cameraMouseDragSpeed;
		Vector3f cameraDistianceVectorToPlayer;
	};
private:
	void CameraEditorUpdate();
	void Init() override;

	CGameWorld* myGameWorld;
	eStateStackMessage myMessage;
	CCameraInstance* myGlobalCamera;
	CDataManager* myDataManager;
	CTextManager* myTextManager;

	SEditorHelpTextToScreenIDList myHelpTextID;
	SEditorHelpTextToScreenList myHelpText;
	SCameraSettings myCameraSettings;

	std::string myTextMode;
	long long myTextModeID;
	std::string myHelpTextShortCuts;
	long long myHelpTextShortCutsID;

	bool MouseZoomActive;
	bool MouseRotationActive;
	bool MouseCameraMovementXZ;
	Vector3f CurrentSavedMousePosition;
};

