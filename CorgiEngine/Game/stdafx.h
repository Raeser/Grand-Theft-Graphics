#pragma once

#define WIN32_LEAN_AND_MEAN    

#include <windows.h>
#include <string>

#include "..\Engine\PostMaster.h"
#include "..\Engine\ModelLoaderSpace.h"
#include "..\Engine\SpriteInstance.h"
#include "..\Engine\SpriteLoader.h"
#include "..\Engine\Engine.h"
#include "..\Engine\CameraInstance.h"

#include "..\Utilities\GrowingArray.h"
#include "..\Utilities\Vector.h"
#include "..\Utilities\Matrix.h"
#include "..\Utilities\InputManager.h"
#include "..\Utilities\Timer.h"
#include "..\Utilities\CorgiMath.h"
#include "..\Utilities\CorgiCommon.h"

#include "..\Component\Entity.h"

#include "ConsoleState.h"
#include "InGameState.h"
#include "MainMenuState.h"

#include "StateStack.h"
#include "GameSettings.h"
#include "GameWorld.h"
#include "Cursor.h"
#include "Button.h"

#include "unitystruct.h"
#include "DataManager.h"
#include "WwiseWrapper.h"

#include "..\Console\GameConsole.h"

#include "GAME_DEFINES.h"
#include "DataStructs.h"

#include "..\Wwise\Audio\Audio\AudioInterface.h"