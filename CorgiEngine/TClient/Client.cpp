#include "stdafx.h"
#include "Client.h"
#include "..\TShared\NetMessage_Connect.h"


CClient::CClient()
{

}


CClient::~CClient()
{
}

bool CClient::Init()
{
	myIsConnectedToServer = false;
	if (!myNetworkManager.Init())
	{
		PRINT_ERROR("FAILED to start NetworkManager\n");
		return false;
	}
	myRecievedMessageLength = 0;
	mySizeOfaddr = sizeof(myNetworkManager.getSockAddr());
	return true;
}

void CClient::Update()
{

	myRecievedMessageLength = recvfrom(myNetworkManager.GetSocket(), myNetworkManager.GetBuffer(), BUFLEN, 0, (sockaddr*)&myNetworkManager.getSockAddr(), &mySizeOfaddr);
	if (myRecievedMessageLength > 0)
	{
		ENetMessage msgType = (ENetMessage)myNetworkManager.GetBuffer()[0];


		if (CheckConnectionToServer(msgType))
		{
			if (msgType == ENetMessage::Chat)
			{
				CNetMessageChatMessage message;
				message.UnPackMessage(myNetworkManager.GetBuffer(), myRecievedMessageLength);
				std::cout << message.GetChatMessage() << std::endl;
			}

			if (msgType == ENetMessage::Connection)
			{
				CNetMessage_Connect message;
				message.UnPackMessage(myNetworkManager.GetBuffer(), myRecievedMessageLength);
				if (message.GetConnectStatus() == EConnectStatus::Ping_ToClient)
				{
					myNetworkManager.PingServer();
				}
			}
		}
	}


	myNetworkManager.Update();
}

void CClient::Shutdown()
{
	myNetworkManager.Cleanup();
}

bool CClient::CheckConnectionToServer(ENetMessage aMessageType)
{
	if (aMessageType == ENetMessage::Confirmation)
	{
		CNetMessage_Confirmation message;
		message.UnPackMessage(myNetworkManager.GetBuffer(), myRecievedMessageLength);
		if (message.GetStatus() == EConfirmationStatus::Connection_Established)
		{
			short id = message.GetTargetID();
			myNetworkManager.SetID(id);
			std::cout << "Connection To Server Successfully! My ID is: " << id << std::endl;
			myNetworkID = id;
			myNetworkManager.SetConnectionStatus(true);
			myIsConnectedToServer = true;
		}
		else
		{
			PRINT_ERROR("ERROR");
		}
	}

	return myIsConnectedToServer;
}
