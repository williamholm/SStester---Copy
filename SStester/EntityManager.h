#pragma once
#include <vector>
#include <algorithm>
#include <iostream>

#include "SparseSet.h"
#include "Object.h"

/* Design:
* 
* Aim to reduce individual SS sizes through condensing them, Aim to reduce largest entity num (if possible reuse number for
ETs with no overlapping components).
* Keep track of which functions are requesting r/w data for MT porpouses (maybe do this inpart within SS)
* Handle batch creation/deletion of entities
*

*/

/* System:
* Must declare what data needs to be accessed and whether its read or r/w
* Must check if data is sorted as wanted. If data isn't sorted but needs to be it must be re-sorted, this is then r/w.
* Must ensure that that data isn't in conflict with another system, and if not then update SS state.
* If read only any deletion/creation of entities must be done by EM in a batch.


*/


class EntityManager
{
private:
	std::vector<void*> mpSparseSets;
	std::vector<std::vector<uint32_t>> mAvailableEntityValues;
public:

	Entity32Bit createEntity(int entityTypeName, uint32_t flags,...);
	//either marks as to be deleted or instant deletetion
	void deleteEntity(Entity32Bit entity);
	//deletes all flagged entities - might be slower to do this not sure
	void deleteEntities();
	void* createComponentMSS(int componentName);
	//access all entitity and its inheritors data.
	void* getData(int entityTypeName);
	//access to components from entitity and its inheritors data. -- want this to be main access patern
	void* getData(int entityTypeName, std::vector<int> components);
	//access all components data This might be uneeded
	void* getData(std::vector<int> components);

	EntityManager();
	~EntityManager();


};


