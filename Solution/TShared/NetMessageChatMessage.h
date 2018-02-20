#pragma once
#include "NetMessage.h"
class CNetMessageChatMessage :
	public CNetMessage
{
public:
	CNetMessageChatMessage();
	virtual ~CNetMessageChatMessage();

	void Init(const std::string& aMessage);

	const std::string& GetChatMessage() const { return myMessage; }
	void SetMessage(const std::string& aMessage);
protected:
	std::string myMessage;

	virtual void DoSerialize(StreamType& aStreamType);
	virtual void DoDeSerialize(StreamType& aStreamType);
};

