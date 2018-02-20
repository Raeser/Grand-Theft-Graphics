#include "stdafx.h"
#include "ScriptCallbackManager.h"
#include "lua_argument_struct.h"

#pragma warning( push ) 
#pragma warning(disable : 4244)
extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

#pragma warning(pop)

CScriptCallbackManager::CScriptCallbackManager()
{
	int size = cast_int(ECallbackEvent::Count);
	myCallbacks.Init(size);
	for (int i = 0; i < size; ++i)
	{
		Corgi::GrowingArray<SCallbackFunction> GA;
		GA.Init(64);
		myCallbacks.Add(GA);
	}
}


CScriptCallbackManager::~CScriptCallbackManager()
{
}

long long CScriptCallbackManager::RegisterCallback(lua_State* aLuaState)
{
	ECallbackEvent event = ECallbackEvent::Uninitialized;
	if (lua_isstring(aLuaState, 1))
	{
		std::string event_str(lua_tostring(aLuaState, 1));
		if (event_str == "MoveDone")
		{
			event = ECallbackEvent::MoveDone;
		}
		else if (event_str == "AnimationDone")
		{
			event = ECallbackEvent::AnimationDone;
		}

		if (event == ECallbackEvent::Uninitialized)
		{
			DL_ERROR(eDebugLogType::Script, "%s%s%s", "CALLBACK EVENT: ", event_str.c_str(), " WAS NOT FOUND!");
			return -1;
		}
	}
	else
	{
		return DL_LUA_ERROR(aLuaState, "Invalid Event (arg 1)");
	}


	std::string key = "";
	if (lua_isstring(aLuaState, 2))
	{
		key = (lua_tostring(aLuaState, 2));
	}
	else
	{
		return DL_LUA_ERROR(aLuaState, "Invalid ID (arg 2");
	}

	SLuaFunction function;
	
	if (lua_isstring(aLuaState, 3))
	{
		function.functionName = (lua_tostring(aLuaState, 3));
	}
	else
	{
		return DL_LUA_ERROR(aLuaState, "Invalid function name (arg 3)");
	}

	int args = lua_gettop(aLuaState) + 1;
	function.args.Init(args);

	for (int i = 4; i < args; ++i)
	{
		Argument arg;
		if (lua_isinteger(aLuaState, i))
		{
			arg.myType = eScriptValueTypes::Int;
			arg.myInt = cast_int(lua_tointeger(aLuaState, i));
		}
		else if (lua_isnumber(aLuaState, i))
		{
			arg.myType = eScriptValueTypes::Float;
			arg.myFloat = cast_f(lua_tonumber(aLuaState, i));
		}
		else if (lua_isstring(aLuaState, i))
		{
			arg.myType = eScriptValueTypes::String;
			arg.myString = lua_tostring(aLuaState, i);
		}
		else if (lua_isboolean(aLuaState, i))
		{
			arg.myType = eScriptValueTypes::Bool;
			switch (lua_toboolean(aLuaState, i))
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

	SCallbackFunction callbackfunction;
	callbackfunction.myFunction = function;
	callbackfunction.myFunction.state = aLuaState;
	callbackfunction.myID = key;


	myCallbacks[cast_int(event)].Add(callbackfunction);

	return 0;
}

void CScriptCallbackManager::UnRegisterCallback(lua_State* aLuaState)
{
	ECallbackEvent event = ECallbackEvent::Uninitialized;
	if (lua_isstring(aLuaState, 1))
	{
		std::string event_str(lua_tostring(aLuaState, 1));
		if (event_str == "MoveDone")
		{
			event = ECallbackEvent::MoveDone;
		}
		else if (event_str == "AnimationDone")
		{
			event = ECallbackEvent::AnimationDone;
		}
		else
		{
			DL_LUA_ERROR(aLuaState, "Invalid ID");
			return;
		}
	}
	else
	{
		DL_LUA_ERROR(aLuaState, "Invalid Event (arg 1)");
		return;
	}

	std::string key = "";
	if (lua_isstring(aLuaState, 2))
	{
		key = (lua_tostring(aLuaState, 2));
	}
	else
	{
		DL_LUA_ERROR(aLuaState, "Invalid ID (arg 2)");
		return;
	}


	for (int i = 0; i < myCallbacks[cast_int(event)].Size(); ++i)
	{
		if (myCallbacks[cast_int(event)][i].myID == key)
		{
			myCallbacks[cast_int(event)].RemoveCyclicAtIndex(i);
		}
	}
}

void CScriptCallbackManager::RecieveMessage(const SMessage & message)
{
	switch (message.messageType)
	{
	case eMessageType::CallbackEvent:
		CallFunctions(message.callbackEvent.callbackEvent, message.callbackEvent.id);
		break;
	default:
		break;
	}
}

void CScriptCallbackManager::CallFunctions(ECallbackEvent aEvent, Callback_ID aID)
{
	for (SCallbackFunction& callback : myCallbacks[cast_int(aEvent)])
	{
		if (callback.myID == aID)
		{
			CScriptManager::GetInstance().CallLuaFunction(callback.myFunction);
		}
	}
}
