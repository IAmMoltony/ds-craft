/**
 * @file config.h
 * @brief File for managing global game configuration
*/

#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief Initialize global configuration system
 *
 * Configuration file is located at nitro:/config.cfg
*/
void configInit(void);

/**
 * @brief Get configuration entry
 * @param key entry key
*/
const char *configGet(const char *key);

/**
 * @brief Get configuration entry as integer
 * @param key entry key
*/
int configGetInt(const char *key);

#ifdef __cplusplus
}
#endif
