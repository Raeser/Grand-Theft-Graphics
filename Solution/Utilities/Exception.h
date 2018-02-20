#pragma once
#if __COUNTER__ > 4
#pragma message("Compiled: " __FILE__) 
#define STRING2(x) #x
#define STRING(x) STRING2(x)
#pragma message("Counter:" STRING(__COUNTER__))
#endif

#pragma warning( disable : 4091 4996 4244)
#include <windows.h>
#include <assert.h>
#include <stdio.h>
#include <tchar.h>
#include <dbghelp.h>
#include <sstream>
#include <fstream>
#include <Lmcons.h>
#include "psapi.h"
#include "..\Engine\Engine.h"
#include "..\Utilities\DebugLog.h"

#pragma comment(lib, "dbghelp.lib")

namespace CorgiCrash { 



}