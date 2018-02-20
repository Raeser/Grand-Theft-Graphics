#pragma once


class CSpline
{
public:
	CSpline();
	~CSpline();

	void Init();
	void AddPoint(const Corgi::Vector3<float>& aPoint);

	Corgi::Vector3<float> GetClosestPositionOnSpline(const Corgi::Vector3<float>& aPoint);

	void ClearPoints();

private:
	Corgi::Vector3<float> GetClosestPosOnLine(const Corgi::Vector3<float>& aLineStart, const Corgi::Vector3<float>& aLineEnd, const Corgi::Vector3<float>& aPoint);

	Corgi::GrowingArray <Corgi::Vector3<float>> myPoints;
};

