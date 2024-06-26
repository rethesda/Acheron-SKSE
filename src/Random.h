
#pragma once

namespace Random
{
	template <class T>
	T draw(T a_min, T a_max)
	{
		static std::mt19937 eng{ std::random_device{}() };
		
		if constexpr (std::is_integral_v<T>)
			return std::uniform_int_distribution<T>{ a_min, a_max }(eng);
		else
			return std::uniform_real_distribution<T>{ a_min, a_max }(eng);
	}

	template <class V>
	void shuffle(V& a_container)
	{
		static std::mt19937 eng{ std::random_device{}() };
		std::ranges::shuffle(a_container, eng);
	}
};