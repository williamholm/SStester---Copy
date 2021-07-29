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


//this technique might be useful, but its still non-static accces for the most part.
template<ET_ID id>
constexpr void testfunc(ET<id> entityType)
{
	for (auto& test : ET<id>::parents)
	{
		//	getParents(test);
	}
}

template<Comp_ID... components>
class test
{
private:
public:
	std::tuple<TwoSortsSparse<components>...> tsss;
	test() {}
	~test() = default;
};

template<typename... components>
class test2
{
public:
	std::tuple<components...> components;
	test2() {}
	~test2() = default;
};
template<int... ints>
constexpr int... sume(std::integer_sequence<int, ints...> seq)
{
	return (2,2);
}


int main()
{
	constexpr auto sequence = std::make_index_sequence<3>();
	constexpr auto seq = std::make_integer_sequence<int, MAX_COMP_ID>();
	constexpr auto blemish = sume<>(seq);
	EntityManager<(Comp_ID...)blemish> berd;
	std::tuple<TwoSortsSparse<MASS>, TwoSortsSparse<STATE>> testuple;
	std::get<1>(testuple).addComponent(Entity32Bit(1,OBJ),4.5f);
	std::cout << "\n\n testuple: " << std::get<1>(testuple).mCDS[1];

	EntityManager<MASS, VELOCITY> tes;
	tes.getComponentData<(Comp_ID)0>(0);
	auto reamed = tes.mSparses;

	constexpr auto tomfoolery = getParents(MAGIC_ARROW);
	constexpr auto domfoolery = getInheritors(PHYS_OBJ);
	constexpr auto vomfoolery = getComponents(ARROW);
	constexpr auto removed = resizeArray(tomfoolery, std::array<ET_ID,noOfParentArray<>::value[MAGIC_ARROW]>());
	int i = 4;
	i++;
	getComponents((ET_ID)i);

	constexpr BoundsArray<Comp_ID, MAX_ET_ID+1,componentBounds<>::value[MAX_ET_ID]> tester(componentBounds<>::value, componentAccess<>::value);
	for (int i = tester.mBounds[ARROW]; i < tester.mBounds[ARROW + 1]; ++i)
	{
		std::cout<<"\nDATA " <<(int)tester.mData[i];
	}
	SparseManager<Comp_ID::MASS> sparsey;

	ET<ARROW>::noOfComponents;
	ET<ARROW>::components;

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