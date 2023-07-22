#include "fs.h"
#include <sys/stat.h>
#include <stdio.h>
#include <dirent.h>
#include <errno.h>
#include <fat.h>
#include <filesystem.h>

fsInitStatus fsInit(void)
{
    if (!nitroFSInit(NULL))
        return FS_INIT_STATUS_NITROFS_ERROR;
    if (!fatInitDefault())
        return FS_INIT_STATUS_FAT_ERROR;
    return FS_INIT_STATUS_OK;
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

void fsDeleteDir(const char *name)
{
    DIR *dir;
    struct dirent *entry;
    char path[PATH_MAX];

    dir = opendir(name);
    if (dir == NULL)
    {
#if FS_ERROR_MESSAGES
        printf("fsDeleteDir: opendir error\n");
        perror(name);
#endif
        return;
    }

    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;
        snprintf(path, PATH_MAX, "%s/%s", name, entry->d_name);
        if (entry->d_type == DT_DIR)
            fsDeleteDir(path);
        else
        {
            if (unlink(path) != 0)
            {
#if FS_ERROR_MESSAGES
                printf("fsDeleteDir: unlink error\n");
                perror(path);
#endif
            }
        }
    }

    closedir(dir);

    if (remove(name) != 0)
    {
#if FS_ERROR_MESSAGES
        printf("fsDeleteDir: rmdir error\n");
        perror(name);
#endif
    }
}

bool fsFileExists(const char *name)
{
    return access(name, F_OK) == 0;
}

bool fsFolderExists(const char *name)
{
    DIR *dir = opendir(name);
    if (dir)
    {
        closedir(dir);
        return true;
    }
    else if (ENOENT == errno)
        return false;
    else
    {
#if FS_ERROR_MESSAGES
        perror("fsFolderExists: opendir() failed");
#endif
        return false;
    }

    return false;
}

bool fsIsDir(const char *name)
{
    struct stat fileStat;

    if (stat(name, &fileStat) == -1)
    {
#if FS_ERROR_MESSAGES
        printf("fsIsDir failed: ");
        perror(name);
#endif
        return false;
    }

    return S_ISDIR(fileStat.st_mode);
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
        printf("fsGetFileSize failed: ");
        perror(name);
#endif
        return -1;
    }

    fseek(fp, 0L, SEEK_END);
    long size = ftell(fp);
    fclose(fp);
    return size;
}

long fsGetDirSize(const char *name)
{
    DIR *d = opendir(name);

    if (!d)
    {
#if FS_ERROR_MESSAGES
        printf("fsGetDirSize: opendir() failed with name ");
        perror(name);
#endif
        return -1;
    }

    struct dirent *de;
    long totalSize = 0;
    for (de = readdir(d); de != NULL; de = readdir(d))
    {
        char *fullFileName = malloc(strlen(name) + strlen(de->d_name) + 2);
        if (!fullFileName)
        {
#if FS_ERROR_MESSAGES
            printf("fsGetDirSize: failed to allocate memory\n");
#endif
            return -1;
        }
        strcpy(fullFileName, name);
        strcat(fullFileName, "/");
        strcat(fullFileName, de->d_name);

        if (fsIsDir(fullFileName))
        {
            if (strcmp(de->d_name, ".") != 0 && strcmp(de->d_name, "..") != 0)
            {
                long subDirSize = fsGetDirSize(fullFileName);
                if (subDirSize < 0)
                {
                    free(fullFileName);
                    closedir(d);
                    return -1;
                }
                totalSize += subDirSize;
            }
        }
        else
            totalSize += fsGetFileSize(fullFileName);

        free(fullFileName);
    }

    closedir(d);
    return totalSize;
}

char *fsHumanreadFileSize(double size)
{
    char *buf = (char *)malloc(10 * sizeof(char));
    int i = 0;
    static const char *units[] =
        {
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
