#pragma once
#include "..\Engine\Subscriber.h"
#include "..\Utilities\GrowingArray.h"
#include "lua_argument_struct.h"

typedef std::string Callback_ID;

enum class ECallbackEvent;

struct SCallbackFunction
{
	SLuaFunction myFunction;
	Callback_ID myID;
};

struct SLuaFunction;
struct lua_State;

class CScriptCallbackManager :
	public CSubscriber
{
public:
	CScriptCallbackManager();
	virtual ~CScriptCallbackManager();

	long long RegisterCallback(lua_State*);
	void UnRegisterCallback(lua_State*);


private:
	void RecieveMessage(const SMessage& message) override;

	void CallFunctions(ECallbackEvent aEvent, Callback_ID aID);
	//Corgi::CMemoryPool<SLuaFunction> myCallbacks;
	Corgi::GrowingArray<Corgi::GrowingArray<SCallbackFunction>> myCallbacks;
};

