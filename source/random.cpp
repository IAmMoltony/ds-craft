#include "random.hpp"
#include <random>

static std::mt19937 mtEngine(1);

void randomSetSeed(u32 seed)
{
    mtEngine.seed(seed);
}

u32 randomGenerate(void)
{
    return mtEngine();
}
