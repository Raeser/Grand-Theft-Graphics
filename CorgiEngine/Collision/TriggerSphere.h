#pragma once
#include "TriggerVolume.h"


struct SUnityExporterTrigger;

class CTriggerSphere :
	public ITriggerVolume
{
public:
	CTriggerSphere();
	CTriggerSphere(const SUnityExporterTrigger& aTrigger);
	virtual ~CTriggerSphere();

	void Init(const SUnityExporterTrigger& aTrigger);
	void Init(const Vector3f& aPos, float aRadius, const std::string& aLuaFilepath);
	
	virtual bool CheckCollision(const Vector3f& aPosition) override;

	float GetRadius() const;
private:
	float myRadius;

	// Inherited via ITriggerVolume
#ifndef _RETAIL
	virtual void DrawWireframe() const override;

	Corgi::GrowingArray<Vector4f> debugpointsXY;
	Corgi::GrowingArray<Vector4f> debugpointsXZ;
	Corgi::GrowingArray<Vector4f> debugpointsZY;
#endif
};

