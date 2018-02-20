#pragma once

class CCpuUsage
{
public:
	CCpuUsage();
	~CCpuUsage();
	void Init();
	void Update();
	double GetCpuUsage();
private:
	float CpuUsageInPercent();
	void InitCPUException();
	double CpuUsage();
	//Cpu Usage stuff
	FILETIME* myPrevSysKernel;
	FILETIME* myPrevSysUser;
	FILETIME* myPrevProcKernel;
	FILETIME* myPrevProcUser;


	ULARGE_INTEGER myLastCPU, myLastSysCPU, myLastUserCPU;
	int myNumProcessors;
	HANDLE self;
	double myAverageCpuLoad;
	double* myHundredLatestLoads;
	int myCurrentPosition;
};

