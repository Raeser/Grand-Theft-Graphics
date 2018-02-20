#pragma once
#include <assert.h>
#include <iostream>

namespace Corgi
{
	template<typename ObjectType, typename SizeType = int>
	class GrowingArray
	{
	public:
		GrowingArray();
		GrowingArray(SizeType aNrOfRecommendedItems, bool aUseSafeModeFlag = true);
		GrowingArray(const GrowingArray& aGrowingArray);
		GrowingArray(GrowingArray&& aGrowingArray);

		~GrowingArray();

		GrowingArray& operator=(const GrowingArray& aGrowingArray);
		GrowingArray& operator=(GrowingArray&& aGrowingArray);
		GrowingArray& operator<<(const GrowingArray& aGrowingArray);

		void Init(SizeType aNrOfRecomendedItems, bool aUseSafeModeFlag = true);
		void ReInit(SizeType aNrOfRecomendedItems, bool aUseSafeModeFlag = true);

		inline ObjectType& operator[](const SizeType& aIndex);
		inline const ObjectType& operator[](const SizeType& aIndex) const;

		inline void Add(const ObjectType& aObject);
		inline void Insert(SizeType aIndex, ObjectType& aObject);
		inline void DeleteCyclic(ObjectType& aObject);
		inline void DeleteCyclicAtIndex(SizeType aItemNumber);
		inline void RemoveCyclic(const ObjectType& aObject);
		inline void RemoveCyclicAtIndex(SizeType aItemNumber);
		inline void RemoveVectoralAtIndex(SizeType& aIndex);
		inline SizeType Find(const ObjectType& aObject);
		inline ObjectType* begin() const;
		inline ObjectType* end() const;
		inline bool IsInited();

		inline ObjectType& GetLast();
		inline const ObjectType& GetLast() const;

		static const SizeType FoundNone = static_cast<SizeType>(-1);

		inline void RemoveAll();
		inline void DeleteAll();

		void Optimize();
		__forceinline SizeType Size() const;
		inline void Reserve(SizeType aNewSize);
		ObjectType* GetPureData();

	private:
		void AssertIfNotInit(const GrowingArray * aGrowingArray) const;
		void CheckSizeAndDouble(const GrowingArray & aGrowingArray);
		void DoubleIfArrayIsFull();
		ObjectType* myData;

		bool myInitialized;
		bool mySafeMode;
		SizeType myMaxSize;
		SizeType myNumberOfElements;
	};



	template<typename ObjectType, typename SizeType>
	inline GrowingArray<ObjectType, SizeType>::GrowingArray()
	{
		myInitialized = false;
		mySafeMode = true;
		myMaxSize = static_cast<SizeType>(0);
		myNumberOfElements = static_cast<SizeType>(0);
		myData = nullptr;
	}
	template<typename ObjectType, typename SizeType>
	inline GrowingArray<ObjectType, SizeType>::GrowingArray(SizeType aNrOfRecommendedItems, bool aUseSafeModeFlag)
	{
		myInitialized = true;
		myNumberOfElements = static_cast<SizeType>(0);
		myMaxSize = aNrOfRecommendedItems;
		myData = new ObjectType [myMaxSize];
		mySafeMode = aUseSafeModeFlag;
	}
	template<typename ObjectType, typename SizeType>
	inline GrowingArray<ObjectType, SizeType>::GrowingArray(const GrowingArray & aGrowingArray)
	{
		myInitialized = true;
		myMaxSize = aGrowingArray.myMaxSize;
		myNumberOfElements = aGrowingArray.Size();
		mySafeMode = aGrowingArray.mySafeMode;

		if (mySafeMode)
		{
			myData = new ObjectType[myMaxSize];
			for (SizeType i = 0; i < myMaxSize; i++)
			{
				myData[i] = aGrowingArray.myData[i];
			}
		}
		else
		{
			myData = new ObjectType[myMaxSize];
			memcpy(myData, aGrowingArray.myData, sizeof(ObjectType)*myMaxSize);
		}
	}
	template<typename ObjectType, typename SizeType>
	inline GrowingArray<ObjectType, SizeType>::GrowingArray(GrowingArray && aGrowingArray)
	{
		myInitialized = true;
		myMaxSize = aGrowingArray.myMaxSize;
		myNumberOfElements = aGrowingArray.myNumberOfElements;
		mySafeMode = aGrowingArray.mySafeMode;
		myData = aGrowingArray.myData;
		aGrowingArray.myData = nullptr;
	}
	template<typename ObjectType, typename SizeType>
	inline GrowingArray<ObjectType, SizeType>::~GrowingArray()
	{
		delete[] myData;
		myInitialized = false;
		mySafeMode = false;
		myMaxSize = static_cast<SizeType>(0);
		myNumberOfElements = static_cast<SizeType>(0);
		myData = nullptr;
	}
	template<typename ObjectType, typename SizeType>
	inline GrowingArray<ObjectType, SizeType> & GrowingArray<ObjectType, SizeType>::operator=(const GrowingArray & aGrowingArray)
	{
		AssertIfNotInit(&aGrowingArray);
		myInitialized = true;
		myMaxSize = aGrowingArray.myMaxSize;
		myNumberOfElements = aGrowingArray.Size();
		mySafeMode = aGrowingArray.mySafeMode;

		if (mySafeMode)
		{
			delete[] myData;
			myData = new ObjectType[myMaxSize];
			for (SizeType i = 0; i < aGrowingArray.Size(); i++)
			{
				myData[i] = aGrowingArray.myData[i];
			}
		}
		else
		{
			delete[] myData;
			myData = new ObjectType[myMaxSize];
			memcpy(myData, aGrowingArray.myData, sizeof(ObjectType)*myMaxSize);
		}


		return *this;
	}

	template<typename ObjectType, typename SizeType>
	inline GrowingArray<ObjectType, SizeType> & GrowingArray<ObjectType, SizeType>::operator<<(const GrowingArray & aGrowingArray)
	{
		AssertIfNotInit(&aGrowingArray);
		myInitialized = true;
		mySafeMode = aGrowingArray.mySafeMode;

		SizeType newNumberOfElements = myNumberOfElements + aGrowingArray.Size();
		assert(newNumberOfElements < myMaxSize && "To small growing array for << operator");

		SizeType aGrowingArraySize = aGrowingArray.Size();
		for (SizeType i = static_cast<SizeType>(0); i < aGrowingArraySize; i++)
		{
			myData[i + myNumberOfElements] = aGrowingArray[i];
		}
		myNumberOfElements = newNumberOfElements;
		return *this;
	}
	template<typename ObjectType, typename SizeType>
	inline GrowingArray<ObjectType, SizeType> & GrowingArray<ObjectType, SizeType>::operator=(GrowingArray && aGrowingArray)
	{
		AssertIfNotInit(&aGrowingArray);

		myInitialized = true;
		delete[] myData;
		myData = aGrowingArray.myData;
		myNumberOfElements = aGrowingArray.Size();
		myMaxSize = aGrowingArray.myMaxSize;
		aGrowingArray.myData = nullptr;

		return *this;
	}
	template<typename ObjectType, typename SizeType>
	inline void GrowingArray<ObjectType, SizeType>::Init(SizeType aNrOfRecomendedItems, bool aUseSafeModeFlag)
	{
		if (myInitialized)
		{
			//std::cout << "TRIED TO INIT ALREADY INITED GROWING ARRAY, CALLING RE INIT INSTEAD" << std::endl;
			ReInit(aNrOfRecomendedItems, aUseSafeModeFlag);
		}
		else
		{
			myInitialized = true;
			myNumberOfElements = 0;
			myMaxSize = aNrOfRecomendedItems;
			myData = new ObjectType[myMaxSize];
			mySafeMode = aUseSafeModeFlag;
		}
	}
	template<typename ObjectType, typename SizeType>
	inline void GrowingArray<ObjectType, SizeType>::ReInit(SizeType aNrOfRecomendedItems, bool aUseSafeModeFlag)
	{
		AssertIfNotInit(this);
		delete[] myData;

		myInitialized = true;
		myNumberOfElements = 0;
		myMaxSize = aNrOfRecomendedItems;
		myData = new ObjectType [myMaxSize];
		mySafeMode = aUseSafeModeFlag;
	}
	template<typename ObjectType, typename SizeType>
	inline ObjectType & GrowingArray<ObjectType, SizeType>::operator[](const SizeType & aIndex)
	{
		AssertIfNotInit(this);
		assert(aIndex >= 0 && "Index is below 0");
		assert(aIndex < myNumberOfElements && "Index is out of bounds");
		return myData[aIndex];
	}
	template<typename ObjectType, typename SizeType>
	inline const ObjectType & GrowingArray<ObjectType, SizeType>::operator[](const SizeType & aIndex) const
	{
		AssertIfNotInit(this);
		assert(aIndex >= 0 && "Index is below 0");
		assert(aIndex < myNumberOfElements && "Index is out of bounds");
		return myData[aIndex];
	}
	template<typename ObjectType, typename SizeType>
	inline void GrowingArray<ObjectType, SizeType>::Add(const ObjectType & aObject)
	{
		AssertIfNotInit(this);
		DoubleIfArrayIsFull();
		myData[myNumberOfElements] = aObject;
		myNumberOfElements++;
	}
	template<typename ObjectType, typename SizeType>
	inline void GrowingArray<ObjectType, SizeType>::Insert(SizeType aIndex, ObjectType & aObject)
	{
		AssertIfNotInit(this);
		assert(aIndex >= 0 && "Index have to be greater than 0");
		assert(aIndex < myNumberOfElements && "Index is out of bounds");
		DoubleIfArrayIsFull();

		for (SizeType i = myNumberOfElements - 1; i > aIndex; i--)
		{
			myData[i] = myData[i - 1];
		}

		myNumberOfElements++;
		myData[aIndex] = aObject;
	}
	template<typename ObjectType, typename SizeType>
	inline void GrowingArray<ObjectType, SizeType>::DeleteCyclic(ObjectType & aObject)
	{
		AssertIfNotInit(this);
		for (SizeType i = myNumberOfElements; i >= 0; i--)
		{
			if (myData[i] == aObject)
			{
				delete myData[i];
				myData[i] = GetLast();
				myNumberOfElements--;
			}
		}
	}
	template<typename ObjectType, typename SizeType>
	inline void GrowingArray<ObjectType, SizeType>::DeleteCyclicAtIndex(SizeType aItemNumber)
	{
		AssertIfNotInit(this);
		assert(aItemNumber >= 0 && "Index is below 0");
		assert(aItemNumber <= myNumberOfElements && "Index is out of bounds");
		myData[aItemNumber] = GetLast();
		delete myData[myNumberOfElements - 1];
		myNumberOfElements--;
	}
	template<typename ObjectType, typename SizeType>
	inline void GrowingArray<ObjectType, SizeType>::RemoveCyclic(const ObjectType & aObject)
	{
		AssertIfNotInit(this);
		for (SizeType i = 0; i < myNumberOfElements; i++)
		{
			if (myData[i] == aObject)
			{
				myData[i] = GetLast();
				myNumberOfElements--;
			}
		}
	}
	template<typename ObjectType, typename SizeType>
	inline void GrowingArray<ObjectType, SizeType>::RemoveCyclicAtIndex(SizeType aItemNumber)
	{
		AssertIfNotInit(this);
		assert(aItemNumber >= 0 && "Index is below 0");
		assert(aItemNumber <= myNumberOfElements && "Index is out of bounds");
		myData[aItemNumber] = GetLast();
		myNumberOfElements--;
	}
	template<typename ObjectType, typename SizeType>
	inline void GrowingArray<ObjectType, SizeType>::RemoveVectoralAtIndex(SizeType& aIndex) 
	{
		AssertIfNotInit(this);
		assert(aIndex >= 0 && "Index is below 0");
		assert(aIndex <= myNumberOfElements && "Index is out of bounds");

		SizeType i = aIndex;

		while (i < myNumberOfElements - 1)
		{
			myData[i] = myData[i + 1];
			++i;
		}
		--myNumberOfElements;
	}

	template<typename ObjectType, typename SizeType>
	inline SizeType GrowingArray<ObjectType, SizeType>::Find(const ObjectType & aObject)
	{
		AssertIfNotInit(this);
		for (SizeType i = 0; i < myNumberOfElements; i++)
		{
			if (myData[i] == aObject)
			{
				return i;
			}
		}
		return FoundNone;
	}
	template<typename ObjectType, typename SizeType>
	inline ObjectType & GrowingArray<ObjectType, SizeType>::GetLast()
	{
		AssertIfNotInit(this);
		return myData[myNumberOfElements - 1];
	}
	template<typename ObjectType, typename SizeType>
	inline const ObjectType & GrowingArray<ObjectType, SizeType>::GetLast() const
	{
		AssertIfNotInit(this);
		return myData[myNumberOfElements - 1];
	}
	template<typename ObjectType, typename SizeType>
	inline void GrowingArray<ObjectType, SizeType>::RemoveAll()
	{
		AssertIfNotInit(this);
		myNumberOfElements = 0;
	}
	template<typename ObjectType, typename SizeType>
	inline void GrowingArray<ObjectType, SizeType>::DeleteAll()
	{
		AssertIfNotInit(this);
		for (SizeType i = myNumberOfElements - 1; i > 0; i--)
		{
			delete myData[i];
		}
		myNumberOfElements = 0;
	}
	template<typename ObjectType, typename SizeType>
	inline void GrowingArray<ObjectType, SizeType>::Optimize()
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
	inline SizeType GrowingArray<ObjectType, SizeType>::Size() const
	{
		AssertIfNotInit(this);
		return myNumberOfElements;
	}
	template<typename ObjectType, typename SizeType>
	inline void GrowingArray<ObjectType, SizeType>::Reserve(SizeType aNewSize)
	{

		myMaxSize = aNewSize;
		ObjectType* ga = new ObjectType[myMaxSize];

		for (SizeType i = 0; i < myNumberOfElements; i++)
		{
			ga[i] = myData[i];
		}

		delete[] myData;
		myData = ga;
		ga = nullptr;
		
	}
	template<typename ObjectType, typename SizeType>
	inline void GrowingArray<ObjectType, SizeType>::AssertIfNotInit(const GrowingArray * aGrowingArray) const
	{
#ifdef _DEBUG
 		assert(aGrowingArray->myInitialized && "Growing Array has not been initialized");
#else
		aGrowingArray;
#endif
	}
	template<typename ObjectType, typename SizeType>
	inline void GrowingArray<ObjectType, SizeType>::CheckSizeAndDouble(const GrowingArray & aGrowingArray)
	{
		while (myMaxSize < aGrowingArray.myMaxSize)
		{
			if (myMaxSize == 0)
			{
				myMaxSize = 1;
			}
			myMaxSize *= 2;
			if (myMaxSize >= aGrowingArray.myMaxSize)
			{
				myData = new ObjectType [myMaxSize];
			}
		}
	}
	template<typename ObjectType, typename SizeType>
	inline void GrowingArray<ObjectType, SizeType>::DoubleIfArrayIsFull()
	{
		if (myNumberOfElements == myMaxSize)
		{	
			#ifdef _DEBUG
					assert(false && "A GrowingArray grew. Fix it or work in release");
			#endif
			if (myMaxSize == 0)
			{
				Reserve(2);
			}
			else 
			{
				Reserve(myMaxSize * 2);
			}
		}
	}

	template<typename ObjectType, typename SizeType>
	inline ObjectType*  GrowingArray<ObjectType, SizeType>::begin() const
	{
		return &myData[0];
	}

	template<typename ObjectType, typename SizeType>
	inline ObjectType*  GrowingArray<ObjectType, SizeType>::end() const
	{
		return &myData[myNumberOfElements];
	}

	template<typename ObjectType, typename SizeType>
	inline bool GrowingArray<ObjectType, SizeType>::IsInited()
	{
		return myInitialized;
	}

	template<typename ObjectType, typename SizeType>
	inline ObjectType*  GrowingArray<ObjectType, SizeType>::GetPureData()
	{
		return myData;
	}

};
