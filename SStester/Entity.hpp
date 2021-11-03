#pragma once
#include <type_traits>
#include "PerfectET.hpp"
#include "ETData.hpp"
template<ET_ID id>
class Entity : public Entity32Bit
{
public:
	//add entity to batch creator in EM - should this include data now to be tansfered later? is there a way to do otherwise?
	Entity() : Entity32Bit{ 0,id }
	{
		
	}
	~Entity() {}
};
