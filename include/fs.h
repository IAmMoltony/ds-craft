#pragma once

#include <nds.h>
#include <fat.h>
#include <filesystem.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include "defines.h"

#ifdef __cplusplus
extern "C"
{
#endif

    void fsInit(void);
    void fsCreateDir(const char *name);
    void fsChangeDir(const char *dir);
    void fsCreateFile(const char *name);
    void fsWrite(const char *file, const char *data);
    void fsDeleteFile(const char *name);
    bool fsFileExists(const char *name);
    char *fsReadFile(const char *name);
    long fsGetFileSize(const char *name); // in bytes
    char *fsHumanreadFileSize(double size);

#ifdef __cplusplus
}
#endif