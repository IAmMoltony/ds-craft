#pragma once
#include "fs.h"
#include "lang.hpp"
#include "font.hpp"
#include <string>
#include <sstream>
#include <nds.h>
#include <gl2d.h>
#include <maxmod9.h>

void showHelp(const std::string &file, Language lang, Font font, Font fontRu, Font fontSmal,
              Font fontSmallRu, u16 &frames);