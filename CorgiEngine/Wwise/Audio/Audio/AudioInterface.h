#pragma once
#include <functional>   // std::bind
#include "SoundObjectIDManager.h"


class CWwiseManager;
class CAudioInterface
{
public:
	static CAudioInterface* GetInstance(){ return myInstance; }
	static void CreateInstance(){ myInstance = new CAudioInterface(); }
	static bool IsInstanced();
	static void Destroy();

	bool Init(const char* aInitBank);
	bool LoadBank(const char* aBankPath);
	void UnLoadBank(const char* aBankPath);

	//Returns a playing ID
	unsigned long PostEvent(const char* aEvent, int anObjectID);
	unsigned long PostEventToAllIDs(const char* aEvent);

	void Update();

	void SetRTPC(const char* aRTPC, int aValue, int anObjectID);
	void SetRTPCForAllObjects(const char* aRTPC, int aValue);
	void SetPosition(float aX, float aY, float aZ, int aObjectID, float aDirectionX = 0, float aDirectionY = 0, float aDirectionZ = 1.f);
	void SetListenerPosition(float aX, float aY, float aZ, float aDirectionX = 0, float aDirectionY = 0, float aDirectionZ = 1.f, float aUpX = 0.f, float aUpY = 1.0f, float aUpZ = 0.0f);

	int RegisterObjectAtFreeID();
	void UnRegisterObjectID(int anObjectID);

	typedef void(*callback_function)(const char*);
	void SetErrorCallBack(callback_function aErrorCallback);
private:
	static CAudioInterface* myInstance;
	CAudioInterface();
	~CAudioInterface();

	CWwiseManager* myWwiseManager;
	SoundObjectIDManager myIDManager;
};

