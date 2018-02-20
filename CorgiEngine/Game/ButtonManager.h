#pragma once
#include "Button.h"

class CEngine;

class CButtonManager
{
public:
	CButtonManager(CEngine& aEngine);
	~CButtonManager();

	void Init();
	void AddButton();
	void UpdateButtons();
	void DestroyButtons();

private:
	CEngine& myGameEngine;
};

