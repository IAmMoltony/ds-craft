#include "fs.h"
#include <sys/stat.h>
#include <stdio.h>
#include <dirent.h>
#include <errno.h>
#include <fat.h>
#include <filesystem.h>
#include "mtnconfig.h"
#include "mtnlog.h"

fsInitStatus fsInit(void)
{
    if (!fatInitDefault())
        return FS_INIT_STATUS_FAT_ERROR;
    if (!nitroFSInit(NULL))
        return FS_INIT_STATUS_NITROFS_ERROR;
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
    if (!fp)
    {
        if (mtnconfigGetInt("fsErrorMessages"))
            mtnlogMessageTag(LOG_ERROR, "fs", "Failed to create file %s: %s", name, strerror(errno));
        return;
    }
    else
        fclose(fp);
}

void fsWrite(const char *file, const char *data)
{
    FILE *fp = fopen(file, "w");
    
    if (fp == NULL)
    {
        if (mtnconfigGetInt("fsErrorMessages"))
            mtnlogMessageTag(LOG_ERROR, "fs", "Failed to open %s: %s", file, strerror(errno));
    }
    else
    {
        if (fputs(data, fp) == EOF && mtnconfigGet("fsErrorMessages"))
            mtnlogMessageTag(LOG_ERROR, "fs", "Failed to write to %s: %s", file, strerror(errno));
        fclose(fp);
    }
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
    if (!dir && mtnconfigGetInt("fsErrorMessages"))
    {
        mtnlogMessageTag(LOG_ERROR, "fs", "Failed to open directory %s for deletion: %s", name, strerror(errno));
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
            if (unlink(path) != 0 && mtnconfigGetInt("fsErrorMessages"))
                mtnlogMessageTag(LOG_ERROR, "fs", "Failed to unlink directory %s: %s", path, strerror(errno));
        }
    }

    closedir(dir);

    if (remove(name) != 0 && mtnconfigGetInt("fsErrorMessages"))
        mtnlogMessageTag(LOG_ERROR, "fs", "Failed to remove %s: %s", name, strerror(errno));
}

bool fsFileExists(const char *name)
{
    struct stat buffer;
    return (stat(name, &buffer) == 0);
}

bool fsDirExists(const char *name)
{
    DIR *dir = opendir(name);
    if (dir)
    {
        closedir(dir);
        return true;
    }
    else if (ENOENT == errno)
        return false; // no such file or directory
    else
    {
        if (mtnconfigGetInt("fsErrorMessages"))
            mtnlogMessageTag(LOG_ERROR, "fs", "Failed to check if dir %s exists: %s", name, strerror(errno));
        return false;
    }

    return false; // should never get here, if it does then say that it doesnt exist
}

bool fsIsDir(const char *name)
{
    struct stat fileStat;

    if (stat(name, &fileStat) == -1)
    {
        if (mtnconfigGetInt("fsErrorMessages"))
            mtnlogMessageTag(LOG_ERROR, "fs", "Failed to check if %s is a directory or not: %s", name, strerror(errno));
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
    else if (mtnconfigGetInt("fsErrorMessages"))
        mtnlogMessageTag(LOG_ERROR, "fs", "Failed to read file %s: %s", name, strerror(errno));

    return buf;
}

long fsGetFileSize(const char *name)
{
    FILE *fp = fopen(name, "r");
    if (!fp)
    {
        if (mtnconfigGetInt("fsErrorMessages"))
            mtnlogMessageTag(LOG_ERROR, "fs", "Failed to open %s: %s", name, strerror(errno));
        return -1;
    }

    fseek(fp, 0L, SEEK_END);
    long size = ftell(fp);
    if (size == -1L) {
        if (mtnconfigGetInt("fsErrorMessages"))
            mtnlogMessageTag(LOG_ERROR, "fs", "ftell failed on %s: %s", name, strerror(errno));
        fclose(fp);
        return -1;
    }
    fclose(fp);
    return size;
}

long fsGetDirSize(const char *name)
{
    DIR *d = opendir(name);

    if (!d)
    {
        if (mtnconfigGetInt("fsErrorMessages"))
            mtnlogMessageTag(LOG_ERROR, "fs", "Failed to open directory %s: %s", name, strerror(errno));
        return -1;
    }

    struct dirent *de;
    long totalSize = 0;
    for (de = readdir(d); de != NULL; de = readdir(d))
    {
        char *fullFileName = malloc(strlen(name) + strlen(de->d_name) + 2);
        if (!fullFileName)
        {
            if (mtnconfigGetInt("fsErrorMessages"))
                mtnlogMessageTag(LOG_ERROR, "fs", "Failed to allocate memory while checking if dir '%s' exists", de->d_name);
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
            "KiB",
            "MiB",
            "GiB",
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
    const struct tm *timeInfo = localtime(&st.st_ctime);
    fsDate date;
    date.year = timeInfo->tm_year + 1900;
    date.month = timeInfo->tm_mon + 1;
    date.day = timeInfo->tm_mday;
    return date;
}
