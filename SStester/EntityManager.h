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
//0 to remove TSS<0> from tuple as Comp_ID 0 is a blank id.
template<int... ints>
constexpr auto testfun(std::integer_sequence<int, 0, ints...> seq)
{
	//there is an int so std::get<Comp_ID> is correctly allinged as ints is has values 1-MAX_COMP_ID
	return std::move(std::tuple<int, TwoSortsSparse<(Comp_ID)ints>...>());
}


/*
* Consider these basic examples:
* 
* move all projectiles within 30 units of player 10 units back. 
* 
* step 1) get player position
* step 2) read projectile positions
* step 3) compare positions
* step 4) update positions
* 
* for all NPCs create a wolf at their location
* 
*/
class EntityManager
{
public:
	//is there a better way to do this?
	inline static auto mSparses = testfun(std::make_integer_sequence<int, MAX_COMP_ID>());

	template<Comp_ID component> //what are the ramifications of access like this? seems bad for Multi Threading
	inline TwoSortsSparse<component>& sparse()
	{
		return std::get<component>(mSparses);
	}
	//should mark entity for deletion. then delete as soon as no ET data being used (multi threading)
	//Or should mark entity for deletion - have system to detect it has been marked and can't be used,
	//then delete at end of cycle for efficieny.
	void deleteEntity(Entity32Bit entity) {}

	
	//does it make more sense for this and other singular functions to be in classes instead of here?
	template<Comp_ID component, typename ReturnType = typename Comp<component>::type>
	inline ReturnType& getComponentData(Entity32Bit entity) { return std::get<component>(mSparses)(entity);	}
	//returns data given index of CDS.
	template<Comp_ID component, typename ReturnType = typename Comp<component>::type>
	inline ReturnType& getComponentData(int index) { return std::get<component>(mSparses)[index]; }
	//returns Entity in EDS at a given index
	template<Comp_ID component>
	inline Entity32Bit getEntity(int index) { return std::get<component>(mSparses).getEntity(index); }
	//return the starting index of ET in sparse<compononent>
	template<Comp_ID component>
	inline uint32_t getETBegining(ET_ID entityType) { return std::get<component>(mSparses).groupBegin(entityType); }
	//return the end + 1 index of ET in sparse<compononent>
	template<Comp_ID component>
	inline uint32_t getETend(ET_ID entityType) { return std::get<component>(mSparses).groupEnd(entityType);}

	template<Comp_ID component, typename ComponentType = typename Comp<component>::type>
	inline ComponentType& operator()(Entity32Bit entity) { return std::get<component>(mSparses)(entity); }
	EntityManager() {};
	~EntityManager() {};
};


template<Comp_ID id, typename ComponentType = typename CompInfo<id>::type>
class SparseManager
{
private:
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


