#pragma once
#include "Component.h"
#include "..\Script\ScriptVarTemplates.h"
#include "..\Script\ScriptManager.h"

struct lua_State;

typedef int LUARESULT;

class CScriptComponent :
	public CComponent
{
public:
	CScriptComponent();
	~CScriptComponent();

	void Init() override;
	void Update() override;
	void FetchData() override;
	void SendData() override;
	void Release() override;
#ifndef _RETAIL
	bool FileExists(const std::string & name);
#endif
	LUARESULT LoadFile(const char * aFilePath);


	template<class ...Types>
	void CallFunction(SLuaFunctionName aFunctionName, Types ... args)
	{
		SLuaFunction func;
		func.functionName = aFunctionName.name;
		func.args.Init(8);

		func.state = myLuaState;
		SArgCount argCount;
		buffer_argument_push(func.args, argCount, args...);
		myFunctionsToCall.Add(func);
	}

	lua_State* GetLuaStatePtr();


private:


	void ReloadAllFiles();
	LUARESULT InternalFileLoader(const char * aFilePath);
	Corgi::GrowingArray<SLuaFunction> myFunctionsToCall;
	Corgi::GrowingArray<std::string> myFiles;
	lua_State* myLuaState;

};

