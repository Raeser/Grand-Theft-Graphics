#pragma once
#include "Client_NetworkManager.h"
#include <mutex>

enum class ENetMessage : char;

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

	short myNetworkID;

	bool CheckConnectionToServer(ENetMessage aMessageType);
};

