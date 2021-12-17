#pragma once
#include <vector>
#include "Object.h"
#include "Comp.h"

/*
Reasons for this class:

1) pre sorted into types
2) delete and add is fast
3) probably more simple to use/implement unless you need fully sorted stuff.
4) due to being sorted into types we have SS<comp1>(entity) has same index as SS<comp2>(entity) i.e only one lookup per entity (assuming
no add/delete since lookup) - can use same SS for all Components????
5) more friendly for MT as no need for pending delete to stall whole system (unless you are mixing in 2SS's).

Reasons against:

1) can't be sorted by component (other than within each vector of mCDS or copying to vector or doing some access shennanigans)
2) slightly more cache misses as data is less continuous
3) Can't do shared component as there is only one sparse set for all components so a shared component cannot have mEDS[group][i] = mCDS[group][i].
Can do pointers to an object stored elsewhere though, which might suffice for most purposes!

*/

class SegmentedSS
{
private:
	std::array<std::vector<Entity32Bit>, MAX_ET_ID> mEDS; //can make this a pointer as well? pointing to shared EDS between all TypeSortedSS
	std::vector<uint32_t> mSparse; //Is there a better way to do this?

public:

	inline bool entityInSet(Entity32Bit entity) noexcept{return (mSparse[entity.number()] != _UI32_MAX);}

	inline Entity32Bit getEntity(const uint32_t group, const uint32_t index) { return mEDS[group][index]; }
	inline uint32_t getIndex(const Entity32Bit entity) { return mSparse[entity.number()]; }
	inline void changeIndex(const Entity32Bit entity, const uint32_t value) { mSparse[entity.number()] = value; }

	void addEntity(const Entity32Bit entity)
	{
		assert(!entityInSet(entity));
		changeIndex(entity, mEDS[entity.group()].size());
		mEDS[entity.group()].push_back(entity);
	}
	void deleteEntity(const Entity32Bit entity)
	{
		assert(entityInSet(entity));
		//change last member in group to point to deleted component;
		changeIndex(*(mEDS[entity.group()].end() - 1), getIndex(entity));
		//swapComponent + delete EDS
		mEDS[entity.group()][getIndex(entity)] = *(mEDS[entity.group()].end() - 1);
		mEDS[entity.group()].pop_back();
		//clear entity in sparse
		changeIndex(entity, _UI32_MAX);
	}
	uint32_t totalSize()
	{
		int size = mEDS[1].size();
		for (int i = 2; i < MAX_ET_ID; ++i)
		{
			size += mEDS[i].size();
		}
		return size;

	}
	uint32_t size(ET_ID id)
	{
		return mEDS[id].size();
	}
	void print()
	{
		for (int i = 1; i < MAX_ET_ID; ++i)
		{
			int size = mEDS[i].size();
			for (int j = 0; j < size; ++j)
			{
				std::cout << "\nGroup: " << i << "Index: " << j << "Entity Number: " << mEDS[i][j].number();
			}
		}
	}

	SegmentedSS():mSparse()
	{
		//for (int i = 0; i < MAX_ET_ID; ++i)
		//{
		//	 mEDS[i].push_back(Entity32Bit()); //do this for EntityNum = 0 being a deleted number.
		//}
		mSparse.resize(maxEntityNumber);
		for (int i = 0; i < maxEntityNumber; ++i)
		{
			mSparse[i] = _UI32_MAX;
		}
	}
	~SegmentedSS() = default;
};


template<Comp_ID mID, typename CompType = typename Comp<mID>::type>
class TypeSortedSS
{
private:
	using component = Comp<mID>;
	std::array<std::vector<CompType>, MAX_ET_ID> mCDS;
	SegmentedSS* mpSS;
public:

	inline bool validEntityGroup(Entity32Bit entity) noexcept { return (component::sparse[entity.group()] != 0); }

	void addComponent(Entity32Bit entity, const CompType& data)
	{
		assert(validEntityGroup(entity));
		mCDS[entity.group()].push_back(data);
	}
	void deleteComponent(Entity32Bit entity)
	{
		//need to do this check here (atleast in debug) as entity in SharedSS is deleted after components
		assert(mpSS->entityInSet(entity) && validEntityGroup(entity));
		mCDS[entity.group()][mpSS->getIndex(entity)] = *(mCDS[entity.group()].end() - 1);
		mCDS[entity.group()].pop_back();
	}

	void shrink_to_fit()
	{
		for (int i = 1; i < MAX_ET_ID; ++i)
		{
			mCDS[i].shrink_to_fit();
		}
	}
	void print()
	{
		for (int i = 1; i < MAX_ET_ID; ++i)
		{
			int size = mCDS[i].size();
			for (int j = 0; j < size; ++j)
			{
				std::cout << "\nGroup: " << i << "Index: " << j << "Entity Number: " << mpSS->getEntity(i,j).number();
			}
		}
	}
	inline std::vector<CompType>& getETComps(ET_ID id) { return mCDS[id]; }
	inline CompType& getComponent(Entity32Bit entity) { return mCDS[entity.group()][mpSS->getIndex(entity)]; }
	inline CompType& getComponent(uint32_t eType, uint32_t index) { return mCDS[eType][index]; }

	inline Entity32Bit getEntity(uint32_t eType, uint32_t index) { return mpSS->getEntity(eType, index); }
	TypeSortedSS(SegmentedSS* sparseSet,int) :mpSS(sparseSet)
	{
		//for (int i = 0; i < MAX_ET_ID; ++i)
		//{
		//	  mCDS[i].emplace_back(CompType()); //do this for if I want 0 to be no entity
		//}
	}
	TypeSortedSS() {}
	~TypeSortedSS() = default;

};

