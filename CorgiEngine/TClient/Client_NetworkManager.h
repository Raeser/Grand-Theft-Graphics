#pragma once
#include "..\TShared\Shared_NetworkManager.h"
class CClient_NetworkManager :
	public CShared_NetworkManager
{
public:
	CClient_NetworkManager();
	~CClient_NetworkManager();
	bool Init();
	bool SendHandShakeMessage(std::string& aName);
	bool SendChatMessage(std::string& aMsg);

	bool SendPing();

	void Cleanup() override;
private:
	std::string myConnectedServerIP;
	unsigned short myID;
};

