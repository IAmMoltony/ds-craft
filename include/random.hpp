#pragma once

#include <nds.h>

void randomSetSeed(u32 seed);
u32 randomGenerate(void);
u32 randomRange(u32 min, u32 max);
