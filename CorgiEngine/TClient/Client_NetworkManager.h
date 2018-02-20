#pragma once
#include "..\TShared\Shared_NetworkManager.h"
#include "..\Utilities\Timer.h"
class CClient_NetworkManager :
	public CShared_NetworkManager
{
public:
	CClient_NetworkManager();
	~CClient_NetworkManager();
	bool Init();
	bool SendHandShakeMessage();
	bool SendChatMessage(std::string& aMsg);
	bool PingServer();
	void Cleanup() override;

	void Update();


	void SetID(short id);
	void SetConnectionStatus(bool aStatus);
private:
	std::string myConnectedServerIP;
	unsigned short myID;

	bool myHasConnected;
	float myConnectionTimer;
	Timer myTimer;
};

