#pragma once
#include <array>
/*
Condsider whole system 2 examples:

Simple, plentiful: trees: inherits from object (if moves with wind or something) or is a static object. 


Changing, complicated: player, inherits from xyz, contains Perhaps other classes like weapon - might need several ids inside it
Eg id for weapon 1, id for armour 1,2.

problem 1: how do you contain multiples of the same component without changing 2SS too much?.

*/

/*
Whole view:

Creating a Component: add type in comp info.
		   Additions: add ordered/unordered/partial Ordering/Static/Volitaile
					: create PerfectComp which has sparse with all ETs containing this.

Creating an EntityType info: give what it inherits from - defines what it belongs too:
						   : what new ETs it contains
						   : what new components it contains
						   : with this spit out compositeET (rename) with all ET_IDs, and floating CompIDs

how do I present these ET_IDs?



Manager:

2SS:


example 1: HostilePerson with gun,sword.

create components: Pos,Model,Ori,'Quantity',damage,animation....

create weapon ET         : inherits from object, has damage
create animated_weapon ET: inherits from animated_object, has damage
create gun ET			 : inherits from a_w, has bullet ET (damage,quantity):object
create sword             : inherits from weapon
HostilePerson			 : inherits from person ET (voicelines, personality):animated_object, Has gun, Sword.


example 2: how would player inv work?


*/

class vec3
{
public:
	float x, y, z;
	float distance() { return (x * x + y * y + z * z); }
	bool operator< (vec3 vec)
	{
		return this->distance() < vec.distance();
	}
	vec3(int x, int y, int z) :x(x), y(y), z(z) {}
	vec3() = default;
};


enum ET_ID
{
	BLANK_FOR_SPARSE,
	//STABLE ETS
	//NORMAL ETS
	OBJ,
	STATIC_OBJ,
	PLANT,
	PHYS_OBJ,
	PROJECTILE,
	CREATURE,
	NPC,
	PC,
	MONSTER,
	WOLF,
	TALKING_WOLF,
	MAGIC,
	ARROW,
	//VOLALTILE ETS
	MAGIC_ARROW,

	MAX_ET_ID
};

enum class Comp_ID
{
	STATE,
	POS3D,
	MOD,
	SIZE,
	VELOCITY,
	MASS,
	PERMA_FORCE,
	ORIENTATION,
	MAX_COMP_ID
};

//used to dertime which SS to use and its functionality.
enum  CompAttributes
{
	UNIQUE = 1, //Only one component of this type in ET
	SORTABLE = 1 << 1,
	CONST = 1 << 2,
	LINKED = 1 << 3, //is ordered by another component (i.e you want masses ordered by position)
};

//of questionable usefulness while using sparse set to order groups within SS's 
//but if that is changed can be used so stable stuff is in front of dense set so deleting/adding is more efficient.
enum  ETattributes
{
	VOLATILE, //short lifespan / constant changes in Components
	PERMANENT, //not expected to be deleted
	CONSTANT //no changes to components.

};
template<class T, int N>
constexpr bool isInArray(T x, std::array<T, N> arr)
{
	for (const auto& y : arr)
	{
		if (y == x)
		{
			return true;
		}
	}
	return false;
}

template<class T, int N, int M>
constexpr std::array<T, N + M> concatinate(const std::array<T, N>& arr1, const std::array<T, M>& arr2)
{
	std::array<T, N + M> temp = {};
	for (int i = 0; i < N; ++i)
	{
		temp[i] = arr1[i];
	}
	for (int i = N; i < N+M; ++i )
	{
		temp[i] = arr2[i - N];
	}

	return temp;
}
template<class T, int N> //for adding a single value to end of array
constexpr std::array<T, N + 1> concatinate(const std::array<T, N>& arr1, const T& x)
{
	std::array<T, N + 1> temp = {};
	for (int i = 0; i < N; ++i)
	{
		temp[i] = arr1[i];
	}
	temp[N] = x;

	return temp;
}

//Information user inputs for new Components(try move to new folder)
#pragma region Compinfo

template<Comp_ID id>
struct CompInfo
{
	using type = int; //component type
	static constexpr uint32_t attributes = 0; //component attributes - used for deciding on what SS options to use
	static constexpr uint32_t expectedAmount = 10000; //how many of this component type are expected - used for optimizing SS and maybe
													  //chosing entinty num
};

template<>
struct CompInfo<Comp_ID::STATE>
{
	using type = uint32_t;
	static constexpr uint32_t attributes = 0;
	static constexpr uint32_t expectedAmount = 10000;

};

template<>
struct CompInfo<Comp_ID::POS3D>
{
	using type = vec3;
	static constexpr uint32_t attributes = (CompAttributes::LINKED | CompAttributes::SORTABLE | CompAttributes::UNIQUE);
	static constexpr uint32_t expectedAmount = 10000;
};
template<>
struct CompInfo<Comp_ID::VELOCITY>
{
	using type = float;
	static constexpr uint32_t attributes = CompAttributes::LINKED;
	static constexpr uint32_t expectedAmount = 10000;

};
template<>
struct CompInfo<Comp_ID::ORIENTATION>
{
	using type = vec3;
	static constexpr uint32_t attributes = 0;
	static constexpr uint32_t expectedAmount = 10000;

};

template<>
struct CompInfo<Comp_ID::MASS>
{
	using type = float;
	static constexpr uint32_t attributes = 0;
	static constexpr uint32_t expectedAmount = 10000;

};

template<>
struct CompInfo<Comp_ID::PERMA_FORCE>
{
	using type = float;
	static constexpr uint32_t attributes = 0;
	static constexpr uint32_t expectedAmount = 10000;

};

#pragma endregion
//information user inports for new entity types(try move to new folder)/make more user friendly
#pragma region ETinfo
//information user inputs for creating new ET, need to do this and ET as otherwise order of ET is an issue, and annoying to deal with.
template<ET_ID id>
struct ETInfo
{
	//who ET inherits from
	static constexpr int inheritsFromSize = 0;
	static constexpr std::array<ET_ID, inheritsFromSize> inheritsFrom = {};

	//contains these ETs
	static constexpr int NoOfETs = 0;
	static constexpr std::array<ET_ID, NoOfETs> ETs = {};

	//what new components are there. 
	static constexpr int noOfNewComponents = 0;
	static constexpr std::array<Comp_ID, noOfNewComponents> newComponents = {};


};

template<>
struct ETInfo<ET_ID::OBJ>
{
	//who ET inherits from
	static constexpr int inheritsFromSize = 0;
	static constexpr std::array<ET_ID, inheritsFromSize> inheritsFrom = {};

	//contains these ETs -- should this be left to other things? what use does this even serve beyond create/deletion?
	static constexpr int NoOfETs = 0;
	static constexpr std::array<ET_ID, NoOfETs> ETs = {};

	//what new components are there. 
	static constexpr int noOfNewComponents = 2;
	static constexpr std::array<Comp_ID, noOfNewComponents> newComponents = { Comp_ID::STATE, Comp_ID::POS3D };

};

template<>
struct ETInfo<ET_ID::STATIC_OBJ>
{
	//who ET inherits from
	static constexpr int inheritsFromSize = 1;
	static constexpr std::array<ET_ID, inheritsFromSize> inheritsFrom = { ET_ID::OBJ };

	//contains these ETs -- should this be left to other things? what use does this even serve beyond create/deletion?
	static constexpr int NoOfETs = 0;
	static constexpr std::array<ET_ID, NoOfETs> ETs = {};

	//what new components are there. 
	static constexpr int noOfNewComponents = 0;
	static constexpr std::array<Comp_ID, noOfNewComponents> newComponents = { };

};

template<>
struct ETInfo<ET_ID::PLANT>
{
	//who ET inherits from
	static constexpr int inheritsFromSize = 1;
	static constexpr std::array<ET_ID, inheritsFromSize> inheritsFrom = { ET_ID::STATIC_OBJ };

	//contains these ETs -- should this be left to other things? what use does this even serve beyond create/deletion?
	static constexpr int NoOfETs = 0;
	static constexpr std::array<ET_ID, NoOfETs> ETs = {};

	//what new components are there. 
	static constexpr int noOfNewComponents = 0;
	static constexpr std::array<Comp_ID, noOfNewComponents> newComponents = { };

};

template<>
struct ETInfo<ET_ID::PHYS_OBJ>
{
	//who ET inherits from
	static constexpr int inheritsFromSize = 1;
	static constexpr std::array<ET_ID, inheritsFromSize> inheritsFrom = { ET_ID::OBJ };

	//contains these ETs -- should this be left to other things? what use does this even serve beyond create/deletion?
	static constexpr int NoOfETs = 0;
	static constexpr std::array<ET_ID, NoOfETs> ETs = {};

	//what new components are there. 
	static constexpr int noOfNewComponents = 2;
	static constexpr std::array<Comp_ID, noOfNewComponents> newComponents = { Comp_ID::VELOCITY, Comp_ID::ORIENTATION };

};

template<>
struct ETInfo<ET_ID::CREATURE>
{
	//who ET inherits from
	static constexpr int inheritsFromSize = 1;
	static constexpr std::array<ET_ID, inheritsFromSize> inheritsFrom = { ET_ID::PHYS_OBJ };

	//contains these ETs -- should this be left to other things? what use does this even serve beyond create/deletion?
	static constexpr int NoOfETs = 0;
	static constexpr std::array<ET_ID, NoOfETs> ETs = {};

	//what new components are there. 
	static constexpr int noOfNewComponents = 0;
	static constexpr std::array<Comp_ID, noOfNewComponents> newComponents = {};

};

template<>
struct ETInfo<ET_ID::NPC>
{
	//who ET inherits from
	static constexpr int inheritsFromSize = 1;
	static constexpr std::array<ET_ID, inheritsFromSize> inheritsFrom = { ET_ID::CREATURE };

	//contains these ETs -- should this be left to other things? what use does this even serve beyond create/deletion?
	static constexpr int NoOfETs = 0;
	static constexpr std::array<ET_ID, NoOfETs> ETs = {};

	//what new components are there. 
	static constexpr int noOfNewComponents = 0;
	static constexpr std::array<Comp_ID, noOfNewComponents> newComponents = {};

};

template<>
struct ETInfo<ET_ID::PC>
{
	//who ET inherits from
	static constexpr int inheritsFromSize = 1;
	static constexpr std::array<ET_ID, inheritsFromSize> inheritsFrom = { ET_ID::CREATURE };

	//contains these ETs -- should this be left to other things? what use does this even serve beyond create/deletion?
	static constexpr int NoOfETs = 0;
	static constexpr std::array<ET_ID, NoOfETs> ETs = {};

	//what new components are there. 
	static constexpr int noOfNewComponents = 0;
	static constexpr std::array<Comp_ID, noOfNewComponents> newComponents = {};

};

template<>
struct ETInfo<ET_ID::MONSTER>
{
	//who ET inherits from
	static constexpr int inheritsFromSize = 1;
	static constexpr std::array<ET_ID, inheritsFromSize> inheritsFrom = { ET_ID::CREATURE };

	//contains these ETs -- should this be left to other things? what use does this even serve beyond create/deletion?
	static constexpr int NoOfETs = 0;
	static constexpr std::array<ET_ID, NoOfETs> ETs = {};

	//what new components are there. 
	static constexpr int noOfNewComponents = 0;
	static constexpr std::array<Comp_ID, noOfNewComponents> newComponents = {};

};

template<>
struct ETInfo<ET_ID::WOLF>
{
	//who ET inherits from
	static constexpr int inheritsFromSize = 1;
	static constexpr std::array<ET_ID, inheritsFromSize> inheritsFrom = { ET_ID::MONSTER };

	//contains these ETs -- should this be left to other things? what use does this even serve beyond create/deletion?
	static constexpr int NoOfETs = 0;
	static constexpr std::array<ET_ID, NoOfETs> ETs = {};

	//what new components are there. 
	static constexpr int noOfNewComponents = 0;
	static constexpr std::array<Comp_ID, noOfNewComponents> newComponents = {};

};
template<>
struct ETInfo<ET_ID::TALKING_WOLF>
{
	//who ET inherits from
	static constexpr int inheritsFromSize = 2;
	static constexpr std::array<ET_ID, inheritsFromSize> inheritsFrom = { ET_ID::NPC, ET_ID::WOLF};

	//contains these ETs -- should this be left to other things? what use does this even serve beyond create/deletion?
	static constexpr int NoOfETs = 0;
	static constexpr std::array<ET_ID, NoOfETs> ETs = {};

	//what new components are there. 
	static constexpr int noOfNewComponents = 0;
	static constexpr std::array<Comp_ID, noOfNewComponents> newComponents = {};

};

template<>
struct ETInfo<ET_ID::PROJECTILE>
{
	//who ET inherits from
	static constexpr int inheritsFromSize = 1;
	static constexpr std::array<ET_ID, inheritsFromSize> inheritsFrom = { ET_ID::PHYS_OBJ };

	//contains these ETs -- should this be left to other things? what use does this even serve beyond create/deletion?
	static constexpr int NoOfETs = 0;
	static constexpr std::array<ET_ID, NoOfETs> ETs = {};

	//what new components are there. 
	static constexpr int noOfNewComponents = 0;
	static constexpr std::array<Comp_ID, noOfNewComponents> newComponents = {};

};

template<>
struct ETInfo<ET_ID::ARROW>
{
	//who ET inherits from
	static constexpr int inheritsFromSize = 1;
	static constexpr std::array<ET_ID, inheritsFromSize> inheritsFrom = { ET_ID::PROJECTILE };

	//contains these ETs -- should this be left to other things? what use does this even serve beyond create/deletion?
	static constexpr int NoOfETs = 0;
	static constexpr std::array<ET_ID, NoOfETs> ETs = {};

	//what new components are there. 
	static constexpr int noOfNewComponents = 0;
	static constexpr std::array<Comp_ID, noOfNewComponents> newComponents = {};

};

template<>
struct ETInfo<ET_ID::MAGIC>
{
	//who ET inherits from
	static constexpr int inheritsFromSize = 0;
	static constexpr std::array<ET_ID, inheritsFromSize> inheritsFrom = {};

	//contains these ETs -- should this be left to other things? what use does this even serve beyond create/deletion?
	static constexpr int NoOfETs = 0;
	static constexpr std::array<ET_ID, NoOfETs> ETs = {};

	//what new components are there. 
	static constexpr int noOfNewComponents = 1;
	static constexpr std::array<Comp_ID, noOfNewComponents> newComponents = { Comp_ID::PERMA_FORCE };

};
template<>
struct ETInfo<ET_ID::MAGIC_ARROW>
{
	//who ET inherits from
	static constexpr int inheritsFromSize = 2;
	static constexpr std::array<ET_ID, inheritsFromSize> inheritsFrom = { ET_ID::MAGIC , ET_ID::ARROW };

	//contains these ETs -- should this be left to other things? what use does this even serve beyond create/deletion?
	static constexpr int NoOfETs = 0;
	static constexpr std::array<ET_ID, NoOfETs> ETs = {};

	//what new components are there. 
	static constexpr int noOfNewComponents = 1;
	static constexpr std::array<Comp_ID, noOfNewComponents> newComponents = { Comp_ID::MASS };

};
#pragma endregion

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
#pragma region getComponentsFunctions

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

#pragma endregion

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
	static constexpr int noOfInheritors = getNoOfInheritors<id>::value;
	static constexpr std::array<ET_ID, noOfInheritors> inheritors = getInheritors<id>::value;
	//contains these ETs
	static constexpr int NoOfETs = ETInfo<id>::NoOfETs;
	static constexpr std::array<ET_ID, NoOfETs> ETs = ETInfo<id>::ETs;
	//what new components are there. 
	static constexpr int noOfComponents = getNoOfComponents<id>::value + ETInfo<id>::noOfNewComponents;
	static constexpr std::array<Comp_ID, noOfComponents> components = concatinate(getComponents<id>::value,ETInfo<id>::newComponents);

};

template<ET_ID ...ids>
struct ETData
{
	//fill in a new entity by extracting from ETInfo
	//this might not be possible, and if it is it might still be better to do via inheritance.
};

/*
Method of compiling data from ET, as for loops don't work to scroll through ET<i> in constexpr function.
Simular methods can be used for other things.
*/
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
constexpr std::array<int, ET_ID::MAX_ET_ID+1> getCompSparse()
{
	int counter = 0;
	std::array<int, ET_ID::MAX_ET_ID+1> sparse = {};
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
template<Comp_ID id, class ComponentType = CompInfo<id>::type>
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
}; 