#include "stdafx.h"
#include "PostMaster.h"
#include "..\Component\Entity.h"

bool CPostMaster::Init()
{
	mySubscribers.Init(cast_int(eMessageType::Count));

	for (int i = 0; i < cast_int(eMessageType::Count); ++i)
	{
		Corgi::GrowingArray<CSubscriber*> subscriberArray;
		subscriberArray.Init(1024);
		mySubscribers.Add(subscriberArray);
	}

	myMessages.Init(256);

	return true;
}

void CPostMaster::Subscribe(CSubscriber * aSubscriber, eMessageType aMessageSubscription)
{
	mySubscribers[cast_int(aMessageSubscription)].Add(aSubscriber);
}

void CPostMaster::Unsubscribe(CSubscriber * aSubscriber, eMessageType aMessageSubscription)
{
	mySubscribers[cast_int(aMessageSubscription)].RemoveCyclic(aSubscriber);
}

void CPostMaster::PostMessage(const SMessage& aMessage)
{
	std::lock_guard<std::mutex> lock(myMutex);
	myMessages.Add(aMessage);
}

void CPostMaster::MailOut()
{
	std::lock_guard<std::mutex> lock(myMutex);
	for (SMessage& message : myMessages)
	{
		for (CSubscriber* subscriber : mySubscribers[cast_int(message.messageType)])
		{
			subscriber->RecieveMessage(message);
		}
	}
	myMessages.RemoveAll();

}

void CPostMaster::Reset()
{
	std::lock_guard<std::mutex> lock(myMutex);
	myMessages.RemoveAll();
	for (int i = 0; i < mySubscribers.Size(); ++i)
	{
		mySubscribers[i].RemoveAll();
	}
	mySubscribers.RemoveAll();
	ReInit();
}

void CPostMaster::ReInit()
{
	mySubscribers.RemoveAll();
	for (int i = 0; i < cast_int(eMessageType::Count); ++i)
	{
		Corgi::GrowingArray<CSubscriber*> subscriberArray;
		subscriberArray.Init(64);
		mySubscribers.Add(subscriberArray);
	}
}

CPostMaster::CPostMaster()
{
}


CPostMaster::~CPostMaster()
{
}
