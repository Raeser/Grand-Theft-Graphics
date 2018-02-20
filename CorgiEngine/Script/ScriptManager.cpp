#include "stdafx.h"

#include "..\Utilities\DebugLog.h"
#include "..\Utilities\FileWatcher\FileWatcher.h"

using namespace std::chrono_literals;
typedef int LUARESULT;


#pragma comment(lib, "../../Libs/lua53.lib")


CScriptManager::CScriptManager()
{
}


CScriptManager::~CScriptManager()
{
	myIsRunning = false;
	while (!myThread.joinable());
	myThread.join();
	CPostMaster::GetInstance().Unsubscribe(&myCallbackManager, eMessageType::CallbackEvent);
}

void CScriptManager::Init()
{
	myFiles.Init(512);
	myExposedFunctions.Init(512);
	myFunctionsToRegister.Init(32);

	myDocumentationFile = "../Assets/Scripts/Documentation.txt";
	ClearDocumentation();
	myLuaState = luaL_newstate();

	myHasNewFiles = false;
	myIsRunning = true;
	myThread = std::thread([&]() {Run(); });

	myTimerManager.Init();

	CPostMaster::GetInstance().Subscribe(&myCallbackManager, eMessageType::CallbackEvent);
}

void CScriptManager::UpdateTimers()
{
	myTimerManager.Update();
}

void CScriptManager::InternalFunctionCaller()
{
	Corgi::GrowingArray<SLuaFunction>& functions = myTripleBuffer.Read();

	for (SLuaFunction& func : functions)
	{

		LUARESULT result = -1;
		
		if (lua_getglobal(func.state, func.functionName.c_str()))
		{
			SArgCount argCount;
			for (Argument& arg : func.args)
			{
				switch (arg.myType)
				{
				case eScriptValueTypes::Double:
					lua_push(func.state, arg.myDouble);
					++argCount.count;
					break;
				case eScriptValueTypes::Float:
					lua_push(func.state, arg.myFloat);
					++argCount.count;
					break;
				case eScriptValueTypes::UINT:
					lua_push(func.state, arg.myUint);
					++argCount.count;
					break;
				case eScriptValueTypes::Int:
					lua_push(func.state, arg.myInt);
					++argCount.count;
					break;
				case eScriptValueTypes::Bool:
					lua_push(func.state, arg.myBool);
					++argCount.count;
					break;
				case eScriptValueTypes::String:
					lua_push(func.state, arg.myString);
					++argCount.count;
					break;
				}
				
			}

			std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
			result = lua_pcall(func.state, argCount.count, 0, 0);
			std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
			std::chrono::duration<float> dur = end - start;
			float durcount = dur.count();

			if (durcount > 1.f)
			{
				std::stringstream ss("");
				ss << "SCRIPT FUNCTION \"";
				ss << func.functionName.c_str();
				ss << "\" RAN TOO LONG (";
				ss << durcount;
				ss << " ms)";
				DL_ERROR(eDebugLogType::Script, "%s", ss.str().c_str());
				//assert(false && "Script ran too long");
			}
			else if (durcount > 0.5f)
			{
				DL_ERROR(eDebugLogType::Script, "%f%s%s", durcount, "  name:  ", func.functionName.c_str());
			}
			else if (durcount > 0.1f)
			{
				DL_WARNING( eDebugLogType::Script, "%f%s%s", durcount, "  name:  ", func.functionName.c_str());
			}
	
			 

			if (result != 0)
			{
				std::string error = lua_tostring(func.state, -1);
				size_t index = error.find("attempt to call a nil value (global");
				if (index != -1)
				{
					index += 37;
					std:: string misspelledfunction = error.substr(index, error.size() - index - 2);

					size_t shortestLevDist = 9999;
					std::string suggestedFunction;
					for (SFunctionRegister& exposedFunction : myExposedFunctions)
					{
						std::string& str = exposedFunction.functionName;

						size_t levdis = LevenshteinDistance(str.c_str(), str.size(), misspelledfunction.c_str(), misspelledfunction.size());

						if (levdis < shortestLevDist && levdis < str.size())
						{
							shortestLevDist = levdis;
							suggestedFunction = str;
						}
					}
					if (shortestLevDist != 9999)
					{
						DL_WARNING(eDebugLogType::Script, "%s%s%s%s%s", "Couldn't find function: ", misspelledfunction.c_str(), "		Did you mean to call: ", suggestedFunction.c_str(), "?");
					}
				}

				else
				{
					DL_ERROR(eDebugLogType::Script, "%s%s%s", "Error running function:	", func.functionName.c_str(), error.c_str());
				}
			}
		}
		else
		{
			DL_ERROR(eDebugLogType::Script, "%s%s%s", "No function with name: ", func.functionName.c_str(), " loaded in any lua files!");
		}
	}

}


size_t CScriptManager::LevenshteinDistance(const char* aStringToCheck, size_t aStringLength, const char* aTargetString, size_t aTargetLength)
{
	++aStringLength; ++aTargetLength;
	size_t* d = new size_t[aStringLength * aTargetLength];

	memset(d, 0, sizeof(size_t) * aStringLength * aTargetLength);

	for (size_t i = 1, im = 0; i < aTargetLength; ++i, ++im)
	{
		for (size_t j = 1, jn = 0; j < aStringLength; ++j, ++jn)
		{
			if (aStringToCheck[jn] == aTargetString[im])
			{
				d[(i * aStringLength) + j] = d[((i - 1) * aStringLength) + (j - 1)];
			}
			else
			{
				d[(i * aStringLength) + j] = min(d[(i - 1) * aStringLength + j] + 1, /* A deletion. */
					min(d[i * aStringLength + (j - 1)] + 1, /* An insertion. */
						d[(i - 1) * aStringLength + (j - 1)] + 1)); /* A substitution. */
			}
		}
	}

#ifdef DEBUG_PRINT
	for (size_t i = 0; i < m; ++i)
	{
		for (size_t j = 0; j < n; ++j)
		{
			cout << d[(i * n) + j] << " ";
		}
		cout << endl;
	}
#endif

	size_t r = d[aStringLength * aTargetLength - 1];

	delete[] d;

	return r;
}

int CScriptManager::LoadFile(const char * aFilePath)
{
	
	LUARESULT result = luaL_dofile(myLuaState, aFilePath);



	if (result == 0)
	{
#ifndef _RETAIL
		int linecount = CountLinesInFile(aFilePath);


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

void CScriptManager::ReloadFile(const char * aFilePath)
{
	LoadFile(aFilePath);
}

void CScriptManager::LoadFunctions()
{
	for (SFunctionRegister& func : myFunctionsToRegister)
	{
		lua_register(myLuaState, func.functionName.c_str(), func.function);	
	}
	myFunctionsToRegister.RemoveAll();
}

void CScriptManager::ClearDocumentation()
{
#ifndef _RETAIL	
	std::filebuf fb;
	fb.open(myDocumentationFile, std::ios::out);
	std::ostream os(&fb);
	os << "";
	fb.close();
#endif // !_RETAIL

}

void CScriptManager::WriteDocumentation(const char * aFunctionName, const char* aArgList, const char * aHelpText)
{
#ifdef _RETAIL
	aFunctionName; aArgList; aHelpText;
#else
	std::filebuf fb;
	fb.open(myDocumentationFile, std::ios::out | std::ios::app);
	std::ostream os(&fb);


	os << "FUNCTION:    " << aFunctionName << "(" << aArgList << ") " << std::endl;
	os << "USE:         " << aHelpText << std::endl;
	os << "----------------------------------------------------" << std::endl << std::endl << std::endl;
	fb.close();
#endif
}

#ifndef _RETAIL
int CScriptManager::CountLinesInFile(const char * aFile)
{

	std::ifstream t;
	t.open(aFile);
	std::string line;
	int lineCount = 0;
	while (t) 
	{
		std::getline(t, line);
		++lineCount;
	}
	t.close();

	return lineCount - 1;
}
#endif

int CScriptManager::LuaRegCallback(lua_State* aLuaState)
{
	GetInstance().myCallbackManager.RegisterCallback(aLuaState);
	return 0;
}

int CScriptManager::LuaUnRegCallback(lua_State* aLuaState)
{ 
	GetInstance().myCallbackManager.UnRegisterCallback(aLuaState);
	return 0;
}

int CScriptManager::Sleep(lua_State * aState)
{
	GetInstance().myTimerManager.SleepCallback(aState);
	return 0;
}


int CScriptManager::Run()
{
	LUARESULT result = 0;
	while (myIsRunning)
	{

		myFileMutex.lock();
		if (myHasNewFiles)
		{
			ReloadAllFiles();
			myHasNewFiles = false;
		}
		myFileMutex.unlock();
		myFunctionMutex.lock();
		if (myHasNewFunctions)
		{
			LoadFunctions();
			myHasNewFunctions = false;
		}
		myFunctionMutex.unlock();

		InternalFunctionCaller();


		myTripleBuffer.FlipWriter();
		myTripleBuffer.FlipReader();

		std::this_thread::sleep_for(1ms);
	}
	return result;
}

void CScriptManager::RegisterFunction(const char* aFunctionName, int(*aFunction)(lua_State*), const char* aArgList, const char* aHelpText, bool aCallableFromConsole)
{
	WriteDocumentation(aFunctionName, aArgList, aHelpText);

	SFunctionRegister func;
	func.functionName = aFunctionName;
	func.function = aFunction;
	func.ExposedToConsole = aCallableFromConsole;

	std::lock_guard<std::mutex> lock(myFunctionMutex);
	myHasNewFunctions = true;
	myFunctionsToRegister.Add(func);
	myExposedFunctions.Add(func);
}

bool CScriptManager::ExposeFunctionToState(lua_State * aState)
{
	for (SFunctionRegister& func : myExposedFunctions)
	{
		lua_register(aState, func.functionName.c_str(), func.function);
	}
	return true;
}

lua_State * CScriptManager::GetLuaStatePtr() const
{
	return myLuaState;
}

int CScriptManager::ReadFile(const char* aFilePath)
{
	std::lock_guard<std::mutex> lock(myFileMutex);
	myFiles.Add(std::string(aFilePath));
	Corgi::CFileWatcher::GetInstance().WatchFileChange(Corgi::ConvertCharPtrToWideString(aFilePath), std::bind(&CScriptManager::ReloadFile, this, aFilePath));
	myHasNewFiles = true;

	return 0;
}

void CScriptManager::ReloadFiles()
{
	myHasNewFiles = true;
}




void CScriptManager::ReloadAllFiles()
{
	for (std::string& str : myFiles)
	{
		ReloadFile(str.c_str());
	}
}

