#pragma once

#include <map>
#include <string>

void statsSetWorld(const std::string &worldName);
float statsGetEntry(const std::string &entryKey);
void statsSetEntry(const std::string &entryKey, float value);
void statsSave(void);
void statsLoad(void);
