#include "stdafx.h"
#include "NetMessage_Connect.h"


CNetMessage_Connect::CNetMessage_Connect()
{
}


CNetMessage_Connect::~CNetMessage_Connect()
{
}

void CNetMessage_Connect::Init(EConnectStatus aStatus)
{
	CNetMessage::Init();
	myID = ENetMessage::Connection;
	myStatus = aStatus;
}

const EConnectStatus CNetMessage_Connect::GetConnectStatus()
{
	return myStatus;
}

void CNetMessage_Connect::DoSerialize(StreamType & aStreamType)
{
	CNetMessage::DoSerialize(aStreamType);
	SERIALIZE(aStreamType, myStatus);
}

void CNetMessage_Connect::DoDeSerialize(StreamType & aStreamType)
{
	CNetMessage::DoDeSerialize(aStreamType);
	DESERIALIZE(aStreamType, myStatus);
}
