#include "stdafx.h"
#include "GameState.h"


CStateStack::CStateStack()
{
}


CStateStack::~CStateStack()
{
}

void CStateStack::Init()
{
	myGameStates.Init(16);
}

void CStateStack::PushSubState(CGameState* aState)
{
	assert(aState != nullptr && "Tried to push a Null-State!");

	Corgi::GrowingArray<CGameState*>& substates = myGameStates[myGameStates.Size() - 1];
	substates.Add(aState);
	aState->AttachStateStack(this);
	aState->OnEnter();

	DL_SUCCESS(eDebugLogType::ResultMessages, "%s%s", "Successfully Pushed Sub State: ", aState->GetName().c_str());
}

void CStateStack::PushMainState(CGameState * aState)
{
	assert(aState != nullptr && "Tried to push a Null-State!");

	Corgi::GrowingArray<CGameState*> substates;
	substates.Init(16);
	substates.Add(aState);
	myGameStates.Add(substates);

	aState->AttachStateStack(this);
	aState->OnEnter();

	DL_SUCCESS(eDebugLogType::ResultMessages, "%s%s", "Successfully Pushed Main State: ", aState->GetName().c_str());

}

void CStateStack::PopSubState()
{
	myGameStates.GetLast().GetLast()->OnExit();
	myGameStates.GetLast().GetLast()->Destroy();
	myGameStates.GetLast().DeleteCyclicAtIndex(myGameStates.GetLast().Size() - 1);

	if (myGameStates.Size() > 0)
	{
		myGameStates.GetLast().GetLast()->OnEnter();
	}
}

void CStateStack::PopMainState()
{
	Corgi::GrowingArray<CGameState*>& substates = myGameStates.GetLast();
	unsigned int numberOfSubStates = substates.Size();
	

	for (int index = (numberOfSubStates - 1); index >= 0; --index)
	{
		substates[index]->OnExit();
		substates[index]->Destroy();
		substates.DeleteCyclicAtIndex(index);
	}

	myGameStates.RemoveCyclicAtIndex(static_cast<unsigned short>(GetLastIndex()));

	if (myGameStates.Size() > 0) 
	{
		myGameStates.GetLast().GetLast()->OnEnter();
	}
}


CGameState * CStateStack::GetCurrentState()
{
	if (myGameStates.Size() == 0) 
	{
		return nullptr;
	}

	Corgi::GrowingArray<CGameState*>* container = &myGameStates.GetLast();
	return container->GetLast();
};
