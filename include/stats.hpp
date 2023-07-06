#pragma once

#include <map>
#include <string>

void statsSetWorld(const std::string &worldName);
int statsGetEntry(const std::string &entryKey);
void statsSetEntry(const std::string &entryKey, int value);
void statsSave(void);
void statsLoad(void);
