#pragma once
#include "Object.h"
#include "PerfectET.hpp"
#include "Comp.h"
/* FEATURES (k = no of Entity Types that this component can have, n is size of CDS)
* 1 indirection, random access via entity number
* o(1) access for each entity group
* o(k) insert/delete (unsorted) 
* o(n) insert/delete (sorted) (dont do this or use buffers and re-sort)
* o(nlogn) sort by val ~800micros for k = 10k
* o(n) sort by type ~100-200 micros for k = 10k. This sort retains val sort within groups.
*/
/*
* Design:
*
* Internal operations use Indices, Extrenal should use individual entity / entity groups
* Minimise add/delete - ideal to do them in batch before a sort by val or type;
* State - Either sorted by type / val or unsorted: maybe automate i.e all requests for r/w data set sortedByVal to false,
maybe have addition state for each group - could help with MT (if current r/w request wait/move on and come back later)
* 
*/


//TODO: 
/*
Delete/Add batch of entities.
add std::move optimizations.
Match Sort -- can we get this to work almost implicitly with typesort? -- might need a seperate matchsort for when sorted by value then.
Shared Resourses - EG, no need for multiple strings with the same name, or monster with everything the same but position / speed / ori?
Improve Value sort -- work on typeSort2 -- Look into merge sorting. Quick sort is ~1.5x slower on this than Sparse Set, need to do testing.

*/


/*
Can move to only one SS for non sorted components maybe but prob not much advantage over TSSS:

have new entity placed as such SS[entity.number()] = amount of entity.group() + 1, then each mCDS is accessed by adding an offset equal too
the sum of all the number of all entities in the groups before it in mCDS, i.e, bounds.start+offset.

however deleting an entity which has a group before will screw this up unless stable delete is used, which requires n swaps everytime.
the same thing happens with insert.

*/
struct Bound
{
	uint32_t mStart;
	uint32_t mEnd;
	Bound(uint32_t start, uint32_t end) : mStart(start), mEnd(end) {}
	~Bound() = default;
};

template<Comp_ID mID, typename DataType = typename Comp<mID>::type>
class TwoSortsSparse
{
	//not entirely sure why DataType needs to be specified here, but functionality is unchanged
	using component = Comp<mID>;

public:
	std::vector<DataType> mCDS;		//component dense set
	std::vector<Entity32Bit> mEDS;  //entity dense set
private:
	std::vector<uint32_t> mSparse;
	std::vector<uint32_t> mBounds;	//mBounds[i] is first index i'th ET for this SS
	std::vector<Entity32Bit> mPendingDeletes;
	const int mNoOfBounds = component::noOfETsWithComp + 1;
public:
	inline bool entityInSet(Entity32Bit entity) noexcept
	{
		return (mSparse[entity.number()] != 0);
	}
	inline bool validEntityGroup(Entity32Bit entity) noexcept
	{
		return (component::sparse[entity.group()] != 0);
	}

	void addComponent(Entity32Bit entity, const DataType& data)
	{
		//std::cout << "\n\n Entity in set: " << entityInSet(entity) << "  Entity num: " << entity.number() << "  Entity group: " << entity.group();
		//remember this switches off in release mode.
		assert(!entityInSet(entity) && validEntityGroup(entity));
		mSparse[entity.number()] = mCDS.size();
		mCDS.push_back(data);
		mEDS.push_back(entity);

		quickInsert(mSparse[entity.number()]);
	}
	void addToPendingDelete(Entity32Bit entity)
	{
		mPendingDeletes.push_back(entity);
	}
	void deleteComponent(Entity32Bit entity)
	{
		assert(entityInSet(entity));
		swapToBack(mSparse[entity.number()]);
		//mCDS.erase(mCDS.begin() + mSparse[entity.number()]); //takes 20x as long for 66 deletes at 10000 size()
		//mEDS.erase(mEDS.begin() + mSparse[entity.number()]);

		mCDS.pop_back();
		mEDS.pop_back();
		mSparse[entity.number()] = 0;
	}
	void deletePending()
	{
		int smallSize = 5000; //test for when this is most effecient -- seemed about equal for 3-5k.
		if ( mPendingDeletes.size() == 0)
		{
			return;
		}
		else if (mPendingDeletes.size() < smallSize)
		{
			for (const auto& entity : mPendingDeletes)
			{
				deleteComponent(entity);
				
			}
			mPendingDeletes.clear();
		}
		else
		{
			int i = mPendingDeletes.size() - 1;
			for (; i >= 0; --i)
			{
				fullSwapComponent(mSparse[mPendingDeletes[i].number()], mCDS.size() - 1);

				mCDS.pop_back();
				mEDS.pop_back();
				mSparse[mPendingDeletes[i].number()] = 0;
				mPendingDeletes.pop_back();
			}
			typeSort();
		}
	}
	//Inserts component into its ET group - requires max Component<mID>::numberOfETs * 2 swaps.
	void quickInsert(int index)
	{
		for (int i = component::sparse[mEDS[index].group()] + 1; i <= mNoOfBounds; ++i)
		{
			fullSwapComponent(index, mBounds[i]);
			++mBounds[i];
		}
	}

	void stableInsert()
	{

	}

	//works but might be a faster way to do this? - its not to slow tho. single core ~200micro secs for 10000 elements
	void typeSort()
	{
		//reset mBounds
		int boundSize = mBounds.size();
		for (int i = 1; i < boundSize; ++i)
		{
			mBounds[i] = 1;
		}
		//re-insert
		int size = mEDS.size();
		for (int i = 1; i < size; ++i)
		{
			quickInsert(i);
		}
	}

	//faster at least if mCDS is quick to copy ~60-200micro secs for sorting, keeps ordering - 100~500ms?? with reconect SS
	void typeSort2()
	{
		auto temp = mBounds;
		auto EDS = mEDS;
		auto CDS = mCDS;
		int size = mEDS.size();

		for (int i = 1; i < size; ++i)
		{
			++temp[component::sparse[EDS[i].group()]];
			std::swap(CDS[i], mCDS[temp[component::sparse[EDS[i].group()]] - 1]); //-1 as you need to increment b4 swapping;
			std::swap(EDS[i], mEDS[temp[component::sparse[EDS[i].group()]] - 1]);
		}
		//links entity to components again after sorting. 
		for (int i = 1; i < size; i++)
		{
			mSparse[mEDS[i].number()] = i;
		}
	}

	//this is done to remain consistant with insert sort, having sorting on [SI,EI)
	void quickSort(uint32_t startIndex, uint32_t endIndex)
	{
		internalQuickSort(startIndex, endIndex - 1);

		//links entity to components again after sorting. 
		for (int i = startIndex; i < endIndex; i++)
		{
			mSparse[mEDS[i].number()] = i;
		}
	}

private:
#pragma region Algorithms

	void stableSwapToBack(int index)
	{
		//insert swap to back of group
		//call middle of swap to back
		//create new component at end, swap last component of all groups, end with dead group
		//What happens for when group is in middle of all groups not start?
	}

	//swaps EDS[index],CDS[index] to back, doesn't gaurentee ordering by val only by type.
	void swapToBack(int index)
	{
		int nextType = component::sparse[mEDS[index].group()] + 1;
		fullSwapComponent(index, --mBounds[nextType]); //swap with last element in group
		index = mBounds[nextType];

		for (int i = mNoOfBounds; i > nextType; --i)
		{
			fullSwapComponent(index, mBounds[i] - 1);//swap with last element of prev type
			--mBounds[i];
		}
	}

	//swap CDS(a,b) and EDS(a,b) and updates sparse set;
	void fullSwapComponent(int a, int b) noexcept
	{
		swapSparse(a, b);
		swapComponent(a, b);
	}
	//swap only entity values - for connecting entity num to component after swapComponent;
	inline void swapSparse(int a, int b) noexcept
	{
		mSparse[mEDS[a].number()] = b;
		mSparse[mEDS[b].number()] = a;
	}
	//swap CDS and EDS without updating sparse set - used for internal purposes
	void swapComponent(int a, int b) noexcept
	{
		std::swap(mCDS[a], mCDS[b]);
		std::swap(mEDS[a], mEDS[b]);
	}
	//basic implementation
	void InsertSort(uint32_t startIndex, uint32_t endIndex)
	{
		for (int subsetSize = startIndex; subsetSize < endIndex; ++subsetSize)
		{
			int prevIndex = subsetSize - 1;
			int currentIndex = subsetSize;
			for (; mCDS[currentIndex] < mCDS[prevIndex] && subsetSize > startIndex; currentIndex--)
			{
				swapComponent(prevIndex--, currentIndex);
			}
		}
	}

	///hoare method
	void internalQuickSort(uint32_t startIndex, uint32_t endIndex)
	{
		if (startIndex < endIndex)
		{
			uint32_t part = partition(startIndex, endIndex);
			//32 is cut off for insertion sorting - needs testing but is ~20% faster than without
			if (part - startIndex < 32)
			{
				InsertSort(startIndex, part);
			}
			else
			{
				internalQuickSort(startIndex, part);
			}
			if (endIndex - (part + 1) < 32)
			{
				InsertSort(part + 1, endIndex);
			}
			else
			{
				internalQuickSort(part + 1, endIndex);
			}
		}

	}

	uint32_t partition(uint32_t startIndex, uint32_t endIndex)
	{
		DataType pivot = mCDS[(endIndex + startIndex) / 2];
		--startIndex;
		++endIndex;
		for (;;)
		{
			//find next index with component < pivot component
			while (mCDS[++startIndex] < pivot)
			{
			}

			//find next index with component > pivot componet
			while (pivot < mCDS[--endIndex])
			{

			}

			if (startIndex >= endIndex)
			{
				return endIndex;
			}
			swapComponent(startIndex, endIndex);
		}

	}

#pragma endregion

public:
	inline uint32_t groupBegin(const ET_ID id) { return mBounds[component::sparse[id]]; }
	inline uint32_t groupEnd(const ET_ID id) { return mBounds[component::sparse[id] + 1]; }
	//this gives unneeded info and can be manipulated by end user - consider returning an array and setting uneeded results to = 0 or making bounds
	//array a sparse set naturally.
	inline Bound getBounds(ET_ID id) { return Bound(mBounds[component::sparse[id]], mBounds[component::sparse[id] + 1]); }
	//returns entity at a given index of dense sets.
	inline Entity32Bit getEntity(const uint32_t index) { return mEDS[index]; }
	//returns indexs of CDS and EDS so that they can be accessed without going back into SparseSet
	inline uint32_t getIndex(const Entity32Bit entity) { return mSparse[entity.number()]; }
	//get component via entity number
	inline DataType& operator() (const Entity32Bit entity) noexcept
	{
		return mCDS[mSparse[entity.number()]];
	}
	//get component via index --- Mainly for testing purposes might remove later
	inline DataType& operator[] (const int index) noexcept
	{
		return mCDS[index];
	}

	TwoSortsSparse(uint32_t maxEntity)
	{
		mCDS.emplace_back(DataType());
		mEDS.push_back(Entity32Bit());
		mBounds.resize(component::noOfETsWithComp + 2); //+2 as first is empty value for sparse set
		mSparse.resize(maxEntity);

		//increment by one to account for 0 being reserved by SS
		for (auto& bound : mBounds) { ++bound; }
	}

	TwoSortsSparse()
	{
		mCDS.emplace_back(DataType());
		mEDS.push_back(Entity32Bit());
		mBounds.resize(component::noOfETsWithComp + 2);
		mSparse.resize(maxEntityNumber);//should change this to be smarter to save space.

		for (auto& bound : mBounds) { ++bound; };

	}

	~TwoSortsSparse() = default;
};