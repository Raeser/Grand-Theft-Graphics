#include "stdafx.h"
#include "NetMessage.h"


CNetMessage::CNetMessage()
{
}


CNetMessage::~CNetMessage()
{
}

void CNetMessage::Init()
{
	myID = ENetMessage::Default;
	myTimeStamp = 0;
	mySenderID = 0;
	myTargetID = 0;
}

void CNetMessage::PackMessage()
{
	DoSerialize(myStream);

	if (sizeof(myStream) > 512)
	{
		assert(false && "Stream too big");
	}
}

void CNetMessage::UnPackMessage(char * aMessage, int aSize)
{
	myStream.resize(aSize);
	memcpy(&myStream[0], aMessage, aSize);
	DoDeSerialize(myStream);
}

void CNetMessage::DoSerialize(StreamType & aStreamType)
{
	SERIALIZE(aStreamType, myID);
	SERIALIZE(aStreamType, myTimeStamp);
	SERIALIZE(aStreamType, mySenderID);
	SERIALIZE(aStreamType, myTargetID);
}

void CNetMessage::DoDeSerialize(StreamType & aStreamType)
{
	DESERIALIZE(aStreamType, myID);
	DESERIALIZE(aStreamType, myTimeStamp);
	DESERIALIZE(aStreamType, mySenderID);
	DESERIALIZE(aStreamType, myTargetID);
}
