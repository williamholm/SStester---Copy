#pragma once
#include "ET.h"

#pragma region ComponentArray Function

template<Comp_ID comp_id, int index = ET_ID::MAX_ET_ID>
struct ComponentArray
{
	static constexpr std::array<bool, index + 1> value =
		concatinate(ComponentArray<comp_id, index - 1>::value, isInArray(comp_id, ET<(ET_ID)index>::components));
};

template<Comp_ID comp_id>
struct ComponentArray<comp_id, ET_ID::MAX_ET_ID>
{
	static constexpr std::array<bool, ET_ID::MAX_ET_ID + 1> value = concatinate(ComponentArray<comp_id, ET_ID::MAX_ET_ID - 1>::value, false);
};

template<Comp_ID comp_id>
struct ComponentArray<comp_id, 0>
{
	static constexpr std::array<bool, 1> value = { isInArray(comp_id, ET<(ET_ID)0>::components) };
};


#pragma endregion


//MAX_ET_ID+1 for now for formatting, might be changed later.
template<Comp_ID id>
constexpr std::array<int, ET_ID::MAX_ET_ID + 1> getCompSparse()
{
	int counter = 0;
	std::array<int, ET_ID::MAX_ET_ID + 1> sparse = {};
	for (int i = 0; i < ET_ID::MAX_ET_ID; ++i)
	{
		if (ComponentArray<id>::value[i])
		{
			sparse[i] = ++counter;
		}
	}
	//last element of array is no of entity types with this component
	sparse[ET_ID::MAX_ET_ID] = counter;
	return sparse;
}
template<Comp_ID id, typename ComponentType = typename CompInfo<id>::type>
struct Comp
{
	//these 3 moved here from CompInfo for ease of access despite duplication.
	using type = ComponentType;
	static constexpr uint32_t attributes = CompInfo<id>::attributes;
	static constexpr uint32_t expectedAmount = CompInfo<id>::expectedAmount;
	//if compArray[ET_ID] = true ET_ID contains component.
	static constexpr auto compArray = ComponentArray<id>::value;
	//sparse set for ordering ETs which have this component, last value is total no of entitys with component.
	static constexpr auto sparse = getCompSparse<id>();
	static constexpr auto noOfETsWithComp = sparse[ET_ID::MAX_ET_ID];
	//offsets for SS so they can be condensed - this doesn't need to be implemented for my uses i think.
	static constexpr std::array<uint32_t, ET_ID::MAX_ET_ID> offsets = {};
};