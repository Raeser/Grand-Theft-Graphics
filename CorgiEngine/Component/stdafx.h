#pragma once

//#define WIN64_LEAN_AND_MEAN          

#include <windows.h>
#include <string>
#include <iostream>
#include <sstream>

#include "..\Engine\ModelLoaderSpace.h"
#include "..\Engine\PostMaster.h"
#include "..\Component\Entity.h"

#include "..\Utilities\FileWatcher\FileWatcher.h"
#include "..\Utilities\Vector.h"
#include "..\Utilities\Matrix.h"
#include "..\Utilities\GrowingArray.h"
#include "..\Utilities\InputManager.h"
#include "..\Utilities\Timer.h"
#include "..\Utilities\CorgiMath.h"
#include "..\Utilities\CorgiCommon.h"

#include "..\Script\ScriptManager.h"

#include "..\Game\Cursor.h"
#include "..\Game\GameSettings.h"
#include "..\Game\EngineWrapper.h"