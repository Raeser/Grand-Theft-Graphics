#pragma once
#include "..\Engine\Subscriber.h"

struct lua_State;

namespace LuaExposedFunction
{
	int Print(lua_State * aState);

	int DebugPrintText(lua_State* aState);
	int DebugDrawLine(lua_State* aState);
	int DebugDrawCube(lua_State* aState);
	int DebugDrawArrow(lua_State* aState);
	int DrawFullNav(lua_State*);
	int DrawLocalNav(lua_State*);
	int DrawFullEdges(lua_State*);
	int DrawLocalEdges(lua_State*);
	int DrawNavMeshColliders(lua_State*);
	int DrawWallColliders(lua_State*);
	int DrawLDDebug(lua_State*);
	
	int TextToSpeech(lua_State * aState);

	int RegisterCallback(lua_State * aState);
	int UnregisterCallback(lua_State * aState);
	int Sleep(lua_State* aState);

	int GetTikiCountInTrigger(lua_State* aState);
	int GetHelp(lua_State* aState);
	int GetTriggerPosition(lua_State* aState);
	int LoadSpriteInTrigger(lua_State* aState);
	int LoadSpriteInTriggerAtPosition(lua_State* aState);

	//int SetTextInTrigger(lua_State* aState);

	int GetPlayerPosition(lua_State* aState);

	int IncrementObjectiveCounter(lua_State*);
	int DecrementObjectiveCounter(lua_State*);
	int GetObjectiveCounter(lua_State*);
	int ResetObjectiveCounter(lua_State*);

	int SpawnTikiAtPosition(lua_State* aState);
	int SpawnTikiAtPositionIdle(lua_State* aState);
	int SpawnTikiAtPositionUpToLimit(lua_State* aState);
	int GetTotalTikiCount(lua_State* aState);
	int GetTotalTikiLimit(lua_State* aState);
	int SetTikiCountLimit(lua_State* aState);

	int KillTikiWithID(lua_State* aState);

	int SpawnTikiAtPlayer(lua_State*);

	int PlaySoundAtPosition(lua_State* aState);

	int IsTiki(lua_State* aState);
	int IsInState(lua_State* L);

	int ChangeLevel(lua_State* aState);

	int MoveObjectToPosition(lua_State* aState);
	int MoveObjectToRelativePosition(lua_State* aState);
	int RotateObject(lua_State* aState);


	int ToggleTriggerWireframe(lua_State* aState);

	int AnimateObject(lua_State* aState);

	int DisableCollider(lua_State* aState);

	int SetPlayerSpeed(lua_State* aState);

	int CreateParticleEmitter(lua_State* aState);

	int SwitchNavmesh(lua_State* aState);


}


