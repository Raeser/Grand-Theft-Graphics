#include "stdafx.h"
#include "Client.h"
#include "..\TShared\NetMessage_Connect.h"


CClient::CClient()
{

}


CClient::~CClient()
{
	myIsRunning = false;
	if (myInputThread.joinable())
	{
		myInputThread.join();
	}
}

bool CClient::Init()
{
	if (!myNetworkManager.Init())
	{
		PRINT_ERROR("FAILED to start NetworkManager\n");
		return false;
	}
	myInput = "";
	recievedMessageLength = 0;
	sizeOfaddr = sizeof(myNetworkManager.getSockAddr());
	myInputThread = std::thread(&CClient::InputThreadFunction, this);
	return true;
}

void CClient::Update()
{
	if (myUsedInput == false && myInput.size() > 0)
	{
		myNetworkManager.SendChatMessage(myInput);
		myInput = "";
		myUsedInput = true;
	}
	recievedMessageLength = recvfrom(myNetworkManager.GetSocket(), myNetworkManager.GetBuffer(), BUFLEN, 0, (sockaddr*)&myNetworkManager.getSockAddr(), &sizeOfaddr);
	if (recievedMessageLength > 0)
	{
		ENetMessage msgType = (ENetMessage)myNetworkManager.GetBuffer()[0];
		if (msgType == ENetMessage::Chat)
		{
			CNetMessageChatMessage message;
			message.UnPackMessage(myNetworkManager.GetBuffer(), recievedMessageLength);
			std::cout << message.GetChatMessage() << std::endl;
		}
		if (msgType == ENetMessage::Confirmation)
		{
		}
		if (msgType == ENetMessage::Connect)
		{
			CNetMessage_Connect message;
			message.UnPackMessage(myNetworkManager.GetBuffer(), recievedMessageLength);
			if (message.GetConnectStatus() == EConnectStatus::Ping)
			{
				myNetworkManager.SendPing();
			}
		}
	}



}

void CClient::Shutdown()
{
	myNetworkManager.Cleanup();
}

void CClient::InputThreadFunction()
{
	while (myIsRunning)
	{
		std::getline(std::cin, myInput);
		if (myInput.size() > 0)
		{
			myUsedInput = false;
		}
		while (myUsedInput == false)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(16));
		}
	}
}
