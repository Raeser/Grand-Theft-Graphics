#include "stdafx.h"
#include "NetMessageChatMessage.h"


CNetMessageChatMessage::CNetMessageChatMessage()
{
	myID = ENetMessage::Chat;
}


CNetMessageChatMessage::~CNetMessageChatMessage()
{
}

void CNetMessageChatMessage::Init(const std::string & aMessage)
{
	CNetMessage::Init();
	myID = ENetMessage::Chat;
	myMessage = aMessage;
}

void CNetMessageChatMessage::SetMessage(const std::string & aMessage)
{
	myMessage = aMessage;
}

void CNetMessageChatMessage::DoSerialize(StreamType & aStreamType)
{
	CNetMessage::DoSerialize(aStreamType);
	SERIALIZE(aStreamType, myMessage);
}

void CNetMessageChatMessage::DoDeSerialize(StreamType & aStreamType)
{
	CNetMessage::DoDeSerialize(aStreamType);
	DESERIALIZE(aStreamType, myMessage);
}
