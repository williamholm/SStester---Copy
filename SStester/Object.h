#pragma once
#include <assert.h>
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
		return ((mEntity >> entityValueBits) & maxEntityFlag); //don't think maxEntityFlag is needed here
	}
	void addFlags(uint32_t flags) noexcept
	{
		assert(flags <= maxEntityFlag);
		mEntity |= (flags << entityValueBits);

	}
	inline void addNumber(const uint32_t entityNum) noexcept
	{
		assert(entityNum <= maxEntityNumber);
		mEntity = entityNum + (this->group() << entityValueBits); //might not be fastest way should be fine though.
	}
	Entity32Bit() :mEntity(0) {}
	~Entity32Bit() = default;

	Entity32Bit(const uint32_t entityNumber, const uint32_t flags) : mEntity(entityNumber)
	{
		assert(entityNumber < maxEntityNumber);
		addFlags(flags);
	}
};

