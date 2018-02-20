#pragma once

struct SSpotLightData
{
	Vector3f myColor = { 1.0f, 0.f, 0.f };
	float myAngle = 90.f;
	float myRange = 1336.f;
	Vector4f myPosition = { 1.f,1.f,1.f,1.f };
};

class CSpotLight
{
public:
	CSpotLight();
	~CSpotLight();

	bool Init(const SSpotLightData& aData);

	const SSpotLightData& GetSpotLightData() const;
	const float GetRange();
	void SetRange(float aRange);
	const Vector3f& GetColor();
	void SetColor(Vector3f& aColor);
	const float GetAngle();
	void SetAngle(float aAngle);

private:
	
	SSpotLightData myData;
	
};

