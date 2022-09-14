#pragma once

#include <fs.h>
#include <nds.h>
#include <limits.h>

#ifdef __cplusplus
extern "C"
{
#endif

    void gameverInit(void);
    u8 getVersionMajor(void);
    u8 getVersionMinor(void);
    u8 getVersionPatch(void);
    char *getVersionPrefix(void);
    char *getVersionString(void);

#ifdef __cplusplus
}
#endif