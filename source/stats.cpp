#include "stats.hpp"
#include "save.hpp"

static std::string _currentWorld = "";
static std::map<std::string, float> _stats;

static std::string _getStatsFile(void)
{
    return std::string(_currentWorld + "/stats.sts");
}

void statsSetWorld(const std::string &worldName)
{
    std::string worldFolder = getWorldFile(worldName);
    if (!fsFolderExists(worldFolder.c_str()))
    {
        printf("statsSetWorld: world %s (%s) doesnt exist\n", worldName.c_str(),
               normalizeWorldFileName(worldName).c_str());
        return;
    }

    _currentWorld = worldFolder;
    if (!fsFileExists(_getStatsFile().c_str()))
        fsCreateFile(_getStatsFile().c_str());
}

float statsGetEntry(const std::string &entryKey)
{
    return _stats[entryKey];
}

void statsSetEntry(const std::string &entryKey, float value)
{
    _stats[entryKey] = value;
}

void statsSave(void)
{
    std::ofstream ofs(_getStatsFile());
    for (std::map<std::string, float>::iterator it = _stats.begin(); it != _stats.end(); ++it)
    {
        std::string key = it->first;
        float value = it->second;
        ofs << key << ' ' << value << '\n';
    }
}

void statsLoad(void)
{
    std::ifstream ifs(_getStatsFile());
    std::string line;
    while (std::getline(ifs, line))
    {
        std::vector<std::string> split;
        std::string line2;
        std::stringstream ss(line);
        while (std::getline(ss, line2, ' '))
            split.push_back(line2);

        std::string key = split[0];
        float value = std::stof(split[1]);
        _stats[key] = value;
    }
}
