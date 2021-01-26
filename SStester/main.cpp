
#include <iostream>
#include "EntityTester.h"
#include "SparseSet.h"
#include "EntityManager.h"
#include "EntityType.h"
#include "2SortsSparse.hpp"
#include "Component.h"
#include "PerfectET.hpp"

/*
Ordered Sparse Requirements:

Sparse rules:

1) entity gives you CDS value
2) EDS gives entity value
3) CDS must be linked to EDS, such that you can get entity from CDS[i] without to much work.

4) implementation detail : all internal arrays have to be considered to start at [1] as 0 is reserved for no entity,
this behaviour could be changed by making reserved value = UINT32_MAX

Access:

1) single access via entity
2) multi unordered(or ordered within ETs) access via EntityType(s)
3) multi access via multi ordered access via EntityTypes (or all)
4) try and keep SS1[enity] = SS2[entity] i.e all components have same indexing for each ET (or change by known offset). 

Sorting:

1) all should be able to sort within ETs
2) given a group of ETs much sort all of CDS containing ETs
3) Match sorting must be possible 


*/
/*
game dev rules:
try and store states outside of gameloop.
*/

int main()
{
	constexpr auto meh = InhertorIndices<PHYSICS_OBJECT>::value;

	for (const auto& eh : meh)
	{
		std::cout << eh << " ";
	}
	constexpr auto blah = Component<TEST_INT>::sparse[OBJECT];


	while (1) {}
	return 0;
}