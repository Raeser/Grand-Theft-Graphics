#pragma once

class CPointLightInstance
{
public:
	CPointLightInstance();
	~CPointLightInstance();

	bool Init(const Vector4f& aPosition);
	bool Init(const Vector3f & aPosition);
	void SetPosition(const Vector4f& aPos);
	void SetPosition(const Vector3f & aPos);
	bool GetShouldShine();

	void SetPointLight(const long long& aID);
	void SetIsOn(bool aIsOn);
	const Vector4f& GetPosition();
	inline const long long& GetID() { return myID; }
	void SetOrientation(const Matrix44& aOrientation);

private:
	Vector4f myPosition;
	long long myID;
	bool myIsInitiated;
	bool myIsOn;
};

