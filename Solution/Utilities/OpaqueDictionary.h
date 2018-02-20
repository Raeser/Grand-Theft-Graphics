#pragma once
#include <string>
#include <map>
#include <new>
#include "Debuglog.h"

enum eOpaqueKey
{
	LookAtPosition,
	Radius,
	LocalSpeed,
	RotationDelta,
	playerCamera,
	DirectionSpeed,
	MovementDirection,

	PlayerJustFiredCameraFeedback,
	PlayerJustFiredCameraReset,
	PlayerJustBoostedCameraFeedback,
	PlayerJustBoostedCameraReset,

	CameraShakeDamage,
	CameraShakeBoost,
	CameraShakeOffset,

	AIShouldShoot,
	IsAI,
	IsAIAvoiding,
	AIAvoidThisPos,
	AICheckLineOfSight,

	CollisionDisplacementVector,

	Health,
	DamageToTake,
	DamageToShield,
	HealthToGive,
	BoostModifier,
	IsAlive,

	PickupIsPickedUp,
	ShouldRenderModel,
	count
};

namespace Corgi
{

	typedef eOpaqueKey Key;

	template<long long Size>
	class OpaqueDictionary
	{
	public:
		OpaqueDictionary();
		~OpaqueDictionary();

		template<typename T>
		const T* GetValue(Key aKey);

		template<typename T>
		void SetValue(Key aKey, const T& aValue);

		template<typename T>
		void Init(Key aKey, const T& aValue);

		template<typename T>
		void ForceInit(Key aKey, const T& aValue);

		void Reset();

	private:

		const bool CheckIfForceInited(Key aKey);
		const bool CheckIfInited(Key aKey);

		//std::map<Key, void*> myData;
		void* myData2[eOpaqueKey::count] = { nullptr };
		bool myInitedTypes[eOpaqueKey::count] = { false };
		bool myForceInitedTypes[eOpaqueKey::count] = { false };

		const std::type_info* myDataTypes2[eOpaqueKey::count];
		//std::map<Key, const std::type_info*> myDataTypes;
		char* myMemoryBucket;
		unsigned long long myIndex;
		unsigned long long myMaxSize;
	};

	template<long long Size>
	inline OpaqueDictionary<Size>::OpaqueDictionary()
	{
		myMaxSize = Size;
		myMemoryBucket = new char[Size];
		myIndex = 0;

	}

	template<long long Size>
	inline OpaqueDictionary<Size>::~OpaqueDictionary()
	{
	}

	template<long long Size>
	inline void OpaqueDictionary<Size>::Reset()
	{
		for (int i = 0; i < static_cast<int>(eOpaqueKey::count); ++i)
		{
			myData2[i] = nullptr;
			myInitedTypes[i] = false;
			myForceInitedTypes[i] = false;
		}
		delete[] myMemoryBucket;
		myMemoryBucket = new char[Size];
		myIndex = 0;
	}

	template<long long Size>
	template <typename T>
	inline const T* OpaqueDictionary<Size>::GetValue(Key aKey)
	{
		if (CheckIfInited(aKey))
		{
			return static_cast<T*>(myData2[static_cast<int>(aKey)]);
			//return static_cast<T*>(myData[aKey]);
		}
		else
		{
			DL_ERROR(eDebugLogType::Error, "%s", "TRIED TO GET VALUE OF A OPAQUE DICTIONARY KEY THAT IS EMPTY (INIT THE VALUE FIRST)!");
			assert(false && "TRIED TO GET VALUE OF A OPAQUE DICTIONARY KEY THAT IS EMPTY(INIT THE VALUE FIRST)!");
			return nullptr;
		}
	}
	template<long long Size>
	template<typename T>
	inline void OpaqueDictionary<Size>::SetValue(Key aKey, const T& aValue)
	{
		 if(CheckIfInited(aKey))
		{
			//if (typeid(aValue) == *myDataTypes[aKey])
			if (typeid(aValue) == *myDataTypes2[static_cast<int>(aKey)])
			{
				*static_cast<T*>(myData2[static_cast<int>(aKey)]) = aValue;
				//*static_cast<T*>(myData[aKey]) = aValue;
			}
			else
			{
				//DL_ERROR(eDebugLogType::Error, "%s%s%s%s%s", "INVALID DATATYPE IN OPAQUE DICTIONARY. EXPECTED: \"", myDataTypes[aKey]->name(), "\", BUT INSTEAD RECIEVED: \"", typeid(aValue).name(), "\"");
				DL_ERROR(eDebugLogType::Error, "%s%s%s%s%s", "INVALID DATATYPE IN OPAQUE DICTIONARY. EXPECTED: \"", myDataTypes2[static_cast<int>(aKey)]->name(), "\", BUT INSTEAD RECIEVED: \"", typeid(aValue).name(), "\"");

			}
		}
		 else
		 {
			 DL_ERROR(eDebugLogType::Error, "%s", "OPAQUE DICTORNARY - You tried to acess a uninitizalized a key value ");
			 assert(false);
		 }



	}

	template<long long Size>
	template<typename T>
	inline void OpaqueDictionary<Size>::Init(Key aKey, const T& aValue)
	{
		if (CheckIfForceInited(aKey) == false)
		{
			if (CheckIfInited(aKey) == false)
			{
				//SetForceInited
				myInitedTypes[static_cast<int>(aKey)] = true;

				//if (myData.find(aKey) != myData.end())
				if (myData2[static_cast<int>(aKey)] != nullptr)
				{
					//SetValue
					SetValue(aKey, aValue);
				}
				else
				{
					//myData[aKey] = new (&myMemoryBucket[myIndex]) T(aValue);
					//myDataTypes[aKey] = &typeid((aValue));

					myData2[static_cast<int>(aKey)] = new (&myMemoryBucket[myIndex]) T(aValue);
					myDataTypes2[static_cast<int>(aKey)] = &typeid((aValue));

					if (myIndex + sizeof(T) > myMaxSize)
					{
						DL_ERROR(eDebugLogType::Error, "%s", "OPAQUE DICK OUT OF SPACE, ALLOCATE MORE MEMORY");
					}
					myIndex += sizeof(T);
				}
			}
		}
	}


	template<long long Size>
	template<typename T>
	inline void OpaqueDictionary<Size>::ForceInit(Key aKey, const T& aValue)
	{
		if (CheckIfForceInited(aKey) == false)
		{
			//SetForceInited
			myForceInitedTypes[static_cast<int>(aKey)] = true;
			//SetInited to true
			myInitedTypes[static_cast<int>(aKey)] = true;

			//if (myData.find(aKey) != myData.end())
			if (myData2[static_cast<int>(aKey)] != nullptr)
			{
				//SetValue
				SetValue(aKey, aValue);
			}
			else
			{
				//myData[aKey] = new (&myMemoryBucket[myIndex]) T(aValue);
				//myDataTypes[aKey] = &typeid((aValue));

				myData2[static_cast<int>(aKey)] = new (&myMemoryBucket[myIndex]) T(aValue);
				myDataTypes2[static_cast<int>(aKey)] = &typeid((aValue));

				if (myIndex + sizeof(T) > myMaxSize)
				{
					DL_ERROR(eDebugLogType::Error, "%s", "OPAQUE DICK OUT OF SPACE, ALLOCATE MORE MEMORY");
				}
				myIndex += sizeof(T);
			}
		}
		else
		{
			DL_ERROR(eDebugLogType::Error, "%s", "OPAQUE DICTORNARY - You tried to forceinit a forcekeyd value");
			assert(false && "You have already tried to ForceInit this Key, only once is accepted!");
		}
	}



	template<long long Size>
	const bool OpaqueDictionary<Size>::CheckIfForceInited(Key aKey)
	{
		return myForceInitedTypes[static_cast<int>(aKey)];
	}

	template<long long Size>
	const bool OpaqueDictionary<Size>::CheckIfInited(Key aKey)
	{
		return myInitedTypes[static_cast<int>(aKey)];
	}

}

