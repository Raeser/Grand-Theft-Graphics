#pragma once

class CEngine;
class CCameraInstance;

class CEngineWrapper
{
public:

	static CEngineWrapper& GetInstance();
	CCameraInstance* GetGlobalCamera();
	~CEngineWrapper();

	void Init(CEngine& aEngine);

private:
	CEngineWrapper();

	CEngine* myEngine;
};

