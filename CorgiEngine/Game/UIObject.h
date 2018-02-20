#pragma once

class CEngine;
class CSpriteInstance;

class CUIObject
{
public:
	CUIObject() { myIsActive = true; mySpriteInstance = nullptr; myPosition = { 0.5f, 0.5f, 0.0f }; };
	~CUIObject() = default;

	virtual void Init(CEngine* aGameEngine) = 0;
	virtual bool Update(const Corgi::Vector2<float>& aMousePos) = 0;
	virtual void Destroy() = 0;

	virtual void SetIsActive(const bool& aIsActive) { myIsActive = aIsActive; };
	virtual Vector3f GetPosition() const& { return myPosition; };
	virtual bool IsActive() { return myIsActive; };


protected:

	virtual void CreateSpriteInstance(const char* aPath);

	CSpriteInstance* mySpriteInstance;
	CEngine* myGameEngine;
	Vector3f myPosition;

	long long mySpriteIndex;
	bool myIsActive;
};

