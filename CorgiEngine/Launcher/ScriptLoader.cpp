#include "stdafx.h"
#include "ScriptLoader.h"
#include "LuaExposedFunction.h"


#define SCRIPT_LOAD_SIZE 64
#define SCRIPT_FILE_SIZE 8

struct SScriptLoadData
{
	const char* functionName;
	int(*functionPtr)(lua_State*);
	const char* argList;
	const char* helpText;
	bool ENABLE_IN_RETAIL = true;
	bool CALLABLE_FROM_CONSOLE = true;
};


CScriptLoader::CScriptLoader()
{
	myScriptLoadData.Init(SCRIPT_LOAD_SIZE);
	myFilesToRead.Init(SCRIPT_FILE_SIZE);
}


CScriptLoader::~CScriptLoader()
{
}

void CScriptLoader::LoadScripts()
{
	SScriptLoadData print;
	print.functionName = "Print";
	print.functionPtr = LuaExposedFunction::Print;
	print.argList = "Any amount of values";
	print.helpText = "This function prints any value you send it";
	myScriptLoadData.Add(print);
#ifndef _RETAIL
	SScriptLoadData printOnScreen;
	printOnScreen.functionName = "PrintOnScreen";
	printOnScreen.functionPtr = LuaExposedFunction::DebugPrintText;
	printOnScreen.argList = "Any amount of values";
	printOnScreen.helpText = "This function prints any value you send it onto the screen";
	myScriptLoadData.Add(printOnScreen);

	SScriptLoadData drawLine;
	drawLine.functionName = "DrawLine";
	drawLine.functionPtr = LuaExposedFunction::DebugDrawLine;
	drawLine.argList = "float \"x From\", float \"y From\", float \"z From\", float \"x To\", float \"y To\", float \"z To\"";
	drawLine.helpText = "Renders a line on from the provided \" FromPosition\" to the provided \"ToPosition\"";
	myScriptLoadData.Add(drawLine);

	SScriptLoadData drawArrow;
	drawArrow.functionName = "DrawArrow";
	drawArrow.functionPtr = LuaExposedFunction::DebugDrawArrow;
	drawArrow.argList = "float \"x From\", float \"y From\", float \"z From\", float \"x To\", float \"y To\", float \"z To\"";
	drawArrow.helpText = "Renders an arrow on from the provided \" FromPosition\" to the provided \"ToPosition\"";
	myScriptLoadData.Add(drawArrow);

	SScriptLoadData SpawnCube;
	SpawnCube.functionName = "SpawnCube";
	SpawnCube.functionPtr = LuaExposedFunction::DebugDrawCube;
	SpawnCube.argList = "float x, y, z";
	SpawnCube.helpText = "Spawns a cube on hte xyz position ";
	myScriptLoadData.Add(SpawnCube);
#endif

	SScriptLoadData tts;
	tts.functionName = "TextToSpeech";
	tts.functionPtr = LuaExposedFunction::TextToSpeech;
	tts.argList = "any amount of values";
	tts.helpText = "Converts your input to speech";
	myScriptLoadData.Add(tts);

	SScriptLoadData playerSpeed;
	playerSpeed.functionName = "SetPlayerSpeed";
	playerSpeed.functionPtr = LuaExposedFunction::SetPlayerSpeed;
	playerSpeed.argList = "float SPEED";
	playerSpeed.helpText = "Sets players speed to SPEED";
	myScriptLoadData.Add(playerSpeed);

	SScriptLoadData regCallback;
	regCallback.functionName = "RegisterCallback";
	regCallback.functionPtr = LuaExposedFunction::RegisterCallback;
	regCallback.argList = "string EVENT, string NAME_ID, string FUNCTION, ARGS ... ";
	regCallback.helpText = "Whenever EVENT occurs to object with NAME_ID, calls your LUA function FUNCTION with the variable amount of arguments ARGS...";
	regCallback.CALLABLE_FROM_CONSOLE = false;
	myScriptLoadData.Add(regCallback);

	SScriptLoadData sleep;
	sleep.functionName = "Sleep";
	sleep.functionPtr = LuaExposedFunction::Sleep;
	sleep.argList = "float TIME, string FUNCTION, ARGS ... ";
	sleep.helpText = "calls your LUA function FUNCTION with the variable amount of arguments ARGS..., after TIME milliseconds";
	sleep.CALLABLE_FROM_CONSOLE = false;
	myScriptLoadData.Add(sleep);

	SScriptLoadData unregCallback;
	unregCallback.functionName = "UnregisterCallback";
	unregCallback.functionPtr = LuaExposedFunction::UnregisterCallback;
	unregCallback.argList = "string EVENT, string NAME_ID";
	unregCallback.helpText = "Unregisters NAME_ID from the list of objects to look for EVENT with";
	unregCallback.CALLABLE_FROM_CONSOLE = false;
	myScriptLoadData.Add(unregCallback);

	SScriptLoadData getHelp;
	getHelp.functionName = "Help";
	getHelp.functionPtr = LuaExposedFunction::GetHelp;
	getHelp.argList = "";
	getHelp.helpText = "Prints all functions that can be used in lua or console";
#ifdef _RETAIL
	getHelp.CALLABLE_FROM_CONSOLE = false;
#endif
	myScriptLoadData.Add(getHelp);

	SScriptLoadData getTikiCount;
	getTikiCount.functionName = "GetTikiCount";
	getTikiCount.functionPtr = LuaExposedFunction::GetTikiCountInTrigger;
	getTikiCount.argList = "";
	getTikiCount.helpText = "Returns the amount of tikis in the trigger with that id as a single integer";
	getTikiCount.CALLABLE_FROM_CONSOLE = false;
	myScriptLoadData.Add(getTikiCount);

	SScriptLoadData isinstate;
	isinstate.functionName = "TikiIsInState";
	isinstate.functionPtr = LuaExposedFunction::IsInState;
	isinstate.argList = "string ID, string STATE";
	isinstate.helpText = "Returns true if Tiki with id ID is in state STATE \nSTATES: \nFollowing\nIdle\nBounce\nThrown\nPreThrow\nDying";
	isinstate.CALLABLE_FROM_CONSOLE = false;
	myScriptLoadData.Add(isinstate);

	SScriptLoadData disCollider;
	disCollider.functionName = "DisableColliderOnObject";
	disCollider.functionPtr = LuaExposedFunction::DisableCollider;
	disCollider.argList = "string ID";
	disCollider.helpText = "Disables the navigation mesh collider linked to the object ID";
	myScriptLoadData.Add(disCollider);

	SScriptLoadData changeNavmesh;
	changeNavmesh.functionName = "ChangeNavmesh";
	changeNavmesh.functionPtr = LuaExposedFunction::SwitchNavmesh;
	changeNavmesh.argList = "string NAVMESH";
	changeNavmesh.helpText = "Replaces the current navigation mesh with NAVMESH";
	changeNavmesh.CALLABLE_FROM_CONSOLE = true;
	myScriptLoadData.Add(changeNavmesh);

	SScriptLoadData createEmitter;
	createEmitter.functionName = "CreateParticleEmitterAtPosition";
	createEmitter.functionPtr = LuaExposedFunction::CreateParticleEmitter;
	createEmitter.argList = "string ParticleSystem, float x, y, z";
	createEmitter.helpText = "Creates a particle emitter from the file ParticleSystem, at position (x, y, z)";
	myScriptLoadData.Add(createEmitter);

	SScriptLoadData loadSprite;
	loadSprite.functionName = "LoadSprite";
	loadSprite.functionPtr = LuaExposedFunction::LoadSpriteInTrigger;
	loadSprite.argList = "string SpritePath";
	loadSprite.helpText = "Loads and renders a sprite at the triggers position";
	loadSprite.CALLABLE_FROM_CONSOLE = false;
	myScriptLoadData.Add(loadSprite);

	SScriptLoadData loadSpritePos;
	loadSpritePos.functionName = "LoadSpriteAtPosition";
	loadSpritePos.functionPtr = LuaExposedFunction::LoadSpriteInTriggerAtPosition;
	loadSpritePos.argList = "string SpritePath, float x, float y, float z";
	loadSpritePos.helpText = "Loads and renders a sprite at the (x, y, z) position";
	myScriptLoadData.Add(loadSpritePos);

	//SScriptLoadData setText;
	//setText.functionName = "SetText";
	//setText.functionPtr = LuaExposedFunction::SetTextInTrigger;
	//setText.argList = "string TEXT";
	//setText.helpText = "Renders TEXT at the triggers position";
	//myScriptLoadData.Add(setText);

	SScriptLoadData incObj;
	incObj.functionName = "IncrementObjectiveCounter";
	incObj.functionPtr = LuaExposedFunction::IncrementObjectiveCounter;
	incObj.argList = "";
	incObj.helpText = "Increments the global objective counter";
	incObj.CALLABLE_FROM_CONSOLE = false;
	myScriptLoadData.Add(incObj);

	SScriptLoadData decObj;
	decObj.functionName = "DecrementObjectiveCounter";
	decObj.functionPtr = LuaExposedFunction::DecrementObjectiveCounter;
	decObj.argList = "";
	decObj.helpText = "Decrements the global objective counter";
	decObj.CALLABLE_FROM_CONSOLE = false;
	myScriptLoadData.Add(decObj);

	SScriptLoadData getObj;
	getObj.functionName = "GetObjectiveCounter";
	getObj.functionPtr = LuaExposedFunction::GetObjectiveCounter;
	getObj.argList = "";
	getObj.helpText = "Returns the global objective counter as an integer";
	getObj.CALLABLE_FROM_CONSOLE = false;
	myScriptLoadData.Add(getObj);

	SScriptLoadData resObj;
	resObj.functionName = "ResetObjectiveCounter";
	resObj.functionPtr = LuaExposedFunction::ResetObjectiveCounter;
	resObj.argList = "";
	resObj.helpText = "Resets the global objective counter to Zero (0)";
	resObj.CALLABLE_FROM_CONSOLE = false;
	myScriptLoadData.Add(resObj);

	SScriptLoadData rotateObject;
	rotateObject.functionName = "RotateObject";
	rotateObject.functionPtr = LuaExposedFunction::RotateObject;
	rotateObject.argList = "string ID, float x, float y, float z, float speed";
	rotateObject.helpText = "Rotates ID around axises (x, y, z) with speed";
	myScriptLoadData.Add(rotateObject);

	SScriptLoadData tikiatplayer;
	tikiatplayer.functionName = "SpawnTikiAtPlayer";
	tikiatplayer.functionPtr = LuaExposedFunction::SpawnTikiAtPlayer;
	tikiatplayer.argList = "";
	tikiatplayer.helpText = "Spawns a tiki boy at the players position";
	myScriptLoadData.Add(tikiatplayer);

	SScriptLoadData tikispawn;
	tikispawn.functionName = "SpawnTikiAtPosition";
	tikispawn.functionPtr = LuaExposedFunction::SpawnTikiAtPosition;
	tikispawn.argList = "float x, float y, float z";
	tikispawn.helpText = "Spawns a tiki boy at this position";
	myScriptLoadData.Add(tikispawn);

	SScriptLoadData tikispawnIdle;
	tikispawnIdle.functionName = "SpawnTikiAtPosition_Idle";
	tikispawnIdle.functionPtr = LuaExposedFunction::SpawnTikiAtPositionIdle;
	tikispawnIdle.argList = "float x, float y, float z";
	tikispawnIdle.helpText = "Spawns a tiki boy at this position, but it wont do anything but idle. Its literally terrain-boi";
	myScriptLoadData.Add(tikispawnIdle);

	SScriptLoadData tikispawnlimit;
	tikispawnlimit.functionName = "SpawnTikiAtPosition_Limited";
	tikispawnlimit.functionPtr = LuaExposedFunction::SpawnTikiAtPositionUpToLimit;
	tikispawnlimit.argList = "float x, float y, float z";
	tikispawnlimit.helpText = "Spawns a tiki boy at this position, until the limit is reached";
	myScriptLoadData.Add(tikispawnlimit);

	SScriptLoadData changelvl;
	changelvl.functionName = "ChangeLevel";
	changelvl.functionPtr = LuaExposedFunction::ChangeLevel;
	changelvl.argList = "string LevelName.json";
	changelvl.helpText = "This function changes the level";
	myScriptLoadData.Add(changelvl);

	SScriptLoadData gettriggerpos;
	gettriggerpos.functionName = "GetTriggerPosition";
	gettriggerpos.functionPtr = LuaExposedFunction::GetTriggerPosition;
	gettriggerpos.argList = "";
	gettriggerpos.helpText = "Returns the triggers position as a vector3 table {x, y, z}";
	gettriggerpos.CALLABLE_FROM_CONSOLE = false;
	myScriptLoadData.Add(gettriggerpos);

	SScriptLoadData getplayerpos;
	getplayerpos.functionName = "GetPlayerPosition";
	getplayerpos.functionPtr = LuaExposedFunction::GetPlayerPosition;
	getplayerpos.argList = "";
	getplayerpos.helpText = "Returns the players position as a vector3 table {x, y, z}";
	myScriptLoadData.Add(getplayerpos);

	SScriptLoadData killTIki;
	killTIki.functionName = "KillTiki";
	killTIki.functionPtr = LuaExposedFunction::KillTikiWithID;
	killTIki.argList = "string ObjectName";
	killTIki.helpText = "Murders Mr.ObjectName. Horribly.";
	myScriptLoadData.Add(killTIki);

	SScriptLoadData isTiki;
	isTiki.functionName = "IsTiki";
	isTiki.functionPtr = LuaExposedFunction::IsTiki;
	isTiki.argList = "string ObjectName";
	isTiki.helpText = "Returns true if Tiki, false if not";
	isTiki.CALLABLE_FROM_CONSOLE = false;
	myScriptLoadData.Add(isTiki);

	SScriptLoadData moveObject;
	moveObject.functionName = "MoveObjectToPosition";
	moveObject.functionPtr = LuaExposedFunction::MoveObjectToPosition;
	moveObject.argList = "string ObjectName, float x, float y, float z, float speed";
	moveObject.helpText = "Start to move <ObjectName> to <x, y, z> with the speed of <speed>";
	myScriptLoadData.Add(moveObject);

	SScriptLoadData moveObjectRel;
	moveObjectRel.functionName = "MoveObjectToPosition_Relative";
	moveObjectRel.functionPtr = LuaExposedFunction::MoveObjectToRelativePosition;
	moveObjectRel.argList = "string ObjectName, float x, float y, float z, float speed";
	moveObjectRel.helpText = "Start to move <ObjectName> to <x, y, z> with the speed of <speed>";
	myScriptLoadData.Add(moveObjectRel);

	SScriptLoadData anime;
	anime.functionName = "Animate";
	anime.functionPtr = LuaExposedFunction::AnimateObject;
	anime.argList = "string ObjectName, string AnimationName";
	anime.helpText = "Start to Animate <ObjectName> with animation <AnimationName>";
	myScriptLoadData.Add(anime);

	SScriptLoadData toggleWF;
	toggleWF.functionName = "ToggleTriggerWireframes";
	toggleWF.functionPtr = LuaExposedFunction::ToggleTriggerWireframe;
	toggleWF.argList = "";
	toggleWF.helpText = "Toggle the wireframe mode for triggers, when not in a retail environment";
	myScriptLoadData.Add(toggleWF);

	SScriptLoadData setTikiLimit;
	setTikiLimit.functionName = "SetTikiLimit";
	setTikiLimit.functionPtr = LuaExposedFunction::SetTikiCountLimit;
	setTikiLimit.argList = "int NewLimt";
	setTikiLimit.helpText = "Sets the maximum amount of tikis that can spawn to NewLimit";
	myScriptLoadData.Add(setTikiLimit);

	SScriptLoadData getTikiLimit;
	getTikiLimit.functionName = "GetTikiLimit";
	getTikiLimit.functionPtr = LuaExposedFunction::GetTotalTikiLimit;
	getTikiLimit.argList = "";
	getTikiLimit.helpText = "Gets the maximum amount of tikis that can spawn";
	myScriptLoadData.Add(getTikiLimit);

	SScriptLoadData getTotalTikiCount;
	getTotalTikiCount.functionName = "GetTotalTikiCount";
	getTotalTikiCount.functionPtr = LuaExposedFunction::GetTotalTikiCount;
	getTotalTikiCount.argList = "";
	getTotalTikiCount.helpText = "Gets the current amount of tikis that is alive and well";
	myScriptLoadData.Add(getTotalTikiCount);

#ifndef _RETAIL
	SScriptLoadData drawFullNav;
	drawFullNav.functionName = "DrawFullNavMesh";
	drawFullNav.functionPtr = LuaExposedFunction::DrawFullNav;
	drawFullNav.argList = "";
	drawFullNav.helpText = "Toggle to draw the full navigation mesh in the current level";
	myScriptLoadData.Add(drawFullNav);
	SScriptLoadData drawlocalNav;
	drawlocalNav.functionName = "DrawLocalNavMesh";
	drawlocalNav.functionPtr = LuaExposedFunction::DrawLocalNav;
	drawlocalNav.argList = "";
	drawlocalNav.helpText = "Toggle to draw the navigation mesh in the player's current quadTree node";
	myScriptLoadData.Add(drawlocalNav);

	SScriptLoadData drawFullEdges;
	drawFullEdges.functionName = "DrawFullOuterEdges";
	drawFullEdges.functionPtr = LuaExposedFunction::DrawFullEdges;
	drawFullEdges.argList = "";
	drawFullEdges.helpText = "Toggle to draw the outer edges of the navigation mesh in the current level";
	myScriptLoadData.Add(drawFullEdges);

	SScriptLoadData drawLocalEdges;
	drawLocalEdges.functionName = "DrawLocalOuterEdges";
	drawLocalEdges.functionPtr = LuaExposedFunction::DrawLocalEdges;
	drawLocalEdges.argList = "";
	drawLocalEdges.helpText = "Toggle to draw the outer edges of the navigation mesh in the player's current quadTree node";
	myScriptLoadData.Add(drawLocalEdges);

	SScriptLoadData drawNavMeshColliders;
	drawNavMeshColliders.functionName = "DrawNavMeshColliders";
	drawNavMeshColliders.functionPtr = LuaExposedFunction::DrawNavMeshColliders;
	drawNavMeshColliders.argList = "";
	drawNavMeshColliders.helpText = "Toggle to draw the navmesh colliders";
	myScriptLoadData.Add(drawNavMeshColliders);

	SScriptLoadData drawWallColliders;
	drawWallColliders.functionName = "DrawWallColliders";
	drawWallColliders.functionPtr = LuaExposedFunction::DrawWallColliders;
	drawWallColliders.argList = "";
	drawWallColliders.helpText = "Toggle to draw the wall colliders";
	myScriptLoadData.Add(drawWallColliders);

	SScriptLoadData drawLDDebug;
	drawLDDebug.functionName = "DrawLDDebug";
	drawLDDebug.functionPtr = LuaExposedFunction::DrawLDDebug;
	drawLDDebug.argList = "";
	drawLDDebug.helpText = "Toggle draw debug : wallcollider,navmeshcollider, triggercolliders, localnavmesh";
	myScriptLoadData.Add(drawLDDebug);

#endif
	SScriptLoadData playSound;
	playSound.functionName = "PlaySoundAtPosition";
	playSound.functionPtr = LuaExposedFunction::PlaySoundAtPosition;
	playSound.argList = "Wwise Event, PosX, PosY, PosZ";
	playSound.helpText = "Plays an Wwise event at given location";
	myScriptLoadData.Add(playSound);

	InternalScriptLoader();

}

void CScriptLoader::ReadFiles()
{
	myFilesToRead.Add("../Assets/Scripts/CPPRedirect.lua");

	InternalFileReader();
}

void CScriptLoader::SubscribeOnScripts()
{

	LuaExposedVariableWrapper& variables = LuaExposedVariableWrapper::GetInstance();

	CPostMaster::GetInstance().Subscribe(&variables, eMessageType::PlayerPosition);
	CPostMaster::GetInstance().Subscribe(&variables, eMessageType::BuildTikiAtPosition);
	CPostMaster::GetInstance().Subscribe(&variables, eMessageType::KillTiki);
	CPostMaster::GetInstance().Subscribe(&variables, eMessageType::Reset);
}

void CScriptLoader::InternalScriptLoader()
{
	for (SScriptLoadData& data : myScriptLoadData)
	{
#ifdef _RETAIL
		if (data.ENABLE_IN_RETAIL)
		{
			CScriptManager::GetInstance().RegisterFunction(data.functionName, data.functionPtr, data.argList, data.helpText, data.CALLABLE_FROM_CONSOLE);
		}
#else
		CScriptManager::GetInstance().RegisterFunction(data.functionName, data.functionPtr, data.argList, data.helpText, data.CALLABLE_FROM_CONSOLE);
#endif
	}
}

void CScriptLoader::InternalFileReader()
{
	for (std::string& str : myFilesToRead)
	{
		CScriptManager::GetInstance().ReadFile(str.c_str());
	}

}
