#pragma once
#include <iostream>
#include "EntityTester.h"
#include "EntityManager.h"
#include "2SortsSparse.hpp"
#include "PerfectET.hpp"

/*
Ordered Sparse Requirements:

Sparse rules:

1) entity gives you CDS value
2) EDS gives entity value
3) CDS must be linked to EDS, such that you can get entity from CDS[i] without to much work.

4) implementation detail : all internal arrays have to be considered to start at [1] as 0 is reserved for no entity,
this behaviour could be changed by making reserved value = UINT32_MAX

Access:

1) single access via entity
2) multi unordered(or ordered within ETs) access via EntityType(s)
3) multi access via multi ordered access via EntityTypes (or all)
4) try and keep SS1[enity] = SS2[entity] i.e all components have same indexing for each ET (or change by known offset). 

Sorting:

1) all should be able to sort within ETs
2) given a group of ETs much sort all of CDS containing ETs
3) Match sorting must be possible 


*/
/*
game dev rules:
try and store states outside of gameloop.
*/

/*
* METHOD 2 Basic Functions
*/

Entity32Bit createObjet()
{

	return(Entity32Bit());

}
Entity32Bit createPhsicsObject()
{
	return(Entity32Bit());
}

template<ET_ID id>
class Entity
{

	Entity() {}
	~Entity() {}
};
template<>
class Entity<OBJ> 
{
	Entity32Bit entityNum;
	static constexpr auto bl = ETInfo<ET_ID::OBJ>::newComponents;
	Entity() {}
	~Entity() {}
};

template<>
class Entity<PHYS_OBJ> : Entity<OBJ>
{

};

template<>
class Entity<ARROW> : Entity<PHYS_OBJ>
{

};
template<>
class Entity<CREATURE> : Entity<PHYS_OBJ>
{

};

template<>
class Entity<PC> : Entity<CREATURE>
{
	//inherits creation via data packets
	void writeComponent();//is this better than doing shit as function with manager / system combo?
	void readComponent();
	//other entitys
	Entity<ARROW> arrow;
};

int main()
{
	ET<ET_ID::OBJ>::noOfDirectInheritors;
	ET<ET_ID::OBJ>::directInheritors;
	ET<ET_ID::OBJ>::noOfInheritors;
	ET<ET_ID::OBJ>::inheritors;

	ET<ET_ID::PHYS_OBJ>::noOfInheritors;
	ET<ET_ID::PHYS_OBJ>::directInheritors;
	ET<ET_ID::PHYS_OBJ>::inheritors;

	ET<ET_ID::CREATURE>::noOfInheritors;
	ET<ET_ID::CREATURE>::directInheritors;

	ET<ET_ID::MAGIC_ARROW>::noOfInheritors;
	ET<ET_ID::MAGIC_ARROW>::inheritors; //how the fuck does an array of size 0 have a value??????


	ET<ET_ID::TALKING_WOLF>::noOfInheritors;
	ET<ET_ID::TALKING_WOLF>::inheritors;

	ET<ET_ID::MAGIC_ARROW>::noOfComponents;
	ET<ET_ID::OBJ>::noOfComponents;
	ET<ET_ID::PROJECTILE>::noOfComponents;
	ET<ET_ID::MAGIC>::noOfComponents;
	ET<MAX_ET_ID>::noOfComponents;
	ET<ET_ID::MAGIC_ARROW>::components;
	ET<ET_ID::OBJ>::components;
	ET<ET_ID::PROJECTILE>::components;
	ET<ET_ID::MAGIC>::components;
	ComponentArray<Comp_ID::PERMA_FORCE>::value;
	Comp<Comp_ID::POS3D>::sparse;


	TwoSortsSparse<Comp_ID::POS3D> bo(100000);

	std::vector<vec3> testVec;

	Testing::Timer timer;
	timer.startTimer();

	for (int i = 1; i < 10000; ++i)
	{
		if (ComponentArray<Comp_ID::POS3D>::value[(i % MAX_ET_ID) + 1])
		{
			bo.addComponent(Entity32Bit(i, (i % MAX_ET_ID)+1), vec3(i % (2*MAX_ET_ID), i % (2 * MAX_ET_ID), i % (2 * MAX_ET_ID)));
			testVec.push_back(vec3(i % 14, i % 14, i % 14));
		}


	}
	timer.printTime(std::string("\nadd component "));

	timer.startTimer();
	bo.quickSort(1, bo.mEDS.size());
	timer.printTime(std::string("\nqsort "));


	timer.startTimer();
	bo.typeSort();
	timer.printTime(std::string("\ntype sort 1 "));

//	for (int i = 5; i < 10000; i += 2 * MAX_ET_ID)
//	{
//		bo.deleteComponent(Entity32Bit(i, 5));
//	}


	for (int j = 1; j < ET_ID::MAX_ET_ID; j++)
	{
		std::cout << "\n group: " << j << "\n ";
		for (int i = bo.groupBegin((ET_ID)j); i < bo.groupEnd((ET_ID)j); ++i)
		{
			std::cout << bo.mCDS[i].x << " ";
		}
	}

	timer.startTimer();
	bo.quickSort(1, bo.mEDS.size());
	timer.printTime(std::string("\nqsort "));

	timer.startTimer();
	bo.typeSort2();
	timer.printTime(std::string("\nTypeSort 2 "));

	timer.startTimer();
	std::sort(testVec.begin(), testVec.end());
	timer.printTime(std::string("\nstd::sort "));


	while (1) {}
	return 0;
}