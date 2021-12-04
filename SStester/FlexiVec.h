#pragma once
#include <vector>
#include "2SortsSparse.hpp"
#include <list>
/*
Concept 1

{x} is deleted component, inbetween bounds. (y) are alive component, in bounds

	A				B			C
[(1,2,3){4,5}		(6,7){}		(8,9,10){11,12}], to add new element to A, increase bound end of A by one.

	
	A				B			C
[(1,2,3,523){5}		(6,7){}		(8,9,10){11,12}], to delete first element of A, swap to end of group and decrease bounds by one.


	A				B			C
[(523,2,3){1,5}		(6,7){}		(8,9,10){11,12}], to add element to B, this is the worst case - basically
need to find nearest deleted element (C) then swap through groups.

problem: instert is costly or unstable, stable delete can get costly if groups are large
*/

/*
Concept 2

Blocks of X / size(T) (using size of T as a guess for cost of swapping), then assign those blocks to groups

A							B
[1,2,3] -> [4,5,deleted] ->	[6,7,8]->[9,del,del]

insert: instert into end of Block, unless there is no room then create new block and insert into structure.
delete: move to of group then set bounds to not include it, if block is fully empty (or 2 blocks) free the block
 
 can be used in both TypeSortedSS and 2SS (if groups added seperately).
*/

template<typename T>
class FlexiVec
{
private:
	std::vector<T> mVec;
	std::vector<Bound> mActiveBounds;
public:
	class FlexiVecIter
	{
		T* mpPos;

	};
	void push_back() {}
	//elements may change location, but in predictable ways
	void unstableInsert() {}
	void stableInsert(){} //need to permutate all groups untill you reach a block of deleted elements (can be bad)
	void unstableDelete(){}
	void stableDelete(){} //need to permutate 1 group for this. - what about using smaller blocks to limit this case at cost of slightly 
						  //more cache misses?
	void resize(){}
	//removes deleted elements (maybe leaves a set amount, or calculated amount (~10% of bound?) and resizes vector.
	void clearInactive() {}
	FlexiVec(int noOfBounds) {}
	~FlexiVec() = default;
};
//include groups here for ease?
template<typename T>
class BlockedList
{
	static const sizeFactor = 2048 / sizeof(T);
	//std::list<std::array<T, sizeFactor>> mList; //No random access this is bad, need a vector of pointers or something.
	std::vector<Bound> mBounds;
	void push_back() {}
	//elements may change location, but in predictable ways
	void unstableInsert() {}
	void stableInsert() {}
	void unstableDelete() {}
	void stableDelete() {}
	//this access seems bad - requires modulus + divide, could prob get rid of divide as it can be deduced by calc of modulus maybe?
	//+ need something to skip deleted elements.
	//T& operator[](Entity32Bit entity) {return mList[entity.group()][index % sizeFactor] }
	BlockedVec()
};
