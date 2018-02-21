#pragma once

#include "..\TClient\Client.h"
#include <windows.h>
#include <string>
#include <iostream>
#include <fstream>


#include <d3d11_1.h>
#pragma comment(lib, "d3d11.lib")
#include "d3dCommon.h"
#include <d3dcompiler.h>
#pragma comment(lib, "d3dCompiler.lib")

#ifndef _RETAIL
#include "DebugDrawer.h"
#endif

#include "Scene.h"
#include "Engine.h"

#include "..\Engine\DXFramework.h"
#include "..\Engine\WindowHandler.h"
#include "..\Engine\Model.h"
#include "..\Engine\ModelLoader.h"
#include "..\Engine\ModelInstance.h"
#include "..\Engine\Camera.h"
#include "..\Engine\CameraInstance.h"
#include "..\Engine\PostMaster.h"

#include "..\Utilities\FileWatcher\FileWatcher.h"
#include "..\Utilities\common_converters.h"
#include "..\Utilities\GrowingArray.h"
#include "..\Utilities\DebugLog.h"
#include "..\Utilities\Vector.h"
#include "..\Utilities\Matrix.h"
#include "..\Utilities\InputManager.h"
#include "..\Utilities\MemoryPool.h"
#include "..\Utilities\Timer.h"
#include "..\Utilities\CorgiCommon.h"

#include "..\DirectX\Inc\ScreenGrab.h"

#include "ShaderCompiler.h"

