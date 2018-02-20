#pragma once
#include "stdafx.h"

class CStateStack;

enum class eStateStackMessage
{
	KeepState,
	PopMainState,
	PopSubState,
	PopSubAndMainState,
	ExitGame
};


class CGameState
{
public:
	CGameState();
	virtual ~CGameState();

	virtual void Init() = 0;

	virtual void OnEnter() = 0;
	virtual void OnExit() = 0;
	virtual void Destroy() = 0;

	virtual eStateStackMessage Update() = 0;

	inline void AttachStateStack(CStateStack* aStateStack) { myStateStack = aStateStack; }

	virtual void UpdateStateTimer();

	virtual void TerminateThread() {};

	bool HasBeenInStateLoongEnough();

	std::string& GetName();

protected:

	float myTimeInState;
	float myMinTimeInState;

	CStateStack* myStateStack;
	std::string myName;
};

