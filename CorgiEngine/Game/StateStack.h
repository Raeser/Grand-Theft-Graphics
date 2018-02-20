#pragma once
#include <map>
#include "GameState.h"

class CStateStack
{
public:
	CStateStack();
	~CStateStack();

	void Init();

	void PushSubState(CGameState* aState);
	void PushMainState(CGameState* aState);

	void PopSubState();
	void PopMainState();
	
	//void Render();

	inline const int GetLastIndex() const { return myGameStates.Size()-1; }
	CGameState* GetCurrentState();
	inline const int GetSize() const { return myGameStates.Size(); }


private:

	//void RenderGameStateAtIndex(int aIndex);
	Corgi::GrowingArray<Corgi::GrowingArray<CGameState*>> myGameStates;
};

