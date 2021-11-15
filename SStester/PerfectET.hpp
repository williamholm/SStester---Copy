#pragma once
#include <array>
#include <math.h>
#include "Comp.h"

//class vec3
//{
//public:
//	float x, y, z;
//	float distance() const { return (x * x + y * y + z * z); }
//	float distance( vec3 vec) { return(sqrt((x-vec.x)* (x - vec.x) + (y - vec.y)* (y - vec.y) + (z - vec.z)* (z - vec.z))); }
//	bool operator< (const vec3& vec)
//	{
//		return this->distance() < vec.distance();
//	}
//	vec3(int x, int y, int z) :x(x), y(y), z(z) {}
//	vec3() = default;
//};
//
//enum ET_ID
//{
//	//UTILITY
//	BLANK_FOR_SPARSE,
//	//STABLE ETS
//	//NORMAL ETS
//	OBJ,
//	STATIC_OBJ,
//	PLANT,
//	PHYS_OBJ,
//	PROJECTILE,
//	CREATURE,
//	NPC,
//	PC,
//	MONSTER,
//	WOLF,
//	TALKING_WOLF,
//	MAGIC,
//	ARROW,
//	//VOLALTILE ETS
//	MAGIC_ARROW,
//	//UTILITY
//	MAX_ET_ID
//};
//
//enum Comp_ID
//{
//	BLANK,
//	STATE,
//	POS3D,
//	MOD,
//	SIZE,
//	VELOCITY,
//	MASS,
//	PERMA_FORCE,
//	ORIENTATION,
//	MAX_COMP_ID
//};
//
////used to dertime which SS to use and its functionality.
//enum  CompAttributes
//{
//	UNIQUE = 1, //Only one component of this type in ET
//	SORTABLE = 1 << 1,
//	CONST = 1 << 2,
//	LINKED = 1 << 3, //is ordered by another component (i.e you want masses ordered by position)
//};
//
////of questionable usefulness while using sparse set to order groups within SS's 
////but if that is changed can be used so stable stuff is in front of dense set so deleting/adding is more efficient.
//enum  ETattributes
//{
//	VOLATILE, //short lifespan / constant changes in Components
//	PERMANENT, //not expected to be deleted
//	CONSTANT //no changes to components.
//
//};
//
////returns true if x is in given array 
//template<class T, int N>
//constexpr bool isInArray(const T& x, const std::array<T, N>& arr)
//{
//	for (const auto& y : arr)
//	{
//		if (y == x)
//		{
//			return true;
//		}
//	}
//	return false;
//}
//
//template<class T, int N, int M> //constexpr concatination of two arrays of the same type.
//constexpr std::array<T, N + M> concatinate(const std::array<T, N>& arr1, const std::array<T, M>& arr2)
//{
//	std::array<T, N + M> temp = {};
//	for (int i = 0; i < N; ++i)
//	{
//		temp[i] = arr1[i];
//	}
//	for (int i = N; i < N+M; ++i )
//	{
//		temp[i] = arr2[i - N];
//	}
//
//	return temp;
//}
//template<class T, int N> //for adding a single value to end of array
//constexpr std::array<T, N + 1> concatinate(const std::array<T, N>& arr1, const T& x)
//{
//	std::array<T, N + 1> temp = {};
//	for (int i = 0; i < N; ++i)
//	{
//		temp[i] = arr1[i];
//	}
//	temp[N] = x;
//
//	return temp;
//}
//template<class T, int N> //for adding a single value to start of array
//constexpr std::array<T, N + 1> concatinate(const T& x, const std::array<T, N>& arr1)
//{
//	std::array<T, N + 1> temp = {};
//	for (int i = 0; i < N; ++i)
//	{
//		temp[i+1] = arr1[i];
//	}
//	temp[0] = x;
//
//	return temp;
//}
//
////returns array with new size, all new T's are default initialized at end of original array. 
//template<typename T, int currentSize, int newSize>
//constexpr auto resizeArray(const std::array<T, currentSize>& arr)
//{
//	return concatinate(arr, std::array<T, newSize - currentSize>());
//}
////different method for constexpr resize
//template<typename T, int currentSize, int newSize>
//constexpr std::array<T, newSize> resizeArray(const std::array<T, currentSize>& arr, const std::array<T, newSize>&)
//{
//	std::array<T, newSize> temp = {};
//	for (int i = 0; i < newSize; ++i)
//	{
//		temp[i] = arr[i];
//	}
//	return temp;
//}
//template<typename T, int size>
//constexpr auto noOfInstancesOf(const T& x, const std::array<T, size>& arr)
//{
//	int noOfX = 0;
//	for (int i = 0; i < size; ++i)
//	{
//		if (x == arr[i])
//		{
//			++noOfX;
//		}
//	}
//	return noOfX;
//}
//template<typename T, int size, int newSize>
//constexpr auto removeInstancesOf(const T& x, const std::array<T, size>& arr)
//{
//
//	std::array<T, newSize> newArray = {};
//	int counter = 0;
//	for (int i = 0; i < size; ++i)
//	{
//		if (x != arr[i])
//		{
//			newArray[counter] = arr[i];
//			++counter;
//		}
//	}
//	return newArray;
//}
//
////counts the number of unique ids in given array
//template<int N>
//constexpr int noOfUniqueET_IDs(const std::array<ET_ID, N>& arr)
//{
//	std::array<bool, ET_ID::MAX_ET_ID> isDuplicate = {};//initialized to false (not sure if this is always standard)
//	int noOfUniqueETs = 0;
//	for (int i = 0; i < N; ++i)
//	{
//		if (!isDuplicate[arr[i]])
//		{
//			++noOfUniqueETs;
//			isDuplicate[arr[i]] = true;
//		}
//	}
//	return noOfUniqueETs;
//}
////returns array of all unique ids in given array
//template< int M, int N>
//constexpr auto uniqueET_IDs(const std::array<ET_ID,N>& arr)
//{
//	std::array<bool, ET_ID::MAX_ET_ID> isDuplicate = {};//initialized to false (not sure if this is always standard)
//	std::array<ET_ID, M> temp = {};
//	int noOfUniqueETs = 0;
//	if (M != 0)
//	{
//		for (int i = 0; i < N; ++i)
//		{
//			if (!isDuplicate[arr[i]])
//			{
//				temp[noOfUniqueETs] = arr[i];
//				++noOfUniqueETs;
//				isDuplicate[arr[i]] = true;
//			}
//		}
//	}
//	return temp;
//
//}
//
//
////Information user inputs for new Components(try move to new folder)
////ComponentType must be default initializable for ETData.
//#pragma region Compinfo
//
//template<Comp_ID id>
//struct CompInfo
//{
//	using type = int; //component type
//	static constexpr uint32_t attributes = 0; //component attributes - used for deciding on what SS options to use
//	static constexpr uint32_t expectedAmount = 10000; //how many of this component type are expected - used for optimizing SS and maybe
//													  //chosing entinty num
//};
//
//template<>
//struct CompInfo<Comp_ID::STATE>
//{
//	using type = uint32_t;
//	static constexpr uint32_t attributes = 0;
//	static constexpr uint32_t expectedAmount = 10000;
//
//};
//
//template<>
//struct CompInfo<Comp_ID::POS3D>
//{
//	using type = vec3;
//	static constexpr uint32_t attributes = (CompAttributes::LINKED | CompAttributes::SORTABLE | CompAttributes::UNIQUE);
//	static constexpr uint32_t expectedAmount = 10000;
//};
//template<>
//struct CompInfo<Comp_ID::VELOCITY>
//{
//	using type = float;
//	static constexpr uint32_t attributes = CompAttributes::LINKED;
//	static constexpr uint32_t expectedAmount = 10000;
//
//};
//template<>
//struct CompInfo<Comp_ID::ORIENTATION>
//{
//	using type = vec3;
//	static constexpr uint32_t attributes = 0;
//	static constexpr uint32_t expectedAmount = 10000;
//
//};
//
//template<>
//struct CompInfo<Comp_ID::MASS>
//{
//	using type = float;
//	static constexpr uint32_t attributes = 0;
//	static constexpr uint32_t expectedAmount = 10000;
//
//};
//
//template<>
//struct CompInfo<Comp_ID::PERMA_FORCE>
//{
//	using type = float;
//	static constexpr uint32_t attributes = 0;
//	static constexpr uint32_t expectedAmount = 10000;
//
//};
//
//#pragma endregion
////information user inports for new entity types(try move to new folder)/make more user friendly
//#pragma region ETinfo
////information user inputs for creating new ET, need to do this and ET as otherwise order of ET is an issue, and annoying to deal with.
//template<ET_ID id>
//struct ETInfo
//{
//	//who ET inherits from
//	static constexpr int inheritsFromSize = 0;
//	static constexpr std::array<ET_ID, inheritsFromSize> inheritsFrom = {};
//
//	//contains these ETs
//	static constexpr int NoOfETs = 0;
//	static constexpr std::array<ET_ID, NoOfETs> ETs = {};
//
//	//what new components are there. 
//	static constexpr int noOfNewComponents = 0;
//	static constexpr std::array<Comp_ID, noOfNewComponents> newComponents = {};
//};
//
//template<>
//struct ETInfo<ET_ID::OBJ>
//{
//	//who ET inherits from
//	static constexpr int inheritsFromSize = 0;
//	static constexpr std::array<ET_ID, inheritsFromSize> inheritsFrom = {};
//
//	//contains these ETs -- should this be left to other things? what use does this even serve beyond create/deletion?
//	static constexpr int NoOfETs = 0;
//	static constexpr std::array<ET_ID, NoOfETs> ETs = {};
//
//	//what new components are there. 
//	static constexpr int noOfNewComponents = 2;
//	static constexpr std::array<Comp_ID, noOfNewComponents> newComponents = { Comp_ID::STATE, Comp_ID::POS3D };
//};
//
//template<>
//struct ETInfo<ET_ID::STATIC_OBJ>
//{
//	//who ET inherits from
//	static constexpr int inheritsFromSize = 1;
//	static constexpr std::array<ET_ID, inheritsFromSize> inheritsFrom = { ET_ID::OBJ };
//
//	//contains these ETs -- should this be left to other things? what use does this even serve beyond create/deletion?
//	static constexpr int NoOfETs = 0;
//	static constexpr std::array<ET_ID, NoOfETs> ETs = {};
//
//	//what new components are there. 
//	static constexpr int noOfNewComponents = 0;
//	static constexpr std::array<Comp_ID, noOfNewComponents> newComponents = { };
//
//};
//
//template<>
//struct ETInfo<ET_ID::PLANT>
//{
//	//who ET inherits from
//	static constexpr int inheritsFromSize = 1;
//	static constexpr std::array<ET_ID, inheritsFromSize> inheritsFrom = { ET_ID::STATIC_OBJ };
//
//	//contains these ETs -- should this be left to other things? what use does this even serve beyond create/deletion?
//	static constexpr int NoOfETs = 0;
//	static constexpr std::array<ET_ID, NoOfETs> ETs = {};
//
//	//what new components are there. 
//	static constexpr int noOfNewComponents = 0;
//	static constexpr std::array<Comp_ID, noOfNewComponents> newComponents = { };
//
//};
//
//template<>
//struct ETInfo<ET_ID::PHYS_OBJ>
//{
//	//who ET inherits from
//	static constexpr int inheritsFromSize = 1;
//	static constexpr std::array<ET_ID, inheritsFromSize> inheritsFrom = { ET_ID::OBJ };
//
//	//contains these ETs -- should this be left to other things? what use does this even serve beyond create/deletion?
//	static constexpr int NoOfETs = 0;
//	static constexpr std::array<ET_ID, NoOfETs> ETs = {};
//
//	//what new components are there. 
//	static constexpr int noOfNewComponents = 2;
//	static constexpr std::array<Comp_ID, noOfNewComponents> newComponents = { Comp_ID::VELOCITY, Comp_ID::ORIENTATION };
//
//};
//
//template<>
//struct ETInfo<ET_ID::CREATURE>
//{
//	//who ET inherits from
//	static constexpr int inheritsFromSize = 1;
//	static constexpr std::array<ET_ID, inheritsFromSize> inheritsFrom = { ET_ID::PHYS_OBJ };
//
//	//contains these ETs -- should this be left to other things? what use does this even serve beyond create/deletion?
//	static constexpr int NoOfETs = 0;
//	static constexpr std::array<ET_ID, NoOfETs> ETs = {};
//
//	//what new components are there. 
//	static constexpr int noOfNewComponents = 0;
//	static constexpr std::array<Comp_ID, noOfNewComponents> newComponents = {};
//
//};
//
//template<>
//struct ETInfo<ET_ID::NPC>
//{
//	//who ET inherits from
//	static constexpr int inheritsFromSize = 1;
//	static constexpr std::array<ET_ID, inheritsFromSize> inheritsFrom = { ET_ID::CREATURE };
//
//	//contains these ETs -- should this be left to other things? what use does this even serve beyond create/deletion?
//	static constexpr int NoOfETs = 0;
//	static constexpr std::array<ET_ID, NoOfETs> ETs = {};
//
//	//what new components are there. 
//	static constexpr int noOfNewComponents = 0;
//	static constexpr std::array<Comp_ID, noOfNewComponents> newComponents = {};
//
//};
//
//template<>
//struct ETInfo<ET_ID::PC>
//{
//	//who ET inherits from
//	static constexpr int inheritsFromSize = 1;
//	static constexpr std::array<ET_ID, inheritsFromSize> inheritsFrom = { ET_ID::CREATURE };
//
//	//contains these ETs -- should this be left to other things? what use does this even serve beyond create/deletion?
//	static constexpr int NoOfETs = 0;
//	static constexpr std::array<ET_ID, NoOfETs> ETs = {};
//
//	//what new components are there. 
//	static constexpr int noOfNewComponents = 0;
//	static constexpr std::array<Comp_ID, noOfNewComponents> newComponents = {};
//
//};
//
//template<>
//struct ETInfo<ET_ID::MONSTER>
//{
//	//who ET inherits from
//	static constexpr int inheritsFromSize = 1;
//	static constexpr std::array<ET_ID, inheritsFromSize> inheritsFrom = { ET_ID::CREATURE };
//
//	//contains these ETs -- should this be left to other things? what use does this even serve beyond create/deletion?
//	static constexpr int NoOfETs = 0;
//	static constexpr std::array<ET_ID, NoOfETs> ETs = {};
//
//	//what new components are there. 
//	static constexpr int noOfNewComponents = 0;
//	static constexpr std::array<Comp_ID, noOfNewComponents> newComponents = {};
//
//};
//
//template<>
//struct ETInfo<ET_ID::WOLF>
//{
//	//who ET inherits from
//	static constexpr int inheritsFromSize = 1;
//	static constexpr std::array<ET_ID, inheritsFromSize> inheritsFrom = { ET_ID::MONSTER };
//
//	//contains these ETs -- should this be left to other things? what use does this even serve beyond create/deletion?
//	static constexpr int NoOfETs = 0;
//	static constexpr std::array<ET_ID, NoOfETs> ETs = {};
//
//	//what new components are there. 
//	static constexpr int noOfNewComponents = 0;
//	static constexpr std::array<Comp_ID, noOfNewComponents> newComponents = {};
//
//};
//template<>
//struct ETInfo<ET_ID::TALKING_WOLF>
//{
//	//who ET inherits from
//	static constexpr int inheritsFromSize = 2;
//	static constexpr std::array<ET_ID, inheritsFromSize> inheritsFrom = { ET_ID::NPC, ET_ID::WOLF};
//
//	//contains these ETs -- should this be left to other things? what use does this even serve beyond create/deletion?
//	static constexpr int NoOfETs = 0;
//	static constexpr std::array<ET_ID, NoOfETs> ETs = {};
//
//	//what new components are there. 
//	static constexpr int noOfNewComponents = 0;
//	static constexpr std::array<Comp_ID, noOfNewComponents> newComponents = {};
//
//};
//
//template<>
//struct ETInfo<ET_ID::PROJECTILE>
//{
//	//who ET inherits from
//	static constexpr int inheritsFromSize = 1;
//	static constexpr std::array<ET_ID, inheritsFromSize> inheritsFrom = { ET_ID::PHYS_OBJ };
//
//	//contains these ETs -- should this be left to other things? what use does this even serve beyond create/deletion?
//	static constexpr int NoOfETs = 0;
//	static constexpr std::array<ET_ID, NoOfETs> ETs = {};
//
//	//what new components are there. 
//	static constexpr int noOfNewComponents = 0;
//	static constexpr std::array<Comp_ID, noOfNewComponents> newComponents = {};
//
//};
//
//template<>
//struct ETInfo<ET_ID::ARROW>
//{
//	//who ET inherits from
//	static constexpr int inheritsFromSize = 1;
//	static constexpr std::array<ET_ID, inheritsFromSize> inheritsFrom = { ET_ID::PROJECTILE };
//
//	//contains these ETs -- should this be left to other things? what use does this even serve beyond create/deletion?
//	static constexpr int NoOfETs = 0;
//	static constexpr std::array<ET_ID, NoOfETs> ETs = {};
//
//	//what new components are there. 
//	static constexpr int noOfNewComponents = 0;
//	static constexpr std::array<Comp_ID, noOfNewComponents> newComponents = {};
//
//};
//
//template<>
//struct ETInfo<ET_ID::MAGIC>
//{
//	//who ET inherits from
//	static constexpr int inheritsFromSize = 0;
//	static constexpr std::array<ET_ID, inheritsFromSize> inheritsFrom = {};
//
//	//contains these ETs -- should this be left to other things? what use does this even serve beyond create/deletion?
//	static constexpr int NoOfETs = 0;
//	static constexpr std::array<ET_ID, NoOfETs> ETs = {};
//
//	//what new components are there. 
//	static constexpr int noOfNewComponents = 1;
//	static constexpr std::array<Comp_ID, noOfNewComponents> newComponents = { Comp_ID::PERMA_FORCE };
//
//};
//template<>
//struct ETInfo<ET_ID::MAGIC_ARROW>
//{
//	//who ET inherits from
//	static constexpr int inheritsFromSize = 2;
//	static constexpr std::array<ET_ID, inheritsFromSize> inheritsFrom = { ET_ID::MAGIC , ET_ID::ARROW };
//
//	//contains these ETs -- should this be left to other things? what use does this even serve beyond create/deletion?
//	static constexpr int NoOfETs = 0;
//	static constexpr std::array<ET_ID, NoOfETs> ETs = {};
//
//	//what new components are there. 
//	static constexpr int noOfNewComponents = 1;
//	static constexpr std::array<Comp_ID, noOfNewComponents> newComponents = { Comp_ID::MASS };
//
//};
//#pragma endregion
//
///*
//+2				+2
//<MA,2,1> --- <PROJ,1,0> --- <Obj,0,-1> (termination)
//	|			  |
//	|		 <PROJ,1,-1> (termination)
//	|
//+1	|
//<MA,2,0> --- (termination) 
//	|
//	|
//<MA,2,-1> (termination)
//
//*/
////iterates though ET_ID's, finding ids direct inheritors
//#pragma region DirectInheritorFunctions
//
//template<ET_ID id, int current_id = 0>
//struct getNoOfDirectInheritors
//{
//	static constexpr int value = getNoOfDirectInheritors<id, ET_ID(current_id + 1)>::value +
//		(int)isInArray(id, ETInfo<(ET_ID)current_id>::inheritsFrom);
//};
//
//template<ET_ID id>
//struct getNoOfDirectInheritors<id, ET_ID::MAX_ET_ID>
//{
//	static constexpr int value = 0;
//};
//
//template<ET_ID id, int current_id = 0, bool ifIsh = isInArray(id, ETInfo<(ET_ID)current_id>::inheritsFrom)>
//struct getDirectInheritors
//{
//	static constexpr std::array<ET_ID, getNoOfDirectInheritors<id>::value> value = getDirectInheritors<id, 1>::value;
//};
//
//template<ET_ID id, int current_id>
//struct getDirectInheritors <id, current_id, true>
//{
//	static constexpr auto value = concatinate(getDirectInheritors<id, current_id + 1>::value, (ET_ID)current_id);
//};
//
//template<ET_ID id, int current_id>
//struct getDirectInheritors <id, current_id, false>
//{
//	static constexpr auto value = getDirectInheritors<id, current_id + 1>::value;
//};
//
//template<ET_ID id>
//struct getDirectInheritors<id, ET_ID::MAX_ET_ID, false>
//{
//	static constexpr std::array<ET_ID, 0> value = {};
//};
//
//
//#pragma endregion
//
//
//#pragma region InheritorsFunctions
//
//template<ET_ID id, int noOfDirectInheritors = getNoOfDirectInheritors<id>::value, int index = 0 >
//struct getNoOfInheritors
//{							
//	//static constexpr int value = 1 + getNoOfInheritors<(ET_ID)getDirectInheritors<id>::value[index]>::value
//	//	+ getNoOfInheritors<id, noOfDirectInheritors, index + 1>::value;
//
//	static constexpr int value = getNoOfInheritors<(ET_ID)getDirectInheritors<id>::value[index]>::value
//		+ getNoOfInheritors<id, noOfDirectInheritors, index + 1>::value;
//};
//
//template<ET_ID id, int noOfDirectInheritors>
//struct getNoOfInheritors<id, noOfDirectInheritors, noOfDirectInheritors>
//{
//	static constexpr int value = noOfDirectInheritors;
//};
//
//
//
//template<ET_ID id, int noOfDirectInheritors = getNoOfDirectInheritors<id>::value, int index = 0>
//struct getInheritors
//{
//	static constexpr auto value = concatinate(getInheritors<id, noOfDirectInheritors, index + 1>::value,
//		getInheritors<getDirectInheritors<id>::value[index]>::value);
//};
//
//template<ET_ID id, int noOfDirectInheritors>
//struct getInheritors<id, noOfDirectInheritors, noOfDirectInheritors>
//{
//	static constexpr std::array<ET_ID, noOfDirectInheritors> value = getDirectInheritors<id>::value;
//};
//
//
//#pragma endregion
//
//#pragma region ParentFunctions
//
//template<ET_ID id, int noOfDirectParents = ETInfo<id>::inheritsFromSize, int index = 0 >
//struct getNoOfParents
//{
//	static constexpr int value = getNoOfParents<ETInfo<id>::inheritsFrom[index]>::value
//		+ getNoOfParents<id, noOfDirectParents, index + 1>::value;
//};
//
//template<ET_ID id, int noOfDirectParents>
//struct getNoOfParents<id, noOfDirectParents, noOfDirectParents>
//{
//	static constexpr int value = noOfDirectParents;
//};
//
//template<ET_ID id, int noOfDirectParents = ETInfo<id>::inheritsFromSize, int index = 0>
//struct getParents
//{
//	static constexpr auto value = concatinate(getParents<id, noOfDirectParents, index + 1>::value,
//		getParents<ETInfo<id>::inheritsFrom[index]>::value);
//};
//
//template<ET_ID id, int noOfDirectParents>
//struct getParents<id, noOfDirectParents, noOfDirectParents>
//{
//	static constexpr std::array<ET_ID, noOfDirectParents> value = ETInfo<id>::inheritsFrom;
//};
//
//#pragma endregion
//
//#pragma region ComponentsFunctions
//
////not 100% sure if this is needed, but no harm in keeping for now - is slightly easier to test than getComponents anway.
//template<ET_ID id, int inheritsFromSize = ETInfo<id>::inheritsFromSize, int index = ETInfo<id>::inheritsFromSize - 1>
//struct getNoOfComponents
//{
//	static constexpr int value = ETInfo<ETInfo<id>::inheritsFrom[index]>::noOfNewComponents
//		+ getNoOfComponents<ETInfo<id>::inheritsFrom[index]>::value
//		+ getNoOfComponents<id, inheritsFromSize, index - 1>::value;
//};
//
//template<ET_ID id, int index>
//struct getNoOfComponents <id, 0, index>
//{
//	static constexpr int value = 0;
//};
//
//template<ET_ID id, int inheritsFromSize>
//struct getNoOfComponents <id, inheritsFromSize, -1>
//{
//	static constexpr int value = 0;
//};
//
////need this specialization for as previous two are equal in this instance
//template<ET_ID id>
//struct getNoOfComponents <id, 0, -1>
//{
//	static constexpr int value = 0;
//};
//
////constexpr function to get all components from parents of an ET. Takes an ET_ID only.
//template<ET_ID id, int inheritsFromSize = ETInfo<id>::inheritsFromSize, int index = ETInfo<id>::inheritsFromSize - 1>
//struct getComponents
//{
//	static constexpr auto value = concatinate
//	(concatinate(ETInfo<ETInfo<id>::inheritsFrom[index]>::newComponents, getComponents<ETInfo<id>::inheritsFrom[index]>::value),
//		getComponents<id, inheritsFromSize, index - 1>::value);
//};
//
//template<ET_ID id, int index>
//struct getComponents <id, 0, index>
//{
//	static constexpr std::array<Comp_ID, 0> value = {};
//};
//
//template<ET_ID id, int inheritsFromSize>
//struct getComponents <id, inheritsFromSize, -1>
//{
//	static constexpr std::array<Comp_ID, 0> value = {};
//};
//
////need this specialization for as previous two are equal in this instance
//template<ET_ID id>
//struct getComponents <id, 0, -1>
//{
//	static constexpr std::array<Comp_ID, 0> value = {};
//};
//
//template<ET_ID id, int index = 0>
//struct GNC
//{ 
//	static constexpr int value = 0;
//};
//#pragma endregion

//template<int N>
//constexpr std::array<int, MAX_COMP_ID> CompSparse(const std::array<Comp_ID, N>& arr)
//{
//	std::array<int, MAX_COMP_ID> sparse = {};
//
//	for (int i = 0; i < MAX_COMP_ID; ++i)
//	{
//		sparse[i] = MAX_COMP_ID; //do this so 0's aren't default state of sparse. not needed for current function.
//	}
//	for (int i = 0; i < N; ++i)
//	{
//		sparse[arr[i]] = i;
//	}
//
//	return sparse;
//}
//
//
////All usefull info of ET, derived from ETInfo. Cant just do ETInfo as functions such as getComponents don't work if put before ETinfo.
////Make this a set of constexpr functions? 
//template<ET_ID id>
//struct ET 
//{
//	//what ET<id> inherits from
//	static constexpr int inheritsFromSize = ETInfo<id>::inheritsFromSize;
//	static constexpr std::array<ET_ID, inheritsFromSize> inheritsFrom = ETInfo<id>::inheritsFrom;
//	//Usefull for scrolling down through inheritance structure
//	static constexpr int noOfDirectInheritors = getNoOfDirectInheritors<id>::value;
//	static constexpr std::array<ET_ID, noOfDirectInheritors> directInheritors = getDirectInheritors<id>::value;
//	//which ETs are an ET<id> (not just direct inheritors), used for searches. 
//	static constexpr int noOfInheritors = noOfUniqueET_IDs(getInheritors<id>::value);
//	static constexpr std::array<ET_ID, noOfInheritors> inheritors = uniqueET_IDs<noOfInheritors>(getInheritors<id>::value);
//	//contains these ETs
//	static constexpr int NoOfETs = ETInfo<id>::NoOfETs;
//	static constexpr std::array<ET_ID, NoOfETs> ETs = ETInfo<id>::ETs;
//	//All parents
//	static constexpr int noOfParents = noOfUniqueET_IDs(getParents<id>::value);
//	static constexpr std::array<ET_ID, noOfParents> parents = uniqueET_IDs<noOfParents>(getParents<id>::value);
//	//what new components are there. 
//	static constexpr int noOfComponents = getNoOfComponents<id>::value + ETInfo<id>::noOfNewComponents;
//	static constexpr std::array<Comp_ID, noOfComponents> components = concatinate(getComponents<id>::value,ETInfo<id>::newComponents);
//	//Sparse for getting order of components - used in ETData for ease of use
//	static constexpr std::array<int, MAX_COMP_ID> sparse = CompSparse(components);
//	//
////	static constexpr uint32_t offset = ETinfo<id>::maxNoOfEntity; //need to do this cumulatively 
//};
#pragma region Static To Non-Static Access
//this is a method to allow for loops in general code. it gives componentAccess[componentBounds[ET_ID]...componentBounds[ET_ID+1]] = all ET<ET_ID>
//components, allowing for for loops as you can do
//
//for(auto : ET<id>::inheritors)
//	for(i = CB[auto]; i < CB[auto+1]; ++i ) {dosomething(CA[i])}
template<class T, int BoundsSize, int DataSize>
class BoundsArray
{
public:

	std::array<int, BoundsSize> mBounds;
	std::array<T, DataSize> mData;
	constexpr inline T operator[](unsigned int i)
	{
		return mData[i];
	}
	constexpr BoundsArray(const std::array<int, BoundsSize>& bounds, const std::array<T, DataSize>& data) : mBounds(bounds), mData(data) {}
	~BoundsArray() = default;
	
};

template<int currentBounds = 0, int index = 0>
struct componentBounds
{
	static constexpr std::array<int,MAX_ET_ID - index + 1> value = concatinate(currentBounds,
		componentBounds<ET<(ET_ID)(index)>::noOfComponents + currentBounds,index+1>::value
		);
};
template<int currentBounds>
struct componentBounds<currentBounds,MAX_ET_ID>
{
	static constexpr std::array<int, 1> value = { currentBounds };
};

template<int index = 0>
struct componentAccess
{
	static constexpr auto value = concatinate(ET<(ET_ID)index>::components, componentAccess<index + 1>::value);
};
template<>
struct componentAccess<MAX_ET_ID>
{
	static constexpr std::array<Comp_ID, 0> value = { };
};

template<ET_ID id = BLANK_FOR_SPARSE, ET_ID maxID = MAX_ET_ID>
struct noOfParentArray
{
	static constexpr std::array<int, maxID - id + 1> value = concatinate(ET<id>::noOfParents, noOfParentArray<(ET_ID)(id + 1)>::value);
};
template<ET_ID maxID>
struct noOfParentArray<maxID,maxID>
{
	static constexpr std::array<int, 1>  value = {};
};

template<ET_ID id = BLANK_FOR_SPARSE, ET_ID maxID = MAX_ET_ID>
struct parentArray
{
	static constexpr std::array<std::array<ET_ID,MAX_ET_ID>,maxID-id+1> value = concatinate(
		resizeArray<ET_ID, ET<id>::noOfParents, MAX_ET_ID>(ET<id>::parents),
		parentArray<(ET_ID)(id + 1), MAX_ET_ID>::value);
};
template<ET_ID maxID>
struct parentArray<maxID, maxID>
{
	static constexpr auto value = std::array<std::array<ET_ID, MAX_ET_ID>,1>();
};
template<ET_ID id = BLANK_FOR_SPARSE, ET_ID maxID = MAX_ET_ID>
struct inheritorArray
{
	static constexpr std::array<std::array<ET_ID, MAX_ET_ID>, maxID - id + 1> value = concatinate(
		resizeArray<ET_ID, ET<id>::noOfInheritors, MAX_ET_ID>(ET<id>::inheritors),
		inheritorArray<(ET_ID)(id + 1), MAX_ET_ID>::value);
};
template<ET_ID maxID>
struct inheritorArray<maxID, maxID>
{
	static constexpr auto value = std::array<std::array<ET_ID, MAX_ET_ID>, 1>();
};
template<ET_ID id = BLANK_FOR_SPARSE, ET_ID maxID = MAX_ET_ID>
struct compArray //might need to change amount that resizeArray changes too
{
	static constexpr std::array<std::array<Comp_ID, (int)Comp_ID::MAX_COMP_ID>, maxID - id + 1> value = concatinate(
		resizeArray<Comp_ID, ET<id>::noOfComponents, (int)Comp_ID::MAX_COMP_ID>(ET<id>::components),
		compArray<(ET_ID)(id + 1), MAX_ET_ID>::value);
};
template<ET_ID maxID>
struct compArray<maxID, maxID>
{
	static constexpr auto value = std::array<std::array<Comp_ID, (int)Comp_ID::MAX_COMP_ID>, 1>();
};

constexpr auto getNoOfParents(ET_ID id)
{
	return noOfParentArray<>::value[id];
}
template<typename T, int size>
class TempSol
{
public:
	std::array<T, size> data;
	int end;
	constexpr TempSol(const std::array<T, size>& arr, int lastValidValue) : data(arr),end(lastValidValue) {}
};

constexpr auto getParents(ET_ID id)
{
	//remove blanks first probably. - can't think of a way to non-constexpr access if arrays are different sizes,
	//so the blanks stay for now.
//	return parentArray<>::value[id];

	return TempSol(parentArray<>::value[id], getNoOfParents(id));
}
constexpr auto getInheritors(ET_ID id)
{
	
	return inheritorArray<>::value[id];
}
constexpr auto getComponents(ET_ID id)
{
	return compArray<>::value[id];
}
#pragma endregion


/*
Method of compiling data from ET, as for loops don't work to scroll through ET<i> in constexpr function.
Simular methods can be used for other things.
*/
//#pragma region ComponentArray Function
//
//template<Comp_ID comp_id, int index = ET_ID::MAX_ET_ID>
//struct ComponentArray
//{
//	static constexpr std::array<bool, index + 1> value =
//		concatinate(ComponentArray<comp_id, index - 1>::value, isInArray(comp_id, ET<(ET_ID)index>::components));
//};
//
//template<Comp_ID comp_id>
//struct ComponentArray<comp_id, ET_ID::MAX_ET_ID>
//{
//	static constexpr std::array<bool, ET_ID::MAX_ET_ID + 1> value = concatinate(ComponentArray<comp_id, ET_ID::MAX_ET_ID - 1>::value, false);
//};
//
//template<Comp_ID comp_id>
//struct ComponentArray<comp_id, 0>
//{
//	static constexpr std::array<bool, 1> value = { isInArray(comp_id, ET<(ET_ID)0>::components) };
//};
//
//
//#pragma endregion

////MAX_ET_ID+1 for now for formatting, might be changed later.
//template<Comp_ID id>
//constexpr std::array<int, ET_ID::MAX_ET_ID+1> getCompSparse()
//{
//	int counter = 0;
//	std::array<int, ET_ID::MAX_ET_ID+1> sparse = {};
//	for (int i = 0; i < ET_ID::MAX_ET_ID; ++i)
//	{
//		if (ComponentArray<id>::value[i])
//		{
//			sparse[i] = ++counter;
//		}
//	}
//	//last element of array is no of entity types with this component
//	sparse[ET_ID::MAX_ET_ID] = counter;
//	return sparse;
//}
//template<Comp_ID id, typename ComponentType = typename CompInfo<id>::type>
//struct Comp
//{
//	//these 3 moved here from CompInfo for ease of access despite duplication.
//	using type = ComponentType;
//	static constexpr uint32_t attributes = CompInfo<id>::attributes;
//	static constexpr uint32_t expectedAmount = CompInfo<id>::expectedAmount;
//	//if compArray[ET_ID] = true ET_ID contains component.
//	static constexpr auto compArray = ComponentArray<id>::value;
//	//sparse set for ordering ETs which have this component, last value is total no of entitys with component.
//	static constexpr auto sparse = getCompSparse<id>();
//	static constexpr auto noOfETsWithComp = sparse[ET_ID::MAX_ET_ID];
//	//offsets for SS so they can be condensed - this doesn't need to be implemented for my uses i think.
//	static constexpr std::array<uint32_t, ET_ID::MAX_ET_ID> offsets = {};
//}; 