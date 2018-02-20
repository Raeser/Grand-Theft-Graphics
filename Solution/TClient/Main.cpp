#include "stdafx.h"
#include "Client.h"
//CLIENT
int main()
{
	system("Title Client");
	CClient myClient;
	bool myShouldRun = myClient.Init();

	while (myShouldRun)
	{
		//MSG msg = { 0 };
		//while (PeekMessage(&msg, 0, 0, 0, WM_DESTROY))
		//{
		//	TranslateMessage(&msg);
		//	DispatchMessage(&msg);

		//	switch (msg.message)
		//	{
		//	case WM_CLOSE:
		//		myShouldRun = false;
		//		break;
		//	case WM_QUIT:
		//		myShouldRun = false;
		//		break;
		//	default:
		//		break;
		//	}
		//}
		myClient.Update();

	}
	myClient.Shutdown();
	return 0;
}