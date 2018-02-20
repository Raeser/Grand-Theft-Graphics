#pragma once

struct SLightInstanceData
{
	Vector4f myDirection;
	Vector4f myColor;
};

class CEnvironmentLightInstance
{
	friend class CForwardRenderer;

public:
	CEnvironmentLightInstance();
	~CEnvironmentLightInstance();
	bool Init(const Vector4f& aDirection, const Vector4f& aColor);
	const SLightInstanceData& GetEnvironmentLightData();
	void SetEnvironmentLight(const long long aLightID);
	inline const long long& GetLightID() { return myLightID; }

	void SetDirection(Vector4f aRot);

private:
	bool myIsInitiated;
	long long myLightID;
	SLightInstanceData myLightInstanceData;
};

