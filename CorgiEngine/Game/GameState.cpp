#include "stdafx.h"
#include "GameState.h"

CGameState::CGameState()
{
	myTimeInState = 0.f;
	myMinTimeInState = .1f;
}


CGameState::~CGameState()
{
}

void CGameState::UpdateStateTimer()
{
	myTimeInState += Timer::GetInstance().GetDeltaTime();
}

bool CGameState::HasBeenInStateLoongEnough()
{
	if (myTimeInState > myMinTimeInState) 
	{
		return true;
	}

	return false;
}

std::string & CGameState::GetName()
{
	return myName;
}
