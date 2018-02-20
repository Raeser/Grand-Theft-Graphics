#include "stdafx.h"
#include "EngineWrapper.h"


CEngineWrapper::CEngineWrapper()
{
	myEngine = nullptr;
}


CEngineWrapper & CEngineWrapper::GetInstance()
{
	static CEngineWrapper engineWrapper;
	return engineWrapper;

}

CCameraInstance* CEngineWrapper::GetGlobalCamera() 
{
	assert(myEngine != nullptr && "CEngine was nullptr, Init Singleton first!");
	return myEngine->GetGlobalCamera();
}

CEngineWrapper::~CEngineWrapper()
{
}

void CEngineWrapper::Init(CEngine & aEngine)
{
	myEngine = &aEngine;
}
