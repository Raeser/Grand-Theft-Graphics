#pragma once
#include "..\Engine\Subscriber.h"

class LuaExposedVariableWrapper :
	public CSubscriber
{
public:
	virtual ~LuaExposedVariableWrapper();
	static LuaExposedVariableWrapper& GetInstance()
	{
		static LuaExposedVariableWrapper me;
		return me;
	}
	void RecieveMessage(const SMessage& message) override;

	int GetTikiCount() const;
	int GetTikiLimit() const;
	void SetTikiLimit(int aLimit);
	const Vector3f& GetPlayerPosition() const;

	void InitPointers(Corgi::CMemoryPool<CEntity>* aEntityPool, Corgi::GrowingArray<long long>* aUpdateArray);

	void ResetPointers();

	Corgi::CMemoryPool<CEntity>* GetEntityPoolPtr();
	Corgi::GrowingArray<long long>* GetUpdateableIDsPtr();

private:


	LuaExposedVariableWrapper();

	Corgi::CMemoryPool<CEntity>* myEntityPoolPtr;
	Corgi::GrowingArray<long long>* myUpdateableIdsPtr;

	Vector3f myPlayerPosition;
	int myTotalTikiCounter;
	int myTikiMaxLimit;
};


