#include "stats.hpp"
#include "save.hpp"
#include "log.h"

static std::string _currentWorld = "";
static std::map<std::string, int> _stats;

static std::string _getStatsFile(void)
{
    return std::string(_currentWorld + "/stats.sts");
}

void statsSetWorld(const std::string &worldName)
{
    std::string worldFolder = getWorldFile(worldName);
    if (!fsDirExists(worldFolder.c_str()))
    {
        logMessage(LOG_ERROR, "statsSetWorld: world %s (%s) doesnt exist", worldName.c_str(),
                   normalizeWorldFileName(worldName).c_str());
        return;
    }

    logMessage(LOG_INFO, "setting stats world to `%s'", worldName.c_str());

    _currentWorld = worldFolder;
    if (!fsFileExists(_getStatsFile().c_str()))
    {
        logMessage(LOG_INFO, "Stats file does not exist. Creating.");
        fsCreateFile(_getStatsFile().c_str());
    }
}

int statsGetEntry(const std::string &entryKey)
{
    if (_stats.find(entryKey) == _stats.end())
        return 0;
    return _stats[entryKey];
}

void statsSetEntry(const std::string &entryKey, int value)
{
    _stats[entryKey] = value;
}

void statsSave(void)
{
    logMessage(LOG_INFO, "saving stats");

    std::ofstream ofs(_getStatsFile());
    for (const auto &entry : _stats)
    {
        std::string key = entry.first;
        int value = entry.second;
        ofs << key << ' ' << value << '\n';
    }
}

void statsLoad(void)
{
    logMessage(LOG_INFO, "Loading stats from file %s", _getStatsFile().c_str());

    std::ifstream ifs(_getStatsFile());
    std::string line;
    while (std::getline(ifs, line))
    {
        StringVector split;
        std::string line2;
        std::stringstream ss(line);
        while (std::getline(ss, line2, ' '))
            split.push_back(line2);

        std::string key = split[0];

        // check if value is number
        if (!(!split[1].empty() && std::find_if(split[1].begin(),
                                                split[1].end(), [](unsigned char c)
                                                { return !std::isdigit(c); }) == split[1].end()))
        {
            logMessage(LOG_WARNING, "Value of key %s in stats file %s is not a number. Skipping.", key.c_str(), _getStatsFile().c_str());
            continue;
        }
        int value = std::stoi(split[1]);
        _stats[key] = value;
    }
}
