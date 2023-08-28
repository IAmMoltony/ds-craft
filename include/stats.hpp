#pragma once

#include <map>
#include <string>

/**
 * @brief Set the world to save and load stats
 * @param worldName name of the world to set (doesn't need to be normalized)
*/
void statsSetWorld(const std::string &worldName);

/**
 * @brief Get stats entry
 * @param entryKey the key of the entry
*/
int statsGetEntry(const std::string &entryKey); // TODO make entry keys into constants

/**
 * @brief Set stats entry
 * @param entryKey the key of the entry
 * @param value the value to set
*/
void statsSetEntry(const std::string &entryKey, int value);

/**
 * @brief Save stats to the world's stats.sts file
*/
void statsSave(void);

/**
 * @brief Load stats from the world's stats.sts file
*/
void statsLoad(void);
