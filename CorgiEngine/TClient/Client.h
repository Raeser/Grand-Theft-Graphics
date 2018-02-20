#pragma once
#include "Client_NetworkManager.h"
#include <mutex>
class CClient
{
public:
	CClient();
	~CClient();
	bool Init();
	void Update();
	void Shutdown();

private:

	bool myIsConnectedToServer;
	int myRecievedMessageLength;
	int mySizeOfaddr;
	CClient_NetworkManager myNetworkManager;
};

