#pragma once
#include "../Utilities/Matrix.h"
#include "Component.h"
#include "..\Utilities\OpaqueDictionary.h"
#include "..\Engine\Subscriber.h"

#include "..\Component\TransformComponent.h"
#include "..\Component\ScriptComponent.h"



//#pragma warning( push ) 
//#pragma warning(disable : 4244)
//extern "C"
//{
//#include "..\Script\lua.h"
//#include "..\Script\lauxlib.h"
//#include "..\Script\lualib.h"
//}
//
//#pragma warning(pop)
#include "..\Script\ScriptManager.h"
//#include "..\Script\ScriptVarTemplates.h"

struct SMessage;

#define NUMBEROFBYTESOFDATA 10000

class CScriptComponent;

typedef std::string EntityID;

enum class EEntityType
{
	Player,
	Tiki,
	Enemy,
	WorldObject,
	LightSource,
	NavMeshCollider,
	Count
};



class CEntity : public CSubscriber
{
public:
	CEntity();
	~CEntity();
	bool Init(EEntityType aEntityType);
	bool Update();
	void Activate();
	void DeActivate();
	void SetIsStatic(const bool & aBool);
	void Release();
	void Reset();

	bool GetActiveStatus();
	bool ShouldRelease();

	//Corgi::OpaqueDictionary<NUMBEROFBYTESOFDATA>& GetData();


	CComponent* GetComponent(eComponentTag aComponentTag) const;

	CTransformComponent& GetTransform();
	const CTransformComponent& GetTransform() const;

	void AddComponent(CComponent* aComponent);
	void AddChild(CEntity* aEntity);

	void UpdateComponentParents();

	template<class ...Types>
	void CallFunction(SLuaFunctionName aFunctionName, Types ... args)
	{
		myScriptComponent->CallFunction(aFunctionName, args...);
	}

	const EntityID& GetID() const;
	void SetID(const EntityID& aID);
	EEntityType GetEntityType() const;

	virtual void RecieveMessage(const SMessage& message) override;

private:

	//Corgi::OpaqueDictionary<NUMBEROFBYTESOFDATA> myData;
	Corgi::GrowingArray<CComponent*> myComponents;
	Corgi::GrowingArray<CEntity*> myChildren;

	void SetAnimationOnModel(const SMessage& message);
	void SetMoveCommandToPositionIfMovable(const SMessage& message);
	void SetMoveCommandToPositionIfMovable_Relative(const SMessage & message);
	void NewID();
	static long long myStaticIdCounter;

	EntityID myStringId;

	EEntityType myEntityType;


	CTransformComponent myTransform;

	CScriptComponent* myScriptComponent;

	void InternalRelease();
	bool myIsActive;
	bool myShouldRelease;
	bool myIsStatic;

};
