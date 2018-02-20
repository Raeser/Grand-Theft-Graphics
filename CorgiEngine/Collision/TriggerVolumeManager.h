#pragma once
#include "stdafx.h"
#include "..\Utilities\GrowingArray.h"


struct SEntityData
{
	EEntityType entityType;
	std::string stringid;
	Vector3f position;
};

class CTriggerSphere;
class CTriggerBox;
class CGameWorld;
class ITriggerVolume;
class CSpriteLoader;
class CCameraInstance;
class CTextManager;
struct SUnityExporterTrigger;
class CSpriteRenderer;

class CTriggerVolumeManager
{
public:
	~CTriggerVolumeManager();

	friend CGameWorld;

	static CTriggerVolumeManager& GetInstance()
	{
		static CTriggerVolumeManager me;
		return me;
	}

	int GetTikiCount(lua_State* aState);
	Vector3f GetTriggerPosition(lua_State* aState);
	int SetText(lua_State * aState);
	int LoadSprite(lua_State * aState);
	int LoadSpriteAtPosition(lua_State * aState);


#ifndef _RETAIL
	int ToggleWireframeMode(lua_State*);
	void SetShouldRenderWireframe(bool b);
#endif

	void AssignSpriteLoader(CSpriteLoader& aSpriteLoader);
	void AssignTextManager(CTextManager& aTextManager);


	void Render(CCameraInstance* aCamera, CSpriteRenderer& aSpriteRenderer);
	void SetShouldRender(bool aShouldRender);
	void Init();
private:
	CTriggerVolumeManager();
	CTextManager* myTextManager;

	CSpriteLoader* mySpriteLoader;
	CSpriteLoader* GetSpriteLoader() const;


	void Reset();
	void Update(const Corgi::CMemoryPool<CEntity>& aEntityMemoryPool, const Corgi::GrowingArray<long long>& aUpdateableIDList);

	void AddTriggerSphere(const SUnityExporterTrigger& aTriggerSphere);
	void AddTriggerBox(const SUnityExporterTrigger& aTriggerBox);

	void Enter(ITriggerVolume* aTrigger, SEntityData entity);
	void Exit(ITriggerVolume* aTrigger, SEntityData entity);

	bool CheckEnter(ITriggerVolume* aTrigger, SEntityData entity);
	bool CheckExit(ITriggerVolume* aTrigger, SEntityData entity);

	Corgi::GrowingArray<ITriggerVolume*> myTriggerVolumes;

	Corgi::CMemoryPool<CTriggerSphere> myTriggerSpheres;
	Corgi::CMemoryPool<CTriggerBox> myTriggerBoxes;

	Corgi::GrowingArray<SEntityData> myEntityDataThisFrame;
	bool myShouldRender;

#ifndef _RETAIL
	bool myShouldRenderWireframe;
	void RenderWireframes() const;



#endif
	Vector3f NegOneToZeroOneSpace(const Vector3f& aPos) const 
	{
		Vector3f out = aPos;

		out.z = 0;
		out.x = (out.x + 1) / 2.f;
		out.y = (out.y + 1) / 2.f;

		return out;
	}
};

