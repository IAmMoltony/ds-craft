#include <fs.h>

void fsInit(void)
{
    if (!fatInitDefault())
    {
        printf("FAT init failed! :(\n");
        printf("This error is most likely caused by the fact that the ");
        printf("ROM is not DLDI-patched or SD card is missing.\n\n");
        printf("Press A to view instructions for real hardware.\n\n");
        printf("Press B to view instructions for emulators.\n\n");
        while (true)
        {
            scanKeys();
            u32 kdown = keysDown();
            if (kdown & KEY_A)
            {
                consoleClear();
                printf("Real hardware\n\n");
                printf("Visit chishm.com/DLDI/index.html for instructions.");
            }
            if (kdown & KEY_B)
            {
                consoleClear();
                printf("Emulators\n\n");
                printf("MelonDS:\n");
                printf("1. Go to melonds.kuribo64.net/board/thread.php?pid=2902 ");
                printf("and create a virtual SD card.\n");
                printf("2. Open MelonDS, go to config -> emu settings -> DLDI -> ");
                printf("check \"Enable DLDI\" and in the \"DLDI SD card\" box, choose ");
                printf("the SD crad image you created earlier.\n\n");
                printf("DeSmuME:\n");
                printf("Visit gbatemp.net/threads/emulating-dldi-reading-from-cartridge");
                printf(".583105/#post-9368395\n");
            }
        }
    }
}

void fsCreateDir(const char *name)
{
    struct stat st = {0};
    if (stat(name, &st) == -1)
    {
        mkdir(name, 0700);
    }
}

void fsCreateFile(const char *name)
{
    FILE *fp = fopen(name, "w");
    fclose(fp);
}

void fsWrite(const char *file, const char *data)
{
    FILE *fp = fopen(file, "w");
    fputs(data, fp);
    fclose(fp);
}

void fsDeleteFile(const char *name)
{
    remove(name);
}

bool fsFileExists(const char *name)
{
    return access(name, F_OK) == 0;
}

char *fsReadFile(const char *name)
{
    char *buf = NULL;
    FILE *fp = fopen(name, "r");

    if (fp)
    {
        int ssize, rsize;
        fseek(fp, 0, SEEK_END);
        ssize = ftell(fp);
        rewind(fp);
        buf = (char*)malloc(sizeof(char) * (ssize + 1));
        rsize = fread(buf, sizeof(char), ssize, fp);
        buf[ssize] = '\0';

        if (ssize != rsize)
        {
            free(buf);
            buf = NULL;
        }
        fclose(fp);
    }

    return buf;
}

int fsGetFileSize(const char *name)
{
    struct stat st;
    stat(name, &st);
    return st.st_size * 1000;
}

char *fsHumanreadFileSize(double size)
{
    char *buf = (char *)malloc(10 * sizeof(char));
    int i = 0;
    static const char *units[] = {
        "B", "KB", "MB", "GB",
    };
    while (size > 1024)
    {
        size /= 1024;
        ++i;
    }
    sprintf(buf, "%.*f %s", i, size, units[i]);
    return buf;
}