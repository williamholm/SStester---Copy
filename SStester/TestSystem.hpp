#pragma once
#include "EntityManager.h"


// step 1) get player position
// step 2) read projectile positions
// step 3) compare positions
// step 4) update velocities
// step 5) update orientations

void testSystem(EntityManager* EM)
{
	//step 1, just assuming pos = 0,0,0 so i don't have to change vec3. 
	vec3 playerPos = EM->getComponentData<POS3D>(Entity32Bit(0, PC));
	//step 2 & step 3, could also do step 4 but want to see process of it done seperatley
	std::vector<Entity32Bit> entitiesInRange;
	for (const auto& inheritor : ET<PROJECTILE>::inheritors) //currently misses projectile.
	{
		for (int i = EM->getETBegining<POS3D>(inheritor); i < EM->getETend<POS3D>(inheritor); ++i)
		{
			if (EM->getComponentData<POS3D>(i).distance() < 30) //assumes PC is at 0,0,0 - need to change vec3, but its temp anyway
			{
				EM->getComponentData<POS3D>(i).x = 100;
				entitiesInRange.push_back(EM->getEntity<POS3D>(i));
			}
		}
	}
	//step 4
	for (const auto& entities : entitiesInRange)
	{
		EM->getComponentData<ORIENTATION>(entities).x = 100;
		EM->getComponentData<VELOCITY>(entities) = 100;
	}
}