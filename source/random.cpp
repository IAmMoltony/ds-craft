#include "random.hpp"
#include <random>

static std::mt19937 _mtEngine(1);

void randomSetSeed(u32 seed)
{
    _mtEngine.seed(seed);
}

u32 randomGenerate(void)
{
    return _mtEngine();
}

u32 randomRange(u32 min, u32 max)
{
    return min + randomGenerate() % ((max + 1) - min)
}
