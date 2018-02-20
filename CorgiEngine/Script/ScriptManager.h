#pragma once

#include "..\Utilities\GrowingArray.h"
#include "..\Utilities\TripleBuffer.h"
#include <string>
#include <thread>
#include <mutex>
#include "lua_argument_struct.h"
#include "..\Script\ScriptCallbackManager.h"

#include "ScriptVarTemplates.h"
#include "ScriptTimeManager.h"

struct SLuaFunctionName
{
	SLuaFunctionName(const char* aName) :
		name(aName) {}
	SLuaFunctionName(std::string aName) :
		name(aName.c_str()) {}
	const char* name;
};

struct SFunctionRegister
{
	std::string functionName;
	int(*function)(lua_State*);
	bool ExposedToConsole;
};

class CScriptManager
{
public:
	static CScriptManager& GetInstance()
	{
		static CScriptManager me;
		return me;
	}
	~CScriptManager();

	void UpdateTimers();

	void Init();

	void RegisterFunction(const char* aFunctionName, int(*aFunction)(lua_State*), const char* aArgList, const char* aHelpText, bool aCallableFromConsole);

	template<class ...Types>
	void CScriptManager::CallFunction(SLuaFunctionName aFunctionName, Types ... args)
	{
		SLuaFunction func;
		func.functionName = aFunctionName.name;
		func.args.Init(8);
		func.state = myLuaState;

		SArgCount argCount;
		buffer_argument_push(func.args, argCount, args...);

		myTripleBuffer.Write(func);
	}

	void CallLuaFunction(const SLuaFunction& aFunction)
	{
		myTripleBuffer.Write(aFunction);
	}

	int ReadFile(const char* aFilePath);
	void ReloadFiles();
	bool ExposeFunctionToState(lua_State* aState);

	lua_State* GetLuaStatePtr() const;


#ifndef _RETAIL
	static int CountLinesInFile(const char* aFile);
#endif

	static int LuaRegCallback(lua_State*);
	static int LuaUnRegCallback(lua_State*);
	static int Sleep(lua_State* aState);

	void WriteDocumentation(const char* aFunctionName, const char* aArgList, const char* aHelpText);

	const Corgi::GrowingArray<SFunctionRegister>& GetExposedFunctions() { return myExposedFunctions; }
	const std::string& GetDocumentationFilePath() { return myDocumentationFile; };
private:
	CScriptCallbackManager myCallbackManager;
	CScriptTimeManager myTimerManager;

	void ReloadAllFiles();
	int Run();
	void InternalFunctionCaller();

	size_t LevenshteinDistance(const char* stringToCheck, size_t stringLength, const char* targetString, size_t targetLength);

	int LoadFile(const char* aFilePath);
	void ReloadFile(const char* aFilePath);
	void LoadFunctions();
	void ClearDocumentation();

	


	CScriptManager();
	Corgi::GrowingArray<std::string> myFiles;
	Corgi::GrowingArray<SFunctionRegister> myExposedFunctions;
	std::atomic<lua_State*> myLuaState;

	std::string myDocumentationFile;

	//Thread
	std::thread myThread;
	TripleBuffer<SLuaFunction> myTripleBuffer;

	Corgi::GrowingArray<SFunctionRegister> myFunctionsToRegister;
	std::mutex myFunctionMutex;
	std::mutex myFileMutex;

	std::atomic<bool> myHasNewFunctions;
	std::atomic<bool> myHasNewFiles;
	std::atomic<bool> myIsRunning;
};

