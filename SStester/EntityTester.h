#pragma once
#include <functional>
#include <chrono>
#include <random>
#include "TestUtility.hpp"
#include "EntityManager.h"
#include "FunctionTimer.h"
#include "2SortsSparse.hpp"
#include "SparseSet.h"
//small test large class. Need to change and have overloaded < operater for some sort functionality.

template<int size>
struct BigData 
{ 
	std::vector<uint32_t> mMemoryFill;

	BigData() { mMemoryFill.resize(size); }
	~BigData() = default;
};

///In long run might be worth switching class to not having any vectors/sparse sets in it and instead having
///the data fill functions return a filled vector/sparse set.

class EntityTester
{
public:
	EntityManager mBasicEM;
	EMTSSS mTypeSortedEM;
	Testing::Timer mTimer;

	void randomFillData();
	auto messUpCache(); //makes sure cache is cleared of useful stuff hopefully, need to make sure compiler isn't ignoring this.
	
	void clearSS();
	void unorderedAccess();//eg update every entities position by flat
	void accessByOrderedValue();//can only be done on 2SS - might be a bad way to fake it on TSSS
	void accessByComp();//eg an position += velocity * timestep can only be done by TSSS without offsets.
	void accessAllETComp();
	template<ET_ID id>
	std::vector<Entity<id>> getSpreadOutEntities(uint32_t amount); //returns evenly spaced Entities in TSSS (might be different for others)
	template<ET_ID id>
	void addEntities(uint32_t amount);
	template<ET_ID id>
	void deleteEntities(uint32_t amount);
	void sortByType();
	void addEntities(uint32_t amount); //adds entities in given distrabution something like 50:40:30:20:10:10:6:5:4:3:2:1
	void deleteEntities(uint32_t amount);
	EntityTester(uint32_t noOfEntities);
	~EntityTester();
};

