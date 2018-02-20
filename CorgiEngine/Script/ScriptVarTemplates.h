#pragma once
#include "lua.hpp"
#include "..\Script\lua_argument_struct.h"

template<typename First, typename... Rest> inline void lua_push(lua_State* L, SArgCount & argCount, First first, Rest... rest)
{
	argCount.count++;
	lua_push(L, first);
	lua_push(L, argCount, rest...);
}
inline void lua_push(lua_State * L, SArgCount & argCount) { L; argCount; }
template<typename T> inline void lua_push(lua_State* L, T&& t)
{
	static_assert(false, "Unsupported Type! Cannot set to lua stack");
}

//template<> inline void lua_push<lua_CFunction>(lua_State* L, lua_CFunction&& func)/*******/ { lua_pushcfunction(L, func); }
template<> inline void lua_push<double&>(lua_State* L, double& d)/************************/ { lua_pushnumber(L, d); }
template<> inline void lua_push<float&>(lua_State* L, float& f)/**************************/ { lua_pushnumber(L, f); }
template<> inline void lua_push<unsigned int&>(lua_State* L, unsigned int& i)/************/ { lua_pushinteger(L, i); }
template<> inline void lua_push<signed int&>(lua_State* L, signed int& i)/****************/ { lua_pushinteger(L, i); }
template<> inline void lua_push<bool&>(lua_State* L, bool& b)/****************************/ { lua_pushboolean(L, b); }
template<> inline void lua_push<std::string&>(lua_State* L, std::string& s)/**************/ { lua_pushlstring(L, s.c_str(), s.size()); }
template<> inline void lua_push<const char*&>(lua_State* L, const char*& s)/**************/ { lua_pushstring(L, s); }



template<typename First, typename... Rest> inline void buffer_argument_push(Corgi::GrowingArray<Argument>& ga, SArgCount & argCount, First first, Rest... rest)
{
	argCount.count++;
	buffer_argument_push(ga, first);
	buffer_argument_push(ga, argCount, rest...);
}
inline void buffer_argument_push(Corgi::GrowingArray<Argument>& ga, SArgCount & argCount) { ga; argCount; }
template<typename T> inline void buffer_argument_push(Corgi::GrowingArray<Argument>& ga, T&& t) { static_assert(false, "Unsupported Type! Cannot set to lua stack"); }

//template<> inline void buffer_argument_push<lua_CFunction>(Corgi::GrowingArray<Argument>& ga, lua_CFunction&& func)/***/ 
//{
//	ArgumentFunction a;
//	a.myType = eScriptValueTypes::Function;
//	a.myValue = func;
//
//	ga.Add(a);
//}
template<> inline void buffer_argument_push<double&>(Corgi::GrowingArray<Argument>& ga, double& d)/********************/
{
	Argument a;
	a.myType = eScriptValueTypes::Double;
	a.myDouble = d;

	ga.Add(a);
}
template<> inline void buffer_argument_push<float&>(Corgi::GrowingArray<Argument>& ga, float& f)/**********************/
{
	Argument a;
	a.myType = eScriptValueTypes::Float;
	a.myFloat = f;

	ga.Add(a);
}
template<> inline void buffer_argument_push<unsigned int&>(Corgi::GrowingArray<Argument>& ga, unsigned int& i)/********/
{
	Argument a;
	a.myType = eScriptValueTypes::UINT;
	a.myUint = i;

	ga.Add(a);
}
template<> inline void buffer_argument_push<signed int&>(Corgi::GrowingArray<Argument>& ga, signed int& i)/************/
{
	Argument a;
	a.myType = eScriptValueTypes::Int;
	a.myInt = i;

	ga.Add(a);
}
template<> inline void buffer_argument_push<bool&>(Corgi::GrowingArray<Argument>& ga, bool& b)/************************/
{
	Argument a;
	a.myType = eScriptValueTypes::Bool;
	a.myBool = b;

	ga.Add(a);
}
template<> inline void buffer_argument_push<std::string&>(Corgi::GrowingArray<Argument>& ga, std::string& s)/**********/
{
	Argument a;
	a.myType = eScriptValueTypes::String;
	a.myString = s;

	ga.Add(a);
}
template<> inline void buffer_argument_push<const char*&>(Corgi::GrowingArray<Argument>& ga, const char*& s)/**********/
{
	Argument a;
	a.myType = eScriptValueTypes::String;
	a.myString = std::string(s);

	ga.Add(a);
}
