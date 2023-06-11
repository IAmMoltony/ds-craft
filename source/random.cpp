#include "random.hpp"

static u64 randomSeed = 1;

void randomSetSeed(u64 seed)
{
    randomSeed = seed;
}

u64 randomGenerate(void)
{
    // taken from wikipedia
    randomSeed ^= randomSeed >> 12;
    randomSeed ^= randomSeed << 25;
    randomSeed ^= randomSeed >> 27;
    return randomSeed * 0x2545f4914f6cdd1ULL;
}
