#pragma once
#include "SerializeHelper.h"
#define SERIALIZE(aStream, aType) serialize(aType, aStream)
#define DESERIALIZE(aStream, aType) aType = deserialize<decltype(aType)>(aStream)

enum class ENetMessage : char
{
	Default,
	Connection,
	Chat,
	Confirmation,
	count
};

class CNetMessage
{
public:
	CNetMessage();
	virtual ~CNetMessage();

	void Init();
	void PackMessage();
	void UnPackMessage(char* aMessage, int aSize);

	ENetMessage GetType() { return myID; }
	const unsigned int GetTimeStamp() const { return myTimeStamp; }
	const unsigned short GetSenderID()const { return mySenderID; }
	const unsigned short GetTargetID()const { return myTargetID; }

	void SetSenderID(unsigned short aIDToSet) { mySenderID = aIDToSet; }
	void SetTargetID(unsigned short aIDToSet) { myTargetID = aIDToSet; }

	StreamType myStream;
protected:
	unsigned int myTimeStamp;
	unsigned short mySenderID;
	unsigned short myTargetID;
	ENetMessage myID;

	virtual void DoSerialize(StreamType& aStreamType);
	virtual void DoDeSerialize(StreamType& aStreamType);
};

