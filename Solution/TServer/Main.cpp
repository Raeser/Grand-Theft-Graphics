#include "stdafx.h"
#include <chrono>
#include <thread>
#include "ServerMain.h"

//SERVER
int main()
{
	system("Title Server");
	CServerMain Server;
	Server.StartServer();

	while (true)
	{
		Server.UpdateServer();
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	Server.ShutDownServer();
	return 0;
}