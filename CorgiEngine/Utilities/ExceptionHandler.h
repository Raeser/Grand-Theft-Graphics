#pragma once
#pragma warning( disable : 4091 4996 4244)

#include <windows.h>
#include <dbghelp.h>

#pragma comment(lib, "dbghelp.lib")



class CEngine;

class ExceptionHandler
{
public:

	static ExceptionHandler& GetInstance()
	{
		static ExceptionHandler me;
		return me;
	}

	~ExceptionHandler();

	

	LONG WINAPI ExceptionFilterFunction(_EXCEPTION_POINTERS* aExceptionP);

	void InitCPUException();

	double GetCPUUsage();
	SIZE_T GetMemUsage();
	SIZE_T GetFreeMem();
	std::string GetTime();
	std::string GetDate();

	void AssignEngine(CEngine* aEngine);

private:

	ExceptionHandler();

	std::string CreateMiniDump(EXCEPTION_POINTERS* aExceptionPointers);
	void CreateTextFile(const std::string& aPath);
	int CreateCrashMessageBox(const std::string& aPath);
	static bool IsDataSectionNeeded(const WCHAR* pModuleName);

	void CopyLogFiles(const std::string& aPath);
	static BOOL CALLBACK MiniDumpCallback(
		PVOID                            pParam,
		const PMINIDUMP_CALLBACK_INPUT   pInput,
		PMINIDUMP_CALLBACK_OUTPUT        pOutput
	);

	ULARGE_INTEGER myLastCPU, myLastSysCPU, myLastUserCPU;
	int myNumProcessors;
	HANDLE self;


	CEngine* myEngine;
};



