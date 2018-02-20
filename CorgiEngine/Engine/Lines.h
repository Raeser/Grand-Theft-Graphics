#pragma once

struct SLinePoint
{
	Vector4f myPosition;
	Vector3f myColor;
	float myWidth;
};
class CLines
{
public:
	CLines();
	~CLines();
	void Init(int myMaxAmountOfPoints);
	void Reinit(int myMaxAmountOfPoints);
	void AddPoint(Vector4f aPosition, Vector3f aColor = {1,0,0}, float aWidth = 1.0f);
	long long GetAmountOfPoints() const;
	Corgi::GrowingArray<SLinePoint, long long>& GetPoints();
private:
	Corgi::GrowingArray<SLinePoint, long long> myPoints;
};

