#pragma once

#include <string>
#include <stdio.h>
#include "fs.h"

void gameverInit(void);
const char *getVersionString(void);
// the smaller the value returned, the older the version
u64 getVersionHash(const std::string &versionString);
