#include "stdafx.h"
#include "ScriptTimeManager.h"
#include "..\Utilities\Timer.h"


CScriptTimeManager::CScriptTimeManager()
{
}


CScriptTimeManager::~CScriptTimeManager()
{
}

void CScriptTimeManager::Init()
{
	myTimers.Init(32);
}

void CScriptTimeManager::SleepCallback(lua_State * aState)
{
	SLuaFunction function;
	float time = 0;

	if (lua_isnumber(aState, 1))
	{
		time = cast_f((lua_tonumber(aState, 1)));
	}
	else if (lua_isinteger(aState, 1))
	{
		time = cast_f((lua_tointeger(aState, 1)));
	}
	else
	{
		DL_LUA_ERROR(aState, "Invalid sleep time (arg 1)");
	}


	if (lua_isstring(aState, 2))
	{
		function.functionName = (lua_tostring(aState, 2));
	}
	else
	{
		DL_LUA_ERROR(aState, "Invalid function name (arg 2)");
	}

	int args = lua_gettop(aState) + 1;
	function.args.Init(args);

	for (int i = 3; i < args; ++i)
	{
		Argument arg;
		if (lua_isinteger(aState, i))
		{
			arg.myType = eScriptValueTypes::Int;
			arg.myInt = cast_int(lua_tointeger(aState, i));
		}
		else if (lua_isnumber(aState, i))
		{
			arg.myType = eScriptValueTypes::Float;
			arg.myFloat = cast_f(lua_tonumber(aState, i));
		}
		else if (lua_isstring(aState, i))
		{
			arg.myType = eScriptValueTypes::String;
			arg.myString = lua_tostring(aState, i);
		}
		else if (lua_isboolean(aState, i))
		{
			arg.myType = eScriptValueTypes::Bool;
			switch (lua_toboolean(aState, i))
			{
			case 0:
				arg.myBool = false;
				break;
			case 1:
				arg.myBool = true;
				break;
			}
		}
		function.args.Add(arg);
	}

	STimedFunctionCallback callbackfunction;
	callbackfunction.function = function;
	callbackfunction.function.state = aState;
	callbackfunction.time = time;

	myTimers.Add(callbackfunction);
}

void CScriptTimeManager::Update()
{
	for (int i = 0; i < myTimers.Size(); ++i)
	{
		STimedFunctionCallback& timer = myTimers[i];
		timer.time -= Timer::GetInstance().GetDeltaTime() * 1000;

		if (timer.time <= 0)
		{
			CScriptManager::GetInstance().CallLuaFunction(timer.function);
			myTimers.RemoveCyclicAtIndex(i);
			--i;
		}
	}
}
