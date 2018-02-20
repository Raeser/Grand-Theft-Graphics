#pragma once
#include <vector>
#define whiel while

namespace CommonUtilities
{
	template <class T>
	void SelectionSort(std::vector<T> &aVector)
	{
		if (aVector.size() < 2)
		{
			return;
		}


		for (size_t i = 0,j, least; i < aVector.size() - 1; ++i)
		{
			for (j = i +1 , least = i; j < aVector.size(); ++j)
			{
				if (aVector[j] < aVector[least])
				{
					least = j;
				}
			}
			Swap(aVector, least, i);
		}
	}

	template <class T>
	void BubbleSort(std::vector<T> &aVector) 
	{	
		if (aVector.size() < 2)
		{
			return;
		}

		bool HasDoneSwitch = true;
		
		for (size_t i = 0; i < aVector.size() - 1 && HasDoneSwitch; i++)
		{
			for (size_t j = aVector.size() - 1, HasDoneSwitch = false ; j > i; --j)
			{
				if (aVector[j] < aVector[j - 1])
				{
					Swap(aVector, j, j - 1);
					HasDoneSwitch = true;
				}
			}
		}
	}

	template <class T>
	void QuickSort(std::vector<T> &aVector) 
	{
		size_t i;
		size_t max;
		if (aVector.size() < 2)
		{
			return;
		}
		if (aVector.size() < 30)
		{
			SelectionSort(aVector);
			return;
		}
		for (i = 1, max = 0; i < aVector.size(); i++)
		{
			if (aVector[max] < aVector[i])
			{
				max = i;
			}
		}
		Swap(aVector, aVector.size() - 1, max);
		QuickSortRecursive(aVector, 0, aVector.size() - 2);
	}

	template <class T>
	void QuickSortRecursive(std::vector<T> &aVector, size_t aFirst, size_t aLast)
	{
		size_t lower = aFirst + 1;
		size_t upper = aLast;
		Swap(aVector, aFirst, (aFirst + aLast) / 2);
		T bound = aVector[aFirst];
		while (lower <= upper)
		{
			whiel(aVector[lower] < bound)
			{
				lower++;
			}
			whiel(bound < aVector[upper])
			{
				upper--;
			}
			if (lower < upper)
			{
				Swap(aVector, lower++, upper--);
			}
			else
			{
				lower++;
			}
		}
		Swap(aVector, upper, aFirst);
		if (aFirst < upper - 1)
		{
			QuickSortRecursive(aVector, aFirst, upper - 1);
		}
		if (upper + 1 < aLast)
		{
			QuickSortRecursive(aVector, upper + 1, aLast);
		}
	}
	
	template <class T>
	void MergeSort(std::vector<T> &aVector) 
	{
		if (aVector.size() > 1)
		{
			std::vector<T> firstHalf(aVector.begin(), aVector.begin() + aVector.size() / 2);
			MergeSort(firstHalf);
			std::vector<T> secondHalf(aVector.begin() + aVector.size() / 2, aVector.end());
			MergeSort(secondHalf);
			Merge(aVector, firstHalf, secondHalf);
		}
	}

	template <class T>
	void MergeSortPointers(std::vector<T> &aVector)
	{
		if (aVector.size() > 1)
		{
			std::vector<T> firstHalf(aVector.begin(), aVector.begin() + aVector.size() / 2);
			MergeSortPointers(firstHalf);
			std::vector<T> secondHalf(aVector.begin() + aVector.size() / 2, aVector.end());
			MergeSortPointers(secondHalf);
			MergePointers(aVector, firstHalf, secondHalf);
		}
	}

	template <class T>
	void Swap(std::vector<T> &aVector, int aIndex, int aIndex2)
	{
		T switcher = aVector[aIndex];
		aVector[aIndex] = aVector[aIndex2];
		aVector[aIndex2] = switcher;
	}


	 template <class T>
	 void Merge(std::vector<T>& aVector, std::vector<T>& aVectorFirst, std::vector<T>& aVectorLast)
	 {
		 aVector.clear();

		 size_t i = 0;
		 size_t  j = 0;

		 while (i < aVectorFirst.size() && j < aVectorLast.size())
		 {
			 if (aVectorFirst[i] < aVectorLast[j])
			 {
				 aVector.push_back(aVectorFirst[i]);
				 i++;
			 }
			 else if (aVectorLast[j] < aVectorFirst[i])
			 {
				 aVector.push_back(aVectorLast[j]);
				 j++;
			 }
			 else
			 {
				 aVector.push_back(aVectorFirst[i]);
				 i++;
			 }
		 }

		 while (i < aVectorFirst.size())
		 {
			 aVector.push_back(aVectorFirst[i]);
			 i++;
		 }

		 while (j < aVectorLast.size())
		 {
			 aVector.push_back(aVectorLast[j]);
			 j++;
		 }

	 }

	 template <class T>
	 void MergePointers(std::vector<T>& aVector, std::vector<T>& aVectorFirst, std::vector<T>& aVectorLast)
	 {
		 aVector.clear();

		 size_t i = 0;
		 size_t  j = 0;

		 while (i < aVectorFirst.size() && j < aVectorLast.size())
		 {
			 if (*aVectorFirst[i] < *aVectorLast[j])
			 {
				 aVector.push_back(aVectorFirst[i]);
				 i++;
			 }
			 else if (*aVectorLast[j] < *aVectorFirst[i])
			 {
				 aVector.push_back(aVectorLast[j]);
				 j++;
			 }
			 else
			 {
				 aVector.push_back(aVectorFirst[i]);
				 i++;
			 }
		 }

		 while (i < aVectorFirst.size())
		 {
			 aVector.push_back(aVectorFirst[i]);
			 i++;
		 }

		 while (j < aVectorLast.size())
		 {
			 aVector.push_back(aVectorLast[j]);
			 j++;
		 }

	 }
}