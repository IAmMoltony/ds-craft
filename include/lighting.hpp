#pragma once

#include "block.hpp"
#include <limits.h>

// if rect x=-1 and y=-1 and w=-1 and h=-1 then it updates all blocks
void updateLighting(BlockList *blocks, Rect updateArea);
