#include "loottable.hpp"
#include "fs.h"
#include "util.h"
#include <fstream>
#include <sstream>

void LootTable::load(const char *tableFile)
{
    std::string fullPath = std::string("nitro:/loottables/") + tableFile + ".tbl";

    // check if the file exists
    if (!fsFileExists(fullPath.c_str()))
    {
        printf("Error: loot table file %s doesn't exist (full path: %s)\n", tableFile, fullPath.c_str());
        hang();
    }

    std::ifstream ifs(fullPath);
    std::string line;
    u16 currentState = 0; // u16 should be enough
    bool numStatesMode = true;
    while (std::getline(ifs, line))
    {
        // TODO make StringVector not exclusive to save.cpp

        // split line
        std::stringstream ss(line);
        std::string line2;
        std::vector<std::string> split;
        while (std::getline(ss, line2))
            split.push_back(line2);

        if (numStatesMode)
        {
            if (split[0] != "numstates")
            {
                printf("Error: 1st line of loot table file should contain numstates\n");
                hang();
            }

            numStates = std::stoi(split[1]);
            if (numStates == 0)
            {
                printf("Error: can't have 0 states in loot table\n");
                hang();
            }

            // allocate states array
            states = (ChestBlock::State *)malloc(numStates * sizeof(ChestBlock::State));

            // allocate states in array
            for (u16 i = 0; i < numStates; ++i)
                states[i] = (ChestBlock::State *)malloc(sizeof(ChestBlock::State));

            numStatesMode = false;
        }

        if (line == "STATE")
        {
            ++currentState;
            continue;
        }

        std::string id = split[0];
        u8 count = split[1];
        u8 x = split[2];
        u8 y = split[3];
        states[currentState][x + y * 5] = InventoryItem(id, count);
    }
}

LootTable::LootTable(const char *tableFile) : states(), numStates(0)
{
    load(tableFile);
}

LootTable::~LootTable()
{
    for (u16 i = 0; i < numStates; ++i)
        free(states[i]);
    free(states);
}

void LootTable::apply(ChestBlock *chest)
{
    chest->clear();
}
