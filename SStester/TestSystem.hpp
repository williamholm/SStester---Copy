#pragma once
#include "EntityManager.h"
#include "FunctionTimer.h"

// step 1) create player/arrows
// step 2) read arrow positions
// step 3) compare positions
// step 4) update velocities

static constexpr std::array<ET_ID, 2> gTestArrowArr = { ARROW,MAGIC_ARROW };

template<int size, const std::array<ET_ID, size>& ids, int index = 0>
constexpr void TS4Loop(EntityManager* EM, vec3 playerPos)
{

	if constexpr (index == size)
	{
		return;
	}
	else
	{
		//code
		const Bound bound = EM->getBound<POS3D>(ids[index]);
		for (int i = bound.mStart; i < bound.mEnd; ++i)
		{

			if (playerPos.distance(EM->getComponentData<POS3D>(i)) < 5)
			{
				//EM->addToPendingDelete(Entity<ids[index]>(EM->getEntity<POS3D>(i).number()));
			}
			else if (playerPos.distance(EM->getComponentData<POS3D>(i)) < 10)
			{
				//get velocity of entity at index i of mEDS in SS<POS3D> and divide by distance from player.
				EM->getComponentData<VELOCITY>(EM->getEntity<POS3D>(i)) /= playerPos.distance(EM->getComponentData<POS3D>(i));
			}
			
		}
		TS4Loop<size, ids, index + 1>(EM, playerPos);
		return;
	}
}

void testSystem(EntityManager* EM)
{
	Testing::Timer timer;
	ET<PC>::components;
	ET<ARROW>::components;
	ET<MAGIC_ARROW>::components;
	//create player
	Entity<PC> player;
	ETData<PC> playerData;
	playerData.get<POS3D>() = vec3(0,0,0); //problem with this method is there is no help with types - need to refer to ET/Comp always.

	EM->addEntity(player, playerData);

	std::vector<Entity<MAGIC_ARROW>> magicArrow(5000);
	std::vector<Entity<ARROW>> arrow(5000);
	ETData<ARROW> arrowData; //only need one of each type of arrow data here as arrows are made one at a time.
	ETData<MAGIC_ARROW> magicArrowData;
	for (int i = 0; i < 5000; ++i)
	{
		arrowData.get<VELOCITY>() = 10;
		arrowData.get<POS3D>() = vec3(i % 21, i% 34, i%23); //kinda random enough for this

		magicArrowData.get<VELOCITY>() = 10;
		magicArrowData.get<POS3D>() = vec3(i % 21, i % 34, i % 23);

		EM->addEntity(arrow[i], arrowData); //realistically should make one which only needs ET_ID, and data, if im keeping TS4Loop style thing
		EM->addEntity(magicArrow[i], magicArrowData);
	}

	vec3 playerPos = EM->getComponentData<POS3D>(player);

	std::vector<Bound> bounds = EM->getAllBounds<ARROW,POS3D>();
	//this works the best, assuming you do not need to delete anything or add anything within the loop.
	timer.startTimer();
	for (const auto& bound : bounds)
	{
		for (int i = bound.mStart; i < bound.mEnd; ++i)
		{
			if (playerPos.distance(EM->getComponentData<POS3D>(i)) < 5)
			{
		//		EM->addToPendingDelete(arrow[i]); invalid, need constexpr access to do this
			}
			else if (playerPos.distance(EM->getComponentData<POS3D>(i)) < 10)
			{
				//get velocity of entity at index i of mEDS in SS<POS3D> and divide by distance from player.
				EM->getComponentData<VELOCITY>(EM->getEntity<POS3D>(i)) /= playerPos.distance(EM->getComponentData<POS3D>(i));
			}
		}
	}
	timer.printTime("Normal Loop: ");

	timer.startTimer();
	//need to make constexpr arrays which are ET<id> + ET<id>::inheritors for this to work, not sure if this is final way though
	TS4Loop<2, gTestArrowArr, 0>(EM, playerPos);
	timer.printTime("TS4Loop: ");
	EM->deletePending();
	timer.printTime("delete timer: ");
	std::cout << "Size after deleting: " << EM->sparse<POS3D>().mCDS.size();
}

//this is unlikely to work for most cases, as ((expresion ),...) isn't that useful for what i need, would need recersion anyway.
template<ET_ID... ids>
void testSystem3(std::tuple<TaggedBound<ids>...>const& bounds, EntityManager* EM)
{
	std::apply
	(
		[](TaggedBound<ids>... bound)
		{
			for (int i = 5; i < 7; ++i)
			{
				((std::cout << "\n\n : " << ET<bound.mID>::parents[0]), ...);
			}
		},
		bounds
	);
}
