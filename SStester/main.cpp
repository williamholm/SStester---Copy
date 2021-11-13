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
4) try and keep SS1[enity] = SS2[entity] + offset i.e all components have same indexing for each ET (or change by known offset for the type). 

Sorting:

1) all should be able to sort within ETs
2) given a group of ETs much sort all of CDS containing ETs
3) Match sorting must be possible 


*/
template<std::size_t I = 0, typename FuncT, typename... Tp>
inline typename std::enable_if<I == sizeof...(Tp), void>::type
for_each(std::tuple<Tp...>&, FuncT) // Unused arguments are given no names.
{ }

template<std::size_t I = 0, typename FuncT, typename... Tp>
inline typename std::enable_if < I < sizeof...(Tp), void>::type
	for_each(std::tuple<Tp...>& t, FuncT f)
{
	f(std::get<I>(t));
	for_each<I + 1, FuncT, Tp...>(t, f);
}

template<ET_ID... ids>
void tester(std::tuple<TaggedBound<ids>...>const& bounds)
{
	std::apply
	(
		[](TaggedBound<ids>... bound)
		{
			((std::cout<<"\n\n bound end: " <<ET<bound.mID>::parents[0]), ...);
		},
		bounds
	);
}
int main()
{
	typedef float Mass; //any value in this for clarity?
	Testing::Timer timer;

	ETData<ARROW> dataTest;
	dataTest.get<VELOCITY>() = 8;
	EntityManager tes;
	testSystem(&tes);
	Entity<ARROW> entityCreationTester;
	for_each(dataTest.data, [](...) {std::cout << "h"; });
	auto teri = tes.getInheritorBounds<ARROW, POS3D>();
	tester(teri);

	//timer.startTimer();
	//for (int i = 1; i < 10000; ++i)
	//{
	//	tes.addEntity(entityCreationTester, dataTest);
	//}
	//timer.printTime("\nNew 10000 creation time for arrows: ");

	//timer.startTimer();
	//for (int i = 1; i < 10000; ++i)
	//{
	//	entityCreationTester.addNumber(i);
	//	tes.deleteEntity(entityCreationTester);
	//}
	//timer.printTime("\BareBones 10000 deletion time for arrows: ");
	constexpr auto tomfoolery = getParents(MAGIC_ARROW);
	constexpr auto domfoolery = getInheritors(PHYS_OBJ);
	constexpr auto vomfoolery = getComponents(ARROW);

	TwoSortsSparse<Comp_ID::POS3D> bo(100000);
	std::vector<vec3> testVec;

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