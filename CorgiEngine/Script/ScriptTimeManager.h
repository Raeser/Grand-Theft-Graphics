#pragma once


struct STimedFunctionCallback
{
	SLuaFunction function;
	float time;
};

class CScriptTimeManager
{
public:
	CScriptTimeManager();
	~CScriptTimeManager();

	void Init();

	void SleepCallback(lua_State* aState);
	void Update();

private:
	Corgi::GrowingArray<STimedFunctionCallback> myTimers;
};

