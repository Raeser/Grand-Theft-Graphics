#include "stdafx.h"
#include "Server_NetworkManager.h"
#include "../TShared/NetMessageConfirmation.h"
#include "../TShared/NetMessage_Connect.h"

CServer_NetworkManager::CServer_NetworkManager()
{
}


CServer_NetworkManager::~CServer_NetworkManager()
{
}

bool CServer_NetworkManager::Init()
{
	CShared_NetworkManager::Init();

	mySocketAddress.sin_family = AF_INET;
	mySocketAddress.sin_addr.s_addr = INADDR_ANY;
	mySocketAddress.sin_port = htons(PORT);

	if (bind(mySocket, (sockaddr*)&mySocketAddress, sizeof(mySocketAddress)) == SOCKET_ERROR)
	{
		PRINT_ERROR("Binding of server socket failed");
		return false;
	}
	PRINT_ERROR("Bind Done \n");
	u_long nonBlock = 1;
	if (ioctlsocket(mySocket, FIONBIO, &nonBlock) == SOCKET_ERROR)
	{
		PRINT_ERROR("Failed To Set Non-Blocking on server socket\n");
		return false;
	}
	PRINT_ERROR("Succeded on Set Non-Blocking on server socket\n");

	return true;
}

bool CServer_NetworkManager::AddClient(sockaddr_in & aSocketAddress)
{
	static short client_counter = 667;
	SClientData newClient;
	newClient.clientSocketAddress = aSocketAddress;
	newClient.id = client_counter;
	++client_counter;
	myClients.push_back(newClient);

	CNetMessage_Confirmation clientConfirmation;
	clientConfirmation.Init(EConfirmationStatus::Success, newClient.id);
	clientConfirmation.SetSenderID(0);
	clientConfirmation.SetTargetID(newClient.id);

	clientConfirmation.PackMessage();
	SendMessageToTarget(&clientConfirmation);

	std::cout << newClient.id << " joined The Server" << std::endl;

	CNetMessageChatMessage connectionMessage;
	connectionMessage.Init(newClient.id + " joined The Server");
	connectionMessage.SetSenderID(newClient.id);
	connectionMessage.SetTargetID(0);

	connectionMessage.PackMessage();
	SendMessageAll(&connectionMessage);

	return true;
}

bool CServer_NetworkManager::RemoveClient(unsigned short aID)
{

	int size = (int)myClients.size();
	for (int i = 0; i < size; ++i)
	{
		if (myClients[i].id == aID)
		{
			std::cout << myClients[i].id << " left the server" << std::endl;


			CNetMessageChatMessage connectionMessage;
			connectionMessage.Init(myClients[i].id + " left the server");
			connectionMessage.SetSenderID(myClients[i].id);
			connectionMessage.SetTargetID(0);
			myClients.erase(myClients.begin() + i);

			connectionMessage.PackMessage();
			SendMessageAll(&connectionMessage);

			return true;
		}
	}
	return false;
}

bool CServer_NetworkManager::SendMessageAll(const CNetMessage* aSerializedMessage)
{
	for (SClientData& client : myClients)
	{
		if (client.id != aSerializedMessage->GetSenderID())
		{
			if (sendto(mySocket, &aSerializedMessage->myStream[0], (int)aSerializedMessage->myStream.size(), 0, (sockaddr*)&client.clientSocketAddress, sizeof(sockaddr_in)) == SOCKET_ERROR)
			{
				PRINT_ERROR("Failed to send chat message");
				return false;
			}
		}
	}
	return true;
}

bool CServer_NetworkManager::SendMessageToTarget(const CNetMessage * aSerializedMessage)
{
	short target = aSerializedMessage->GetTargetID();
	for (SClientData& client : myClients)
	{
		if (client.id == target)
		{
			if (sendto(mySocket, &aSerializedMessage->myStream[0], (int)aSerializedMessage->myStream.size(), 0, (sockaddr*)&client.clientSocketAddress, sizeof(sockaddr_in)) == SOCKET_ERROR)
			{
				PRINT_ERROR("Failed to send chat message");
				return false;
			}
			return true;
		}

	}
	return false;
}


void CServer_NetworkManager::RemoveOldClients(std::vector<unsigned short>& oldPings)
{
	for (unsigned short s : oldPings)
	{
		for (SClientData& client : myClients)
		{
			if (s == client.id)
			{
				RemoveClient(s);
				break;
			}
		}
	}


	oldPings.clear();


	for (SClientData& client : myClients)
	{
		CNetMessage_Connect ping;
		ping.Init(EConnectStatus::Ping);
		ping.PackMessage();
		ping.SetTargetID(client.id);

		SendMessageToTarget(&ping);

		oldPings.push_back(client.id);
	}

}

void CServer_NetworkManager::PingAllClients()
{
	for (SClientData& client : myClients)
	{
		CNetMessage_Connect ping;
		ping.Init(EConnectStatus::Ping);
		ping.PackMessage();
		ping.SetTargetID(client.id);

		SendMessageToTarget(&ping);
	}
}
