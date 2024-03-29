#pragma once
#include <vector>
#include <algorithm>
#include <iostream>
#include "PerfectET.hpp"
#include "Object.h"
#include "2SortsSparse.hpp"
#include "Entity.hpp"
#include "TypeSortedSS.h"
#include "ETData.hpp"
//#include "Object.h"

/* Design:
* 
* Aim to reduce individual SS sizes through condensing them, Aim to reduce largest entity num (if possible reuse number for
ETs with no overlapping components).
* Keep track of which functions are requesting r/w data for MT porpouses (maybe do this inpart within SS)
* Handle batch creation/deletion of entities
*

*/

/* Multi threading to keep in mind:
* Must declare what data needs to be accessed and whether its read or r/w
* Must check if data is sorted as wanted. If data isn't sorted but needs to be it must be re-sorted, this is then r/w.
* Must ensure that that data isn't in conflict with another system, and if not then update SS state.
* If read only any deletion/creation of entities must be done by EM in a batch.


*/


//helper function to create tuple of sparses.
//0 to remove TSS<0> from tuple as Comp_ID 0 is a blank id.
template<int... ints>
constexpr auto testfun(std::integer_sequence<int, 0, ints...> seq)
{
	//there is an int so std::get<Comp_ID> is correctly allinged as ints is has values 1-MAX_COMP_ID
	return std::move(std::tuple<int, TwoSortsSparse<(Comp_ID)ints>...>());
}
typedef decltype(testfun(std::make_integer_sequence<int, MAX_COMP_ID>())) BasicSSTuple;

//create tuple types for EM
template<int... ints>
constexpr auto testfun3(std::integer_sequence<int, 0, ints...> seq)
{

	return std::tuple<int, TypeSortedSS<(Comp_ID)ints>...>();
}

typedef decltype(testfun3(std::make_integer_sequence<int, MAX_COMP_ID>())) TypeSortedSSTuple;




template<ET_ID id>
class TaggedBound
{
public:
	static constexpr ET_ID mID = id;
	int start;
	int end;
	TaggedBound(const Bound& bound) :end(bound.mEnd), start(bound.mStart) {}
	~TaggedBound() = default;
};

class EntityManager
{
public:
	//is there a better way to do this?
	//inline static auto mSparses = testfun(std::make_integer_sequence<int, MAX_COMP_ID>());
	BasicSSTuple mSparses;
	uint32_t mNextEntityNum;
	std::vector<uint32_t> mDeletedEntityNum; 
	template<Comp_ID component> //what are the ramifications of access like this? seems bad for Multi Threading
	inline TwoSortsSparse<component>& sparse(){	return std::get<component>(mSparses);}

	template<ET_ID id>
	void addEntity(Entity<id>& entity, ETData<id>& data) //giving up on ETData as moving from ETData seems inefficient.
	{
		if(mDeletedEntityNum.size() == 0) //this if statement isn't great - should be a way to predict state of deleted entities
		{
			assert(mNextEntityNum < maxEntityNumber);
			entity.addNumber(mNextEntityNum++);
			//std::cout << "\n entity num: " << entity.number() << "    next: " << mNextEntityNum << "     entity group: " << entity.group();
		}
		else
		{
			entity.addNumber(*mDeletedEntityNum.end());
			mDeletedEntityNum.pop_back();
		}
		addData(entity, data);
	}

	template<ET_ID id, int index = ET<id>::noOfComponents-1>
	void addData(Entity32Bit entity, ETData<id>& data)//can remove entity and just add directly to CDS after chaning 2SS - test speed.
	{
		//data.get was causing stack overflow here. no idea why
		std::get<ET<id>::components[index]>(mSparses).addComponent(entity, data.get<ET<id>::components[index]>());
	//	std::get<ET<id>::components[index]>(mSparses).addComponent(entity, std::get<index>(data.data));
		if constexpr(index != 0)
		{
			addData<id, index - 1>(entity, data);
		}
	}

	//should mark entity for deletion. then delete as soon as no ET data being used (multi threading)
	//Or should mark entity for deletion - have system to detect it has been marked and can't be used,
	//then delete at end of cycle for efficieny.
	template<ET_ID id>
	void deleteEntity(Entity<id>& entity)
	{
		removeData<id>(entity);
		mDeletedEntityNum.push_back(entity.number());
	//	entity.~Entity(); //seems okay - not sure if this is final design
	}
	template<ET_ID id, int index = ET<id>::noOfComponents - 1>
	void removeData(Entity32Bit entity)//can remove entity and just add directly to CDS after chaning 2SS - test speed.
	{
		std::get<ET<id>::components[index]>(mSparses).deleteComponent(entity);
		if constexpr (index != 0)
		{
			removeData<id, index - 1>(entity);
		}
	}

	template<ET_ID id, int index = ET<id>::noOfComponents - 1>
	void addToPendingDelete(Entity<id> entity)
	{
		std::get<ET<id>::components[index]>(mSparses).addToPendingDelete(entity);
		if constexpr (index != 0)
		{
			addToPendingDelete<id, index - 1>(entity);
		}
	}

	template<ET_ID id, int index = ET<id>::noOfComponents - 1>
	void addToPendingDelete(const TaggedBound<id>& bound, Entity32Bit entityNum)
	{
		std::get<ET<id>::components[index]>(mSparses).addToPendingDelete(entityNum);
		if constexpr (index != 0)
		{
			addToPendingDelete<id, index - 1>(bound, entityNum);
		}
	}

	//has problem that it essentially means all threads must complete before this can be called, leading to a lot of stalling in system
	//if not handled properly
	template<Comp_ID component = (Comp_ID)1>
	void deletePending()
	{
		if constexpr (component != MAX_COMP_ID)
		{
			std::get<component>(mSparses).deletePending();
			deletePending<(Comp_ID)(component + 1)>();
		}
		return;
	}


	template<Comp_ID id>
	void shrink()
	{
		if constexpr (id > 1)
		{
			shrink<(Comp_ID)(id - 1)>();
		}

		std::get<id>(mSparses).shrink_to_fit();

	}
	void shrink_to_fit()
	{
		shrink<(Comp_ID)(MAX_COMP_ID - 1)>();
	}

	template<Comp_ID component>
	int size()
	{
		return std::get<component>(mSparses).size();
	}
	template<Comp_ID component, typename ReturnType = typename Comp<component>::type>
	inline ReturnType& getComponentData(Entity32Bit entity) { return std::get<component>(mSparses)(entity);	}
	//returns data given index of CDS.
	template<Comp_ID component, typename ReturnType = typename Comp<component>::type>
	inline ReturnType& getComponentData(int index) { return std::get<component>(mSparses)[index]; }
	//returns Entity in EDS at a given index
	template<Comp_ID component>
	inline Entity32Bit getEntity(int index) { return std::get<component>(mSparses).getEntity(index); }
	//return the starting index of ET in sparse<compononent>
	//template<Comp_ID component>
	//inline uint32_t getETBegining(ET_ID entityType) { return std::get<component>(mSparses).groupBegin(entityType); }
	//return the end index of ET in sparse<compononent>
	//template<Comp_ID component>
	//inline uint32_t getETend(ET_ID entityType) { return std::get<component>(mSparses).groupEnd(entityType);}
	//
	template<Comp_ID component>
	inline Bound getBound(ET_ID id) { return std::get<component>(mSparses).getBounds(id); }
private:
	//bounds of id + its inheritors.
	template<ET_ID id, Comp_ID component, int index>
	auto inheritorBounds()
	{
		TaggedBound<ET<id>::inheritors[index]> bounds(getBound<component>(ET<id>::inheritors[index]));
		if constexpr (index <= 0)
		{
			return bounds;
		}
		else
		{
			auto bs = std::make_tuple(bounds, inheritorBounds<id, component, index - 1>());
			return bs;
		}
	}
public:
	//tuple bounds, not for before c++23 or unless i have sol for iteration over tuple, SO sols don't match what i need for this.
	template<ET_ID id, Comp_ID component, int index = ET<id>::noOfInheritors>
	auto getInheritorBounds()
	{
		TaggedBound<id> bounds(getBound<component>(id));
		auto bs = std::make_tuple(bounds, inheritorBounds<id, component, index - 1>());
		return bs;
	}

	template<ET_ID id, Comp_ID component>
	std::vector<Bound> getAllBounds()
	{
		std::vector<Bound> bounds;
		bounds.push_back(getBound<component>(id));
		for (const auto& inheritor : ET<id>::inheritors)
		{
			bounds.push_back(getBound<component>(inheritor));
		}
		return bounds;
	}

	template<Comp_ID component, typename ComponentType = typename Comp<component>::type>
	inline ComponentType& operator()(Entity32Bit entity) { return std::get<component>(mSparses)(entity); }
	EntityManager() noexcept :mNextEntityNum(1) {};
};

/*
--------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------

*/

//horrible way to initialize tuple in way I want
template<int... ints>
struct sparseConstuctor
{
	std::tuple<int,TypeSortedSS<(Comp_ID)ints>...> tup;
	sparseConstuctor(SegmentedSS* ss) :tup(0,{ ss,ints }...) {}
};
template<int... ints>
constexpr auto testfun2(std::integer_sequence<int, 0, ints...> seq,SegmentedSS* ss)
{
	return std::move(sparseConstuctor<ints...>(ss).tup);
}

class EMTSSS
{
public:
	//inline static SegmentedSS mSharedSS;
	SegmentedSS mSharedSS;
	TypeSortedSSTuple mSparses;
	//inline static auto mSparses = testfun2(std::make_integer_sequence<int, MAX_COMP_ID>(),&mSharedSS);
	uint32_t mNextEntityNum;
	std::vector<uint32_t> mDeletedEntityNum;

	template<Comp_ID component>
	inline auto& sparse() { return std::get<component>(mSparses); } //for testing
	template<ET_ID id>
	void addEntity(Entity<id>& entity, ETData<id>& data)
	{
		if (mDeletedEntityNum.size() == 0) //this if statement isn't great - should be a way to predict state of deleted entities
		{
			assert(mNextEntityNum < maxEntityNumber);
			entity.addNumber(mNextEntityNum++);
		}
		else
		{
			entity.addNumber(*mDeletedEntityNum.end());
			mDeletedEntityNum.pop_back();
		}
		mSharedSS.addEntity(entity);
		addData(entity, data);
	}

	template<ET_ID id, int index = ET<id>::noOfComponents - 1>
	void addData(Entity32Bit entity, ETData<id>& data)//can remove entity and just add directly to CDS after chaning 2SS - test speed.
	{
		//data.get was causing stack overflow here. no idea why
		std::get<ET<id>::components[index]>(mSparses).addComponent(entity, data.get<ET<id>::components[index]>());
		//	std::get<ET<id>::components[index]>(mSparses).addComponent(entity, std::get<index>(data.data));
		if constexpr (index != 0)
		{
			addData<id, index - 1>(entity, data);
		}
	}

	template<ET_ID id>
	void deleteEntity(Entity<id>& entity)
	{
		removeData<id>(entity);
		mSharedSS.deleteEntity(entity);
		mDeletedEntityNum.push_back(entity.number());
	}
	template<ET_ID id, int index = ET<id>::noOfComponents - 1>
	void removeData(Entity32Bit entity)//can remove entity and just add directly to CDS after chaning 2SS - test speed.
	{
		std::get<ET<id>::components[index]>(mSparses).deleteComponent(entity);
		if constexpr (index != 0)
		{
			removeData<id, index - 1>(entity);
		}
	}

	template<Comp_ID id>
	void shrink()
	{
		if constexpr (id > 1)
		{
			shrink<(Comp_ID)(id - 1)>();
		}

		std::get<id>(mSparses).shrink_to_fit();

	}
	void shrink_to_fit()
	{
		shrink<(Comp_ID)(MAX_COMP_ID-1)>();
	}

	inline int size() { return mSharedSS.totalSize(); }

	template<Comp_ID component, typename ReturnType = typename Comp<component>::type>
	inline std::vector<ReturnType>& getETComps(ET_ID id) { return std::get<component>(mSparses).getETcomps(id); }

	template<Comp_ID component, typename ReturnType = typename Comp<component>::type>
	inline ReturnType& getComp(Entity32Bit entity) { return std::get<component>(mSparses).getComponent(entity); }

	template<Comp_ID component, typename ReturnType = typename Comp<component>::type>
	inline ReturnType& getComp(ET_ID eType, uint32_t index) { return std::get<component>(mSparses).getComponent(eType,index); }

	inline Entity32Bit getEntity(uint32_t group, uint32_t index) { return mSharedSS.getEntity(group,index); }
	inline uint32_t getNoOfET(ET_ID id) { return mSharedSS.size(id); }

	EMTSSS() noexcept :mNextEntityNum(1), mSharedSS()
	{
		mSparses = testfun2(std::make_integer_sequence<int, MAX_COMP_ID>(), &mSharedSS);
	};
};



