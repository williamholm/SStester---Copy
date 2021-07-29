#pragma once
#include <vector>
#include <algorithm>
#include <iostream>
#include "PerfectET.hpp"
#include "Object.h"
#include "2SortsSparse.hpp"
//#include "Object.h"

/* Design:
* 
* Aim to reduce individual SS sizes through condensing them, Aim to reduce largest entity num (if possible reuse number for
ETs with no overlapping components).
* Keep track of which functions are requesting r/w data for MT porpouses (maybe do this inpart within SS)
* Handle batch creation/deletion of entities
*

*/

/* System:
* Must declare what data needs to be accessed and whether its read or r/w
* Must check if data is sorted as wanted. If data isn't sorted but needs to be it must be re-sorted, this is then r/w.
* Must ensure that that data isn't in conflict with another system, and if not then update SS state.
* If read only any deletion/creation of entities must be done by EM in a batch.


*/

/*
Test functions:
Basic:
createEntity(id,DataPack<id>)
moveAllInheritors(id,X) {for all ET<id>inheritors, pos + X}
deleteAll(id) {deletes all of type id}
Med:
2^n entity nums to save memory for small components.
createBatch(id,amount,std::vector<DataPack<id>> data){creates amount of entities}
Advanced:
delete/creating shared components.
*/


//helper function to create tuple of sparses.
template<int... ints>
auto testfun(std::integer_sequence<int, 0 ,ints...> seq)
{
	return std::move(std::tuple<TwoSortsSparse<(Comp_ID)ints>...>());
}

template<Comp_ID... components>
class EntityManager
{
	std::tuple<TwoSortsSparse<components>...> mSparseSets;
public:
	//is there a better way to do this?
	inline static auto mSparses = testfun(std::make_integer_sequence<int, MAX_COMP_ID>());
	template<Comp_ID component, typename ReturnType = typename Comp<component>::type>
	ReturnType getComponentData(int entityNum)
	{
		return std::get<(int)component>(mSparses)(entityNum);
	}
	EntityManager() {};
	~EntityManager() {};
};


template<Comp_ID id, class ComponentType = CompInfo<id>::type>
class SparseManager
{
private:
	//this should almost always point to same vector - should be moved to EM? why is this needed?
	std::array<uint32_t, maxEntityNumber>* pAvailableEntityNums;
	//This is general - if you want to overload an entity num this won't help
	uint32_t* pNextAvailableEntityNum;
	TwoSortsSparse<id, ComponentType> mSparseSet;
public:

	
	void readET(ET_ID id);
	void writeET(ET_ID id);
	void addEntity(Entity32Bit entity);
	void deleteEntity(Entity32Bit entity);
	
	//wtf is the point of doing this outside of SS?? scheduler/batch adding can be done here i guess?
	SparseManager() {}
	~SparseManager() = default;


};


