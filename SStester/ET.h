#pragma once
#include "ETInfo.h"
#include "CosntexprUtility.h"

/*
+2				+2
<MA,2,1> --- <PROJ,1,0> --- <Obj,0,-1> (termination)
	|			  |
	|		 <PROJ,1,-1> (termination)
	|
+1	|
<MA,2,0> --- (termination)
	|
	|
<MA,2,-1> (termination)

*/
//iterates though ET_ID's, finding ids direct inheritors
#pragma region DirectInheritorFunctions

template<ET_ID id, int current_id = 0>
struct getNoOfDirectInheritors
{
	static constexpr int value = getNoOfDirectInheritors<id, ET_ID(current_id + 1)>::value +
		(int)isInArray(id, ETInfo<(ET_ID)current_id>::inheritsFrom);
};

template<ET_ID id>
struct getNoOfDirectInheritors<id, ET_ID::MAX_ET_ID>
{
	static constexpr int value = 0;
};

template<ET_ID id, int current_id = 0, bool ifIsh = isInArray(id, ETInfo<(ET_ID)current_id>::inheritsFrom)>
struct getDirectInheritors
{
	static constexpr std::array<ET_ID, getNoOfDirectInheritors<id>::value> value = getDirectInheritors<id, 1>::value;
};

template<ET_ID id, int current_id>
struct getDirectInheritors <id, current_id, true>
{
	static constexpr auto value = concatinate(getDirectInheritors<id, current_id + 1>::value, (ET_ID)current_id);
};

template<ET_ID id, int current_id>
struct getDirectInheritors <id, current_id, false>
{
	static constexpr auto value = getDirectInheritors<id, current_id + 1>::value;
};

template<ET_ID id>
struct getDirectInheritors<id, ET_ID::MAX_ET_ID, false>
{
	static constexpr std::array<ET_ID, 0> value = {};
};


#pragma endregion


#pragma region InheritorsFunctions

template<ET_ID id, int noOfDirectInheritors = getNoOfDirectInheritors<id>::value, int index = 0 >
struct getNoOfInheritors
{
	//static constexpr int value = 1 + getNoOfInheritors<(ET_ID)getDirectInheritors<id>::value[index]>::value
	//	+ getNoOfInheritors<id, noOfDirectInheritors, index + 1>::value;

	static constexpr int value = getNoOfInheritors<(ET_ID)getDirectInheritors<id>::value[index]>::value
		+ getNoOfInheritors<id, noOfDirectInheritors, index + 1>::value;
};

template<ET_ID id, int noOfDirectInheritors>
struct getNoOfInheritors<id, noOfDirectInheritors, noOfDirectInheritors>
{
	static constexpr int value = noOfDirectInheritors;
};



template<ET_ID id, int noOfDirectInheritors = getNoOfDirectInheritors<id>::value, int index = 0>
struct getInheritors
{
	static constexpr auto value = concatinate(getInheritors<id, noOfDirectInheritors, index + 1>::value,
		getInheritors<getDirectInheritors<id>::value[index]>::value);
};

template<ET_ID id, int noOfDirectInheritors>
struct getInheritors<id, noOfDirectInheritors, noOfDirectInheritors>
{
	static constexpr std::array<ET_ID, noOfDirectInheritors> value = getDirectInheritors<id>::value;
};


#pragma endregion

#pragma region ParentFunctions

template<ET_ID id, int noOfDirectParents = ETInfo<id>::inheritsFromSize, int index = 0 >
struct getNoOfParents
{
	static constexpr int value = getNoOfParents<ETInfo<id>::inheritsFrom[index]>::value
		+ getNoOfParents<id, noOfDirectParents, index + 1>::value;
};

template<ET_ID id, int noOfDirectParents>
struct getNoOfParents<id, noOfDirectParents, noOfDirectParents>
{
	static constexpr int value = noOfDirectParents;
};

template<ET_ID id, int noOfDirectParents = ETInfo<id>::inheritsFromSize, int index = 0>
struct getParents
{
	static constexpr auto value = concatinate(getParents<id, noOfDirectParents, index + 1>::value,
		getParents<ETInfo<id>::inheritsFrom[index]>::value);
};

template<ET_ID id, int noOfDirectParents>
struct getParents<id, noOfDirectParents, noOfDirectParents>
{
	static constexpr std::array<ET_ID, noOfDirectParents> value = ETInfo<id>::inheritsFrom;
};

#pragma endregion

#pragma region ComponentsFunctions

//not 100% sure if this is needed, but no harm in keeping for now - is slightly easier to test than getComponents anway.
template<ET_ID id, int inheritsFromSize = ETInfo<id>::inheritsFromSize, int index = ETInfo<id>::inheritsFromSize - 1>
struct getNoOfComponents
{
	static constexpr int value = ETInfo<ETInfo<id>::inheritsFrom[index]>::noOfNewComponents
		+ getNoOfComponents<ETInfo<id>::inheritsFrom[index]>::value
		+ getNoOfComponents<id, inheritsFromSize, index - 1>::value;
};

template<ET_ID id, int index>
struct getNoOfComponents <id, 0, index>
{
	static constexpr int value = 0;
};

template<ET_ID id, int inheritsFromSize>
struct getNoOfComponents <id, inheritsFromSize, -1>
{
	static constexpr int value = 0;
};

//need this specialization for as previous two are equal in this instance
template<ET_ID id>
struct getNoOfComponents <id, 0, -1>
{
	static constexpr int value = 0;
};

//constexpr function to get all components from parents of an ET. Takes an ET_ID only.
template<ET_ID id, int inheritsFromSize = ETInfo<id>::inheritsFromSize, int index = ETInfo<id>::inheritsFromSize - 1>
struct getComponents
{
	static constexpr auto value = concatinate
	(concatinate(ETInfo<ETInfo<id>::inheritsFrom[index]>::newComponents, getComponents<ETInfo<id>::inheritsFrom[index]>::value),
		getComponents<id, inheritsFromSize, index - 1>::value);
};

template<ET_ID id, int index>
struct getComponents <id, 0, index>
{
	static constexpr std::array<Comp_ID, 0> value = {};
};

template<ET_ID id, int inheritsFromSize>
struct getComponents <id, inheritsFromSize, -1>
{
	static constexpr std::array<Comp_ID, 0> value = {};
};

//need this specialization for as previous two are equal in this instance
template<ET_ID id>
struct getComponents <id, 0, -1>
{
	static constexpr std::array<Comp_ID, 0> value = {};
};

template<ET_ID id, int index = 0>
struct GNC
{
	static constexpr int value = 0;
};
#pragma endregion

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


//All usefull info of ET, derived from ETInfo. Cant just do ETInfo as functions such as getComponents don't work if put before ETinfo.
//Make this a set of constexpr functions? 
template<ET_ID id>
struct ET
{
	//what ET<id> inherits from
	static constexpr int inheritsFromSize = ETInfo<id>::inheritsFromSize;
	static constexpr std::array<ET_ID, inheritsFromSize> inheritsFrom = ETInfo<id>::inheritsFrom;
	//Usefull for scrolling down through inheritance structure
	static constexpr int noOfDirectInheritors = getNoOfDirectInheritors<id>::value;
	static constexpr std::array<ET_ID, noOfDirectInheritors> directInheritors = getDirectInheritors<id>::value;
	//which ETs are an ET<id> (not just direct inheritors), used for searches. 
	static constexpr int noOfInheritors = noOfUniqueET_IDs(getInheritors<id>::value);
	static constexpr std::array<ET_ID, noOfInheritors> inheritors = uniqueET_IDs<noOfInheritors>(getInheritors<id>::value);
	//contains these ETs
	static constexpr int NoOfETs = ETInfo<id>::NoOfETs;
	static constexpr std::array<ET_ID, NoOfETs> ETs = ETInfo<id>::ETs;
	//All parents
	static constexpr int noOfParents = noOfUniqueET_IDs(getParents<id>::value);
	static constexpr std::array<ET_ID, noOfParents> parents = uniqueET_IDs<noOfParents>(getParents<id>::value);
	//what new components are there. 
	static constexpr int noOfComponents = getNoOfComponents<id>::value + ETInfo<id>::noOfNewComponents;
	static constexpr std::array<Comp_ID, noOfComponents> components = concatinate(getComponents<id>::value, ETInfo<id>::newComponents);
	//Sparse for getting order of components - used in ETData for ease of use
	static constexpr std::array<int, MAX_COMP_ID> sparse = CompSparse(components);
	//
//	static constexpr uint32_t offset = ETinfo<id>::maxNoOfEntity; //need to do this cumulatively 
};