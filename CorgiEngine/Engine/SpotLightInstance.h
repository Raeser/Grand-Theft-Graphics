#pragma once

#include "../Utilities/Vector4.h"
#include "../Utilities/Matrix44.h"

class CSpotLightInstance
{
public:
	CSpotLightInstance();
	~CSpotLightInstance();

	bool Init(const Vector4f& aPosition, const Vector4f& aDirection);

	void SetPosition(const Vector3f & aPos);

	void SetPosition(const Vector4f& aPos);
	void SetDirection(const Vector4f& aDir);

	void SetSpotLight(const long long& aID);
	void SetIsOn(bool aIsOn);
	bool GetShouldShine();
	const Vector4f& GetPosition();
	const Vector4f& GetDirection();

	void SetOrientation(const Matrix44& aOrientation);

	inline const long long& GetID() { return myID; }

private:
	Vector4f myPosition;
	Vector4f myDirection;
	bool myIsOn;
	long long myID;
	bool myIsInitiated;
};

