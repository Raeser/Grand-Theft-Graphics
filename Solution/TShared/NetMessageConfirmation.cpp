#include "stdafx.h"
#include "NetMessageConfirmation.h"


CNetMessage_Confirmation::CNetMessage_Confirmation()
{
}


CNetMessage_Confirmation::~CNetMessage_Confirmation()
{
}

void CNetMessage_Confirmation::Init(EConfirmationStatus aStatus, int aClientID)
{
	CNetMessage::Init();
	myID = ENetMessage::Confirmation;
	myClientID = aClientID;
	myStatus = aStatus;
}

void CNetMessage_Confirmation::DoSerialize(StreamType & aStreamType)
{
	CNetMessage::DoSerialize(aStreamType);
	SERIALIZE(aStreamType, myStatus);
	SERIALIZE(aStreamType, myClientID);
}

void CNetMessage_Confirmation::DoDeSerialize(StreamType & aStreamType)
{
	CNetMessage::DoDeSerialize(aStreamType);
	DESERIALIZE(aStreamType, myStatus);
	DESERIALIZE(aStreamType, myClientID);
}
