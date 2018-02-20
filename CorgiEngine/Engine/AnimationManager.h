#pragma once

class CAnimationInstance;
class CAnimationLoader;
class CAnimationManager
{
public:
	CAnimationManager(CAnimationLoader& myLoader);
	~CAnimationManager();

	void Init(int aMaxAmountOfAnimatedObjects);
	CAnimationInstance* GetAnimationInstance();
	void FreeAnimationInstanceAtID(long long aID);

	void Update();
	void Reset();
private:

	//START TEST SHIT
	void TestInstanceFunctions(long long aInstanceToTestOn);

	long long testinstanceID = 0;

	//END TEST SHIT
	CAnimationLoader& myLoader;
	Corgi::CMemoryPool<CAnimationInstance> myAnimationInstances;
};

