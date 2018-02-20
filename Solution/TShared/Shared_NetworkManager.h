#pragma once
#include "../TShared/CommonNetworkIncludes.h"
#include "../TShared/CommonErrorHandling.h"
#include <string>
#include <vector>

#define BUFLEN 512
#define PORT 6666

class CShared_NetworkManager
{
public:
	CShared_NetworkManager();
	virtual ~CShared_NetworkManager();
	virtual bool Init();

	sockaddr_in& getSockAddr() { return mySocketAddress; }
	SOCKET& GetSocket() { return mySocket; }
	char* GetBuffer() { return myBuffer; }

	virtual void Cleanup();
protected:
	SOCKET mySocket;
	sockaddr_in mySocketAddress;
	WSADATA myWSAData;
	char myBuffer[BUFLEN];
};

