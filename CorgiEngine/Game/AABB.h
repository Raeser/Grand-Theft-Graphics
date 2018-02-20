#pragma once


class CAABB
{
public:
	CAABB();
	~CAABB();

	void Init(float aWidth, float aHeight, const Vector2f& aPos);

	bool IsCollidingWithPoint(const Corgi::Vector2<float>& aPoint);
	void UpdatePosition(const Vector2f& aPos);

private:

	float myMinX;
	float myMaxX;
	
	float myMinY;
	float myMaxY;

	float myWidth;
	float myHeight;

	Vector2f myPosition;

};

