#pragma once
#include "NetMessage.h"

enum class EConnectStatus : char
{
	Connect,
	Connection_Established,
	Disconnect,
	Ping_ToServer,
	Ping_ToClient,
	count
};

class CNetMessage_Connect :
	public CNetMessage
{
public:
	CNetMessage_Connect();
	virtual ~CNetMessage_Connect();
	void Init(EConnectStatus aStatus);
	const EConnectStatus GetConnectStatus();

private:
	EConnectStatus myStatus;
	virtual void DoSerialize(StreamType& aStreamType) override;
	virtual void DoDeSerialize(StreamType& aStreamType) override;
};

