#pragma once
#include <type_traits>
#include "PerfectET.hpp"
#include "ETData.hpp"
template<ET_ID id>
class Entity : public Entity32Bit
{
public:
	Entity() : Entity32Bit{ 0,id }
	{
		
	}
	Entity(uint32_t entityNum) : Entity32Bit{ entityNum,id } {}
	~Entity() {}
};
