#include "stdafx.h"
#include "ExceptionHandler.h"
#include "..\Engine\Engine.h"

#include <tchar.h>
#include <Lmcons.h>
#include "psapi.h"

ExceptionHandler::ExceptionHandler()
{
}


ExceptionHandler::~ExceptionHandler()
{
}


LONG WINAPI ExceptionHandler::ExceptionFilterFunction(_EXCEPTION_POINTERS* aExceptionP)
{
	std::string path = CreateMiniDump(aExceptionP);
	CreateTextFile(path);
	CreateCrashMessageBox(path);
	if (myEngine)
	{
		myEngine->Screendump(path);
	}
	CopyLogFiles(path);
	return EXCEPTION_EXECUTE_HANDLER;
}


std::string ExceptionHandler::CreateMiniDump(EXCEPTION_POINTERS* aExceptionPointers)
{
	// Open the file 


	CreateDirectory(L"Crashes", NULL);

	int crashCount = 0;
	std::stringstream directoryname;
	directoryname << "Crashes\\Crash" << crashCount;


	do
	{
		++crashCount;
		directoryname.str("");
		directoryname << "Crashes\\Crash_" << crashCount;
		int errorCode = CreateDirectory(Corgi::ConvertStringToWide(directoryname.str()).c_str(), NULL);
		if (errorCode != 0)
		{
			break;
		}

	
	} while (GetLastError() == ERROR_ALREADY_EXISTS);


	std::stringstream dumpFile;
	dumpFile << directoryname.rdbuf();
	dumpFile << "\\MiniDump.dmp";
	HANDLE hFile = CreateFile((Corgi::ConvertStringToWide(dumpFile.str()).c_str()), GENERIC_READ | GENERIC_WRITE,
		0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if ((hFile != NULL) && (hFile != INVALID_HANDLE_VALUE))
	{
		// Create the minidump 

		MINIDUMP_EXCEPTION_INFORMATION mdei;

		mdei.ThreadId = GetCurrentThreadId();
		mdei.ExceptionPointers = aExceptionPointers;
		mdei.ClientPointers = FALSE;

		MINIDUMP_CALLBACK_INFORMATION mci;

		mci.CallbackRoutine = (MINIDUMP_CALLBACK_ROUTINE)MiniDumpCallback;
		mci.CallbackParam = 0;

		MINIDUMP_TYPE mdt = (MINIDUMP_TYPE)(MiniDumpWithPrivateReadWriteMemory |
			MiniDumpWithDataSegs |
			MiniDumpWithHandleData |
			MiniDumpWithFullMemoryInfo |
			MiniDumpWithThreadInfo |
			MiniDumpWithUnloadedModules);

		BOOL rv = MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(),
			hFile, mdt, (aExceptionPointers != 0) ? &mdei : 0, 0, &mci);

		if (!rv)
			_tprintf(_T("MiniDumpWriteDump failed. Error: %u \n"), GetLastError());
		else
			_tprintf(_T("Minidump created.\n"));

		// Close the file 

		CloseHandle(hFile);

	}
	else
	{
		_tprintf(_T("CreateFile failed. Error: %u \n"), GetLastError());
	}

	return directoryname.str();
}

inline void ExceptionHandler::CreateTextFile(const std::string & aPath)
{
	std::stringstream ss;
	ss << aPath << "\\CrashInformation.txt";

	WCHAR username[UNLEN + 1];
	DWORD username_len = UNLEN + 1;
	GetUserName(username, &username_len);

	std::ofstream file(ss.str());


	file << "*****************************************************************\n";

	file << "[USER]: " << username << "\n";
	file << "[DATE]: " << GetDate() << "\n";
	file << "[TIME]: " << GetTime() << "\n";
	file << "[VERSION]: " << GetVersion() << "\n";
	file << "[CPU]: " << GetCPUUsage() << "%" << "\n";
	file << "[MEMORY]: " << GetMemUsage() / 1000000.0 << "Mb" << "\n";
	file << "[FREE MEMORY]: " << GetFreeMem() / 1000000.0 << "Mb" << "\n";

	file << "[MEMERY]: " << "Pepe XDD" << "\n";

	file << "*****************************************************************\n";

	file.close();
}

int ExceptionHandler::CreateCrashMessageBox(const std::string& aPath)
{

	std::stringstream ss;
	ss << "A minidump was created at \"" << aPath << "\\MiniDump.dmp\". Please tell a programmer or an adult!";
	MessageBox(
		NULL,
		Corgi::ConvertStringToWide(ss.str()).c_str(),
		L"Program crashed! ",
		MB_ICONEXCLAMATION | MB_OK
	);



	return 0;
}

BOOL CALLBACK ExceptionHandler::MiniDumpCallback(
	PVOID                            pParam,
	const PMINIDUMP_CALLBACK_INPUT   pInput,
	PMINIDUMP_CALLBACK_OUTPUT        pOutput
)
{
	BOOL bRet = FALSE;
	pParam;

	// Check parameters 

	if (pInput == 0)
		return FALSE;

	if (pOutput == 0)
		return FALSE;


	// Process the callbacks 

	switch (pInput->CallbackType)
	{
	case IncludeModuleCallback:
	{
		// Include the module into the dump 
		bRet = TRUE;
	}
	break;

	case IncludeThreadCallback:
	{
		// Include the thread into the dump 
		bRet = TRUE;
	}
	break;

	case ModuleCallback:
	{
		// Are data sections available for this module ? 

		if (pOutput->ModuleWriteFlags & ModuleWriteDataSeg)
		{
			// Yes, they are, but do we need them? 

			if (!IsDataSectionNeeded(pInput->Module.FullPath))
			{
				wprintf(L"Excluding module data sections: %s \n", pInput->Module.FullPath);

				pOutput->ModuleWriteFlags &= (~ModuleWriteDataSeg);
			}
		}

		bRet = TRUE;
	}
	break;

	case ThreadCallback:
	{
		// Include all thread information into the minidump 
		bRet = TRUE;
	}
	break;

	case ThreadExCallback:
	{
		// Include this information 
		bRet = TRUE;
	}
	break;

	case MemoryCallback:
	{
		// We do not include any information here -> return FALSE 
		bRet = FALSE;
	}
	break;

	case CancelCallback:
		break;
	}

	return bRet;

}

void ExceptionHandler::InitCPUException() {
	SYSTEM_INFO sysInfo;
	FILETIME ftime, fsys, fuser;

	GetSystemInfo(&sysInfo);
	myNumProcessors = sysInfo.dwNumberOfProcessors;

	GetSystemTimeAsFileTime(&ftime);
	memcpy(&myLastCPU, &ftime, sizeof(FILETIME));

	self = GetCurrentProcess();
	GetProcessTimes(self, &ftime, &ftime, &fsys, &fuser);
	memcpy(&myLastSysCPU, &fsys, sizeof(FILETIME));
	memcpy(&myLastUserCPU, &fuser, sizeof(FILETIME));
}

void ExceptionHandler::CopyLogFiles(const std::string& aPath)
{


	Corgi::GrowingArray<std::string> logs = DebugLog::GetInstance().GetActiveLogs();

	for (int i = 0; i < logs.Size(); ++i)
	{


		std::string inputfile = logs[i] + "_log.txt";
		std::stringstream outputFile;
		outputFile << aPath.c_str() << "\\" << logs[i] << "_log.txt";


		std::string TEMPDATA;
		std::string OUTPUTDATA;

		std::ifstream reader(inputfile);

		while (getline(reader, TEMPDATA))
		{
			OUTPUTDATA += TEMPDATA + "\n";
		}

		reader.close();

		std::ofstream writer(outputFile.str());
		writer << OUTPUTDATA;
		writer.close();
	}


}

inline double ExceptionHandler::GetCPUUsage()
{
	FILETIME ftime, fsys, fuser;
	ULARGE_INTEGER now, sys, user;
	double percent;

	GetSystemTimeAsFileTime(&ftime);
	memcpy(&now, &ftime, sizeof(FILETIME));

	GetProcessTimes(self, &ftime, &ftime, &fsys, &fuser);
	memcpy(&sys, &fsys, sizeof(FILETIME));
	memcpy(&user, &fuser, sizeof(FILETIME));
	percent = (sys.QuadPart - myLastSysCPU.QuadPart) +
		(user.QuadPart - myLastUserCPU.QuadPart);
	percent /= (now.QuadPart - myLastCPU.QuadPart);
	percent /= myNumProcessors;
	myLastCPU = now;
	myLastUserCPU = user;
	myLastSysCPU = sys;
	return percent * 100;

}

inline SIZE_T ExceptionHandler::GetMemUsage()
{
	MEMORYSTATUSEX memInfo;
	memInfo.dwLength = sizeof(MEMORYSTATUSEX);
	GlobalMemoryStatusEx(&memInfo);
	//DWORDLONG totalVirtualMem = memInfo.ullTotalPageFile;
	//DWORDLONG virtualMemUsed = memInfo.ullTotalPageFile - memInfo.ullAvailPageFile;

	PROCESS_MEMORY_COUNTERS pmc;
	GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));

	SIZE_T physMemUsedByMe = pmc.WorkingSetSize;

	return physMemUsedByMe;
}

SIZE_T ExceptionHandler::GetFreeMem()
{
	MEMORYSTATUSEX memInfo;
	memInfo.dwLength = sizeof(MEMORYSTATUSEX);
	GlobalMemoryStatusEx(&memInfo);
	DWORDLONG physMemUsed = memInfo.ullTotalPhys - memInfo.ullAvailPhys;
	DWORDLONG totalPhysMem = memInfo.ullTotalPhys;
	return totalPhysMem - physMemUsed;
}


bool ExceptionHandler::IsDataSectionNeeded(const WCHAR* pModuleName)
{
	// Check parameters 

	if (pModuleName == 0)
	{
		_ASSERTE(_T("Parameter is null."));
		return false;
	}


	// Extract the module name 

	WCHAR szFileName[_MAX_FNAME] = L"";

	_wsplitpath(pModuleName, NULL, NULL, szFileName, NULL);


	// Compare the name with the list of known names and decide 

	// Note: For this to work, the executable name must be "mididump.exe"
	if (wcsicmp(szFileName, L"mididump") == 0)
	{
		return true;
	}
	else if (wcsicmp(szFileName, L"ntdll") == 0)
	{
		return true;
	}


	// Complete 

	return false;

}


std::string ExceptionHandler::GetTime()
{

	auto now = std::chrono::system_clock::now();

	std::time_t t = std::chrono::system_clock::to_time_t(now);

	char str[26];
	ctime_s(str, sizeof str, &t);

	std::stringstream returnSS;

	//removes unnecessary data
	for (int i = 0; i < 8; ++i)
	{
		returnSS << str[11 + i];
	}

	return returnSS.str();

}

inline std::string ExceptionHandler::GetDate()
{
	auto now = std::chrono::system_clock::now();

	std::time_t t = std::chrono::system_clock::to_time_t(now);

	char str[26];
	ctime_s(str, sizeof str, &t);

	std::stringstream returnSS;

	//removes unnecessary data
	for (int i = 0; i < 10; ++i)
	{
		returnSS << str[i];
	}

	return returnSS.str();
}

void ExceptionHandler::AssignEngine(CEngine * aEngine)
{
	myEngine = aEngine;
}
