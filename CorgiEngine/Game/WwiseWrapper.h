#pragma once
class CAudioInterface;

class CWwiseWrapper
{
public:
	~CWwiseWrapper();

	void Init();
	void Destroy();

	unsigned long PostEvent(const char* aEvent, int aObjectIndex);
	void PostNon3DEvent(const char* aEvent);
	void PostEventToAllIDs(const char* aEvent);

	int RegisterObjectAtFreeID();
	void UnRegisterObjectID(int aID);

	void SetPosition(int aID, const Corgi::Vector3<float>& aPosition);
	void SetRTPC(const char* aRTPC, int aID, int aValue);

	void SetRTPCForAllObjects(const char * aRTPC, int aValue);

	static CWwiseWrapper& GetInstance();

	void PlaySoundAtPosition(const char* aEvent, const Corgi::Vector3<float>& aPosition);

private:
	CWwiseWrapper(CAudioInterface& aAudio);
	CAudioInterface& myAudioInterface;

	Corgi::GrowingArray<int> myWwiseObjects;

	int myCurrentIndex;
};

