#include "random.hpp"
#include <random>
#include <stdio.h>

static std::mt19937 mtEngine(1);

void randomSetSeed(u32 seed)
{
    mtEngine.seed(seed);
    printf("set seed to %lu\n", seed);
}

u32 randomGenerate(void)
{
    return mtEngine();
}
