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
	void InputThreadFunction();

	int recievedMessageLength;
	int sizeOfaddr;
	std::string myInput;
	CClient_NetworkManager myNetworkManager;
	bool myIsRunning;
	bool myUsedInput;
	std::thread myInputThread;
};

