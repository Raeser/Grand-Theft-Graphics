#include "stdafx.h"

#include "LuaExposedFunction.h"
#include "LuaExposedVariableWrapper.h"
#include "..\Script\ScriptManager.h"

#ifndef _RETAIL
#include "..\Engine\DebugDrawer.h" //We dont use debugdrawer in retail environment
#endif

#include "..\Engine\TextToSpeechManager.h"

#include "..\Utilities\DebugLog.h"

#include "..\Collision\TriggerVolumeManager.h"
#include "..\Script\ObjectiveManager.h"
#include "..\Game\WwiseWrapper.h"

namespace LuaExposedFunction
{
	int Print(lua_State * aState)
	{
		int args = lua_gettop(aState) + 1;

		std::stringstream ss("");

		for (int i = 1; i < args; ++i)
		{

			if (lua_isinteger(aState, i))
			{
				ss << lua_tointeger(aState, i);
			}
			else if (lua_isnumber(aState, i))
			{
				ss << lua_tonumber(aState, i);
			}
			else if (lua_isstring(aState, i))
			{
				ss << lua_tostring(aState, i);
			}
			else if (lua_isboolean(aState, i))
			{
				switch (lua_toboolean(aState, i))
				{
				case 0:
					ss << "false";
					break;
				case 1:
					ss << "true";
					break;
				}
			}
			ss << " ";
		}

		DL_WRITELOG(eConsoleColor::LIGHTBLUE, eDebugLogType::Script, "%s", ss.str().c_str());
		return 0;
	}


#ifdef _RETAIL
	int DebugPrintText(lua_State*)
	{
		return 0;
	}
	int DebugDrawLine(lua_State*)
	{
		return 0;
	}
	int DebugDrawCube(lua_State*)
	{
		return 0;
	}
	int DebugDrawArrow(lua_State*)
	{
		return 0;
	}
	int ToggleTriggerWireframe(lua_State *)
	{
		return 0;
	}
#else
	int DebugPrintText(lua_State* aState)
	{
		return CDebugDrawer::GetInstance().PrintTextFromLua(aState);
	}
	int DebugDrawLine(lua_State* aState)
	{
		return CDebugDrawer::GetInstance().DrawLineFromLua(aState);
	}
	int DebugDrawCube(lua_State* aState)
	{
		return CDebugDrawer::GetInstance().DrawCubeFromLua(aState);
	}
	int DebugDrawArrow(lua_State* aState)
	{
		return CDebugDrawer::GetInstance().DrawArrowFromLua(aState);
	}
	int ToggleTriggerWireframe(lua_State * aState)
	{
		return CTriggerVolumeManager::GetInstance().ToggleWireframeMode(aState);
	}
	
	int DrawFullNav(lua_State*)
	{
		SMessage mess;
		mess.messageType = eMessageType::DebugDrawFullNav;
		CPostMaster::GetInstance().PostMessageA(mess);
		return 0;
	}
	int DrawLocalNav(lua_State*)
	{
		SMessage mess;
		mess.messageType = eMessageType::DebugDrawLocalNav;
		CPostMaster::GetInstance().PostMessageA(mess);
		return 0;
	}
	int DrawFullEdges(lua_State*)
	{
		SMessage mess;
		mess.messageType = eMessageType::DebugDrawFullOuterEdges;
		CPostMaster::GetInstance().PostMessageA(mess);
		return 0;
	}
	int DrawLocalEdges(lua_State*)
	{
		SMessage mess;
		mess.messageType = eMessageType::DebugDrawLocalOuterEdges;
		CPostMaster::GetInstance().PostMessageA(mess);
		return 0;
	}

	int DrawNavMeshColliders(lua_State*)
	{
		SMessage mess;
		mess.messageType = eMessageType::DebugDrawNavMeshColliders;
		CPostMaster::GetInstance().PostMessageA(mess);
		return 0;
	}

	int DrawWallColliders(lua_State*)
	{
		SMessage mess;
		mess.messageType = eMessageType::DebugDrawWallColliders;
		CPostMaster::GetInstance().PostMessageA(mess);
		return 0;
	}

	int DrawLDDebug(lua_State*)
	{
		SMessage mess;
		mess.messageType = eMessageType::DebugDrawWallColliders;
		CPostMaster::GetInstance().PostMessageA(mess);
		mess.messageType = eMessageType::DebugDrawNavMeshColliders;
		CPostMaster::GetInstance().PostMessageA(mess);
		mess.messageType = eMessageType::DebugDrawLocalNav;
		CPostMaster::GetInstance().PostMessageA(mess);
		CTriggerVolumeManager::GetInstance().ToggleWireframeMode(nullptr);
		return 0;
	}



#endif

	int TextToSpeech(lua_State * aState)
	{

		int args = lua_gettop(aState) + 1;

		std::stringstream ss("");

		for (int i = 1; i < args; ++i)
		{

			if (lua_isinteger(aState, i))
			{
				ss << lua_tointeger(aState, i);
			}
			else if (lua_isnumber(aState, i))
			{
				ss << lua_tonumber(aState, i);
			}
			else if (lua_isstring(aState, i))
			{
				ss << lua_tostring(aState, i);
			}
			else if (lua_isboolean(aState, i))
			{
				switch (lua_toboolean(aState, i))
				{
				case 0:
					ss << "false";
					break;
				case 1:
					ss << "true";
					break;
				}
			}
			ss << " ";
		}

		CTextToSpeechManager::GetInstance().Speak(ss.str());
		return 0;
	}


	int RegisterCallback(lua_State * aState)
	{
		return CScriptManager::LuaRegCallback(aState);
	}

	int UnregisterCallback(lua_State * aState)
	{
		return CScriptManager::LuaUnRegCallback(aState);
	}

	int Sleep(lua_State * aState)
	{
		return CScriptManager::Sleep(aState);
	}

	int GetHelp(lua_State *)
	{
		const std::string documentationPath = CScriptManager::GetInstance().GetDocumentationFilePath();
		if(Corgi::FileExists(documentationPath.c_str()))
		{
			std::ifstream t;
			t.open(documentationPath);
			std::string line;
			while (t)
			{
				std::getline(t, line);
				if (line != "")
				{
					DL_WRITELOG(eConsoleColor::WHITE, eDebugLogType::ResultMessages, "%s%s", "	- ", line.c_str());
				}
	
			}
			t.close();
		}	
		return 0;
	}

	int GetTikiCountInTrigger(lua_State * aState)
	{

		int tikicount = CTriggerVolumeManager::GetInstance().GetTikiCount(aState);
		lua_pushinteger(aState, tikicount);
		return 1;

	}
	int GetTriggerPosition(lua_State * aState)
	{
		lua_pushnumber(aState, CTriggerVolumeManager::GetInstance().GetTriggerPosition(aState).x);
		lua_pushnumber(aState, CTriggerVolumeManager::GetInstance().GetTriggerPosition(aState).y);
		lua_pushnumber(aState, CTriggerVolumeManager::GetInstance().GetTriggerPosition(aState).z);
		return 3;
	}
	int LoadSpriteInTrigger(lua_State * aState)
	{
		return CTriggerVolumeManager::GetInstance().LoadSprite(aState);
	}
	int LoadSpriteInTriggerAtPosition(lua_State * aState)
	{
		return CTriggerVolumeManager::GetInstance().LoadSpriteAtPosition(aState);
	}
	//int SetTextInTrigger(lua_State * aState)
	//{
	//	return CTriggerVolumeManager::GetInstance().SetText(aState);
	//}
	int GetPlayerPosition(lua_State * aState)
	{
		lua_pushnumber(aState, LuaExposedVariableWrapper::GetInstance().GetPlayerPosition().x);
		lua_pushnumber(aState, LuaExposedVariableWrapper::GetInstance().GetPlayerPosition().y);
		lua_pushnumber(aState, LuaExposedVariableWrapper::GetInstance().GetPlayerPosition().z);
		return 3;
	}
	int IncrementObjectiveCounter(lua_State *)
	{
		CObjectiveManager::GetInstance().IncrementObjectiveCounter();
		return 0;
	}
	int DecrementObjectiveCounter(lua_State *)
	{
		CObjectiveManager::GetInstance().DecrementObjectiveCounter();
		return 0;
	}
	int GetObjectiveCounter(lua_State * aState)
	{
		lua_pushinteger(aState, CObjectiveManager::GetInstance().GetObjectiveCounter());
		return 1;
	}
	int ResetObjectiveCounter(lua_State *)
	{
		return CObjectiveManager::GetInstance().Reset();
	}
	int SpawnTikiAtPosition(lua_State * aState)
	{
		SMessage mess;
		mess.messageType = eMessageType::BuildTikiAtPosition;

		if (lua_isinteger(aState, 1))
		{
			mess.buildTikiAtPosition.x = cast_f(lua_tointeger(aState, 1));
		}
		else if (lua_isnumber(aState, 1))
		{
			mess.buildTikiAtPosition.x = cast_f(lua_tonumber(aState, 1));
		}
		else
		{
			DL_LUA_ERROR(aState, "Invalid argument at slot X for SpawnTikiAtPosition");
			return 0;
		}

		if (lua_isinteger(aState, 2))
		{
			mess.buildTikiAtPosition.y = cast_f(lua_tointeger(aState, 2));
		}
		else if (lua_isnumber(aState, 2))
		{
			mess.buildTikiAtPosition.y = cast_f(lua_tonumber(aState, 2));
		}
		else
		{
			DL_LUA_ERROR(aState, "Invalid argument at slot Y for SpawnTikiAtPosition");
			return 0;
		}

		if (lua_isinteger(aState, 3))
		{
			mess.buildTikiAtPosition.z = cast_f(lua_tointeger(aState, 3));
		}
		else if (lua_isnumber(aState, 3))
		{
			mess.buildTikiAtPosition.z = cast_f(lua_tonumber(aState, 3));
		}
		else
		{
			DL_LUA_ERROR(aState, "Invalid argument at slot Z for SpawnTikiAtPosition");
			return 0;
		}

		CPostMaster::GetInstance().PostMessageA(mess);
		return 0;
	}
	int SpawnTikiAtPositionIdle(lua_State * aState)
	{
		SMessage mess;
		mess.messageType = eMessageType::BuildTikiAtPosition_Idle;

		if (lua_isinteger(aState, 1))
		{
			mess.buildTikiAtPosition.x = cast_f(lua_tointeger(aState, 1));
		}
		else if (lua_isnumber(aState, 1))
		{
			mess.buildTikiAtPosition.x = cast_f(lua_tonumber(aState, 1));
		}
		else
		{
			DL_LUA_ERROR(aState, "Invalid argument at slot X for SpawnTikiAtPosition");
			return 0;
		}

		if (lua_isinteger(aState, 2))
		{
			mess.buildTikiAtPosition.y = cast_f(lua_tointeger(aState, 2));
		}
		else if (lua_isnumber(aState, 2))
		{
			mess.buildTikiAtPosition.y = cast_f(lua_tonumber(aState, 2));
		}
		else
		{
			DL_LUA_ERROR(aState, "Invalid argument at slot Y for SpawnTikiAtPosition");
			return 0;
		}

		if (lua_isinteger(aState, 3))
		{
			mess.buildTikiAtPosition.z = cast_f(lua_tointeger(aState, 3));
		}
		else if (lua_isnumber(aState, 3))
		{
			mess.buildTikiAtPosition.z = cast_f(lua_tonumber(aState, 3));
		}
		else
		{
			DL_LUA_ERROR(aState, "Invalid argument at slot Z for SpawnTikiAtPosition");
			return 0;
		}

		CPostMaster::GetInstance().PostMessageA(mess);
		return 0;
	}
	int SpawnTikiAtPositionUpToLimit(lua_State * aState)
	{
		int count = LuaExposedVariableWrapper::GetInstance().GetTikiCount();
		int lim = LuaExposedVariableWrapper::GetInstance().GetTikiLimit();

		Vector3f pos;

		if (lua_isinteger(aState, 1))
		{
			pos.x = cast_f(lua_tointeger(aState, 1));
		}
		else if (lua_isnumber(aState, 1))
		{
			pos.x = cast_f(lua_tonumber(aState, 1));
		}
		else
		{
			DL_LUA_ERROR(aState, "Invalid argument at slot X for SpawnTikiAtPosition_Limited");
			return 0;
		}

		if (lua_isinteger(aState, 2))
		{
			pos.y = cast_f(lua_tointeger(aState, 2));
		}
		else if (lua_isnumber(aState, 2))
		{
			pos.y = cast_f(lua_tonumber(aState, 2));
		}
		else
		{
			DL_LUA_ERROR(aState, "Invalid argument at slot Y for SpawnTikiAtPosition_Limited");
			return 0;
		}

		if (lua_isinteger(aState, 3))
		{
			pos.z = cast_f(lua_tointeger(aState, 3));
		}
		else if (lua_isnumber(aState, 3))
		{
			pos.z = cast_f(lua_tonumber(aState, 3));
		}
		else
		{
			DL_LUA_ERROR(aState, "Invalid argument at slot Z for SpawnTikiAtPosition_Limited");
			return 0;
		}


		for (int i = count; i < lim; ++i)
		{
			SMessage mess;
			mess.messageType = eMessageType::BuildTikiAtPosition;
			mess.buildTikiAtPosition.x = pos.x + 0.01f * i;
			mess.buildTikiAtPosition.y = pos.y;
			mess.buildTikiAtPosition.z = pos.z + 0.01f * i;
			CPostMaster::GetInstance().PostMessageA(mess);
		}
		return 0;
	}
	int GetTotalTikiCount(lua_State * aState)
	{
		int count = LuaExposedVariableWrapper::GetInstance().GetTikiCount();
		lua_pushinteger(aState, count);
		return 1;
	}
	int GetTotalTikiLimit(lua_State * aState)
	{
		int limit = LuaExposedVariableWrapper::GetInstance().GetTikiLimit();
		lua_pushinteger(aState, limit);
		return 1;
	}
	int SetTikiCountLimit(lua_State * aState)
	{
		int newLimit;
		if (lua_isinteger(aState, 1))
		{
			newLimit = cast_int(lua_tointeger(aState, 1));
		}
		else if (lua_isnumber(aState, 1))
		{
			newLimit = cast_int(lua_tonumber(aState, 1));
		}
		else
		{
			return DL_LUA_ERROR(aState, "Couldnt set limit to a valid number");
		}
		LuaExposedVariableWrapper::GetInstance().SetTikiLimit(newLimit);
		return 0;
	}
	int KillTikiWithID(lua_State * aState)
	{

		if (lua_isstring(aState, 1))
		{
			SMessage mess;
			mess.messageType = eMessageType::KillTiki;

			mess.killTiki.ID = lua_tostring(aState, 1);
			CPostMaster::GetInstance().PostMessageA(mess);
		}
		else
		{
			return DL_LUA_ERROR(aState, "Invalid ID to kill tiki with");
		}

		return 0;
	}

	int SpawnTikiAtPlayer(lua_State *)
	{
		Vector3f playerPos;
		playerPos = LuaExposedVariableWrapper::GetInstance().GetPlayerPosition();

		SMessage mess;
		mess.messageType = eMessageType::BuildTikiAtPosition;

		const float distance = 1.5f;

		mess.buildTikiAtPosition.x = playerPos.x + sin(Corgi::RandomFloat(0, 2* PI)) * distance;
		mess.buildTikiAtPosition.y = playerPos.y;
		mess.buildTikiAtPosition.z = playerPos.z  +cos(Corgi::RandomFloat(0, 2 * PI)) * distance;

		CPostMaster::GetInstance().PostMessageA(mess);



		return 0;
	}

	int PlaySoundAtPosition(lua_State * aState)
	{
		const char* event;

		if (lua_isstring(aState, 1))
		{
			event = lua_tostring(aState, 1);
		}
		else
		{
			return DL_LUA_ERROR(aState, "Must be string");
		}

		Corgi::Vector3<float> pos;

		if (lua_isinteger(aState, 2))
		{
			pos.x = cast_f(lua_tointeger(aState, 2));
		}
		else if (lua_isnumber(aState, 2))
		{
			pos.x = cast_f(lua_tonumber(aState, 2));
		}
		else
		{
			return DL_LUA_ERROR(aState, "Invalid X coordinate");
		}

		if (lua_isinteger(aState, 3))
		{
			pos.y = cast_f(lua_tointeger(aState, 3));
		}
		else if (lua_isnumber(aState, 3))
		{
			pos.y = cast_f(lua_tonumber(aState, 3));
		}
		else
		{
			return DL_LUA_ERROR(aState, "Invalid Y coordinate");
		}

		if (lua_isinteger(aState, 4))
		{
			pos.z = cast_f(lua_tointeger(aState, 4));
		}
		else if (lua_isnumber(aState, 4))
		{
			pos.z = cast_f(lua_tonumber(aState, 4));
		}
		else
		{
			return DL_LUA_ERROR(aState, "Invalid Z coordinate");
		}

		CWwiseWrapper::GetInstance().PlaySoundAtPosition(event, pos);

		return 0;
	}

	int IsTiki(lua_State * aState)
	{
		bool isTiki;
		if (lua_isstring(aState, 1))
		{
			std::string tikiid(lua_tostring(aState, 1));
			isTiki = tikiid.find("Tiki") != -1;
		}
		else
		{
			return DL_LUA_ERROR(aState, "Invalid ID");
		}
		lua_pushboolean(aState, isTiki);
		return 1;
	}

	int IsInState(lua_State * L)
	{
		std::string id;
		std::string state;

		if (lua_isstring(L, 1))
		{
			id = lua_tostring(L, 1);
		}
		else
		{
			DL_LUA_ERROR(L, "invalid string in arg 1");
			lua_pushboolean(L, false);
			return 1;
		}

		if (lua_isstring(L, 2))
		{
			state = lua_tostring(L, 2);
		}
		else
		{
			DL_LUA_ERROR(L, "invalid string in arg 2");
			lua_pushboolean(L, false);
			return 1;
		}


		Corgi::CMemoryPool<CEntity>* entPoolPtr = LuaExposedVariableWrapper::GetInstance().GetEntityPoolPtr();
		Corgi::GrowingArray<long long>* updIdsPtr = LuaExposedVariableWrapper::GetInstance().GetUpdateableIDsPtr();

		if (!entPoolPtr || !updIdsPtr)
		{
			DL_LUA_ERROR(L, "Invalid pointers to entity arrays - this is a C++ side error!");
			lua_pushboolean(L, false);
			return 1;
		}

		for (long long LLID : *updIdsPtr)
		{
			CEntity& entity = (*entPoolPtr)[LLID];
			if (entity.GetEntityType() == EEntityType::Tiki && entity.GetID() == id)
			{
				CActorComponent* actorComp = dynamic_cast<CActorComponent*>(entity.GetComponent(eComponentTag::Actor));
				
				Corgi::GrowingArray<CController*> controllers = actorComp->GetController();
			}
		}

		lua_pushboolean(L, false);
		return 1;
	}

	int ChangeLevel(lua_State * aState)
	{
		SMessage mess;
		mess.messageType = eMessageType::ChangeLevelName;

		if (lua_isstring(aState, 1))
		{
			mess.changeLevelName.levelName = lua_tostring(aState, 1);
		}
		else
		{
			return DL_LUA_ERROR(aState, "Invalid level name");
		}

		CPostMaster::GetInstance().PostMessageA(mess);

		return 0;
	}
	int MoveObjectToPosition(lua_State * aState)
	{
		SMessage mess;
		mess.messageType = eMessageType::MoveObjectToPosition;

		if (lua_isstring(aState, 1))
		{
			mess.moveObjectToPosition.nameID = lua_tostring(aState, 1);
		}
		else
		{
			return DL_LUA_ERROR(aState, "Invalid ID");
		}

		if (lua_isinteger(aState, 2))
		{
			mess.moveObjectToPosition.x = cast_f(lua_tointeger(aState, 2));
		}
		else if (lua_isnumber(aState, 2))
		{
			mess.moveObjectToPosition.x = cast_f(lua_tonumber(aState, 2));
		}
		else
		{
			return DL_LUA_ERROR(aState, "Invalid X coordinate");
		}

		if (lua_isinteger(aState, 3))
		{
			mess.moveObjectToPosition.y = cast_f(lua_tointeger(aState, 3));
		}
		else if (lua_isnumber(aState, 3))
		{
			mess.moveObjectToPosition.y = cast_f(lua_tonumber(aState, 3));
		}
		else
		{
			return DL_LUA_ERROR(aState, "Invalid Y coordinate");
		}

		if (lua_isinteger(aState, 4))
		{
			mess.moveObjectToPosition.z = cast_f(lua_tointeger(aState, 4));
		}
		else if (lua_isnumber(aState, 4))
		{
			mess.moveObjectToPosition.z = cast_f(lua_tonumber(aState, 4));
		}
		else
		{
			return DL_LUA_ERROR(aState, "Invalid Z coordinate");
		}


		if (lua_isinteger(aState, 5))
		{
			mess.moveObjectToPosition.speed = cast_f(lua_tointeger(aState, 5));
		}
		else if (lua_isnumber(aState, 5))
		{
			mess.moveObjectToPosition.speed = cast_f(lua_tonumber(aState, 5));
		}
		else
		{
			return DL_LUA_ERROR(aState, "Invalid speed modifier");
		}

		CPostMaster::GetInstance().PostMessageA(mess);

		return 0;
	}
	int MoveObjectToRelativePosition(lua_State * aState)
	{

		Vector3f triggerPos = CTriggerVolumeManager::GetInstance().GetTriggerPosition(aState);

		SMessage mess;
		mess.messageType = eMessageType::MoveObjectToPositionRelative;

		if (lua_isstring(aState, 1))
		{
			mess.moveObjectToPosition.nameID = lua_tostring(aState, 1);
		}
		else
		{
			return DL_LUA_ERROR(aState, "Invalid ID");
		}

		if (lua_isinteger(aState, 2))
		{
			mess.moveObjectToPosition.x = cast_f(lua_tointeger(aState, 2));
		}
		else if (lua_isnumber(aState, 2))
		{
			mess.moveObjectToPosition.x = cast_f(lua_tonumber(aState, 2));
		}
		else
		{
			return DL_LUA_ERROR(aState, "Invalid X coordinate");
		}

		if (lua_isinteger(aState, 3))
		{
			mess.moveObjectToPosition.y = cast_f(lua_tointeger(aState, 3));
		}
		else if (lua_isnumber(aState, 3))
		{
			mess.moveObjectToPosition.y = cast_f(lua_tonumber(aState, 3));
		}
		else
		{
			return DL_LUA_ERROR(aState, "Invalid Y coordinate");
		}

		if (lua_isinteger(aState, 4))
		{
			mess.moveObjectToPosition.z = cast_f(lua_tointeger(aState, 4));
		}
		else if (lua_isnumber(aState, 4))
		{
			mess.moveObjectToPosition.z = cast_f(lua_tonumber(aState, 4));
		}
		else
		{
			return DL_LUA_ERROR(aState, "Invalid Z coordinate");
		}


		if (lua_isinteger(aState, 5))
		{
			mess.moveObjectToPosition.speed = cast_f(lua_tointeger(aState, 5));
		}
		else if (lua_isnumber(aState, 5))
		{
			mess.moveObjectToPosition.speed = cast_f(lua_tonumber(aState, 5));
		}
		else
		{
			return DL_LUA_ERROR(aState, "Invalid speed modifier");
		}

		CPostMaster::GetInstance().PostMessageA(mess);

		return 0;
	}
	int RotateObject(lua_State * aState)
	{
		SMessage mess;
		mess.messageType = eMessageType::RotateObject;

		if (lua_isstring(aState, 1))
		{
			mess.rotateObject.name = lua_tostring(aState, 1);
		}
		else
		{
			return DL_LUA_ERROR(aState, "Invalid ID");
		}

		if (lua_isinteger(aState, 2))
		{
			mess.rotateObject.x = cast_f(lua_tointeger(aState, 2));
		}
		else if (lua_isnumber(aState, 2))
		{
			mess.rotateObject.x = cast_f(lua_tonumber(aState, 2));
		}
		else
		{
			return DL_LUA_ERROR(aState, "Invalid X coordinate");
		}

		if (lua_isinteger(aState, 3))
		{
			mess.rotateObject.y = cast_f(lua_tointeger(aState, 3));
		}
		else if (lua_isnumber(aState, 3))
		{
			mess.rotateObject.y = cast_f(lua_tonumber(aState, 3));
		}
		else
		{
			return DL_LUA_ERROR(aState, "Invalid Y coordinate");
		}

		if (lua_isinteger(aState, 4))
		{
			mess.rotateObject.z = cast_f(lua_tointeger(aState, 4));
		}
		else if (lua_isnumber(aState, 4))
		{
			mess.rotateObject.z = cast_f(lua_tonumber(aState, 4));
		}
		else
		{
			return DL_LUA_ERROR(aState, "Invalid Z coordinate");
		}


		if (lua_isinteger(aState, 5))
		{
			mess.rotateObject.speed = cast_f(lua_tointeger(aState, 5));
		}
		else if (lua_isnumber(aState, 5))
		{
			mess.rotateObject.speed = cast_f(lua_tonumber(aState, 5));
		}
		else
		{
			return DL_LUA_ERROR(aState, "Invalid speed modifier");
		}

		CPostMaster::GetInstance().PostMessageA(mess);

		return 0;
	}
	int AnimateObject(lua_State * aState)
	{
		SMessage mess;
		mess.messageType = eMessageType::StartAnimationOnObject;

		if (lua_isstring(aState, 1))
		{
			mess.startAnimationOnObject.nameID = lua_tostring(aState, 1);
		}
		else
		{
			return DL_LUA_ERROR(aState, "Invalid object ID (argument 1)");
		}

		if (lua_isstring(aState, 2))
		{
			mess.startAnimationOnObject.animationName = lua_tostring(aState, 2);
		}
		else
		{
			return DL_LUA_ERROR(aState, "Invalid animation name (argument 2)");
		}
		CPostMaster::GetInstance().PostMessageA(mess);

		return 0;
	}
	int DisableCollider(lua_State * aState)
	{

		SMessage message;
		message.messageType = eMessageType::DisableCollider;

		if (lua_isstring(aState, 1))
		{
			message.disableCollider.id = lua_tostring(aState, 1);

		}
		else
		{
			DL_LUA_ERROR(aState, "INVALID STRING ID");
		}

		CPostMaster::GetInstance().PostMessageA(message);
		return 0;
	}

	int SetPlayerSpeed(lua_State * aState)
	{
		if (lua_isnumber(aState, 1))
		{

		SMessage message;
		message.messageType = eMessageType::PlayerData;
		message.playerData.myPlayerSpeed = cast_f(lua_tonumber(aState, 1));

		CPostMaster::GetInstance().PostMessageA(message);
		}
		else
		{
			DL_LUA_ERROR(aState, "Invalid number in player speed");
		}
		return 0;
	}

	int CreateParticleEmitter(lua_State * aState)
	{

		SMessage mess;
		mess.messageType = eMessageType::CreateParticleEmitter;

		if (lua_isstring(aState, 1))
		{
			mess.createParticleEmitter.particlesystem = lua_tostring(aState, 1);
		}
		else
		{
			return DL_LUA_ERROR(aState, "Invalid Particle system");
		}

		if (lua_isinteger(aState, 2))
		{
			mess.createParticleEmitter.x = cast_f(lua_tointeger(aState, 2));
		}
		else if (lua_isnumber(aState, 2))
		{
			mess.createParticleEmitter.x = cast_f(lua_tonumber(aState, 2));
		}
		else
		{
			return DL_LUA_ERROR(aState, "Invalid X coordinate");
		}

		if (lua_isinteger(aState, 3))
		{
			mess.createParticleEmitter.y = cast_f(lua_tointeger(aState, 3));
		}
		else if (lua_isnumber(aState, 3))
		{
			mess.createParticleEmitter.y = cast_f(lua_tonumber(aState, 3));
		}
		else
		{
			return DL_LUA_ERROR(aState, "Invalid Y coordinate");
		}

		if (lua_isinteger(aState, 4))
		{
			mess.createParticleEmitter.z = cast_f(lua_tointeger(aState, 4));
		}
		else if (lua_isnumber(aState, 4))
		{
			mess.createParticleEmitter.z = cast_f(lua_tonumber(aState, 4));
		}
		else
		{
			return DL_LUA_ERROR(aState, "Invalid Z coordinate");
		}

		CPostMaster::GetInstance().PostMessageA(mess);

		return 0;
	}

	int SwitchNavmesh(lua_State * L)
	{

		if (lua_isstring(L, 1))
		{
			SMessage mess;
			mess.messageType = eMessageType::ChangeNavmesh;
			mess.changeNavmesh.name = lua_tostring(L, 1);

			CPostMaster::GetInstance().PostMessage(mess);
			return 0;
		}
		return DL_LUA_ERROR(L, "Invalid string in change navmesh");
	}

}