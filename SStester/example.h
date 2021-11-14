#pragma once
#include "EntityManager.h"
/*
user defines all components and all Entity Types (ET) in PerfectET.h using CompInfo and ETInfo, this is then compiled into useful
info in ET<id> and Comp<id>. Using these two classes EM navigates/provides interface of a tuple of all TwoSortsSparse<Comp_ID> that it contains

*/

void exampleSystem(EntityManager* EM)
{
	//EM contains data of all entities


	//this loop just sets position of all arrows, stored in EM to 2,3,4, and their velocities to 5;

	auto bound = EM->getBound<POS3D>(ARROW); //this gets start and end index of entity type arrow in mCDS of 2SortsSparse<POS3D>
	for (int i = bound.mStart; i < bound.mEnd; ++i) //for loop iterating through all component POS3D for Entity Type ARROW
	{
		EM->getComponentData<POS3D>(i) = vec3(2,3,4); //returns a reference to i'th index of mCDS in POS3D, that can be manipulated
		Entity32Bit entity = EM->getEntity<POS3D>(i); //returns entity that contains data at the i'th index of mCDS
		EM->getComponentData<VELOCITY>(entity) = 5;	  //returns a reference to velocity of entity
	}
	
}