#pragma once

#include <nds.h>
#include <string>

void gameverInit(void);
const char *getVersionString(void);
// the smaller the value returned, the older the version
u64 getVersionHash(const std::string &versionString);
