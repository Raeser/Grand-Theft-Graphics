#pragma once

#include <string>
#include "..\Utilities\GrowingArray.h"

struct SArgCount
{
	int count = 0;
};

enum class eScriptValueTypes
{
	UNINITIALIZED,
	Function,
	Double,
	Float,
	UINT,
	Int,
	Bool,
	String
};


struct Argument
{
	eScriptValueTypes myType = eScriptValueTypes::UNINITIALIZED;
	double			myDouble;
	float			myFloat;
	unsigned int	myUint;
	int				myInt;
	bool			myBool;
	std::string		myString;
};

struct lua_State;

struct SLuaFunction
{
	std::string functionName;
	Corgi::GrowingArray<Argument> args;
	lua_State* state;
};

enum class ECallbackEvent
{
	Uninitialized,
	MoveDone,
	AnimationDone,
	Count
};