#pragma once
#include <vector>
#include <assert.h>
#include <functional>
#include <algorithm>


/*
	writing to a sorted vector
	CCDS[i] = 5
	CEDS[i] = eval

	CCDS[i] *= 2
	CDS[SS[CEDS[i]]] = CCDS[i];

	pros - if you want unsorted data you can get easily, perhaps more compatible with unsorted sets

	cons - basically 2x work for every write call.
		 - every assignment needs to happen twice - can create struct with Sorted and unsorted DS then overload assignment operator.
		 - more memory needed

	Or

	unsorted CDS = std::vector<data*> CDS
	unsorted EDS = std::vector<uint32_t> EDS (sorted, unsorted or both?)
	sorted CDS = std::vector<data> SCDS

	get data by Etype -> use CDS
	use data sorted -> use SCDS 

	pros - less copying, less assignment,

	cons - not great for getting per eType, not great for getting unsorted data back ,
		   More swapping (need to swap CDS,EDS(maybe),SDCS,SEDS(maybe)) (maybe)

	Or

	datalink -> basically same as pointer, just easier to use, less swapping as well!
*/

template<typename DataType>
class SparseSet 
{
	using Entity = uint32_t;
	using datalink = std::reference_wrapper<DataType>;
private:


	std::vector<DataType> mComponentDenseSet;
	std::vector<datalink> mUnsortedCDS;
	std::vector<uint32_t> mEntityDenseSet;
	std::vector<uint32_t> mSparseSet;
	std::vector<uint32_t> mETbounds;
//creaters/deleters/getters
public:
	void addComponent(uint32_t entity, const DataType& data)
	{
		//make sure entity isn't already having this component -- for components which are needed multiple times for same entity
		//a different solution is needed
		assert(mSparseSet[entity] == 0);
		mSparseSet[entity] = mComponentDenseSet.size(); //changed for size-1 and moved up 3 lines
		mComponentDenseSet.push_back(data);
		mEntityDenseSet.push_back(entity);
		mUnsortedCDS.push_back(mComponentDenseSet[mUnsortedCDS.size()]);
	}

	//if doing many deletes use unsortedDelete then re sort after.
	void deleteSortedComponent(uint32_t entity) 
	{
		assert(mSparseSet[entity] != 0);
		//swap entity component with last component 
		swapInsert(mEntityDenseSet.size()-1, mSparseSet[entity]);

		//delete component
		mComponentDenseSet.pop_back();
		mEntityDenseSet.pop_back();
		mSparseSet[entity] = 0;

	}

	void deleteUnsortedComponent(uint32_t entity)
	{
		assert(mSparseSet[entity] != 0);

		//sets the entity that contains the last element of CDS to have index = mSparseSet[entity] before swap
		mSparseSet[mEntityDenseSet[mComponentDenseSet.size() - 1]] = mSparseSet[entity];
		//swaps entity component with last component in CDS, then deletes from CDS.
		std::swap(mComponentDenseSet[mSparseSet[entity]], mComponentDenseSet[mComponentDenseSet.size() - 1]);
		std::swap(mEntityDenseSet[mSparseSet[entity]], mEntityDenseSet[mComponentDenseSet.size() - 1]);

		//delete component
		mComponentDenseSet.pop_back();
		mEntityDenseSet.pop_back();
		mSparseSet[entity] = 0;
	}

	void resize(uint32_t maxEntityNumber) noexcept
	{
		mSparseSet.resize(maxEntityNumber);
	}

	//returns reference to entity in EDS - used for public swapping
	uint32_t& getEntity(const uint32_t entity) { return mEntityDenseSet[mSparseSet[entity]]; };
	const std::vector<uint32_t>& readEntities() noexcept { return mEntityDenseSet; }
	const std::vector<uint32_t>& readSparseSet() noexcept { return mSparseSet; }
	const std::vector<DataType>& readData() noexcept { return mComponentDenseSet; }
	std::vector<DataType>* writeData() noexcept { return mComponentDenseSet.data(); }

	//return index at entity - might change this to ()opertator later.
	uint32_t SparseSetIndex(const uint32_t entity) noexcept { return mSparseSet[entity]; }

//Algorithm -- To do set component swaps to private, make public one swap using entity values.
public:
	//swap CDS(a,b) and EDS(a,b) and update sparse set; --Note confusion when using as you are swapping indexes not entities
	void fullSwapComponent(int a, int b) noexcept
	{

	//	std::swap(mSparseSet[mEntityDenseSet[a]],mSparseSet[mEntityDenseSet[b]]);
		mSparseSet[mEntityDenseSet[a]] = b;
		mSparseSet[mEntityDenseSet[b]] = a;

		std::swap(mComponentDenseSet[a], mComponentDenseSet[b]);
		std::swap(mEntityDenseSet[a], mEntityDenseSet[b]);
	}

	//swap CDS and EDS without updating sparse set - used for internal purposes
	void swapComponent(int a, int b) noexcept
	{
		std::swap(mComponentDenseSet[a], mComponentDenseSet[b]);
		std::swap(mEntityDenseSet[a], mEntityDenseSet[b]);
	}

	void swapComponent(const uint32_t entity1, const uint32_t entity2)
	{
		std::swap(mSparseSet[entity1], mSparseSet[entity2]);
		std::swap(getEntity(mSparseSet[entity1]), getEntity(mSparseSet[entity2]));
	}

	//inserts component with index b at a
	void swapInsert(int a, int b)
	{
		while (a > b)
		{
			fullSwapComponent(b,b+1);
			++b;
		}
		while(a < b)
		{
			fullSwapComponent(a++, b);
		}
	}

	//sorts elements belonging to [startIndex,endInxdex) - meant for elements < 100 or elements which are mostly sorted
	void InsertSort(uint32_t startIndex, uint32_t endIndex)
	{

		for (int subsetSize = startIndex; subsetSize < endIndex; ++subsetSize)
		{
			int prevIndex = subsetSize - 1;
			int currentIndex = subsetSize;
			for (; mComponentDenseSet[currentIndex] < mComponentDenseSet[prevIndex] && subsetSize > startIndex; currentIndex--)
			{
				swapComponent(prevIndex--, currentIndex);
			}
		}

		//links entity to components again after sorting. 
		///This might be better done within the algo for insert sort as many entities won't have changed
		//for (int i = startIndex; i <= endIndex; i++)
		//{
		//	mSparseSet[mEntityDenseSet[i]] = i;
		//}
	}

	//this is done to remain consistant with insert sort, having sorting on [SI,EI)
	void quickSort(uint32_t startIndex, uint32_t endIndex)
	{
		internalQuickSort(startIndex, endIndex-1);

		//links entity to components again after sorting. 
		for (int i = startIndex; i < endIndex; i++)
		{
			mSparseSet[mEntityDenseSet[i]] = i;
		}
	}

	///to be implemented
	void blockQuickSort(uint32_t startIndex, uint32_t endIndex)
	{
		//32 is cut off for insertion sorting
		if (startIndex < endIndex && endIndex - startIndex > 32)
		{
			uint32_t part = partition(startIndex, endIndex);
			internalQuickSort(startIndex, part);
			internalQuickSort(part + 1, endIndex);
		}
		else if (endIndex - startIndex > 1)
		{
			InsertSort(startIndex, endIndex);
		}
	}


	void matchSparseSet(uint32_t startIndex, uint32_t endIndex,const SparseSet<DataType>& masterSS)
	{
		std::vector<uint32_t> condensedMasterSS;
		//looks this goes through EDS,
		for (; startIndex < endIndex; ++startIndex)
		{

		}

	}
//internal algorithm
private:
	///hoare method
	void internalQuickSort(uint32_t startIndex, uint32_t endIndex)
	{
		//32 is cut off for insertion sorting
		if (startIndex < endIndex)
		{
			uint32_t part = partition(startIndex, endIndex);
			internalQuickSort(startIndex, part);
			internalQuickSort(part + 1, endIndex);
		}

	}

	uint32_t partition(uint32_t startIndex, uint32_t endIndex)
	{
		DataType pivot = mComponentDenseSet[(endIndex + startIndex) / 2];
		--startIndex;
		++endIndex;
		for (;;)
		{
			//find next index with component < pivot component
			while (mComponentDenseSet[++startIndex] < pivot)
			{
			}

			//find next index with component > pivot componet
			while (pivot < mComponentDenseSet[--endIndex])
			{

			}

			if (startIndex >= endIndex)
			{
				return endIndex;
			}
			swapComponent(startIndex, endIndex);
		}

	}

//opertators and constructors
public:
		
	//get component via entity number
	DataType& operator[] (const int entity) noexcept 
	{
		return mComponentDenseSet[mSparseSet[entity]];
	}
	//get component via index --- Mainly for testing purposes might remove later
	DataType& operator() (const int index) noexcept
	{
		return mComponentDenseSet[index];
	}

	//due to entity default constructor all sparse set values are set to 0, can change to maxEntity if prefered
	SparseSet(uint32_t maxEntity)
	{
		
		mComponentDenseSet.emplace_back(DataType());
		mEntityDenseSet.push_back(0);

		mSparseSet.resize(maxEntity);
	}

	SparseSet()
	{
		mComponentDenseSet.emplace_back(DataType());
		mEntityDenseSet.push_back(0);
	}

	~SparseSet() = default;
};
