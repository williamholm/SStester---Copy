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


*/

class SegmentedSS
{
private:
	std::array<std::vector<Entity32Bit>, MAX_ET_ID> mEDS; //can make this a pointer as well? pointing to shared EDS between all TSSS
	std::vector<uint32_t> mSparse; //Is there a better way to do this?

public:
	inline Entity32Bit getEntity(uint32_t group, uint32_t index) { return mEDS[group][index]; }
	inline uint32_t getIndex(Entity32Bit entity) { return mSparse[entity.number()]; }
	inline void changeIndex(Entity32Bit entity, uint32_t value) { mSparse[entity.number()] = value; }
	void addEntity(Entity32Bit entity)
	{
		changeIndex(entity, mEDS[entity.group()].size());
		mEDS[entity.group()].push_back(entity);

	}
	void deleteEntity(Entity32Bit entity)
	{
		//change last member in group to point to deleted component;
		changeIndex(*(mEDS[entity.group()].end() - 1), getIndex(entity));
		//swapComponent + delete EDS
		mEDS[entity.group()][getIndex(entity)] = *(mEDS[entity.group()].end() - 1);
		mEDS[entity.group()].pop_back();
		//clear entity in sparse
		changeIndex(entity, 0);
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
		for (int i = 0; i < MAX_ET_ID; ++i)
		{
			mEDS[i].push_back(Entity32Bit());
		}
		mSparse.resize(maxEntityNumber);
	}
	~SegmentedSS() = default;
};


template<Comp_ID mID, typename CompType = typename Comp<mID>::type>
class TSSS
{
private:
	using component = Comp<mID>;
	std::array<std::vector<CompType>, MAX_ET_ID> mCDS;
	SegmentedSS* mpSS;
public:

	void addComponent(Entity32Bit entity, const CompType& data)
	{
		mCDS[entity.group()].push_back(data);
	}
	void deleteComponent(Entity32Bit entity)
	{
		mCDS[entity.group()][mpSS->getIndex(entity)] = *(mCDS[entity.group()].end() - 1);
		mCDS[entity.group()].pop_back();
	}

	uint32_t size()
	{
		int size = mCDS[1].size();
		for (int i = 2; i < MAX_ET_ID; ++i)
		{
			size += mCDS[i].size();
		}
		return size;

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

	inline Entity32Bit getEntity(uint32_t group, uint32_t index) { return mpSS->getEntity(group, index); }
	TSSS(SegmentedSS* sparseSet,int) :mpSS(sparseSet)
	{
		for (int i = 0; i < MAX_ET_ID; ++i)
		{
			mCDS[i].emplace_back(CompType());
		}
	}
	TSSS() {}
	~TSSS() = default;

};



template<Comp_ID mID, typename CompType = typename Comp<mID>::type>
class TypeSortedSS
{
private:
	using component = Comp<mID>;
	std::array<std::vector<CompType>, MAX_ET_ID> mCDS;
	std::array<std::vector<Entity32Bit>, MAX_ET_ID> mEDS; //can make this a pointer as well? pointing to shared EDS between all TSSS
	std::array<uint32_t,maxEntityNumber>* mpSparse; //Is there a better way to do this?
	
public:

	inline uint32_t getIndex(Entity32Bit entity) { return (*mpSparse)[entity.number()]; }
	inline void changeIndex(Entity32Bit entity, uint32_t value) { (*mpSparse)[entity.number()] = value; }
	void addComponent(Entity32Bit entity, const CompType& data)
	{
		changeIndex(entity, mCDS[entity.group()].size());//shouldn't need to do this every component as SS is shared!
		mCDS[entity.group()].push_back(data);
		mEDS[entity.group()].push_back(entity);
		
	}
	void deleteComponent(Entity32Bit entity)
	{
		//std::cout << "\n CDS size: " << mCDS[4].size() << " EDS size: " << mEDS[4].size() << " Entity Number: " <<
		//	entity.number() << " Entity Group: " << entity.group();

		//change last member in group to point to deleted component;
		changeIndex(*(mEDS[entity.group()].end() - 1), getIndex(entity));
		//swapComponent + delete entity CDS and EDS
		mCDS[entity.group()][getIndex(entity)] = *(mCDS[entity.group()].end() - 1);
		mCDS[entity.group()].pop_back();
		mEDS[entity.group()][getIndex(entity)] = *(mEDS[entity.group()].end()-1);
		mEDS[entity.group()].pop_back();
		//clear entity in sparse
		changeIndex(entity, 0);
	}
	uint32_t size()
	{
		int size = mCDS[1].size();
		for(int i = 2; i < MAX_ET_ID; ++i)
		{
			size += mCDS[i].size();
		}
		return size;
		
	}
	void print()
	{
		for (int i = 1; i < MAX_ET_ID; ++i)
		{
			int size = mCDS[i].size();
			for (int j = 0; j < size; ++j)
			{
				std::cout << "\nGroup: " << i << "Index: " << j << "Entity Number: " << mEDS[i][j].number();
			}
		}
	}
	inline CompType& getComponent(Entity32Bit entity) { return mCDS[entity.group()][getIndex(entity)]; }

	TypeSortedSS(std::array<uint32_t,maxEntityNumber>* sparse):mpSparse(sparse)
	{
	//	mSparse.resize(maxEntityNumber);
	}

};
