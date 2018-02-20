#pragma once
#include <CommonNetworkIncludes.h>
#include "Server_NetworkManager.h"
#include <chrono>

class CServerMain
{
public:
	CServerMain();
	~CServerMain();
	void StartServer();
	void UpdateServer();
	void ShutDownServer();

private:
	CServer_NetworkManager myNetworkManager;
	sockaddr_in myClient;
	int myRecievedMessageLength;
	int mySocketLength;

	float myTimer;
	int myPingCounter;
	std::vector<unsigned short> myClientsToRemoveIfOld;
};

