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

	std::cout << "Input Server IP to connect to: ";
	std::getline(std::cin, myConnectedServerIP);

	if (myConnectedServerIP.size() < 1)
	{
		myConnectedServerIP = "127.0.0.1";
	}

	std::cout << "\nConnecting to: " << myConnectedServerIP << std::endl;
	mySocketAddress.sin_family = AF_INET;
	mySocketAddress.sin_port = htons(PORT);
	inet_pton(AF_INET, myConnectedServerIP.c_str(), &(mySocketAddress.sin_addr));

	std::cout << "Enter Username: ";
	std::string name;
	std::getline(std::cin, name);

	if (name.size() < 1)
	{
		name = "Ralle";
	}
	std::cout << "Hello " << name << std::endl;
	if (!SendHandShakeMessage(name))
	{
		return false;
	}

	u_long nonBlock = 1;
	if (ioctlsocket(mySocket, FIONBIO, &nonBlock) == SOCKET_ERROR)
	{
		PRINT_ERROR("Failed To Set Non-Blocking on client socket\n");
		return false;
	}
	PRINT_ERROR("Succeded on Set Non-Blocking on client socket\n");


	return true;
}

bool CClient_NetworkManager::SendHandShakeMessage(std::string& aName)
{

	CNetMessage_Connect handShakeMessage;
	handShakeMessage.Init(EConnectStatus::Connect);
	handShakeMessage.PackMessage();

	if (sendto(mySocket, &handShakeMessage.myStream[0], handShakeMessage.myStream.size(), 0, (sockaddr*)&mySocketAddress, sizeof(sockaddr_in)) == SOCKET_ERROR)
	{
		PRINT_ERROR("Failed to send Handshake message\n");
		return false;
	}

	ZeroMemory(myBuffer, sizeof(myBuffer));
	int socketSize = sizeof(mySocketAddress);
	int messageSize = 0;

	if ((messageSize = recvfrom(mySocket, myBuffer, BUFLEN, 0, (sockaddr*)&mySocketAddress, &socketSize)) == SOCKET_ERROR)
	{
		PRINT_ERROR("recvfrom() failed to receive confirmation Message\n");
		return false;
	}
	
	if ((ENetMessage)myBuffer[0] == ENetMessage::Confirmation)
	{
		CNetMessage_Confirmation message;
		message.UnPackMessage(myBuffer, messageSize);
		if (message.GetStatus() == EConfirmationStatus::Success)
		{
			myID = message.GetMessageID();
			std::cout << "Connection To Server Successfully! My ID is: " << myID << std::endl;
		}
		else
		{
			PRINT_ERROR("ERROR");
		}
	}


	return true;
}

bool CClient_NetworkManager::SendChatMessage(std::string & aMsg)
{
	CNetMessageChatMessage chatMessage;
	chatMessage.Init(aMsg);
	chatMessage.SetSenderID(myID);
	chatMessage.PackMessage();

	if (sendto(mySocket, &chatMessage.myStream[0], chatMessage.myStream.size(), 0, (sockaddr*)&mySocketAddress, sizeof(sockaddr_in)) == SOCKET_ERROR)
	{
		PRINT_ERROR("Failed to send chat message");
		return false;
	}
	return true;
}

bool CClient_NetworkManager::SendPing()
{
	CNetMessage_Connect pingmess;
	pingmess.Init(EConnectStatus::Ping);
	pingmess.SetSenderID(myID);
	pingmess.PackMessage();

	if (sendto(mySocket, &pingmess.myStream[0], pingmess.myStream.size(), 0, (sockaddr*)&mySocketAddress, sizeof(sockaddr_in)) == SOCKET_ERROR)
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

	if (sendto(mySocket, &disconnectMessage.myStream[0], disconnectMessage.myStream.size(), 0, (sockaddr*)&mySocketAddress, sizeof(sockaddr_in)) == SOCKET_ERROR)
	{
		PRINT_ERROR("Failed to send Disconnect message\n");
	}

	CShared_NetworkManager::Cleanup();
}
