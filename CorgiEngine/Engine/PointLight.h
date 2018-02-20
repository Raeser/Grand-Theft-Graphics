#pragma once

struct SPointLightData
{
	Vector3f myColor = { 1.0f, 0.f, 1.f};
	float myRange = 1000.f;
};

class CPointLight
{
public:
	CPointLight();
	~CPointLight();
	bool Init(const SPointLightData& aData);

	const SPointLightData& GetPointLightData() const;
	const float GetRange();
	void SetRange(float aRange);
	const Vector3f& GetColor();
	void SetColor(Vector3f& aColor);
private:
	SPointLightData myData;
};

