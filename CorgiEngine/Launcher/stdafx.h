#pragma once

#include "..\Utilities\GrowingArray.h"
#include "..\Utilities\Vector.h"
#include "..\Utilities\Matrix.h"
#include <windows.h>
#include <string>
#include <iostream>


#include "..\Engine\Engine.h"
#include "..\Game\Game.h"
#include "..\Engine\WindowHandler.h"
#include "..\Engine\ModelLoader.h"
#include "..\Engine\ModelInstance.h"
#include "..\Utilities\DebugLog.h"
#include "..\Utilities\CommandLineManager.h"
#include "..\Utilities\InputManager.h"
#include "..\Utilities\Vector.h"

#include "LuaExposedVariableWrapper.h"

//TEXT TO SPEECH
#define _ATL_APARTMENT_THREADED

#include <atlbase.h>
//You may derive a class from CComModule and use it if you want to override something,
//but do not change the name of _Module
extern CComModule _Module;
#include <atlcom.h>
