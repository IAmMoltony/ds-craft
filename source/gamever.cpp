#include "gamever.hpp"

static std::string versionPrefix = "";
static std::string versionString = "";
static u8 versionMajor = 0;
static u8 versionMinor = 0;
static u8 versionPatch = 0;

void gameverInit(void)
{
    if (!fsFileExists("nitro:/game.ver"))
    {
        printf("game.ver not found");
        while (true)
            ;
    }

    FILE *f = fopen("nitro:/game.ver", "r");
    u8 count = 0;
    if (f)
    {
        char line[20];
        while (fgets(line, sizeof(line), f) != NULL)
        {
            if (line[strlen(line) - 1] == '\n')
                line[strlen(line) - 1] = 0; // remove \n

            if (count == 0)
                versionMajor = atoi(line);
            else if (count == 1)
                versionMinor = atoi(line);
            else if (count == 2)
                versionPatch = atoi(line);
            else if (count == 3)
                versionPrefix = std::string(line);

            ++count;
        }
        fclose(f);
    }
    else
    {
        printf("there was problem opening game.ver file");
        while (true)
            ;
    }

    if (versionPatch == 0)
        versionString = versionPrefix + std::to_string(versionMajor) + '.' +
                        std::to_string(versionMinor);
    else
        versionString = versionPrefix + std::to_string(versionMajor) + '.' +
                        std::to_string(versionMinor) + '.' + std::to_string(versionPatch);
}

const char *getVersionString(void)
{
    return versionString.c_str();
}