#include "stdafx.h"
#include "../Console/GameConsole.h"
#include "..\Script\lua.hpp"
#include "../Script/ScriptManager.h"

std::string string_vsprintf(const char* format, std::va_list args)
{
	format;
	args;
	va_list tmp_args; //unfortunately you cannot consume a va_list twice
	va_copy(tmp_args, args); //so we have to copy it
	const int required_len = _vscprintf(format, tmp_args) + 1;
	va_end(tmp_args);

	char buff[256];
	memset(buff, 0, required_len);
	if (vsnprintf_s(buff, required_len, format, args) < 0)
	{
		return "string_vsprintf encoding error";
	}
	return std::string(buff);
}



void DebugLog::SetConsole(CGameConsole * aConsole)
{
	myActiveConsole = aConsole;
}

Vector4f DebugLog::GetColor(eConsoleColor aColor)
{
	switch (aColor)
	{
	case GRAY:
		return{ 0.5f,0.5f,0.5f,1.f };
		break;
	case BLUE:
		return{ 0.f,0.f,1.f,1.f };
		break;
	case GREEN:
		return{ 0.f,1.f,0.f,1.f };
		break;
	case LIGHTBLUE:
		return{ 173 / 255.f, 216.f / 255.f,230.f / 255.f, 1.f };
		break;
	case RED:
		return{ 1.f, 0.f, 0.f, 1.f };
		break;
	case PURPLE:
		return{ 85.f / 255.f, 26.f / 255.f, 139.f / 255.f, 1.f };
		break;
	case YELLOW:
		return{ 1.f, 1.f, 0.f, 1.f };
		break;
	case WHITE:
		return{ 1.f,1.f,1.f,1.f };
		break;
	default:
		return{ 1.f,1.f,1.f,1.f };
		break;
	}

}
void DebugLog::WriteLog(eConsoleColor color, eDebugLogType enumDebugLogType, char * format, ...)
{
#ifndef _RETAIL
	std::string logString = GetLogStringFromEnum(enumDebugLogType);

	if (myActiveLogs.Find(logString) != -1)
	{
		SetConsoleColor(color);
		std::string file = std::string(logString) + "_log.txt";

		std::filebuf fb;
		fb.open(file, std::ios::out | std::ios::app);
		std::ostream os(&fb);

		va_list argptr;
		va_start(argptr, format);
		std::string str{ string_vsprintf(format, argptr) };
		va_end(argptr);
		std::string time = GetTime();
		std::string output = "[" + time + "] [" + logString + "] " + str;
		if (myActiveConsole != nullptr)
		{
			//not threadsafe...
			myActiveConsole->AddResultText(output);
		}
		std::cout << "[" << time.c_str() << "] [" << logString << "] "<< str.c_str() << std::endl;
		os << "[" << time.c_str() << "] [" << logString << "] " << str.c_str() << std::endl;
		fb.close();
		SetConsoleColor(eConsoleColor::WHITE);
	}
#else
	color; enumDebugLogType; format;
#endif
}

int DebugLog::LuaError(lua_State * L, const char* str)
{
	lua_Debug ar;
	if (lua_getstack(L, 1, &ar) == 1)
	{
		if (lua_getinfo(L, "nSl", &ar) != 0)
		{
			int line = ar.currentline;
			const char* file = ar.short_src;
			WriteLog(eConsoleColor::RED, eDebugLogType::Script, "%s%s%s%i%s%s", "[LUA ERROR in File: ", file, " -> Line: ", line, "] | ", str);
		}
		else
		{
			WriteLog(eConsoleColor::RED, eDebugLogType::Script, "%s", "[LUA ERROR]");
		}
	}

	return 0;
}

void DebugLog::ActivateFilterlog(eDebugLogType enumDebugLogType)
{
	std::string logString = GetLogStringFromEnum(enumDebugLogType);

	if (myActiveLogs.Find(logString) == -1)
	{
		myActiveLogs.Add(logString);
	}
}

void DebugLog::DeactivateFilterlog(eDebugLogType enumDebugLogType)
{
	std::string logString = GetLogStringFromEnum(enumDebugLogType);

	if (myActiveLogs.Find(logString) == -1)
	{
		myActiveLogs.RemoveCyclic(logString);
	}
}


Corgi::GrowingArray<std::string> DebugLog::GetActiveLogs()
{
	return myActiveLogs;
}

std::string DebugLog::GetLogStringFromEnum(eDebugLogType enumDebugLogType)
{
	std::string logString = "";
	switch (enumDebugLogType)
	{
	case eDebugLogType::Resource:
		logString = "Resource";
		break;
	case eDebugLogType::UnityExporter:
		logString = "UnityExporter";
		break;
	case  eDebugLogType::Engine:
		logString = "Engine";
		break;
	case  eDebugLogType::Render:
		logString = "Render";
		break;
	case  eDebugLogType::ResultMessages:
		logString = "ResultMessages";
		break;
	case  eDebugLogType::Timer:
		logString ="Timer";
		break;
	case  eDebugLogType::Error:
		logString = "Error";
		break;
	case  eDebugLogType::Script:
		logString = "Script";
		break;
	case  eDebugLogType::Shader:
		logString = "Shader";
		break;
	default:
		assert(false && "Unexpected enumLogType");
		break;
	}
	return logString;
}

std::string DebugLog::GetTime()
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

DebugLog::DebugLog()
{
	myActiveLogs.Init(static_cast<int>(eDebugLogType::count));
}


DebugLog::~DebugLog()
{
}
