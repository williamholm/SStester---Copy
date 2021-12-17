#pragma once
#include <iostream>
#include <cstdint>
#include <type_traits>
#include "EntityTester.h"
#include "EntityManager.h"
#include "2SortsSparse.hpp"
#include "PerfectET.hpp"
#include "TestSystem.hpp"
#include "Entity.hpp"
#include "TypeSortedSS.h"
/*
Ordered Sparse Requirements:

Sparse rules:

1) entity gives you CDS value
2) EDS gives entity value
3) CDS must be linked to EDS, such that you can get entity from CDS[i] without to much work.

4) implementation detail : all internal arrays have to be considered to start at [1] as 0 is reserved for no entity,
this behaviour could be changed by making reserved value = UINT32_MAX
*/
/*
Access:

1) single access via entity
2) multi unordered(or ordered within ETs) access via EntityType(s)
3) multi access via multi ordered access via EntityTypes (or all)
4) try and keep SS1[enity] = SS2[entity] + offset i.e all components have same indexing for each ET (or change by known offset for the type). 

Sorting:

1) all should be able to sort within ETs
2) given a group of ETs much sort all of CDS containing ETs
3) Match sorting must be possible 

*/

int main()
{
	Testing::Timer timer;
	//EMTSSS emt;
	//EntityManager tes;
	EntityTester tester(1000000);
	//tester.messUpCache();
	//testSystem(&tes);
	//testSystem2(&emt);
	tester.deleteEntities(500000);
	tester.addEntities(500000);
	constexpr auto parent = getParents(MAGIC_ARROW); //slightly nicer syntax, however not much use in practice as arrays are all same size.
	constexpr auto inheritor = getInheritors(PHYS_OBJ);
	constexpr auto component = getComponents(TALKING_WOLF);

//	static std::array<uint32_t, maxEntityNumber> sparse;
//	TypeSortedSS<POS3D> posSS(&sparse);
//
//	
//
//	//example of using sparse without EM
//	TwoSortsSparse<Comp_ID::POS3D> bo(100000);
//	//
//	std::vector<vec3> testVec;
//	timer.startTimer();
//	for (int i = 1; i < 10000; ++i)
//	{
//		if (ComponentArray<Comp_ID::POS3D>::value[(i % MAX_ET_ID) + 1])
//		{
//			posSS.addComponent(Entity32Bit(i, (i % MAX_ET_ID) + 1), vec3(i % (2 * MAX_ET_ID), i % (2 * MAX_ET_ID), i % (2 * MAX_ET_ID)));
//
//			bo.addComponent(Entity32Bit(i, (i % MAX_ET_ID)+1), vec3(i % (2*MAX_ET_ID), i % (2 * MAX_ET_ID), i % (2 * MAX_ET_ID)));
//			testVec.push_back(vec3(i % 14, i % 14, i % 14));
//		}
//
//
//	}
//	std::cout << "\n  vol size = " << posSS.size() << " 2SS size = " << bo.mCDS.size()-1;
//	timer.printTime(std::string("\nadd component "));
//
//	timer.startTimer();
//	bo.quickSort(1, bo.mEDS.size());
//	timer.printTime(std::string("\nqsort "));
//	ET<(ET_ID)4>::components;
//	Comp<POS3D>::sparse;
//	timer.startTimer();
//	bo.typeSort();
//	timer.printTime(std::string("\ntype sort 1 "));
//
//	timer.startTimer();
//	for (int i = 3; i < 10000; i += MAX_ET_ID)
//	{
//		posSS.deleteComponent(Entity32Bit(i, 4));
//	}
//	timer.printTime("delete Timere Vol: ");
////	std::cout << "\n  vol size = " << posSS.size() << " 2SS size = " << bo.mCDS.size() - 1;
////	posSS.print();
//	for (int j = 1; j < ET_ID::MAX_ET_ID; j++)
//	{
//		std::cout << "\n group: " << j << "\n ";
//		for (int i = bo.groupBegin((ET_ID)j); i < bo.groupEnd((ET_ID)j); ++i)
//		{
//			std::cout << bo.mCDS[i].x << " ";
//		}
//	}
//	
//	timer.startTimer();
//	bo.quickSort(1, bo.mEDS.size());
//	timer.printTime(std::string("\nqsort "));
//
//	timer.startTimer();
//	bo.typeSort2();
//	timer.printTime(std::string("\nTypeSort 2 "));
//
//	timer.startTimer();
//	std::sort(testVec.begin(), testVec.end());
//	timer.printTime(std::string("\nstd::sort "));
	while (1) {}
	return 0;
}