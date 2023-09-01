#include "gamever.hpp"
#include <stdio.h>
#include "defines.h"
#include "fs.h"
#include "util.h"
#include "config.h"
#include "log.h"

static std::string _versionPrefix = "";
static std::string _versionString = "";
static u8 _versionMajor = 0;
static u8 _versionMinor = 0;
static u8 _versionPatch = 0;

void gameverInit(void)
{
    FILE *f = fopen(configGet("gameVersionFile"), "r");
    if (!f)
    {
        logMessage(LOG_ERROR, "Failed opening game version file because %s", strerror(errno));
        hang();
    }
    u8 count = 0;
    if (f)
    {
        char line[20];
        while (fgets(line, sizeof(line), f) != NULL)
        {
            if (line[strlen(line) - 1] == '\n')
                line[strlen(line) - 1] = 0; // remove \n

            if (count == 0)
                _versionMajor = atoi(line);
            else if (count == 1)
                _versionMinor = atoi(line);
            else if (count == 2)
                _versionPatch = atoi(line);
            else if (count == 3)
                _versionPrefix = std::string(line);

            ++count;
        }
        fclose(f);
    }
    else
    {
        printf("there was problem opening game.ver file");
        hang();
    }

    // if patch is 0, then don't put patch in version string
    if (_versionPatch == 0)
        _versionString = _versionPrefix + std::to_string(_versionMajor) + '.' +
                        std::to_string(_versionMinor);
    else
        _versionString = _versionPrefix + std::to_string(_versionMajor) + '.' +
                        std::to_string(_versionMinor) + '.' + std::to_string(_versionPatch);
}

const char *getVersionString(void)
{
    return _versionString.c_str();
}

enum class VersionType
{
    Alpha, Beta, Release,
};

u64 getVersionHash(const std::string &_versionString)
{
    // extract the major, minor, and patch numbers from the version string
    int major = 0;
    int minor = 0;
    int patch = 0;

    sscanf(_versionString.c_str(), "%*[^0-9]%d.%d.%d", &major, &minor, &patch);
    if (patch == 0)
        sscanf(_versionString.c_str(), "%*[^0-9]%d.%d", &major, &minor);

    // determine version type
    VersionType versionType = VersionType::Release;
    if (_versionString.find("alpha") != std::string::npos)
        versionType = VersionType::Alpha;
    else if (_versionString.find("beta") != std::string::npos)
        versionType = VersionType::Beta;

    // calculate the hash based on type and numbers
    u64 versionHash = static_cast<u32>(versionType);
    versionHash <<= 24;
    versionHash |= (major << 16) + (minor << 8) + patch;

    return versionHash;
}
