#pragma once

#include "..\Utilities\Vector.h"

class CCameraInstance;

class CSpriteInstance
{
	friend class CModel;
	friend class CSpriteRenderer;
public:
	CSpriteInstance();
	~CSpriteInstance();

	const Vector3f& GetPosition() { return myPosition; };
	const Vector4f& GetColor() { return myColor; };
	const Vector2f& GetUVOffset() { return myUVOffset; };

	void SetSpriteMap(long long aSpriteID, int amountOfStepsX, int amountOfStepsY);
	void Step();
	void SetStep(int aIndex);

	const long long& GetSpriteID() const { return mySprite; }
	void SetSprite(long long aSpriteID) { mySprite = aSpriteID; }

	//Changed by Zeb
	void SetPosition0To1(const Vector3f& aPos) 
	{
		Corgi::Vector3<float> aZeroToOnePos;

		aZeroToOnePos.x = (aPos.x * 2.f) - 1.f;
		aZeroToOnePos.y = -((aPos.y * 2.f) - 1.f);

		myPosition = aZeroToOnePos;
	}
	void SetPositionNeg1toPos1(const Vector3f& aPos)
	{
		myPosition = aPos;
	}

	void SetPositionFromWorldPosition(const Vector3f& aPos, const CCameraInstance* aCameraPtr);

	void SetColor(const Vector4f& aColor) { myColor = aColor; }
	void SetUVOffset(const Vector2f& aOffset) { myUVOffset = aOffset; }

	void Move(const Vector3f& aMovement);
	void SetShouldRender(bool aShouldRender);
	bool GetShouldRender();

	int GetAmountOfSteps();

	void Remove(); //ONLY USE THIS WHEN YOU ARE HANDLING ONLY INSTANCES, NEVER USE THIS ON COMPONENTS UNLESS THE COMP IS REMOVED! 
	bool GetShouldRemove();
protected:
	Vector3f myPosition;
	Vector4f myColor;
	Vector2f myUVOffset;
	Vector2f myStep;
	Vector2int myAmountOfSteps;
	Vector2int myCurrentStep;
	long long mySprite;
	bool myShouldRender;
	bool myShouldBeRemoved;
};

