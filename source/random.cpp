#include "random.hpp"
#include <random>

namespace rng
{
	static std::mt19937 _mtEngine(1);

	void setSeed(u32 seed)
	{
		_mtEngine.seed(seed);
	}

	u32 generate(void)
	{
		return _mtEngine();
	}

	u32 range(u32 min, u32 max)
	{
		return min + generate() % ((max + 1) - min);
	}

	s32 rangeSigned(s32 min, s32 max)
	{
		return min + generate() % ((max + 1) - min);
	}

	bool chance(u8 chance)
	{
		if (chance > 100)
			// >100% chance = always success
			return true;
		if (chance == 0)
			// 0% chance = never success
			return false;
		return range(1, 100) <= chance;
	}
}