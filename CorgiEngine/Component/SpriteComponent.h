#pragma once
#include "..\Component\Component.h"
class CSpriteInstance;

class CSpriteComponent :
	public CComponent
{
	friend class CComponentBuilder;
public:
	CSpriteComponent();
	~CSpriteComponent();
	void Init(Vector3f aPos, long long aID);
	void Update() override;
	void Release() override;
	void FetchData() override;
	void SendData() override;
	void SetIsActive(bool aIsActive) override;
	void SetNewSprite(long long aID);
	void SetShouldRender(bool aShouldRender);
	void SetPosition(Vector3f& aPos);
	void SetColor(Vector4f& aColor);
	void SetUVOffset(Vector2f& aOffset);

	const Vector3f& GetPosition();
	const Vector4f& GetColor();
	const Vector2f& GetUVOffset();

private:
	void SetSpriteInstance(CSpriteInstance* aInstance);

private:
	CSpriteInstance* myInstance;
};

