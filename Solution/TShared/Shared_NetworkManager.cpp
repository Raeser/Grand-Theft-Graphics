#include "stdafx.h"
#include "Shared_NetworkManager.h"


CShared_NetworkManager::CShared_NetworkManager()
{
}


CShared_NetworkManager::~CShared_NetworkManager()
{
}

bool CShared_NetworkManager::Init()
{
	ZeroMemory(&mySocketAddress, sizeof(mySocketAddress));

	PRINT_ERROR("Initializing Winsock...");

	if (WSAStartup(MAKEWORD(2, 2), &myWSAData) != 0)
	{
		PRINT_ERROR(" Failed. Error Code");
		PRINT_ERROR(WSAGetLastError());
		return false;
	}
	PRINT_ERROR(" INTIALIZATION SUCCESS!\n");

	if ((mySocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR)
	{
		PRINT_ERROR("Socket initialization failed!");
		PRINT_ERROR(WSAGetLastError());	
		return false;
	}
	return true;
}



void CShared_NetworkManager::Cleanup()
{
	closesocket(mySocket);
	WSACleanup();
}


