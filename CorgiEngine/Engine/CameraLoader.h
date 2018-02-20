#pragma once
#include "CameraInstance.h"

class CDXFramework;

class CCameraLoader
{
public:
	CCameraLoader();
	~CCameraLoader();

	CCameraInstance* CreateCameraInstance(CCameraInstance::SCameraData aCameraData, CDXFramework* aFramework);
	
};

