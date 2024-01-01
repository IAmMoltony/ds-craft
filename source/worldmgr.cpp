#include "worldmgr.hpp"
#include "mtnconfig.h"
#include "fs.h"
#include "save.hpp"
#include <sstream>
#include <dirent.h>

WorldManager::WorldList WorldManager::getWorlds(void)
{
    // first we iterate through the world directory
    DIR *dp;
    dp = opendir(mtnconfigGet("worldsDir"));
    struct dirent *ep;
    std::string dls; // directory list string
    while ((ep = readdir(dp)) != NULL)
        dls += std::string(ep->d_name) + "\n";
    (void)closedir(dp);

    // then we parse it
    std::vector<WorldInfo> worlds;
    std::stringstream ss(dls);
    std::string line;
    while (std::getline(ss, line))
    {
        // . and .. are current dir and parent dir, skip them
        if (line == "." || line == "..")
            continue;

        std::string worldFile = std::string(mtnconfigGet("worldsDir")) + "/" + line;

        if (!fsIsDir(worldFile.c_str()))
            continue; // not directory

        std::string worldName = getWorldName(std::string(worldFile.c_str()));
        if (worldName == "\1\4\3\2")
            continue; // world.meta corrupted

        int size = fsGetDirSize(worldFile.c_str());
        worlds.push_back({worldName, size});
    }

    return worlds;
}

std::vector<std::string> WorldManager::getFavoriteWorlds(void)
{
    // <data dir>/favWorlds.txt
    std::string favWorldsPath = std::string(std::string(mtnconfigGet("dataDir")) + "/favWorlds.txt");

    // check if the favorites file exists
    if (!fsFileExists(favWorldsPath.c_str()))
        return std::vector<std::string>();

    std::vector<std::string> favWorlds;

    // each line in the file is fav world name (not folder name of the world)
    std::ifstream stream(favWorldsPath);
    std::vector<WorldInfo> worlds;
    std::string line;
    while (std::getline(stream, line))
        favWorlds.push_back(line);
    return favWorlds;
}
