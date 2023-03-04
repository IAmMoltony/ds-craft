#pragma once

#include <nds.h>
#include <fat.h>
#include <filesystem.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include "defines.h"

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct
    {
        u16 year;
        u8 month;
        u8 day;
    } fsDate;

    void fsInit(void);
    void fsCreateDir(const char *name);
    void fsCreateFile(const char *name);
    void fsWrite(const char *file, const char *data);
    void fsDeleteFile(const char *name);
    void fsDeleteDir(const char *name);
    bool fsFileExists(const char *name);
    bool fsFolderExists(const char *name);
    bool fsIsDir(const char *name);
    char *fsReadFile(const char *name);
    long fsGetFileSize(const char *name); // in bytes
    long fsGetDirSize(const char *name); // in bytes
    char *fsHumanreadFileSize(double size);
    fsDate fsGetFileCreationDate(const char *name);

#ifdef __cplusplus
}
#endif
