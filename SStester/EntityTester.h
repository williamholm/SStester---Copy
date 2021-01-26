#pragma once
#include "EntityManager.h"
#include <functional>
#include <chrono>
#include <random>
#include "FunctionTimer.h"
#include "2SortsSparse.hpp"
//small test large class. Need to change and have overloaded < operater for some sort functionality.


class BigData 
{ 
	std::vector<uint32_t> mMemoryFill;
	int mSize;

	BigData(int size) :mSize(size) { mMemoryFill.resize(mSize); }
	~BigData() = default;
};

///In long run might be worth switching class to not having any vectors/sparse sets in it and instead having
///the data fill functions return a filled vector/sparse set.

class EntityTester
{
public:
	uint32_t mMaxEntityNumber;
	//used to get a baseline spead for read/writing with ideal packed data
	uint32_t mNoOfComponents;
	//used to space out entity numbers so no artificial caching benifit from being perfectly packed!
	//in future make it so entity numbers arent in order as well.
	int mEntityOffset;
	Testing::Timer mTimer;
	//Completely random data to be sorted
	std::vector<vec3> mVec3Baseline;
	std::vector<uint32_t> mIntBaseline;
	
	SparseSet<vec3> mVec3SparseSet;
	SparseSet<uint32_t> mIntSparseSet;
	
//	TwoSortsSparse<POSITION> mVec32SS;
//	TwoSortsSparse<TEST_INT> mInt2SS;

	//For testing sorting in worst case scenario 
	std::vector<vec3> mVec3ReverseOrder;
	std::vector<uint32_t> mIntReverseOrder;

	SparseSet<vec3> mVec3SparseSetRO;
	SparseSet<uint32_t> mIntSparseSetRO;

	//For testing sorting when only a few elements out of place
	std::vector<vec3> mVec3MostlySorted;
	std::vector<uint32_t> mIntMostlySorted;

	SparseSet<vec3> mVec3SparseSetMS;
	SparseSet<uint32_t> mIntSparseSetMS;

	void resizeTestVectors(uint32_t size);
	float findData();
	float readData();
	float writeData();
	float insertData();
	void removeData();

	void sortData();
	void insertionSort();
	void swapSort();
	void stdSort();
	//fills all data with randomly generated vec3s (floats) and ints.
	void randomFillData();
	//fills data in order other than n elements which are randomized
	void mostlySortedFillData(int n);

	EntityTester(uint32_t numOfComponent, uint32_t maxEntityNumber);
	~EntityTester();
};

