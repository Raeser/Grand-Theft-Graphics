#include "stdafx.h"
#include "TriggerVolume.h"

int ITriggerVolume::ID_COUNT = 0;

ITriggerVolume::ITriggerVolume()
{

}


ITriggerVolume::~ITriggerVolume()
{
}


void ITriggerVolume::SetTriggerShaper(ETriggerShape aShape)
{
	myTriggerShape = aShape;
}

void ITriggerVolume::Init(const std::string& aFilepath)
{
	myCurrentIDs.Init(32);
	myPreviousIDs.Init(32);

	myTikiCount = 0;
	std::string str = "../Assets/Scripts/";
	str += aFilepath;
	myLuaScript.Init();
	myLuaScript.LoadFile(str.c_str());
	NewID();

	myLuaScript.CallFunction("Init");
	myLuaScript.Update();

	mySpriteWorldPosition = Vector3f();
	myHasSprite = false;
	//myHasText = false;
}

void ITriggerVolume::NewFrame()
{
	myPreviousIDs = myCurrentIDs;
	myCurrentIDs.RemoveAll();
}

void ITriggerVolume::AddID(Trigger_ID aID)
{
	myCurrentIDs.Add(aID);
}

bool ITriggerVolume::GetStatusOfID(Trigger_ID aID)
{
	for (Trigger_ID id : myCurrentIDs)
	{
		if (id == aID)
		{
			return true;
		}
	}
	return false;
}

bool ITriggerVolume::GetPreviousStatusOfID(Trigger_ID aID)
{
	for (Trigger_ID id : myPreviousIDs)
	{
		if (id == aID)
		{
			return true;
		}
	}
	return false;
}

void ITriggerVolume::OnEnter(Trigger_ID id)
{
	myLuaScript.CallFunction("OnEnter", id.c_str());
	myLuaScript.Update();
}

void ITriggerVolume::OnExit(Trigger_ID id)
{
	myLuaScript.CallFunction("OnExit", id.c_str());
	myLuaScript.Update();
}

const Vector3f & ITriggerVolume::GetPosition() const
{
	return myPosition;
}

ETriggerShape ITriggerVolume::GetTriggerShape() const
{
	return myTriggerShape;
}

int ITriggerVolume::GetTikiCount() const
{
	return myTikiCount;
}

void ITriggerVolume::IncrementTikiCount()
{
	++myTikiCount;
}

void ITriggerVolume::DecrementTikiCount()
{
	--myTikiCount;
}

int ITriggerVolume::SetSprite(CSpriteInstance* aSpriteInstance)
{
	mySpriteInstance.SetSprite(aSpriteInstance);
	mySpriteInstance.GetSprite()->SetShouldRender(false);
	myHasSprite = true;
	return 0;
}

CSpriteInWorldInstance&  ITriggerVolume::GetSpriteInstance()
{
	return mySpriteInstance;
}

void ITriggerVolume::SetSpritePosition(const Vector3f & aSpriteWorldPosition)
{
	mySpriteWorldPosition = aSpriteWorldPosition;
}

const Vector3f & ITriggerVolume::GetSpritePosition() const
{
	return mySpriteWorldPosition;
}
//
//void ITriggerVolume::SetText(const char* aText, bool hasText)
//{
//	myText = aText;
//	myHasText = hasText;
//}
//
//void ITriggerVolume::SetText(const std::string& aText, bool hasText)
//{
//	myText = aText;
//	myHasText = hasText;
//}
//
//std::string& ITriggerVolume::GetTextRef()
//{
//	return myText;
//}
//
//TextID ITriggerVolume::GetTextID() const
//{
//	return myTextId;
//}
//
//void ITriggerVolume::SetTextID(TextID id)
//{
//	myTextId = id;
//}
//
//bool ITriggerVolume::HasText() const
//{
//	return myHasText;
//}
//
//void ITriggerVolume::ReserveTextArray()
//{
//	myText.reserve(64);
//}

Trigger_ID ITriggerVolume::GetStringID() const
{
	return myStringId;
}

CScriptComponent & ITriggerVolume::GetScript()
{
	return myLuaScript;
}

const CScriptComponent & ITriggerVolume::GetScript() const
{
	return myLuaScript;
}

bool ITriggerVolume::HasSprite() const
{
	return myHasSprite;
}

void ITriggerVolume::NewID()
{
	++ID_COUNT;
	myStringId = "Trigger" + std::to_string(ID_COUNT);
}
