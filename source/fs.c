#include "fs.h"

void fsInit(void)
{
    if (!fatInitDefault())
    {
        printf("FAT init failed! :(\n");
        printf("This error is probably caused by:\n");
        printf(" - ROM is not patched\n - SD card is missing.");
        while (true)
            ;
    }
    if (!nitroFSInit(NULL))
    {
        printf("NitroFS init failed!\n");
        printf("please make sure it is set up correctly");
        while (true)
            ;
    }
}

void fsCreateDir(const char *name)
{
    struct stat st = {0};
    if (stat(name, &st) == -1)
        mkdir(name, 0700);
}

void fsCreateFile(const char *name)
{
    FILE *fp = fopen(name, "w");
#if FS_ERROR_MESSAGES
    if (!fp)
    {
        printf("fsCreateFile failed (name: %s)\n", name);
        perror(name);
        return;
    }
#endif
    fclose(fp);
}

void fsWrite(const char *file, const char *data)
{
    FILE *fp = fopen(file, "w");
#if FS_ERROR_MESSAGES
    if (fputs(data, fp) < 0)
    {
        printf("fsWrite failed (name: %s)\n", file);
        perror(file);
    }
#else
    fputs(data, fp);
#endif
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
        buf = (char *)malloc(sizeof(char) * (ssize + 1));
        rsize = fread(buf, sizeof(char), ssize, fp);
        buf[ssize] = '\0';

        if (ssize != rsize)
        {
            free(buf);
            buf = NULL;
        }
        fclose(fp);
    }
#if FS_ERROR_MESSAGES
    else
    {
        printf("fsReadFile failed (name: %s)\n", name);
        perror(name);
    }
#endif

    return buf;
}

long fsGetFileSize(const char *name)
{
    FILE *fp = fopen(name, "r");
    if (!fp)
    {
#if FS_ERROR_MESSAGES
        printf("fsGetFileSize failed (name: %s)\n", name);
        perror(name);
#endif
        return -1;
    }

    fseek(fp, 0L, SEEK_END);
    long size = ftell(fp);
    fclose(fp);
    return size;
}

char *fsHumanreadFileSize(double size)
{
    char *buf = (char *)malloc(10 * sizeof(char));
    int i = 0;
    static const char *units[] = {
        "B",
        "KB",
        "MB",
        "GB",
    };
    while (size > 1024)
    {
        size /= 1024;
        ++i;
    }
    sprintf(buf, "%.*f %s", i, size, units[i]);
    return buf;
}

fsDate fsGetFileCreationDate(const char *name)
{
    struct stat st;
    stat(name, &st);
    struct tm *timeInfo = localtime(&st.st_ctime);
    fsDate date;
    date.year = timeInfo->tm_year + 1900;
    date.month = timeInfo->tm_mon + 1;
    date.day = timeInfo->tm_mday;
    return date;
}
