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

enum  ET_ID
{
	OBJ,
	STATIC_OBJ,
	PHYS_OBJ,
	PROJECTILE,
	MAGIC,
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
//test
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


//Information user inputs for new Components
#pragma region Compinfo

template<Comp_ID>
struct CompInfo
{
	using type = int;
};

template<>
struct CompInfo<Comp_ID::STATE>
{
	using type = uint32_t;

};

template<>
struct CompInfo<Comp_ID::POS3D>
{
	using type = vec3;

};
template<>
struct CompInfo<Comp_ID::VELOCITY>
{
	using type = float;

};
template<>
struct CompInfo<Comp_ID::ORIENTATION>
{
	using type = vec3;

};

template<>
struct CompInfo<Comp_ID::MASS>
{
	using type = float;

};

template<>
struct CompInfo<Comp_ID::PERMA_FORCE>
{
	using type = float;

};

#pragma endregion

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
struct ETInfo<ET_ID::PROJECTILE>
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
	static constexpr std::array<ET_ID, inheritsFromSize> inheritsFrom = { ET_ID::MAGIC , ET_ID::PROJECTILE };

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

//not 100% sure if this is needed, but no harm in keeping for now - is slightly easier to test than getComponents anway.
template<ET_ID id, int inheritsFromSize = ETInfo<id>::inheritsFromSize, int index = ETInfo<id>::inheritsFromSize-1>
struct getNoOfComponents
{
	static constexpr int value = ETInfo<ETInfo<id>::inheritsFrom[index]>::noOfNewComponents
		+ getNoOfComponents<ETInfo<id>::inheritsFrom[index]>::value
		+ getNoOfComponents<id, inheritsFromSize, index - 1>::value;
};

template<ET_ID id ,int index>
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

//All usefull info of ET, derived from ETInfo. Cant just do ETInfo as functions such as getComponents don't work if put before ETinfo.
template<ET_ID id>
struct ET
{
	//who ET inherits from
	static constexpr int inheritsFromSize = ETInfo<id>::inheritsFromSize;
	static constexpr std::array<ET_ID, inheritsFromSize> inheritsFrom = ETInfo<id>::inheritsFrom;

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

//Better way to do this? 
constexpr int noOfAcvtiveETs = 4;
constexpr std::array<ET_ID, noOfAcvtiveETs> activeETs = { ET_ID::OBJ, ET_ID::PROJECTILE, ET_ID::MAGIC, ET_ID::MAGIC_ARROW };

template<Comp_ID componentID, ET_ID entityID>
constexpr bool isCompInET()
{
	for (int i = 0; i < ET<entityID>::noOfComponents; ++i)
	{
		if (componentID == ET<entityID>::components[i])
		{
			return true;
		}
	}
	return false;
}

//needed functions to create Component for 2SS
template<Comp_ID id>
constexpr std::array<int, ET_ID::MAX_ET_ID> getETContainingComp()
{
	std::array<int, ET_ID::MAX_ET_ID> sparse = {};
	int counter = 0;
	for ( auto et_id : activeETs)
	{
		//if ET has this component increase counter, add sparse
		if (isCompInET<id, et_id>())
		{
			sparse[(int) et_id] = ++counter;
		}
	}
	return sparse;
}


template<Comp_ID id, class ComponentType = CompInfo<id>::type>
struct PerfectComp
{
	using type = ComponentType;
	//	static constexpr auto sparse = getETContainingComp(id);
};