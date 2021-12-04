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

template<int index = 0, ET_ID... ids>
void tupleIterationExample(const std::tuple<TaggedBound<ids>...>& bounds, EntityManager* EM, vec3 playerPos)
{
	if constexpr (index == sizeof...(ids))
	{
		return;
	}
	else
	{
		for (int i = std::get<index>(bounds).start; i < std::get<index>(bounds).end; ++i)
		{

			if (playerPos.distance(EM->getComponentData<POS3D>(i)) < 5)
			{
				//uses TaggedBounds<id> to deduce template argument for addToPendingDelete
				EM->addToPendingDelete(std::get<index>(bounds), EM->getEntity<POS3D>(i));

			}
			else if (playerPos.distance(EM->getComponentData<POS3D>(i)) < 10)
			{
				//get velocity of entity at index i of mEDS in SS<POS3D> and divide by distance from player.
				EM->getComponentData<VELOCITY>(EM->getEntity<POS3D>(i)) /= playerPos.distance(EM->getComponentData<POS3D>(i));
			}
		}
		tupleIterationExample<index+1>(bounds, EM, playerPos);
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

	//Sol1 of iterating cds while being able to know ET_ID at compile time
	//need to make constexpr arrays which are ET<id> + ET<id>::inheritors for this to work, not sure if this is final way though
//	timer.startTimer();
//	TS4Loop<2, gTestArrowArr, 0>(EM, playerPos);
//	timer.printTime("TS4Loop: ");


	//Sol2 of iterating cds while being able to know ET_ID at compile time
	//this is better as it is more in line with what will come in c++23,
//	auto tupleBounds = EM->getInheritorBounds<ARROW, POS3D>();
//	timer.startTimer();
//	tupleIterationExample(tupleBounds, EM, playerPos);
//	timer.printTime("TS4Loop: ");
//	timer.startTimer();
//	EM->deletePending();
//	timer.printTime("Delete Timer: ");

	std::cout << "Size after deleting: " << EM->sparse<POS3D>().mCDS.size();
}



void testSystem2(EMTSSS* EM)
{
	Testing::Timer timer;
	ET<PC>::components;
	ET<ARROW>::components;
	ET<MAGIC_ARROW>::components;
	//create player
	Entity<PC> player;
	ETData<PC> playerData;
	playerData.get<POS3D>() = vec3(0, 0, 0); //problem with this method is there is no help with types - need to refer to ET/Comp always.

	EM->addEntity(player, playerData);

	std::vector<Entity<MAGIC_ARROW>> magicArrow(5000);
	std::vector<Entity<ARROW>> arrow(5000);
	ETData<ARROW> arrowData; //only need one of each type of arrow data here as arrows are made one at a time.
	ETData<MAGIC_ARROW> magicArrowData;
	for (int i = 0; i < 5000; ++i)
	{
		arrowData.get<VELOCITY>() = 10;
		arrowData.get<POS3D>() = vec3(i % 21, i % 34, i % 23); //kinda random enough for this

		magicArrowData.get<VELOCITY>() = 10;
		magicArrowData.get<POS3D>() = vec3(i % 21, i % 34, i % 23);

		EM->addEntity(arrow[i], arrowData); //realistically should make one which only needs ET_ID, and data, if im keeping TS4Loop style thing
		EM->addEntity(magicArrow[i], magicArrowData);
	}

	vec3 playerPos = EM->getComp<POS3D>(player);

	std::array<ET_ID, 2> inhArr = { ARROW,MAGIC_ARROW };
	timer.startTimer();
	for (auto entityType : inhArr)
	{
		int noOfET = EM->getNoOfET(entityType);
		for (int i = 1; i < noOfET; ++i)
		{
			if (playerPos.distance(EM->getComp<POS3D>(entityType,i)) < 5)
			{
			}
			else if (playerPos.distance(EM->getComp<POS3D>(entityType,i)) < 10)
			{
				//get velocity of entity at index i of mEDS in SS<POS3D> and divide by distance from player.
				EM->getComp<VELOCITY>(EM->getEntity(entityType,i)) /= playerPos.distance(EM->getComp<POS3D>(entityType,i));
			}
		}
	}
	timer.printTime("Normal Loop: ");

	std::cout << "Size after deleting: " << EM->mSharedSS.totalSize() << "\n";
}


