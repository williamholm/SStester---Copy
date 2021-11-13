#pragma once
#include "EntityManager.h"
#include "FunctionTimer.h"

// step 1) create player/arrows
// step 2) read arrow positions
// step 3) compare positions
// step 4) update velocities

//what about passing as a template for system a param pack or tuple of ids to be used?
void testSystem(EntityManager* EM)
{
	Testing::Timer timer;
	timer.startTimer();
	ET<PC>::components;
	ET<ARROW>::components;
	ET<MAGIC_ARROW>::components;
	//create player
	Entity<PC> player;
	ETData<PC> playerData;
	playerData.get<POS3D>() = vec3(0,0,0); //problem with this method is there is no help with types - need to refer to ET/Comp always.
	//add player to manager
	EM->addEntity(player, playerData);

	std::vector<vec3> vecA;
	std::vector<vec3> vecMA;

	std::vector<Entity<MAGIC_ARROW>> magicArrow(5000);
	std::vector<Entity<ARROW>> arrow(5000);
	ETData<ARROW> arrowData; //only need one of each type of arrow data here as arrows are made one at a time.
	ETData<MAGIC_ARROW> magicArrowData;
	for (int i = 0; i < 5000; ++i)
	{
		arrowData.get<VELOCITY>() = 10;
		arrowData.get<POS3D>() = vec3(i % 21, i% 34, i%23); //kinda random enough for this

		vecA.emplace_back(vec3(i % 21, i % 34, i % 23));
		vecMA.emplace_back(vec3(i % 21, i % 34, i % 23));


		magicArrowData.get<VELOCITY>() = 10;
		magicArrowData.get<POS3D>() = vec3(i % 21, i % 34, i % 23);

		EM->addEntity(arrow[i], arrowData);
		EM->addEntity(magicArrow[i], magicArrowData);
	}

	vec3 playerPos = EM->getComponentData<POS3D>(player);

	//common coding patern for scrolling through CDS.
	//Can be expanded if ordering of entitys in CDS is same across all components, allowing i + offset to access mCDS without any trouble.
	

	std::vector<Bound> bounds = EM->getAllBounds<ARROW,POS3D>();
	for (const auto& bound : bounds)
	{
		for (int i = bound.mStart; i < bound.mEnd; ++i)
		{
			if (playerPos.distance(EM->getComponentData<POS3D>(i)) < 5)
			{
				//how to deal with this? needs to be tuples again? look into constexpr arrays again?
				/*
				1) tuples
				2) some kind of way to go from entity num -> location of arrow[i]?

				3) array based stuff - that seems infeasble, is almost entirely incompatible with tuples due to std::get.
				*/
		//		EM->addToPendingDelete(arrow[i]);
			}
			else if (playerPos.distance(EM->getComponentData<POS3D>(i)) < 10)
			{
				//get velocity of entity at index i of mEDS in SS<POS3D> and divide by distance from player.
				EM->getComponentData<VELOCITY>(EM->getEntity<POS3D>(i)) /= playerPos.distance(EM->getComponentData<POS3D>(i));
			}
	//	std::cout << "\n i: " << i;
		}
	}

	timer.startTimer();
	EM->deletePending();
	timer.printTime("delete timer: ");
	std::cout << "Size: " << EM->sparse<POS3D>().mCDS.size();
}