#pragma once
#include <assert.h>

namespace Corgi
{
	template<typename ObjectType, typename SizeType = long long>
	class CMemoryPool
	{
	public:
		CMemoryPool();
		CMemoryPool(SizeType aAmountOfItemsToUse, bool aUseSafeModeFlag = true);
		CMemoryPool(const CMemoryPool& aMemoryPool);
		CMemoryPool(CMemoryPool&& aMemoryPool);

		~CMemoryPool();


		void Init(SizeType aAmountOfItemsToUse, bool aUseSafeModeFlag = true);

		inline SizeType GetFreeItem();
		inline ObjectType& NewItem();
		inline ObjectType & ReleaseObject(const SizeType& aIndex);

		inline ObjectType& operator[](const SizeType& aIndex);
		inline const ObjectType& operator[](const SizeType& aIndex) const;
		inline bool IsCurrentlyUsed(const SizeType& aIndex) const;
		inline void RemoveAll();
		inline void DeleteAll();
		inline void FreeAll();

		inline ObjectType* begin() const;
		inline ObjectType* end() const;

		void Optimize();
		__forceinline SizeType Size() const;


	private:
		void AssertIfNotInit(const CMemoryPool * aMemoryPool) const;
		ObjectType* myData;
		bool* myFreeObjects;

		bool myInitialized;
		bool mySafeMode;
		SizeType myMaxSize;
		SizeType myNumberOfElements;
		//SizeType myLargestIndex;
	};


	template<typename ObjectType, typename SizeType>
	inline CMemoryPool<ObjectType, SizeType>::CMemoryPool()
	{
		myInitialized = false;
		myFreeObjects = nullptr;
		mySafeMode = true;
		myMaxSize = 0;
		myNumberOfElements = 0;
		//myLargestIndex = 0;
		myData = nullptr;
	}
	template<typename ObjectType, typename SizeType>
	inline CMemoryPool<ObjectType, SizeType>::CMemoryPool(SizeType aAmountOfItemsToUse, bool aUseSafeModeFlag)
	{
		myInitialized = true;
		myNumberOfElements = 0;
		myMaxSize = aAmountOfItemsToUse;
		myData = new ObjectType[myMaxSize];
		myFreeObjects = new bool[myMaxSize];
		for (int i = 0; i < myMaxSize; ++i)
		{
			myFreeObjects[i] = true;
		}
		mySafeMode = aUseSafeModeFlag;
		//myLargestIndex = 0;
	}
	template<typename ObjectType, typename SizeType>
	inline CMemoryPool<ObjectType, SizeType>::CMemoryPool(const CMemoryPool & aMemoryPool)
	{
		myInitialized = true;
		myMaxSize = aMemoryPool.Size();
		mySafeMode = aMemoryPool.mySafeMode;


		if (mySafeMode)
		{
			myFreeObjects = new bool[myMaxSize];
			for (int i = 0; i < myMaxSize; ++i)
			{
				myFreeObjects[i] = true;
			}
			myData = new ObjectType[myMaxSize];
			for (SizeType i = 0; i < myMaxSize; i++)
			{
				myFreeObjects[i] = aMemoryPool.myFreeObjects[i];
				myData[i] = aMemoryPool.myData[i];
			}
		}
		else
		{
			myData = new ObjectType[myMaxSize];
			myFreeObjects = new bool[myMaxSize];
			for (int i = 0; i < myMaxSize; ++i)
			{
				myFreeObjects[i] = true;
			}
			memcpy(myData, aMemoryPool.myData, sizeof(ObjectType)*myMaxSize);
			memcpy(myFreeObjects, aMemoryPool.myFreeObjects, sizeof(bool) * myMaxSize);
		}
		myNumberOfElements = myMaxSize;
		//myLargestIndex = myMaxSize;
	}
	template<typename ObjectType, typename SizeType>
	inline CMemoryPool<ObjectType, SizeType>::CMemoryPool(CMemoryPool && aMemoryPool)
	{
		myInitialized = true;
		myMaxSize = aMemoryPool.myMaxSize;
		myNumberOfElements = aMemoryPool.myNumberOfElements;
		mySafeMode = aMemoryPool.mySafeMode;
		myData = aMemoryPool.myData;
		myFreeObjects = aMemoryPool.myFreeObjects;
		aMemoryPool.myData = nullptr;
		//myLargestIndex = aMemoryPool.myLargestIndex;
	}
	template<typename ObjectType, typename SizeType>
	inline CMemoryPool<ObjectType, SizeType>::~CMemoryPool()
	{
		delete[] myData;
		delete[] myFreeObjects;
		myFreeObjects = nullptr;
		myInitialized = false;
		mySafeMode = false;
		myMaxSize = 0;
		myNumberOfElements = 0;
		//myLargestIndex = 0;
		myData = nullptr;
	}

	template<typename ObjectType, typename SizeType>
	inline void CMemoryPool<ObjectType, SizeType>::Init(SizeType aAmountOfItemsToUse, bool aUseSafeModeFlag)
	{
		if (myInitialized)
		{
			delete[] myData;
			delete[] myFreeObjects;
			std::cout << "INIT TWICE ON MEME POOL" << std::endl;
		}
		myInitialized = true;
		myNumberOfElements = 0;
		//myLargestIndex = 0;
		myMaxSize = aAmountOfItemsToUse;
		myData = new ObjectType[myMaxSize];
		myFreeObjects = new bool[myMaxSize];
		for (int i = 0; i < myMaxSize; ++i)
		{
			myFreeObjects[i] = true;
		}
		mySafeMode = aUseSafeModeFlag;
	}

	template<typename ObjectType, typename SizeType>
	inline SizeType CMemoryPool<ObjectType, SizeType>::GetFreeItem()
	{
		for (int i = 0; i < myMaxSize; ++i)
		{
			if (myFreeObjects[i])
			{
				myFreeObjects[i] = false;
				if (i > myNumberOfElements)
				{
					myNumberOfElements = i;
				}
				return i;
			}
		}
		assert(false && "MemoryPool is full, maybe it should be larger or you might need to release unused objects");
		return -1;
	}

	template<typename ObjectType, typename SizeType>
	inline ObjectType & CMemoryPool<ObjectType, SizeType>::NewItem()
	{
		return myData[GetFreeItem()];
	}

	template<typename ObjectType, typename SizeType>
	inline ObjectType & CMemoryPool<ObjectType, SizeType>::ReleaseObject(const SizeType & aIndex)
	{
		//--myNumberOfElements;
		//myData[aIndex] = myData[myNumberOfElements];
		myFreeObjects[aIndex] = true;
		return myData[aIndex];
	}
	template<typename ObjectType, typename SizeType>
	inline ObjectType & CMemoryPool<ObjectType, SizeType>::operator[](const SizeType & aIndex)
	{
		AssertIfNotInit(this);
		assert(aIndex >= 0 && "Index is below 0");
		assert(aIndex < myMaxSize && "Index is out of bounds");
		//assert(!myFreeObjects[aIndex] && "Index is not assigned");
		return myData[aIndex];
	}
	template<typename ObjectType, typename SizeType>
	inline const ObjectType & CMemoryPool<ObjectType, SizeType>::operator[](const SizeType & aIndex) const
	{
		AssertIfNotInit(this);
		assert(aIndex >= 0 && "Index is below 0");
		assert(aIndex < myMaxSize && "Index is out of bounds");
		//assert(!myFreeObjects[aIndex] && "Index is not assigned");
		return myData[aIndex];
	}

	template<typename ObjectType, typename SizeType>
	inline bool CMemoryPool<ObjectType, SizeType>::IsCurrentlyUsed(const SizeType & aIndex) const
	{
		return !myFreeObjects[aIndex];
	}

	template<typename ObjectType, typename SizeType>
	inline void CMemoryPool<ObjectType, SizeType>::RemoveAll()
	{
		AssertIfNotInit(this);
		FreeAll();
	}
	template<typename ObjectType, typename SizeType>
	inline void CMemoryPool<ObjectType, SizeType>::DeleteAll()
	{
		AssertIfNotInit(this);
		for (SizeType i = myNumberOfElements; i > 0; i--)
		{
			delete myData[i];
			myData[i] = nullptr;
		}
		FreeAll();
	}
	template<typename ObjectType, typename SizeType>
	inline void CMemoryPool<ObjectType, SizeType>::FreeAll()
	{
		for (int i = 0; i < myMaxSize; ++i)
		{
			myFreeObjects[i] = true;
		}
		myNumberOfElements = 0;
	}
	template<typename ObjectType, typename SizeType>
	inline ObjectType * CMemoryPool<ObjectType, SizeType>::begin() const
	{
		return &myData[0];
	}
	template<typename ObjectType, typename SizeType>
	inline ObjectType * CMemoryPool<ObjectType, SizeType>::end() const
	{
		return &myData[myNumberOfElements + 2];
	}
	template<typename ObjectType, typename SizeType>
	inline void CMemoryPool<ObjectType, SizeType>::Optimize()
	{
		AssertIfNotInit(this);
		if (myMaxSize > myNumberOfElements)
		{
			myMaxSize = myNumberOfElements;
			ObjectType* ga = new ObjectType[myMaxSize];
			for (SizeType i = 0; i < myNumberOfElements; i++)
			{
				ga[i] = myData[i];
			}
			myData = ga;
		}
	}
	template<typename ObjectType, typename SizeType>
	inline SizeType CMemoryPool<ObjectType, SizeType>::Size() const
	{
		AssertIfNotInit(this);
		return myNumberOfElements + 1;
	}
	template<typename ObjectType, typename SizeType>
	inline void CMemoryPool<ObjectType, SizeType>::AssertIfNotInit(const CMemoryPool * aMemoryPool) const
	{
		assert(aMemoryPool->myInitialized && "MemoryPool has not been initialized");
	}
};
