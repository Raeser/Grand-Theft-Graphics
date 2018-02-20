#pragma once
#include "..\Utilities\Vector3.h"
#include "..\Component\ScriptComponent.h"
#include "..\Engine\SpriteInWorldInstance.h"
using TextID = long long;

typedef std::string Trigger_ID;

class CSpriteInWorldInstance;

enum class ETriggerShape
{
	Sphere,
	AABB,
	Count
};

class ITriggerVolume
{
public:
	ITriggerVolume();
	~ITriggerVolume();


	void SetTriggerShaper(ETriggerShape aShape);

	virtual bool CheckCollision(const Vector3f& aPosition) = 0;

	void Init(const std::string& aFilepath);

	void NewFrame();
	void AddID(Trigger_ID aID);

	bool GetStatusOfID(Trigger_ID aID);
	bool GetPreviousStatusOfID(Trigger_ID aID);

	void OnEnter(Trigger_ID id);
	void OnExit(Trigger_ID id);

	const Vector3f& GetPosition() const;
	ETriggerShape GetTriggerShape() const;
	int GetTikiCount() const;

	void IncrementTikiCount();
	void DecrementTikiCount();

	int SetSprite(CSpriteInstance* aSpriteInstance);
	CSpriteInWorldInstance& GetSpriteInstance();
	void SetSpritePosition(const Vector3f& aSpriteWorldPosition);
	const Vector3f& GetSpritePosition() const;
	bool HasSprite() const;

	//void SetText(const char* aText, bool hasText = true);
	//void SetText(const std::string & aText, bool hasText = true);
	//std::string& GetTextRef();
	//TextID GetTextID() const;
	//void SetTextID(TextID id);
	//bool HasText() const;
	//void ReserveTextArray();

	Trigger_ID GetStringID() const;

	CScriptComponent& GetScript();
	const CScriptComponent& GetScript() const;

#ifndef _RETAIL
	virtual void DrawWireframe() const = 0;
#endif

protected:


	void NewID();
	static int ID_COUNT;
	Trigger_ID myStringId;

	CScriptComponent myLuaScript;
	Vector3f myPosition;

	ETriggerShape myTriggerShape;

	int myTikiCount;

	//TextID myTextId;
	//std::string myText;
	//bool myHasText;

	CSpriteInWorldInstance mySpriteInstance;
	Vector3f mySpriteWorldPosition;
	bool myHasSprite;


	Corgi::GrowingArray<Trigger_ID> myCurrentIDs;
	Corgi::GrowingArray<Trigger_ID> myPreviousIDs;
};