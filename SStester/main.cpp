#pragma once
#include <iostream>
#include "EntityTester.h"
#include "EntityManager.h"
#include "2SortsSparse.hpp"
#include "PerfectET.hpp"
#include <cstdint>
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
Flaws

cannot do something like for loops as non static cannot be used as template args

can't
void foo(ET_ID id){ ET<id>::components}
can
template<ET_ID id>
void boo() {ET<id>::components}.

1)Live with it
2)Just make the editor

*/
/*
* METHOD 2 Basic Functions
*/

union ETs
{
	ET<ARROW> Arrow;
	ET<MAGIC_ARROW> MagicArrow;
};

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

//want to make this a for loop to use an array to cycle through ET<id>.
/*
 Most likey use is ET<id>::someIDs -> for all SomeIds func(std::array<comp_id>)
*/
template<int N, const std::array<ET_ID,N>& arr, int index = 0>
struct static_for
{
	static void loop(std::function< void(Comp_ID)> func)
	{
		std::cout <<"\n"<< arr[index] << "\n";
		//either need to write each loop here, or find a way to utilize 
		for (int i = 0; i < ET<arr[index]>::noOfComponents; ++i)
		{
			//apply func to each comp_id;
			func(ET<arr[index]>::components[i]);
		}
		//continue looping
		static_for< N, arr, index + 1>::loop(func);
	}
};
//termination specialization
template<int N, const std::array<ET_ID, N>& arr>
struct static_for<N,arr,N>
{
	static void loop(std::function< void(Comp_ID)> func){}
};

//likely wont work with ET<xyz>;
template <auto Start, auto End, auto Inc, class F>
constexpr void constexpr_for(F&& f)
{
	if constexpr (Start < End)
	{
		f(std::integral_constant<decltype(Start), Start>());
		constexpr_for<Start + Inc, End, Inc>(f);
	}
}

template<ET_ID id>
constexpr void testfunc(ET<id> entityType)
{
	for (auto& test : ET<id>::parents)
	{
	//	getParents(test);
	}
}
int main()
{
	testfunc(ET<ARROW>());
	//this is the way, can more functions be made like this?
	constexpr auto tomfoolery = getPareents(ET<MAGIC_ARROW>());
	constexpr auto domfoolery = getInheritors(PHYS_OBJ);
	constexpr auto vomfoolery = getComponents(ARROW);
	constexpr BoundsArray<Comp_ID, MAX_ET_ID+1,componentBounds<>::value[MAX_ET_ID]> tester(componentBounds<>::value, componentAccess<>::value);
	componentBounds<>::value;
	constexpr auto brackish = componentBounds<>::value; //need to shift so its {0,0,2...}
	componentAccess<>::value;
	SparseManager<Comp_ID::MASS> sparsey;
	ET<ET_ID::TALKING_WOLF>::noOfParents; 
	ET<ET_ID::TALKING_WOLF>::parents;
	ET<ET_ID::MAGIC_ARROW>::parents;

	ET<ET_ID::OBJ>::noOfDirectInheritors;
	ET<ET_ID::OBJ>::directInheritors;
	ET<ET_ID::OBJ>::noOfInheritors;
	ET<ET_ID::OBJ>::inheritors;

	ET<ET_ID::PHYS_OBJ>::noOfInheritors;
	ET<ET_ID::PHYS_OBJ>::directInheritors;
	ET<ET_ID::PHYS_OBJ>::inheritors;
	ET<ET_ID::MAGIC_ARROW>::noOfComponents;
	constexpr auto t3 = componentBounds<>::value[MAGIC_ARROW+1]-componentBounds<>::value[MAGIC_ARROW];
	ET<ET_ID::OBJ>::noOfComponents;
	constexpr auto t2 = componentBounds<>::value[OBJ+1] - componentBounds<>::value[OBJ];
	ET<ET_ID::PROJECTILE>::noOfComponents;
	constexpr auto t1 = componentBounds<>::value[PROJECTILE];
	ET<ET_ID::MAGIC>::noOfComponents;
	ET<MAX_ET_ID>::noOfComponents;
	ET<ET_ID::MAGIC_ARROW>::components;
	ET<ET_ID::OBJ>::components;
	ET<ET_ID::PHYS_OBJ>::components;

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