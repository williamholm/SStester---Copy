#pragma once
#include <array>

enum class ET_ID
{
	OBJ,
	STATIC_OBJ,
	PHYS_OBJ,
	PROJECTILE,
	MAGIC,
	MAGIC_ARROW
};

enum class CompID
{
	STATE,
	POS3D,
	MOD,
	SIZE,
	VELOCITY,
	MASS,
	PERMA_FORCE,
	ORIENTATION
};
//test
template<typename T, int N, int M>
constexpr std::array<T, N + M> concatinate(const std::array<T, N>& arr1, const std::array<T, N>& arr2)
{
	std::array<T, N + M> temp;
	for (int i = 0; i < N; ++i)
	{
		temp[i] = arr1[i];
	}
	for (int i = N; N < N+M; ++i )
	{
		temp[i] = arr2[i - N];
	}

	return temp;
}

template<ET_ID id>
struct ET
{
	static constexpr int inheritsFromSize = 0;
	static constexpr std::array<ET_ID, inheritsFromSize> InheritsFrom = {};

	static constexpr int noOfComponents = 0;
	static constexpr std::array<ET_ID, noOfComponents> components = {};
};

constexpr auto getComponents();
constexpr auto getInheritors();
constexpr auto getParents();