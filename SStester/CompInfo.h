#pragma once
#include <array>

class vec3
{
public:
	float x, y, z;
	float distance() const { return (x * x + y * y + z * z); }
	float distance(vec3 vec) { return(sqrt((x - vec.x) * (x - vec.x) + (y - vec.y) * (y - vec.y) + (z - vec.z) * (z - vec.z))); }
	bool operator< (const vec3& vec)
	{
		return this->distance() < vec.distance();
	}
	vec3(int x, int y, int z) :x(x), y(y), z(z) {}
	vec3() = default;
};

enum Comp_ID
{
	BLANK,
	STATE,
	POS3D,
	MOD,
	SIZE,
	VELOCITY,
	MASS,
	PERMA_FORCE,
	ORIENTATION,
	MAX_COMP_ID
};

//used to dertime which SS to use and its functionality.
enum  CompAttributes
{
	UNIQUE = 1, //Only one component of this type in ET
	SORTABLE = 1 << 1,
	CONST = 1 << 2,
	LINKED = 1 << 3, //is ordered by another component (i.e you want masses ordered by position)
};



//ComponentType must be default initializable for ETData.
#pragma region Compinfo

template<Comp_ID id>
struct CompInfo
{
	using type = int; //component type
	static constexpr uint32_t attributes = 0; //component attributes - used for deciding on what SS options to use
	static constexpr uint32_t expectedAmount = 10000; //how many of this component type are expected - used for optimizing SS and maybe
													  //chosing entinty num
};

template<>
struct CompInfo<Comp_ID::STATE>
{
	using type = uint32_t;
	static constexpr uint32_t attributes = 0;
	static constexpr uint32_t expectedAmount = 10000;

};

template<>
struct CompInfo<Comp_ID::POS3D>
{
	using type = vec3;
	static constexpr uint32_t attributes = (CompAttributes::LINKED | CompAttributes::SORTABLE | CompAttributes::UNIQUE);
	static constexpr uint32_t expectedAmount = 10000;
};
template<>
struct CompInfo<Comp_ID::VELOCITY>
{
	using type = float;
	static constexpr uint32_t attributes = CompAttributes::LINKED;
	static constexpr uint32_t expectedAmount = 10000;

};
template<>
struct CompInfo<Comp_ID::ORIENTATION>
{
	using type = vec3;
	static constexpr uint32_t attributes = 0;
	static constexpr uint32_t expectedAmount = 10000;

};

template<>
struct CompInfo<Comp_ID::MASS>
{
	using type = float;
	static constexpr uint32_t attributes = 0;
	static constexpr uint32_t expectedAmount = 10000;

};

template<>
struct CompInfo<Comp_ID::PERMA_FORCE>
{
	using type = float;
	static constexpr uint32_t attributes = 0;
	static constexpr uint32_t expectedAmount = 10000;

};

#pragma endregion