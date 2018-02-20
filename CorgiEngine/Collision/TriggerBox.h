#pragma once
#include "TriggerVolume.h"

struct SUnityExporterTrigger;

struct SLineCollider
{
	Vector3f point;
	Vector3f normal;
};

class CTriggerBox :
	public ITriggerVolume
{
public:
	//CTriggerBox(const SUnityExporterTrigger& aTrigger);
	CTriggerBox();
	virtual ~CTriggerBox();

	void Init(const SUnityExporterTrigger& aTrigger);
	void Init(const Vector3f& aPos, const Vector3f& aSize, const Vector3f& aRotation, const std::string& aLuaFilepath);

	bool CheckCollision(const Vector3f & aPosition) override;

#ifndef _RETAIL
	void DrawWireframe() const override;
#endif

private:
	Corgi::GrowingArray<SLineCollider> myEdges;

#ifndef _RETAIL
	Corgi::GrowingArray<Vector4f> myWireframeLinesBottom;
	Corgi::GrowingArray<Vector4f> myWireframeLinesTop;
#endif

};

