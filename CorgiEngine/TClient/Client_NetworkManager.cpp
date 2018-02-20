#include "stdafx.h"
#include "Client_NetworkManager.h"
#include "..\TShared\NetMessage_Connect.h"


CClient_NetworkManager::CClient_NetworkManager()
{
}


CClient_NetworkManager::~CClient_NetworkManager()
{
}

bool CClient_NetworkManager::Init()
{
	CShared_NetworkManager::Init();


	myConnectedServerIP = "127.0.0.1";
	

	std::cout << "\nConnecting to: " << myConnectedServerIP << std::endl;
	mySocketAddress.sin_family = AF_INET;
	mySocketAddress.sin_port = htons(PORT);
	inet_pton(AF_INET, myConnectedServerIP.c_str(), &(mySocketAddress.sin_addr));

	myHasConnected = false;
	myConnectionTimer = 1.0f;

	u_long nonBlock = 1;
	if (ioctlsocket(mySocket, FIONBIO, &nonBlock) == SOCKET_ERROR)
	{
		PRINT_ERROR("Failed To Set Non-Blocking on client socket\n");
		return false;
	}
	PRINT_ERROR("Succeded on Set Non-Blocking on client socket\n");


	if (!SendHandShakeMessage())
	{
		return false;
	}
	return true;
}

bool CClient_NetworkManager::SendHandShakeMessage()
{

	CNetMessage_Connect handShakeMessage;
	handShakeMessage.Init(EConnectStatus::Connect);
	handShakeMessage.PackMessage();

	if (sendto(mySocket, &handShakeMessage.myStream[0], (int)handShakeMessage.myStream.size(), 0, (sockaddr*)&mySocketAddress, sizeof(sockaddr_in)) == SOCKET_ERROR)
	{
		PRINT_ERROR("Failed to send Handshake message\n");
		return false;
	}

	//ZeroMemory(myBuffer, sizeof(myBuffer));


	return true;
}

bool CClient_NetworkManager::SendChatMessage(std::string & aMsg)
{
	CNetMessageChatMessage chatMessage;
	chatMessage.Init(aMsg);
	chatMessage.SetSenderID(myID);
	chatMessage.PackMessage();

	if (sendto(mySocket, &chatMessage.myStream[0], (int)chatMessage.myStream.size(), 0, (sockaddr*)&mySocketAddress, sizeof(sockaddr_in)) == SOCKET_ERROR)
	{
		PRINT_ERROR("Failed to send chat message");
		return false;
	}
	return true;
}

bool CClient_NetworkManager::PingServer()
{
	CNetMessage_Connect pingmess;
	pingmess.Init(EConnectStatus::Ping_ToServer);
	pingmess.SetSenderID(myID);
	pingmess.PackMessage();

	if (sendto(mySocket, &pingmess.myStream[0], (int)pingmess.myStream.size(), 0, (sockaddr*)&mySocketAddress, sizeof(sockaddr_in)) == SOCKET_ERROR)
	{
		PRINT_ERROR("Failed to send chat message");
		return false;
	}
	return true;
}

void CClient_NetworkManager::Cleanup()
{
	CNetMessage_Connect disconnectMessage;
	disconnectMessage.Init(EConnectStatus::Disconnect);
	disconnectMessage.SetSenderID(myID);
	disconnectMessage.PackMessage();

	if (sendto(mySocket, &disconnectMessage.myStream[0], (int)disconnectMessage.myStream.size(), 0, (sockaddr*)&mySocketAddress, sizeof(sockaddr_in)) == SOCKET_ERROR)
	{
		PRINT_ERROR("Failed to send Disconnect message\n");
	}

	CShared_NetworkManager::Cleanup();
}

void CClient_NetworkManager::Update()
{
	myTimer.Update();
	if (!myHasConnected)
	{
		myConnectionTimer -= myTimer.GetDeltaTime();
		if (myConnectionTimer <= 0)
		{
			std::cout << "Couldn't connect to server. Trying again!" << std::endl;
			SendHandShakeMessage();
			myConnectionTimer = 1.0f;
		}
	}


}

void CClient_NetworkManager::SetID(short id)
{
	myID = id;
}

void CClient_NetworkManager::SetConnectionStatus(bool aStatus)
{
	myHasConnected = aStatus;
}
