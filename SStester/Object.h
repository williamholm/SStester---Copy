#pragma once
#include "PerfectET.hpp"
static constexpr uint32_t maxEntityFlag = 0xFFF;
static constexpr uint32_t maxEntityNumber = 0xFFFFF;
static constexpr uint32_t entityValueBits = 20;
static constexpr uint32_t Full20Bit = 0xFFFFF;

inline uint32_t getEntityNum(uint32_t entity) { return (maxEntityNumber & entity); }
inline uint32_t getEntityId(uint32_t entity) { return ((entity >> entityValueBits) & maxEntityFlag); }


//options with this set up: max 1m entities, 4095 different entity types for quick searching (maybe split larger groups for MergeSort?)
class Entity32Bit
{
private:
	uint32_t mEntity;
public:
	inline uint32_t number() const  noexcept
	{
		return mEntity & maxEntityNumber;
	}
	//
	inline uint32_t group() const noexcept
	{
		return ((mEntity >> entityValueBits) & maxEntityFlag);
	}
	void addFlags(uint32_t flags) noexcept
	{
		assert(flags <= maxEntityFlag);
		mEntity |= (flags << entityValueBits);

	}
	Entity32Bit() = default;
	~Entity32Bit() = default;

	Entity32Bit(const uint32_t entityNumber, const uint32_t flags) : mEntity(entityNumber)
	{
		assert(entityNumber < maxEntityNumber);
		//might be worth doing forwarding here, but copying is cheep anyway.
		addFlags(flags);
	}
};


/*
* METHOD 1 classes
* 
* Pros:
* 
* Cons:
*/

class Object : public Entity32Bit
{
public:
	//directly add to sparse set OR add to buffer in a manager then do group creates/deletes
	//or use add unsorted even for sorted sets and just make sure to sort before reusing a sorted set.
	void addPosition(int pos) { std::cout << "\n pos"; }
	void addModel(int model) { std::cout << "\n model"; }
	Object(uint32_t entityNumber, uint32_t flags, int pos, int model) : Entity32Bit{ entityNumber, flags }
	{
		addPosition(pos);
		addModel(model);
	}
	~Object() = default;
};

class PhysicsObject : public Object
{
public:
	void addOrientation(int ori) { std::cout << "\n ori"; }
	void addSpeed(int speed) { std::cout << "\n speed"; }
	
	PhysicsObject(uint32_t entityNumber, uint32_t flags, int pos, int model, int speed, int ori): Object{entityNumber,flags,pos,model}
	{	
		addOrientation(ori);
		addSpeed(speed);
	}
	~PhysicsObject() = default;
};

