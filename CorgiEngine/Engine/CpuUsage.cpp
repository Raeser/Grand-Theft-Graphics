#include "stdafx.h"
#include "CpuUsage.h"

float GetCPUUsageUgly(FILETIME *prevSysKernel, FILETIME *prevSysUser, FILETIME *prevProcKernel, FILETIME *prevProcUser, bool firstRun /* = false */);
CCpuUsage::CCpuUsage()
{
	myPrevSysKernel = new FILETIME;
	myPrevSysUser = new FILETIME;
	myPrevProcKernel = new FILETIME;
	myPrevProcUser = new FILETIME;
}


CCpuUsage::~CCpuUsage()
{
	delete myPrevSysKernel;
	myPrevSysKernel = nullptr;

	delete myPrevSysUser;
	myPrevSysUser = nullptr;

	delete myPrevProcKernel;
	myPrevProcKernel = nullptr;

	delete myPrevProcUser;
	myPrevProcUser = nullptr;

	delete[] myHundredLatestLoads;
	myHundredLatestLoads = nullptr;
}

void CCpuUsage::Init()
{
	InitCPUException();
	GetCPUUsageUgly(myPrevSysKernel, myPrevSysUser, myPrevProcKernel, myPrevProcUser, true);
	myHundredLatestLoads = new double[100];
	myCurrentPosition = 0;
}

void CCpuUsage::Update()
{
	myHundredLatestLoads[myCurrentPosition] = CpuUsage();
	myCurrentPosition++;
	if (myCurrentPosition > 99)
	{
		myCurrentPosition = 0;
	}

	myAverageCpuLoad = 0;
	int positionToReadFrom = myCurrentPosition;
	for (int i = 0; i < 100; ++i)
	{
		if ((positionToReadFrom - i) < 0)
		{
			positionToReadFrom = 99 + i;
		}
		myAverageCpuLoad += myHundredLatestLoads[positionToReadFrom - i];
	}
	myAverageCpuLoad /= 100;
}

double CCpuUsage::GetCpuUsage()
{
	return myAverageCpuLoad;
}

ULONGLONG FixCPUTimings(const FILETIME &a, const FILETIME &b)
{
	LARGE_INTEGER la, lb;
	la.LowPart = a.dwLowDateTime;
	la.HighPart = a.dwHighDateTime;
	lb.LowPart = b.dwLowDateTime;
	lb.HighPart = b.dwHighDateTime;

	return la.QuadPart - lb.QuadPart;
}

void CCpuUsage::InitCPUException()
{
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


#pragma warning(push)
#pragma warning(disable:4244)
double CCpuUsage::CpuUsage()
{
	FILETIME ftime, fsys, fuser;
	ULARGE_INTEGER now, sys, user;
	double percent;

	GetSystemTimeAsFileTime(&ftime);
	memcpy(&now, &ftime, sizeof(FILETIME));

	GetProcessTimes(self, &ftime, &ftime, &fsys, &fuser);
	memcpy(&sys, &fsys, sizeof(FILETIME));
	memcpy(&user, &fuser, sizeof(FILETIME));
	percent = (sys.QuadPart - myLastSysCPU.QuadPart) + (user.QuadPart - myLastUserCPU.QuadPart);
	percent = percent / (now.QuadPart - myLastCPU.QuadPart);
	percent = percent / myNumProcessors;
	myLastCPU = now;
	myLastUserCPU = user;
	myLastSysCPU = sys;
	return percent * 100.0;
}
#pragma warning(pop)


float GetCPUUsageUgly(FILETIME *prevSysKernel, FILETIME *prevSysUser,
	FILETIME *prevProcKernel, FILETIME *prevProcUser,
	bool firstRun = false)
{
	FILETIME sysIdle, sysKernel, sysUser;
	FILETIME procCreation, procExit, procKernel, procUser;

	if (!GetSystemTimes(&sysIdle, &sysKernel, &sysUser) ||
		!GetProcessTimes(GetCurrentProcess(), &procCreation, &procExit, &procKernel, &procUser))
	{
		// can't get time info so return
		return -1.;
	}

	// check for first call
	if (firstRun)
	{
		// save time info before return
		prevSysKernel->dwLowDateTime = sysKernel.dwLowDateTime;
		prevSysKernel->dwHighDateTime = sysKernel.dwHighDateTime;

		prevSysUser->dwLowDateTime = sysUser.dwLowDateTime;
		prevSysUser->dwHighDateTime = sysUser.dwHighDateTime;

		prevProcKernel->dwLowDateTime = procKernel.dwLowDateTime;
		prevProcKernel->dwHighDateTime = procKernel.dwHighDateTime;

		prevProcUser->dwLowDateTime = procUser.dwLowDateTime;
		prevProcUser->dwHighDateTime = procUser.dwHighDateTime;

		return -1.;
	}

	ULONGLONG sysKernelDiff = FixCPUTimings(sysKernel, *prevSysKernel);
	ULONGLONG sysUserDiff = FixCPUTimings(sysUser, *prevSysUser);

	ULONGLONG procKernelDiff = FixCPUTimings(procKernel, *prevProcKernel);
	ULONGLONG procUserDiff = FixCPUTimings(procUser, *prevProcUser);

	ULONGLONG sysTotal = sysKernelDiff + sysUserDiff;
	ULONGLONG procTotal = procKernelDiff + procUserDiff;

	return (float)((100.0 * procTotal) / sysTotal);
}

float CCpuUsage::CpuUsageInPercent()
{
	return GetCPUUsageUgly(myPrevSysKernel, myPrevSysUser, myPrevProcKernel, myPrevProcUser);
}