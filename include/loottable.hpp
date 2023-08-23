#pragma once

#include "block.hpp"

class LootTable
{
private:
    u16 numStates;
    ChestBlock::State *states[];

    void load(const char *tableFile);

public:
    LootTable(const char *tableFile);
    ~LootTable();

    void apply(ChestBlock *chest);
};
