#pragma once
#include "GameState.h"
#include "UIManager.h"

class CButtonManager;
class CEngine;
class CSpriteInstance;
struct MenuSpriteData;
class CButtonManager;

enum class EInfoBoxAnswer 
{
	Yes,
	No,
	UnAnswered,
	Finished
};

enum class eInfoBoxType
{
	OK,
	YES_NO
};

class CInfoBoxState :
	public CGameState
{
public:
	CInfoBoxState(CEngine& aEngine);
	~CInfoBoxState();

	void Init(const char* aInfoBoxSprite, eInfoBoxType aType, EInfoBoxAnswer& aAnswer);
	void OnEnter() override;
	void OnExit() override;
	void Destroy() override;


	eStateStackMessage Update() override;


private:
	void InitOKButtons();
	void InitYESNOButtons();
	void PopSubstateAndConfirm();
	void PopSubstateAndDeny();
	void Init() override;
	void PopSubstate();

	eStateStackMessage myMessage;

	CSpriteInstance* myBackgroundSpriteInstance;
	CSpriteInstance* myInfoBoxSpriteInstance;

	EInfoBoxAnswer* myAnswer;

	CEngine& myGameEngine;
	CUIManager myUIManager;

};
