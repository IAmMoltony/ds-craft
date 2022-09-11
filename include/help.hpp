#pragma once
#include <nds.h>
#include <gl2d.h>
#include <maxmod9.h>
#include <fs.h>
#include <lang.hpp>
#include <font.hpp>
#include <string>
#include <sstream>

void showHelp(const std::string &file, Language lang, Font font, Font fontRu, Font fontSmal,
              Font fontSmallRu, u16 &frames);