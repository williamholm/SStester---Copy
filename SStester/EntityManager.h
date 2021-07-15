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

template<Comp_ID id>
class SparseManager
{
private:
	//this should almost always point to same vector
	std::array<uint32_t, maxEntityNumber>* pAvailableEntityNums;
	//This is general - if you want to overload an entity num this won't help
	uint32_t* pNextAvailableEntityNum;
//	TwoSortsSparse<id> sparse;
public:

	
	SparseManager();
	~SparseManager();


};


