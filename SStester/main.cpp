#pragma once
#include <iostream>
#include "EntityTester.h"
#include "EntityManager.h"
#include "2SortsSparse.hpp"
#include "PerfectET.hpp"
#include <cstdint>
#include "TestSystem.hpp"
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

//problems 1: get a tuple of component types for each ET<id>


//trick to get type for ids, for some reason you can't use Comp<id>::type in Data. Might be better way.
template<Comp_ID id, typename ReturnType = typename Comp<id>::type>
constexpr ReturnType faker()
{
	return ReturnType();
}

template<int N>
constexpr std::array<int, MAX_COMP_ID> CompSparse(const std::array<Comp_ID, N>& arr)
{
	std::array<int, MAX_COMP_ID> sparse = {};

	for (int i = 0; i < MAX_COMP_ID; ++i)
	{
		sparse[i] = MAX_COMP_ID; //do this so 0's aren't default state of sparse. not needed for current function.
	}
	for (int i = 0; i < N; ++i)
	{
		sparse[arr[i]] = i;
	}

	return sparse;
}

template<Comp_ID id, typename CompType = typename Comp<id>::type>
struct CompData
{
	using type = CompType;
	CompType data;
};

template<ET_ID id, int... seq>
struct ETData
{
	static constexpr std::array<int, MAX_COMP_ID> sparse = CompSparse(ET<id>::components); //move to ET<id>.

	std::tuple<decltype(faker<ET<id>::components[seq]>())...> data;
	std::tuple<CompData<ET<id>::components[seq]>...> rata;
	CompData<MASS>::type borin;
	template<Comp_ID id, typename ComponentType = typename Comp<id>::type>
	inline constexpr void writeData(const ComponentType& Data)
	{
		std::get<sparse[id]>(data) = Data;
	}

	template<Comp_ID id, typename ComponentType = typename Comp<id>::type>
	inline constexpr ComponentType&& moveData()
	{
		return std::move(std::get<sparse[id]>(data));
	}
};

template<int... ints>
constexpr auto ETDataWrapper(std::integer_sequence<int, ints...> galling)
{
	return ETData<ARROW, ints...>();
}


//move this to ET<id>, or use to replace ETData?
template<ET_ID id, int compIndex = 0, int lastComp = ET<id>::noOfComponents - 1> //-1 for easiers specialization
struct idata
{
	using CompType = decltype(faker<ET<id>::components[compIndex]>());
	static constexpr auto data = std::tuple_cat(std::make_tuple(CompType()), idata<id, compIndex + 1, lastComp>::data);
};

template<ET_ID id, int compIndex>
struct idata<id, compIndex, compIndex>
{
	static constexpr std::tuple<decltype(faker<ET<id>::components[compIndex]>())> data = {};
};

template<ET_ID id>
class Entity : public Entity32Bit
{
public:
	//add entity to batch creator in EM - should this include data now to be tansfered later? is there a way to do otherwise?
	Entity(EntityManager* EM)
	{
		std::cout << "\nhere" << ET<id>::components[1] << "\n\n\n";
	}
	//make single entity instantly
	Entity(EntityManager* EM, decltype(idata<id>::data) data)
	{

	}
	~Entity() {}
};

int main()
{
	idata<ARROW> dita;
	auto blata = dita.data;

	typedef float Mass; //any value in this for clarity?
	constexpr auto sequ = std::make_integer_sequence<int,4>();
	auto newland = ETDataWrapper(sequ);
	typedef decltype(newland) ArrowData;
	newland.data;
	ArrowData test;
	test.data;
	test.writeData<VELOCITY>(4);
	auto moved = test.moveData<VELOCITY>();
	std::cout << "fghfgh" << std::get<0>(test.data);


	std::tuple<TwoSortsSparse<MASS>, TwoSortsSparse<STATE>> testuple;
	std::get<1>(testuple).addComponent(Entity32Bit(1,OBJ),4.5f);
	std::cout << "\n\n testuple: " << std::get<1>(testuple).mCDS[1];

	EntityManager tes;
	testSystem(&tes);
	Entity<ARROW> entityCreationTester(&tes);


	constexpr auto tomfoolery = getParents(MAGIC_ARROW);
	constexpr auto domfoolery = getInheritors(PHYS_OBJ);
	constexpr auto vomfoolery = getComponents(ARROW);


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