#pragma once
#include "..\TShared\Shared_NetworkManager.h"


class CNetMessage_Connect;

class CServer_NetworkManager :
	public CShared_NetworkManager
{
public:
	CServer_NetworkManager();
	~CServer_NetworkManager();
	bool Init();
	short AddClient(sockaddr_in& aSocketAddress);
	bool RemoveClient(unsigned short aID);
	bool SendMessageAll(const CNetMessage* aSerializedMessage);
	bool SendMessageToTarget(const CNetMessage* aSerializedMessage);

	//std::string& GetClientName(unsigned int aID);


	void RemoveOldClients(std::vector<unsigned short>& oldPings);
	void PingAllClients();
private:

	struct SClientData 
	{
		short id;
		sockaddr_in clientSocketAddress;
	};
	std::vector<SClientData> myClients;
};

