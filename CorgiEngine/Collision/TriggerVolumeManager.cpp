#include "stdafx.h"
#include "TriggerVolumeManager.h"
#include "TriggerSphere.h"
#include "TriggerBox.h"
#include "..\Collision\TriggerVolume.h"
#include "..\Engine\SpriteLoader.h"
#include "..\Engine\CameraInstance.h"
#include "..\Engine\TextManager.h"
#include "..\Engine\SpriteInWorldInstance.h"
#include "..\Game\unitystruct.h"
#include "..\Engine\SpriteRenderer.h"

#define NUMBER_OF_EXPECTED_TRIGGER_SPHERES 256
#define NUMBER_OF_EXPECTED_TRIGGER_BOXES 256

void CTriggerVolumeManager::Render(CCameraInstance* aCamera, CSpriteRenderer& aSpriteRenderer)
{
	if (myShouldRender)
		for (ITriggerVolume* trigger : myTriggerVolumes)
		{
			if (trigger->HasSprite())
			{
				trigger->GetSpriteInstance().GetSprite()->SetPositionFromWorldPosition(trigger->GetSpritePosition(), aCamera);
				trigger->GetSpriteInstance().SetWorldPositionFromOwnSprite();
				trigger->GetSpriteInstance().Render(aSpriteRenderer);
			}

			//if (trigger->HasText())
			//{
			//	TextID id = trigger->GetTextID();
			//	myTextManager->ChangePosition(id, aCamera->FromWorldSpaceToProjectionSpace(trigger->GetPosition()));
			//}
		}
}

void CTriggerVolumeManager::SetShouldRender(bool aShouldRender)
{
	myShouldRender = aShouldRender;
}

CTriggerVolumeManager::CTriggerVolumeManager()
{
}


CTriggerVolumeManager::~CTriggerVolumeManager()
{
}

CSpriteLoader * CTriggerVolumeManager::GetSpriteLoader() const
{
	return mySpriteLoader;
}

void CTriggerVolumeManager::Init()
{
	myTriggerVolumes.Init(NUMBER_OF_EXPECTED_TRIGGER_BOXES + NUMBER_OF_EXPECTED_TRIGGER_SPHERES);
	myTriggerSpheres.Init(NUMBER_OF_EXPECTED_TRIGGER_SPHERES);
	myTriggerBoxes.Init(NUMBER_OF_EXPECTED_TRIGGER_BOXES);
	myEntityDataThisFrame.Init(1024);
#ifndef _RETAIL
	myShouldRenderWireframe = false;
#endif
}

void CTriggerVolumeManager::Reset()
{
	myTriggerVolumes.ReInit(NUMBER_OF_EXPECTED_TRIGGER_BOXES + NUMBER_OF_EXPECTED_TRIGGER_SPHERES);
	myTriggerSpheres.FreeAll();
	myTriggerBoxes.FreeAll();
	myEntityDataThisFrame.ReInit(1024);
}

void CTriggerVolumeManager::AddTriggerSphere(const SUnityExporterTrigger & aData)
{

	CTriggerSphere& sphere = myTriggerSpheres.NewItem();
	sphere.Init(aData);
	myTriggerVolumes.Add(&sphere);

	//sphere.ReserveTextArray();
	//long long textId = myTextManager->AddText(sphere.GetTextRef(), Vector2f());
	//sphere.SetText("", false);
	//sphere.SetTextID(textId);

}

void CTriggerVolumeManager::AddTriggerBox(const SUnityExporterTrigger & aData)
{
	CTriggerBox& box = myTriggerBoxes.NewItem();
	box.Init(aData);
	myTriggerVolumes.Add(&box);

	//box.ReserveTextArray();
	//long long textId = myTextManager->AddText(box.GetTextRef(), Vector2f());
	//box.SetText("", false);
	//box.SetTextID(textId);
}

void CTriggerVolumeManager::Enter(ITriggerVolume * aTrigger, SEntityData aEntity)
{
	if (aEntity.entityType == EEntityType::Tiki)
	{
		aTrigger->IncrementTikiCount();
	}
	aTrigger->OnEnter(aEntity.stringid);
}

void CTriggerVolumeManager::Exit(ITriggerVolume * aTrigger, SEntityData aEntity)
{
	if (aEntity.entityType == EEntityType::Tiki)
	{
  		aTrigger->DecrementTikiCount();
	}
	aTrigger->OnExit(aEntity.stringid);
}

bool CTriggerVolumeManager::CheckEnter(ITriggerVolume * aTrigger, SEntityData entity)
{
	return aTrigger->GetStatusOfID(entity.stringid) == true && aTrigger->GetPreviousStatusOfID(entity.stringid) == false;
}

bool CTriggerVolumeManager::CheckExit(ITriggerVolume * aTrigger, SEntityData entity)
{
	return aTrigger->GetStatusOfID(entity.stringid) == false && aTrigger->GetPreviousStatusOfID(entity.stringid) == true;
}

#ifndef _RETAIL
void CTriggerVolumeManager::RenderWireframes() const
{
	for (ITriggerVolume* trigger : myTriggerVolumes)
	{
		trigger->DrawWireframe();
	}
}

void CTriggerVolumeManager::SetShouldRenderWireframe(bool b)
{
	DL_WRITELOG(eConsoleColor::LIGHTBLUE, eDebugLogType::Script, "%s%s", "RENDERING WIREFRAMES SET TO: ", std::to_string(b).c_str())
	myShouldRenderWireframe = b;
}
#endif
void CTriggerVolumeManager::AssignSpriteLoader(CSpriteLoader& aSpriteLoader)
{
	mySpriteLoader = &aSpriteLoader;
}

void CTriggerVolumeManager::AssignTextManager(CTextManager& aTextManager)
{
	myTextManager = &aTextManager;
}

int CTriggerVolumeManager::GetTikiCount(lua_State* aState)
{
	for (ITriggerVolume* trigger : myTriggerVolumes)
	{
		if (trigger->GetScript().GetLuaStatePtr() == aState)
		{
			return trigger->GetTikiCount();
		}
	}
	return 0;
}

Vector3f CTriggerVolumeManager::GetTriggerPosition(lua_State * aState)
{
	for (ITriggerVolume* trigger : myTriggerVolumes)
	{
		if (trigger->GetScript().GetLuaStatePtr() == aState)
		{
			return trigger->GetPosition();
		}
	}
	return Vector3f();
}
//
//int CTriggerVolumeManager::SetText(lua_State * aState)
//{
//
//	std::string str;
//	if (lua_isstring(aState, 1))
//	{
//		str = lua_tostring(aState, 1);
//	}
//	else if (lua_isnumber(aState, 1))
//	{
//		str = std::to_string(lua_tonumber(aState, 1));
//	}
//	else if (lua_isinteger(aState, 1))
//	{
//		str = std::to_string(lua_tointeger(aState, 1));
//	}
//	else if (lua_isboolean(aState, 1))
//	{
//		str = std::to_string(lua_toboolean(aState, 1));
//	}
//	else
//	{
//		return DL_LUA_ERROR(aState, "Invalid text");
//	}
//
//	for (ITriggerVolume* trigger : myTriggerVolumes)
//	{
//		if (trigger->GetScript().GetLuaStatePtr() == aState)
//		{
//			trigger->SetText(str.c_str());
//			return 0;
//		}
//	}
//
//
//	return DL_LUA_ERROR(aState, "Trigger does not exist in the manager for some god awful reason. Get a c++ programmer");
//	
//}

int CTriggerVolumeManager::LoadSprite(lua_State* aState)
{
	if (lua_isstring(aState, 1))
	{
		for (ITriggerVolume* trigger : myTriggerVolumes)
		{
			if (trigger->GetScript().GetLuaStatePtr() == aState)
			{
				long long spriteID = mySpriteLoader->CreateSprite(lua_tostring(aState, 1));
				CSpriteInstance* sprite = mySpriteLoader->CreateSpriteInstance(spriteID);
				trigger->SetSprite(sprite);
				trigger->SetSpritePosition(trigger->GetPosition());
				return 0;
			}
		}
		return DL_LUA_ERROR(aState, "Something went wrong. Peta på Robin");
	}
	else
	{
		return DL_LUA_ERROR(aState, "Invalid path to sprite");
	}
}

int CTriggerVolumeManager::LoadSpriteAtPosition(lua_State* aState)
{
	if (lua_isstring(aState, 1))
	{
		for (int i = 2; i < 5; ++i)
		{
			if (!lua_isnumber(aState, i))
			{
				return DL_LUA_ERROR(aState, "Invalid position");
			}

		}
		Vector3f spritePos({ cast_f(lua_tonumber(aState, 2)), cast_f(lua_tonumber(aState, 3)), cast_f(lua_tonumber(aState, 4)) });

		for (ITriggerVolume* trigger : myTriggerVolumes)
		{
			if (trigger->GetScript().GetLuaStatePtr() == aState)
			{
				long long spriteID = mySpriteLoader->CreateSprite(lua_tostring(aState, 1));
				CSpriteInstance* sprite = mySpriteLoader->CreateSpriteInstance(spriteID);
				trigger->SetSpritePosition(spritePos);
				trigger->SetSprite(sprite);
				return 0;
			}
		}
		return DL_LUA_ERROR(aState, "Something went wrong");
	}
	else
	{
		return DL_LUA_ERROR(aState, "Invalid path to sprite");
	}
}

#ifndef _RETAIL
int CTriggerVolumeManager::ToggleWireframeMode(lua_State *)
{
	SetShouldRenderWireframe(!myShouldRenderWireframe);
	return 0;
}
#endif


void CTriggerVolumeManager::Update(const Corgi::CMemoryPool<CEntity>& aEntityMemoryPool, const Corgi::GrowingArray<long long>& aUpdateableIDList)
{
#ifndef _RETAIL
	if (myShouldRenderWireframe)
	{
		RenderWireframes();
	}
#endif


	myEntityDataThisFrame.RemoveAll();

	for(long long id : aUpdateableIDList)
	{
		if (aEntityMemoryPool[id].GetEntityType() == EEntityType::Player || aEntityMemoryPool[id].GetEntityType() == EEntityType::Tiki)
		{
			SEntityData data;
			data.entityType = aEntityMemoryPool[id].GetEntityType();
			data.stringid = aEntityMemoryPool[id].GetID();
			data.position = aEntityMemoryPool[id].GetTransform().GetPosition();
			myEntityDataThisFrame.Add(data);
		}

	}



	for (ITriggerVolume* trigger : myTriggerVolumes)
	{
		trigger->NewFrame();

		for (SEntityData& entity : myEntityDataThisFrame)
		{
			const std::string& str_id = entity.stringid;

			if (trigger->CheckCollision(entity.position))
			{
				trigger->AddID(str_id);
			}

			if (CheckEnter(trigger, entity))
			{
				Enter(trigger, entity);
			}
			else if (CheckExit(trigger, entity))
			{
				Exit(trigger, entity);
			}
		}
	}


}
