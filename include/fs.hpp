#pragma once
#include <nds.h>
#include <filesystem.h>
#include <fstream>
#include <sstream>
#include <string>
#include <stdio.h>

void fsInit(void);
std::string readFile(const char *path);
bool fileExists(const char *path);