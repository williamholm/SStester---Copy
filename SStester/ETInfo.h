#pragma once
#include "CompInfo.h"

enum ET_ID
{
	//UTILITY
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
	//UTILITY
	MAX_ET_ID
};

enum  ETattributes
{
	VOLATILE, //short lifespan / constant changes in Components
	PERMANENT, //not expected to be deleted
	CONSTANT //no changes to components.

};


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
	static constexpr std::array<ET_ID, inheritsFromSize> inheritsFrom = { ET_ID::NPC, ET_ID::WOLF };

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