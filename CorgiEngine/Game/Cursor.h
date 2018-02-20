#pragma once
#include "UIObject.h"

class CEngine;
class CSpriteInstance;


class CCursor : public CUIObject
{

public:
	static CCursor& GetInstance();

	void Init(CEngine* aGameEngine) override;
	void ReInit();
	Corgi::Vector3<float> GetMousePos();

	Corgi::Vector2<int> GetResolution();

	bool Update(const Corgi::Vector2<float>& aMousePos = { 0.f, 0.f }) override;
	void SetIsActive(const bool& aIsActive) override;
	void Destroy() override;

	~CCursor();
private:
	CCursor();
	float mySpriteWidth;
	float mySpriteHeight;
};

