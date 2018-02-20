#pragma once
#include "..\Utilities\GrowingArray.h"
#include "..\Engine\Subscriber.h"
#include <mutex>

class CEntity;

class CPostMaster
{
public:
	~CPostMaster();
	
	static CPostMaster& GetInstance()
	{
		static CPostMaster me;
		return me;
	}

	bool Init();

	void Subscribe(CSubscriber* aSubscriber, eMessageType aMessageSubscription);
	void Unsubscribe(CSubscriber* aSubscriber, eMessageType aMessageSubscription);
	void PostMessage(const SMessage& message);
	void MailOut();


	void Reset();
	void ReInit();

private:
	CPostMaster();

	std::mutex myMutex;
	Corgi::GrowingArray<Corgi::GrowingArray<CSubscriber*>, int> mySubscribers;
	Corgi::GrowingArray<SMessage> myMessages;


};

