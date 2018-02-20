#pragma once

#include <stdarg.h>
#include "GrowingArray.h"
#include <chrono>
#include "Vector4.h"
//
#define DL_LUA_ERROR(L, str) DebugLog::GetInstance().LuaError(L, str);

#ifndef _RETAIL
#define DL_ERROR(enumDebugLogType, ...) DebugLog::GetInstance().WriteLog(eConsoleColor::RED, enumDebugLogType, __VA_ARGS__);
#define DL_SCRIPT_ERROR(...) DebugLog::GetInstance().WriteLog(eConsoleColor::RED, eDebugLogType::Script, __VA_ARGS__);
#define DL_WARNING(enumDebugLogType, ...) DebugLog::GetInstance().WriteLog(eConsoleColor::YELLOW, enumDebugLogType, __VA_ARGS__);
#define DL_SUCCESS(enumDebugLogType, ...) DebugLog::GetInstance().WriteLog(eConsoleColor::GREEN, enumDebugLogType, __VA_ARGS__);
#define DL_WRITELOG(enumConsoleColor, enumDebugLogType, ...) DebugLog::GetInstance().WriteLog(enumConsoleColor, enumDebugLogType, __VA_ARGS__);
#define DL_INITCHECK(result, systemName, functionLocation) if (result){DL_SUCCESS(eDebugLogType::ResultMessages, "%s%s", systemName, " Initialization succeded");}else{DL_ERROR(eDebugLogType::ResultMessages, "%s%s%s", systemName, " Initialization Failed in ", functionLocation); return false;};
#else

#define DL_ERROR(enumDebugLogType, ...) enumDebugLogType; __VA_ARGS__;
#define DL_SCRIPT_ERROR(...) __VA_ARGS__;
#define DL_WARNING(enumDebugLogType, ...) enumDebugLogType;__VA_ARGS__;
#define DL_SUCCESS(enumDebugLogType, ...) enumDebugLogType;__VA_ARGS__;
#define DL_WRITELOG(enumConsoleColor, enumDebugLogType, ...)enumConsoleColor;enumDebugLogType; __VA_ARGS__;
#define DL_INITCHECK(result, systemName, functionLocation)result;systemName;functionLocation;
#endif 

#ifdef USE_FILTERLOG
#define RESOURCE_LOG(...) DL_WRITELOG(eDebugLogType::Resource, __VA_ARGS__);
#define ENGINE_LOG(...) DL_WRITELOG(eDebugLogType::Engine, __VA_ARGS__);
#else
#define RESOURCE_LOG(...)
#define ENGINE_LOG(...)
#endif

struct lua_State;

enum class eDebugLogType
{
	Resource,
	UnityExporter,
	Engine,
	Render,
	Shader,
	ResultMessages,
	Timer,
	Error,
	Script,
	count,
};

enum eConsoleColor
{
	GRAY = 8,
	BLUE = 9,
	GREEN = 10,
	LIGHTBLUE = 11,
	RED = 12,
	PURPLE = 13,
	YELLOW = 14,
	WHITE = 15

};
class CGameConsole;

class DebugLog
{
public:
	~DebugLog();
	
	static DebugLog& GetInstance()
	{
		static DebugLog me;
		return me;
	}
	void SetConsole(CGameConsole* aConsole);
	Vector4f GetColor(eConsoleColor aColor);
	void WriteLog(eConsoleColor color, eDebugLogType enumDebugLogType, char* format, ...);

	void ActivateFilterlog(eDebugLogType enumDebugLogType);
	void DeactivateFilterlog(eDebugLogType enumDebugLogType);
	
	int LuaError(lua_State * L, const char* str);

	Corgi::GrowingArray<std::string> GetActiveLogs();

private:
	std::string GetTime();
	DebugLog();
	std::string GetLogStringFromEnum(eDebugLogType enumLogType);
	Corgi::GrowingArray<std::string> myActiveLogs;
	CGameConsole* myActiveConsole;
	//std::chrono::steady_clock::time_point myStartOfTime;
};

