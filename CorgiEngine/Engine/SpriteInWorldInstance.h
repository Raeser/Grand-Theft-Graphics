#pragma once
#include "SpriteInstance.h"

class CSpriteInWorldInstance
{
public:
	CSpriteInWorldInstance();
	~CSpriteInWorldInstance();


	void SetSprite(CSpriteInstance* aSpriteInstance);
	CSpriteInstance* GetSprite() const;

	void Render(CSpriteRenderer& aRenderer);

	void SetWorldPositionFromOwnSprite();
private:
	CSpriteInstance* mySprite;
	Vector3f myWorldPosition;
};

