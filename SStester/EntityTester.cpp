#include "EntityTester.h"



EntityTester::EntityTester(uint32_t noOfEntities) : mBasicEM(), mTypeSortedEM(), mTimer()
{
	addEntities(noOfEntities);

}


EntityTester::~EntityTester()
{
}
_NODISCARD auto EntityTester::messUpCache()
{
	static constexpr int size = 100000; //400000bytes;
	std::vector<BigData<size>> data;
	for (int i = 0; i < 60; ++i)
	{
		data.push_back(BigData<size>());
	}
	for (int i = 0; i < 60; ++i)
	{
		for (int j = 0; j < size; ++j)
		{
			data[i].mMemoryFill[j] = 4;
		}
	}
	return data;
}
void EntityTester::randomFillData()
{
	std::default_random_engine RNG;
	std::uniform_real_distribution<float> floatDistribution(-100.0f, 100.0f);
	std::uniform_int_distribution<uint32_t> intDistribution(0, 1000);

}

template<ET_ID id>
void EntityTester::addEntities(uint32_t amount)
{
	ETData<id> data;
	Entity<id> entity;

	std::cout << "\nAdding " << amount << " " << ET<id>::name;
	mTimer.startTimer();
	for (int i = 0; i < amount; ++i)
	{
		mBasicEM.addEntity(entity, data);
	}
	mTimer.printTime("\nBasicEM: ");
	mTimer.startTimer();
	for (int i = 0; i < amount; ++i)
	{
		mTypeSortedEM.addEntity(entity, data);
	}
	mTimer.printTime("\tTypeSortedEM: ");

}

/*
* Realistically you wouldn't do large adds like this as there are lot of resizes going on.
* Need to add batch creation or something of the sort to handle this. Not to high priority atm
*/
void EntityTester::addEntities(uint32_t noOfEntities)
{
	assert(noOfEntities > 1000);
	addEntities<OBJ>(noOfEntities / 5);
	addEntities<MAGIC>(noOfEntities / 6);
	addEntities<PHYS_OBJ>(noOfEntities / 6); //32/60
	addEntities<PLANT>(noOfEntities / 10);
	addEntities<STATIC_OBJ>(noOfEntities / 10); //44/60
	addEntities<MONSTER>(noOfEntities / 15);
	addEntities<NPC>(noOfEntities / 15);
	addEntities<ARROW>(noOfEntities / 15); //56/60
	addEntities<MAGIC_ARROW>(noOfEntities / 20);//59/60
	addEntities<WOLF>(5);
	addEntities<TALKING_WOLF>(1); //in actual program want to avoid this, especially for TSSS, not so much for basicSS.
	addEntities<PC>(1);
	const int r = (noOfEntities / 60.0f) - 7;
	if (r >= 0)
	{
		addEntities<CREATURE>(r);
	}
	std::cout << "\nnumber of Entities Added = " << noOfEntities
		<<"\ncurrent size = " << mTypeSortedEM.size(); //nb slightly different numbers due to rounding, not important
	

}

template<ET_ID id>
std::vector<Entity<id>> EntityTester::getSpreadOutEntities(uint32_t amount)
{
	const int noOfEntities = mTypeSortedEM.getNoOfET(id);
	const int gap = noOfEntities / amount;
	assert(amount < noOfEntities);
	std::vector<Entity<id>> temp;
	temp.reserve(amount);
	mTimer.startTimer();
	for (int i = 0; i < noOfEntities; i += gap)
	{
		temp.emplace_back(Entity<id>(mTypeSortedEM.getEntity(id,i)));
	}
	return temp;

}

template<ET_ID id>
void EntityTester::deleteEntities(uint32_t amount)
{

	std::vector<Entity<id>> temp = getSpreadOutEntities<id>(amount);
	messUpCache();
	std::cout << "\nDeleting " << amount << " " << ET<id>::name;
	mTimer.startTimer();
	for (int i = 0; i < amount; ++i)
	{
		mBasicEM.deleteEntity(temp[i]);
	}
	mTimer.printTime("\nBasicEM: ");
	mTimer.startTimer();
	for (int i = 0; i < amount; ++i)
	{
		mTypeSortedEM.deleteEntity(temp[i]);
	}
	mTimer.printTime("\tTypeSortedEM: ");
}

void EntityTester::deleteEntities(const uint32_t noOfEntities)
{
	assert(noOfEntities > 1000 &&  noOfEntities < mTypeSortedEM.size());
	deleteEntities<OBJ>(noOfEntities / 5);
	deleteEntities<MAGIC>(noOfEntities / 6);
	deleteEntities<PHYS_OBJ>(noOfEntities / 6); //32/60
	deleteEntities<PLANT>(noOfEntities / 10);
	deleteEntities<STATIC_OBJ>(noOfEntities / 10); //44/60
	deleteEntities<MONSTER>(noOfEntities / 15);
	deleteEntities<NPC>(noOfEntities / 15);
	deleteEntities<ARROW>(noOfEntities / 15); //56/60
	deleteEntities<MAGIC_ARROW>(noOfEntities / 20);//59/60
	deleteEntities<WOLF>(5);
	deleteEntities<TALKING_WOLF>(1); //in actual program want to avoid this, especially for TSSS, not so much for basicSS.
	deleteEntities<PC>(1);
	const int r = (noOfEntities / 60.0f) - 7;
	if (r >= 0)
	{
		deleteEntities<CREATURE>(r);
	}
	//mTypeSortedEM.shrink_to_fit(); //do this to check for memory leaks although it is inefficient
	//mBasicEM.shrink_to_fit();
	std::cout << "\nnumber of Entities deleted = " << noOfEntities
		<< "\ncurrent size = " << mTypeSortedEM.size(); //nb slightly different numbers due to rounding, not important

}

void EntityTester::accessAllETComp()
{
	
}

void EntityTester::accessByComp()
{

}

void EntityTester::unorderedAccess()
{
	messUpCache();
	const int noOfEntities = mBasicEM.size<POS3D>();
	std::cout << "\nUpdating positions of " << noOfEntities << " entities\n";
	
	mTimer.startTimer();
	for (int i = 1;i<noOfEntities;++i)
	{
		mBasicEM.sparse<POS3D>()[i].x = 5;
		mBasicEM.sparse<POS3D>()[i].y = 5;
		mBasicEM.sparse<POS3D>()[i].z = 5;
	}
	mTimer.printTime("\nTime taken for unordered access of basicSS: ");
	mTimer.startTimer();
	for (const auto id : Comp<POS3D>::ETsWithComp)
	{
		const int size = mTypeSortedEM.getNoOfET(id);
		for (int i = 0; i < size; ++i)
		{
			mTypeSortedEM.getComp<POS3D>(id, i).x = 5;
			mTypeSortedEM.getComp<POS3D>(id, i).y = 5;
			mTypeSortedEM.getComp<POS3D>(id, i).z = 5;
		}
	}
	mTimer.printTime("\nTime taken for unordered access of TSSS: ");

}