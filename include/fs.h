#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <nds.h>
#include <nds/arm9/input.h>
#include <fat.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

void fsInit(void);
void fsCreateDir(const char *name);
void fsCreateFile(const char *name);
void fsWrite(const char *file, const char *data);
bool fsFileExists(const char *name);
char *fsReadFile(const char *name);

#ifdef __cplusplus
}
#endif