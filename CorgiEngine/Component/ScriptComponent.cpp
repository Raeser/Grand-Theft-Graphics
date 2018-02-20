#include "stdafx.h"
#include "ScriptComponent.h"

#define FUNCTIONS_TO_CALL_INITSIZE 16
#define NUMBER_OF_FILES 64

CScriptComponent::CScriptComponent()
{
	myLuaState = luaL_newstate();
	myFunctionsToCall.Init(FUNCTIONS_TO_CALL_INITSIZE);
	myFiles.Init(NUMBER_OF_FILES);
}


CScriptComponent::~CScriptComponent()
{
}

void CScriptComponent::Init()
{
	CComponent::Init();
	lua_close(myLuaState);
	myLuaState = luaL_newstate();
	CScriptManager::GetInstance().ExposeFunctionToState(myLuaState);
	myComponentTag = eComponentTag::Script;
	myFiles.ReInit(NUMBER_OF_FILES);
	myFunctionsToCall.ReInit(FUNCTIONS_TO_CALL_INITSIZE);
}

void CScriptComponent::Update()
{
	for (SLuaFunction& function : myFunctionsToCall)
	{
		CScriptManager::GetInstance().CallLuaFunction(function);
	}
	myFunctionsToCall.RemoveAll();
}

void CScriptComponent::FetchData()
{
}

void CScriptComponent::SendData()
{
}

void CScriptComponent::Release()
{
	CComponent::Release();
}


#ifndef _RETAIL
inline bool CScriptComponent::FileExists(const std::string& name)
{
	std::ifstream f(name.c_str());
	if (f.good())
	{
		f.close();
		return true;
	}

	f.close();
	return false;
}
#endif
LUARESULT CScriptComponent::LoadFile(const char * aFilePath)
{

#ifndef _RETAIL
	if (!FileExists(aFilePath))
	{
		return 1;
	}
#endif
	myFiles.Add(aFilePath);

	Corgi::CFileWatcher::GetInstance().WatchFileChangeWithDependencies(Corgi::ConvertCharPtrToWideString(aFilePath), std::bind(&CScriptComponent::ReloadAllFiles, this));

	//Corgi::CFileWatcher::GetInstance().WatchFileChangeWithDependencies(Corgi::ConvertCharPtrToWideString(aFilePath), std::bind([&](const char* file) {this->InternalFileLoader(file); }, aFilePath));


	return InternalFileLoader(aFilePath);
}

lua_State * CScriptComponent::GetLuaStatePtr()
{
	return myLuaState;
}

void CScriptComponent::ReloadAllFiles()
{
	for (std::string& file : myFiles)
	{
		InternalFileLoader(file.c_str());
	}
}

LUARESULT CScriptComponent::InternalFileLoader(const char * aFilePath)
{

	LUARESULT result = luaL_dofile(myLuaState, aFilePath);

	if (result == 0)
	{
#ifndef _RETAIL
		int linecount = CScriptManager::CountLinesInFile(aFilePath);


		if (linecount > 500)
		{
			DL_ERROR(eDebugLogType::Script, "%s%s%s%i", "Too many lines in lua file: ", aFilePath, "! Linecount: ", linecount);
			DL_WARNING(eDebugLogType::Script, "%s%s%s", "READ LUA FILE:		", aFilePath, "WITH ERRORS");
		}
		else
		{
			DL_SUCCESS(eDebugLogType::Script, "%s%s", "SUCCESS READING LUA FILE:		", aFilePath);
		}
#endif
	}
	else
	{
		const char* failString = lua_tostring(myLuaState, -1);
		DL_ERROR(eDebugLogType::Script, "%s%s%s", failString, "		IN FILE:		", aFilePath);
	}
	return result;
}
