#include "stdafx.h"
#include "CameraLoader.h"

CCameraLoader::CCameraLoader()
{
}


CCameraLoader::~CCameraLoader()
{
}

CCameraInstance * CCameraLoader::CreateCameraInstance(CCameraInstance::SCameraData aCameraData, CDXFramework* aFramework)
{
	CCameraInstance* cameraInstance = new CCameraInstance();
	cameraInstance->Init(aCameraData, aFramework);
	return cameraInstance;
}

