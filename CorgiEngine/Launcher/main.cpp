#include "stdafx.h"
#include "CorgiEngineLauncher.h"


#ifdef _RETAIL
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
	hInstance; hPrevInstance; lpCmdLine; nShowCmd;
	return CorgiEngineLauncher::StartNewEngine(0, nullptr);
}
#else
int main(int argc, char* argv[])
{
	return CorgiEngineLauncher::StartNewEngine(argc, argv);
}
#endif