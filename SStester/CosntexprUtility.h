#pragma once
#include<array>

//returns true if x is in given array 
template<class T, int N>
constexpr bool isInArray(const T& x, const std::array<T, N>& arr)
{
	for (const auto& y : arr)
	{
		if (y == x)
		{
			return true;
		}
	}
	return false;
}

template<class T, int N, int M> //constexpr concatination of two arrays of the same type.
constexpr std::array<T, N + M> concatinate(const std::array<T, N>& arr1, const std::array<T, M>& arr2)
{
	std::array<T, N + M> temp = {};
	for (int i = 0; i < N; ++i)
	{
		temp[i] = arr1[i];
	}
	for (int i = N; i < N + M; ++i)
	{
		temp[i] = arr2[i - N];
	}

	return temp;
}
template<class T, int N> //for adding a single value to end of array
constexpr std::array<T, N + 1> concatinate(const std::array<T, N>& arr1, const T& x)
{
	std::array<T, N + 1> temp = {};
	for (int i = 0; i < N; ++i)
	{
		temp[i] = arr1[i];
	}
	temp[N] = x;

	return temp;
}
template<class T, int N> //for adding a single value to start of array
constexpr std::array<T, N + 1> concatinate(const T& x, const std::array<T, N>& arr1)
{
	std::array<T, N + 1> temp = {};
	for (int i = 0; i < N; ++i)
	{
		temp[i + 1] = arr1[i];
	}
	temp[0] = x;

	return temp;
}

//returns array with new size, all new T's are default initialized at end of original array. 
template<typename T, int currentSize, int newSize>
constexpr auto resizeArray(const std::array<T, currentSize>& arr)
{
	return concatinate(arr, std::array<T, newSize - currentSize>());
}
//different method for constexpr resize
template<typename T, int currentSize, int newSize>
constexpr std::array<T, newSize> resizeArray(const std::array<T, currentSize>& arr, const std::array<T, newSize>&)
{
	std::array<T, newSize> temp = {};
	for (int i = 0; i < newSize; ++i)
	{
		temp[i] = arr[i];
	}
	return temp;
}
template<typename T, int size>
constexpr auto noOfInstancesOf(const T& x, const std::array<T, size>& arr)
{
	int noOfX = 0;
	for (int i = 0; i < size; ++i)
	{
		if (x == arr[i])
		{
			++noOfX;
		}
	}
	return noOfX;
}
template<typename T, int size, int newSize>
constexpr auto removeInstancesOf(const T& x, const std::array<T, size>& arr)
{

	std::array<T, newSize> newArray = {};
	int counter = 0;
	for (int i = 0; i < size; ++i)
	{
		if (x != arr[i])
		{
			newArray[counter] = arr[i];
			++counter;
		}
	}
	return newArray;
}

//counts the number of unique ids in given array, assumes array < MAX_ET_ID, can choose better value later
template<typename T, int N>
constexpr int noOfUniqueET_IDs(const std::array<T, N>& arr)
{
	//MAX_ET_ID rather than N as compiler heap space is messed up otherwise. Can change to another constant as needed.
	std::array<bool, MAX_ET_ID> isDuplicate = {};//initialized to false (not sure if this is always standard)
	int noOfUniqueETs = 0;
	for (int i = 0; i < N; ++i)
	{
		if (!isDuplicate[arr[i]])
		{
			++noOfUniqueETs;
			isDuplicate[arr[i]] = true;
		}
	}
	return noOfUniqueETs;
}
//returns array of all unique ids in given array
template<int M, typename T, int N>
constexpr auto uniqueET_IDs(const std::array<T, N>& arr)
{
	std::array<bool, ET_ID::MAX_ET_ID> isDuplicate = {};//initialized to false (not sure if this is always standard)
	std::array<T, M> temp = {};
	int noOfUniqueETs = 0;
	if (M != 0)
	{
		for (int i = 0; i < N; ++i)
		{
			if (!isDuplicate[arr[i]])
			{
				temp[noOfUniqueETs] = arr[i];
				++noOfUniqueETs;
				isDuplicate[arr[i]] = true;
			}
		}
	}
	return temp;

}