#pragma once
#include <type_traits>
#include "PerfectET.hpp"
#include "ETData.hpp"
template<ET_ID id>
class Entity : public Entity32Bit
{
public:
	//add entity to batch creator in EM - should this include data now to be tansfered later? is there a way to do otherwise?
	Entity()
	{
		std::cout << "\nhere" << ET<id>::components[1] << "\n\n\n";
	}
	//make single entity instantly
	Entity(ETData<id> data)
	{
		std::cout << "\nhere" << std::get<0>(data.data) << "\n\n\n";
	}
	~Entity() {}
};
