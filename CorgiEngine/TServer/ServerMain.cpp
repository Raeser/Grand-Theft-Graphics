#include "stdafx.h"
#include "ServerMain.h"
#include "..\Utilities\Timer.h"
#include "..\TShared\NetMessage_Connect.h"


CServerMain::CServerMain()
{
}


CServerMain::~CServerMain()
{
}

void CServerMain::StartServer()
{
	myTimer = 5.f;
	myPingCounter = 5;
	myNetworkManager.Init();
	mySocketLength = sizeof(myClient);
	std::cout << "Listening for new clients" << std::endl;
}

void CServerMain::UpdateServer()
{

	Timer::GetInstance().Update();
	myRecievedMessageLength = recvfrom(myNetworkManager.GetSocket(), myNetworkManager.GetBuffer(), BUFLEN, 0, (sockaddr*)&myClient, &mySocketLength);
	if (myRecievedMessageLength > 0)
	{
		ENetMessage msgType = (ENetMessage)myNetworkManager.GetBuffer()[0];
		if (msgType == ENetMessage::Confirmation)
		{

		}
		else if (msgType == ENetMessage::Connection)
		{
			CNetMessage_Connect message;
			message.UnPackMessage(myNetworkManager.GetBuffer(), myRecievedMessageLength);
			EConnectStatus messageStatus = message.GetConnectStatus();
			if (messageStatus == EConnectStatus::Connect)
			{
				short new_id = myNetworkManager.AddClient(myClient);

				CNetMessage_Confirmation confirmation_message;
				confirmation_message.Init(EConfirmationStatus::Connection_Established, new_id);
				confirmation_message.SetTargetID(new_id);

				confirmation_message.PackMessage();
				myNetworkManager.SendMessageToTarget(&confirmation_message);

			}
			else if (messageStatus == EConnectStatus::Disconnect)
			{
				myNetworkManager.RemoveClient(message.GetSenderID());
			}
			if (messageStatus == EConnectStatus::Ping_ToServer)
			{
				int it = 0;
				for (unsigned short s : myClientsToRemoveIfOld)
				{
					if (s == message.GetSenderID())
					{
						myClientsToRemoveIfOld.erase(myClientsToRemoveIfOld.begin() + it);
						break;
					}
					++it;
				}
			}
		}
		else if (msgType == ENetMessage::Chat)
		{
			CNetMessageChatMessage message;
			message.UnPackMessage(myNetworkManager.GetBuffer(), myRecievedMessageLength);
			if (message.GetTargetID() == 0)
			{
				std::string output = std::to_string(message.GetSenderID()) + ": " + message.GetChatMessage();
				message.SetMessage(output);
				std::cout << output << std::endl;
				message.PackMessage();
				myNetworkManager.SendMessageAll(&message);
			}
		}
	}


	myTimer -= Timer::GetInstance().GetDeltaTime();
	if (myTimer <= 0)
	{
		--myPingCounter;
		if (myPingCounter <= 0)
		{
			myNetworkManager.RemoveOldClients(myClientsToRemoveIfOld);
			myPingCounter = 5;
		}
		else
		{
			myNetworkManager.PingAllClients();
		}
		myTimer = 0.2f;
	}

}

void CServerMain::ShutDownServer()
{
	myNetworkManager.Cleanup();
}
