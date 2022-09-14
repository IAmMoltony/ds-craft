#include <gamever.h>

static u8 major = 0;
static u8 minor = 0;
static u8 patch = 0;
static char *prefix;
static char *verstr;

int numPlaces(int n)
{
    if (n < 0)
        return numPlaces((n == INT_MIN) ? INT_MAX : -n);
    if (n < 10)
        return 1;
    return 1 + numPlaces(n / 10);
}

void gameverInit(void)
{
    if (!fsFileExists("nitro:/game.ver"))
    {
        printf("game.ver not found (this should never happen)");
        while (true)
            ;
    }

    FILE *gameverFile = fopen("nitro:/game.ver", "r");
    uint8_t count = 0;
    if (gameverFile != NULL)
    {
        char *line = malloc(10 * sizeof(char));
        while (fgets(line, 10, gameverFile) != NULL)
        {
            line[strcspn(line, "\r\n")] = 0; // just to be safe

            switch (count)
            {
            case 0:
                prefix = malloc(strlen(line) * sizeof(char));
                strcpy(prefix, line);
                break;
            case 1:
                major = atoi(line);
                break;
            case 2:
                minor = atoi(line);
                break;
            case 3:
                patch = atoi(line);
                break;
            }
            ++count;
        }
        free(line);
        fclose(gameverFile);
    }
    else
    {
        printf("unable to open game.ver");
        while (true)
            ;
    }

    verstr = malloc((strlen(prefix) + numPlaces(major) + numPlaces(minor) + numPlaces(patch) + 2) *
                    sizeof(char));
    sprintf(verstr, "%s%d.%d.%d", prefix, major, minor, patch);
}

u8 getVersionMajor(void)
{
    return major;
}

u8 getVersionMinor(void)
{
    return minor;
}

u8 getVersionPatch(void)
{
    return patch;
}

char *getVersionPrefix(void)
{
    return prefix;
}

char *getVersionString(void)
{
    return verstr;
}