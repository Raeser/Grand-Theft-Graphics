#pragma once
#include "NetMessage.h"

enum class EConfirmationStatus : char 
{
	Success,
	Failed,
	Banned,
	count
};

#define ERROR_CODE_FIRST 0
#define ERROR_CODE_LAST 666

enum class EConfirmationErrorCode : short
{
	Zero = ERROR_CODE_FIRST,
	Count = ERROR_CODE_LAST
};
class CNetMessage_Confirmation : public CNetMessage
{
public:
	CNetMessage_Confirmation();
	virtual ~CNetMessage_Confirmation();
	void Init(EConfirmationStatus aStatus, int aClientID);
	const EConfirmationStatus& GetStatus() { return myStatus; }
	short GetMessageID() { return myClientID; }
protected:
	EConfirmationStatus myStatus;
	short myClientID;
	virtual void DoSerialize(StreamType& aStreamType) override;
	virtual void DoDeSerialize(StreamType& aStreamType) override;
};

