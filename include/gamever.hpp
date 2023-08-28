#pragma once

#include <nds.h>
#include <string>

/**
 * @brief Initialize game version
*/
void gameverInit(void);

/**
 * @brief Get the version string
*/
const char *getVersionString(void);

/**
 * @brief Get the hash of the specified version
 * @param versionString the version string to get the hash of (e.g. "beta1.2")
*/
u64 getVersionHash(const std::string &versionString);
