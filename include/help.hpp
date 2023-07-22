#pragma once

#include <nds.h>
#include <string>
#include "lang.hpp"
#include "font.hpp"

void showHelp(const std::string &file, Language lang, Font font, Font fontRu, u16 &frames);
